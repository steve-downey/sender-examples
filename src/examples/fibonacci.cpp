// [[file:../../../async_control.org::*General Recursion][General Recursion:1]]
#include <cassert>
#include <stdexec/execution.hpp>
#include <exec/static_thread_pool.hpp>
#include <exec/any_sender_of.hpp>
#include <iostream>

auto getDefaultPool() -> exec::static_thread_pool& {
    static exec::static_thread_pool pool(1);
    return pool;
}

auto getDefaultScheduler() -> stdexec::scheduler auto& {
    static stdexec::scheduler auto sch = getDefaultPool().get_scheduler();
    return sch;
}

template <class... Ts>
using any_sender_of = typename exec::any_receiver_ref<
    stdexec::completion_signatures<Ts...>>::template any_sender<>;

using any_int_sender = any_sender_of<stdexec::set_value_t(int),
                                     stdexec::set_stopped_t(),
                                     stdexec::set_error_t(std::exception_ptr)>;
// General Recursion:1 ends here

// [[file:../../../async_control.org::*General Recursion][General Recursion:2]]
auto fib(int n) -> any_int_sender {
    if (n == 0)
        return stdexec::on(getDefaultScheduler(),  stdexec::just(0));

    if (n == 1)
        return stdexec::on(getDefaultScheduler(), stdexec::just(1));

    auto work = stdexec::when_all(
                    stdexec::on(getDefaultScheduler(), stdexec::just(n - 1)) |
                        stdexec::let_value([](int k) { return fib(k); }),
                    stdexec::on(getDefaultScheduler(), stdexec::just(n - 2)) |
                        stdexec::let_value([](int k) { return fib(k); })) |
                stdexec::then([](auto i, auto j) { return i + j; });

    return work;
}
// General Recursion:2 ends here

// [[file:../../../async_control.org::*General Recursion][General Recursion:3]]
auto fibr(int n) -> int {
    if (n == 0)
        return 0;

    if (n == 1)
        return 1;

    return fibr(n - 1) + fibr(n - 2);
}

int main() {
    stdexec::scheduler auto sch = getDefaultScheduler();

    stdexec::sender auto begin = stdexec::schedule(sch);
// General Recursion:3 ends here

// [[file:../../../async_control.org::*General Recursion][General Recursion:4]]
    int                  k = 30;
    stdexec::sender auto fibonacci =
        begin | stdexec::then([=]() { return k; }) |
        stdexec::let_value([](int k) { return fib(k); });

    std::cout << "fibonacci built\n";

    auto [i] = stdexec::sync_wait(std::move(fibonacci)).value();
    std::cout << "fibonacci " << k << " = " << i << '\n';
// General Recursion:4 ends here

// [[file:../../../async_control.org::*General Recursion][General Recursion:5]]
    std::cout << "fibonacci " << k << " = " << fibr(k) << '\n';
}
// General Recursion:5 ends here
