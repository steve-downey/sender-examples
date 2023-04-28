#include <cassert>
#include <stdexec/execution.hpp>
#include <exec/static_thread_pool.hpp>
#include <exec/any_sender_of.hpp>
#include <iostream>

template <class... Ts>
using any_sender_of = typename exec::any_receiver_ref<
    stdexec::completion_signatures<Ts...>>::template any_sender<>;

using any_int_sender = any_sender_of<stdexec::set_value_t(int),
                                     stdexec::set_stopped_t(),
                                     stdexec::set_error_t(std::exception_ptr)>;

auto fib(int n) -> any_int_sender {
    if (n == 0)
        return stdexec::just(0);

    if (n == 1)
        return stdexec::just(1);

    auto work =
        stdexec::when_all(stdexec::just(n - 1)
                          | stdexec::let_value([](int k) {
                              return fib(k);
                          }),
                          stdexec::just(n - 2)
                          | stdexec::let_value([](int k) {
                              return fib(k);
                          }))
        | stdexec::then([](auto i, auto j) { return i + j; });

    return work;
}

auto fibr(int n) -> int {
    if (n == 0)
        return 0;

    if (n == 1)
        return 1;

    return fibr(n - 1) + fibr(n - 2);
}

int main() {
    exec::static_thread_pool pool(1);

    stdexec::scheduler auto sch = pool.get_scheduler();

    stdexec::sender auto begin = stdexec::schedule(sch);

    int                  k = 36;
    stdexec::sender auto fibonacci =
        begin | stdexec::then([=]() { return k; }) |
        stdexec::let_value([](int k) { return fib(k); });

    std::cout << "fibonacci built\n";

    auto [i] = stdexec::sync_wait(std::move(fibonacci)).value();
    std::cout << "fibonacci " << k << " = " << i << '\n';

    std::cout << "fibonacci " << k << " = " << fibr(k) << '\n';
}
