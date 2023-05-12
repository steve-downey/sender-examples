#include <cassert>
#include <stdexec/execution.hpp>
#include <exec/static_thread_pool.hpp>
#include <exec/any_sender_of.hpp>
#include <exec/repeat_effect_until.hpp>
#include <iostream>

auto snder(int t) {
    int n = 0;
    int acc = 0;
    stdexec::sender auto snd =
        stdexec::just(t)
        | stdexec::let_value([&acc, &n](int k) {
            return stdexec::just()
                | stdexec::then([&n, &acc, k] {
                    acc += n;
                    ++n;
                    return n == k;
                })
                | exec::repeat_effect_until();
        })
        | stdexec::then([&acc]() { return acc; });

    return snd;
}

int main() {
    stdexec::sender auto work = stdexec::just(101) | stdexec::let_value(snder);

    auto [i] = stdexec::sync_wait(std::move(work)).value();
    std::cout << "work "
              << " = " << i << '\n';
}
