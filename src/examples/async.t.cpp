#include <cassert>
#include <stdexec/execution.hpp>
#include <exec/static_thread_pool.hpp>
#include <exec/any_sender_of.hpp>
#include <iostream>


#include <gtest/gtest.h>

TEST(AsyncTest, TestGTest) { ASSERT_EQ(1, 1); }

TEST(AsyncTest, Breathing) {}

template <class... Ts>
using any_sender_of = typename exec::any_receiver_ref<
    stdexec::completion_signatures<Ts...>>::template any_sender<>;

using any_int_sender = any_sender_of<stdexec::set_value_t(int),
                                     stdexec::set_stopped_t(),
                                     stdexec::set_error_t(std::exception_ptr)>;


auto fac(int n) -> any_int_sender {
    if (n == 0)
        return stdexec::just(1);

    return stdexec::just(n - 1) |
           stdexec::let_value([](int k) { return fac(k); }) |
           stdexec::then([n](int k) { return k * n; });
}

TEST(AsyncTest, Factorial) {
    stdexec::sender auto factorial5 =
        stdexec::just(5) |
        stdexec::let_value([](int k) { return fac(k); });

    stdexec::sender auto factorial10 =
        stdexec::just(10) |
        stdexec::let_value([](int k) { return fac(k); });

    stdexec::sender auto factorial15 =
        stdexec::just(15) |
        stdexec::let_value([](int k) { return fac(k); });

    auto [i] = stdexec::sync_wait(std::move(factorial5)).value();
    ASSERT_EQ(i, 120);

    auto [j] = stdexec::sync_wait(std::move(factorial10)).value();
    ASSERT_EQ(j, 3628800);

    auto [k] = stdexec::sync_wait(std::move(factorial15)).value();
    ASSERT_EQ(k, 2004310016);

    std::cout << "factorial " << k << " = " << i << '\n';
}
