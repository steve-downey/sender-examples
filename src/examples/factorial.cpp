// [[file:../../../async_control.org::*Simple Recursion][Simple Recursion:1]]
#include <cassert>
#include <stdexec/execution.hpp>
#include <exec/static_thread_pool.hpp>
#include <exec/any_sender_of.hpp>
#include <iostream>

template <class... Ts>
using any_sender_of = typename exec::any_receiver_ref<
    stdexec::completion_signatures<Ts...>>::template any_sender<>;
// Simple Recursion:1 ends here

// [[file:../../../async_control.org::*Simple Recursion][Simple Recursion:2]]
using any_int_sender =
    any_sender_of<stdexec::set_value_t(int),
                  stdexec::set_stopped_t(),
                  stdexec::set_error_t(std::exception_ptr)>;

auto fac(int n) -> any_int_sender {
    std::cout << "factorial of " << n << "\n";
    if (n == 0)
        return stdexec::just(1);

    return stdexec::just(n - 1)
        | stdexec::let_value([](int k) { return fac(k); })
        | stdexec::then([n](int k) { return k * n; });
}
// Simple Recursion:2 ends here

// [[file:../../../async_control.org::*Simple Recursion][Simple Recursion:3]]
int main() {
    exec::static_thread_pool pool(8);

    stdexec::scheduler auto sch = pool.get_scheduler();

    stdexec::sender auto begin = stdexec::schedule(sch);
// Simple Recursion:3 ends here

// [[file:../../../async_control.org::*Simple Recursion][Simple Recursion:4]]
    int                  k = 10;
    stdexec::sender auto factorial =
        begin
        | stdexec::then([=]() { return k; })
        | stdexec::let_value([](int k) { return fac(k); });

    std::cout << "factorial built\n\n";

    auto [i] = stdexec::sync_wait(std::move(factorial)).value();
    std::cout << "factorial " << k << " = " << i << '\n';
// Simple Recursion:4 ends here

// [[file:../../../async_control.org::*Simple Recursion][Simple Recursion:5]]
    }
// Simple Recursion:5 ends here
