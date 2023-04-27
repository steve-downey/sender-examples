#include "stdexec/__detail/__execution_fwd.hpp"
#include <stdexec/execution.hpp>
#include <exec/static_thread_pool.hpp>
#include <exec/task.hpp>
#include <iostream>

inline auto&& left_sender = [](auto&& left, auto&& right) {
    return std::move(left);
};

inline auto&& right_sender = [](auto&& left, auto&& right) {
    return std::move(right);
};


inline auto test = [](bool                 cond,
                      stdexec::sender auto left,
                      stdexec::sender auto right) noexcept {
    if (cond)
        return left_sender(left, right);
    else
        return right_sender(left, right);
};

template <class... Ts>
using any_sender_of = typename exec::any_receiver_ref<
    stdexec::completion_signatures<Ts...>>::template any_sender<>;

int main() {
    // Declare a pool of 8 worker threads:
    exec::static_thread_pool pool(8);

    stdexec::scheduler auto sch = pool.get_scheduler();

    stdexec::sender auto begin  = stdexec::schedule(sch);
    stdexec::sender auto seven  = stdexec::just(7);
    stdexec::sender auto eleven = stdexec::just(11);

    stdexec::sender auto next =
        stdexec::then(begin, []() { return std::make_tuple(5, 4); });

    stdexec::sender auto hi = stdexec::let_value(
        next,
        [seven, eleven](auto t) {
            auto const& [i,j] = t;

            any_sender_of<stdexec::set_value_t()> l =
                seven | stdexec::then([&](int i) noexcept {
                    std::cout << "true branch " << i << '\n';
                });

            any_sender_of<stdexec::set_value_t()> r =
                eleven | stdexec::then([&](int i) noexcept {
                    std::cout << "false branch " << i << '\n';
                });

            any_sender_of<stdexec::set_value_t()> d =
                test((i > j), std::move(l), std::move(r));

            return d;
        });

    stdexec::sync_wait(std::move(hi));
}
