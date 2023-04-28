// [[file:../../../async_control.org::*Decision][Decision:1]]
#include <stdexec/execution.hpp>
#include <exec/static_thread_pool.hpp>
#include <exec/any_sender_of.hpp>
#include <exec/variant_sender.hpp>
#include <iostream>

template <class... Ts>
using any_sender_of = typename exec::any_receiver_ref<
    stdexec::completion_signatures<Ts...>>::template any_sender<>;

inline auto tst =
    [](bool                 cond,
       stdexec::sender auto left,
       stdexec::sender auto right) -> exec::variant_sender<decltype(left), decltype(right)> {
    if (cond)
        return left;
    else
        return right;
};

int main() {
// Decision:1 ends here

// [[file:../../../async_control.org::*Decision][Decision:2]]
exec::static_thread_pool pool(8);

stdexec::scheduler auto sch = pool.get_scheduler();

stdexec::sender auto begin  = stdexec::schedule(sch);
stdexec::sender auto seven  = stdexec::just(7);
stdexec::sender auto eleven = stdexec::just(11);

stdexec::sender auto branch =
    begin
    | stdexec::then([]() { return std::make_tuple(5, 4); })
    | stdexec::let_value(
        [=](auto tpl) {
        auto const& [i, j] = tpl;

        return tst((i > j),
                   seven | stdexec::then([&](int k) noexcept {
                       std::cout << "true branch " << k << '\n';
                   }),
                   eleven | stdexec::then([&](int k) noexcept {
                       std::cout << "false branch " << k << '\n';
                   }));
    });

stdexec::sync_wait(std::move(branch));
// Decision:2 ends here

// [[file:../../../async_control.org::*Decision][Decision:3]]
}
// Decision:3 ends here
