#include <cassert>
#include <stdexec/execution.hpp>
#include <exec/static_thread_pool.hpp>
#include <exec/any_sender_of.hpp>
#include <iostream>
#include <ranges>

template <class... Ts>
using any_sender_of = typename exec::any_receiver_ref<
    stdexec::completion_signatures<Ts...>>::template any_sender<>;

using any_int_sender = any_sender_of<stdexec::set_value_t(int),
                                     stdexec::set_stopped_t(),
                                     stdexec::set_error_t(std::exception_ptr)>;

struct fold_left_fn {
    template <std::input_iterator I, std::sentinel_for<I> S, class T, class F>
    constexpr auto operator()(I first, S last, T init, F f) const
        -> any_sender_of<
            stdexec::set_value_t(
                std::decay_t<
                    std::invoke_result_t<F&, T, std::iter_reference_t<I>>>),
            stdexec::set_stopped_t(),
            stdexec::set_error_t(std::exception_ptr)> {
        using U = std::decay_t<
            std::invoke_result_t<F&, T, std::iter_reference_t<I>>>;

        if (first == last) {
            return stdexec::just(U(std::move(init)));
        }

        auto nxt =
            stdexec::just(std::invoke(f, std::move(init), *first)) |
            stdexec::let_value([this,
                                first = first,
                                last = last,
                                f = f
                                ](U u) {
                I i = first;
                return (*this)(++i, last, u, f);
            });
        return std::move(nxt);
    }

    template <std::ranges::input_range R, class T, class F>
    constexpr auto operator()(R&& r, T init, F f) const {
        return (*this)(std::ranges::begin(r),
                       std::ranges::end(r),
                       std::move(init),
                       std::ref(f));
    }
};

inline constexpr fold_left_fn fold_left;

int main() {
    exec::static_thread_pool pool(1);

    stdexec::scheduler auto sch = pool.get_scheduler();

    stdexec::sender auto begin = stdexec::schedule(sch);

    auto v = std::ranges::iota_view{1, 10'000};

    stdexec::sender auto work =
        begin
        | stdexec::let_value([i = std::ranges::begin(v),
                              s = std::ranges::end(v)]() {
            return fold_left(i, s, 0, [](int i, int j) { return i + j; });
        });

    auto [i] = stdexec::sync_wait(std::move(work)).value();

    std::cout << "work " << " = " << i << '\n';
}
