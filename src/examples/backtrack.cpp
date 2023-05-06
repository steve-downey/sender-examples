// [[file:../../../async_control.org::*Backtrack][Backtrack:1]]
#include <cassert>

#include <stdexec/execution.hpp>
#include <exec/static_thread_pool.hpp>
#include <exec/any_sender_of.hpp>
#include <tree/tree.h>

#include <iostream>
#include <ranges>

template <class... Ts>
using any_sender_of = typename exec::any_receiver_ref<
    stdexec::completion_signatures<Ts...>>::template any_sender<>;

using any_int_sender = any_sender_of<stdexec::set_value_t(int),
                                     stdexec::set_stopped_t(),
                                     stdexec::set_error_t(std::exception_ptr)>;
// Backtrack:1 ends here

// [[file:../../../async_control.org::*Backtrack][Backtrack:2]]
using any_node_sender =
    any_sender_of<stdexec::set_value_t(tree::NodePtr<int>),
                  stdexec::set_stopped_t(),
                  stdexec::set_error_t(std::exception_ptr)>;

auto search_tree(auto                    test,
                 tree::NodePtr<int>      tree,
                 stdexec::scheduler auto sch,
                 any_node_sender&&       fail) -> any_node_sender {
    if (tree == nullptr) {
        return std::move(fail);
    }
    if (test(tree)) {
        return stdexec::just(tree);
    }
    return stdexec::on(sch, stdexec::just()) |
           stdexec::let_value([=, fail = std::move(fail)]() mutable {
               return search_tree(
                   test,
                   tree->left(),
                   sch,
                   stdexec::on(sch, stdexec::just()) |
                       stdexec::let_value(
                           [=, fail = std::move(fail)]() mutable {
                               return search_tree(
                                   test, tree->right(), sch, std::move(fail));
                           }));
           });
    return fail;
}
// Backtrack:2 ends here

// [[file:../../../async_control.org::*Backtrack][Backtrack:3]]
int main() {
    exec::static_thread_pool pool(8);

    stdexec::scheduler auto sch = pool.get_scheduler();

    stdexec::sender auto begin = stdexec::schedule(sch);
// Backtrack:3 ends here

// [[file:../../../async_control.org::*Backtrack][Backtrack:4]]
    tree::NodePtr<int> t;
    for (auto i : std::ranges::views::iota(1, 10'000)) {
        tree::Tree<int>::insert(i, t);
    }

    auto test = [](tree::NodePtr<int> t) -> bool {
        return t ? t->data() == 500 : false;
    };

    auto fail = begin | stdexec::then([]() { return tree::NodePtr<int>{}; });

    stdexec::sender auto work =
        begin | stdexec::let_value([=]() {
            return search_tree(test, t, sch, std::move(fail));
        });

    auto [n] = stdexec::sync_wait(std::move(work)).value();

    std::cout << "work "
              << " = " << n->data() << '\n';
// Backtrack:4 ends here

// [[file:../../../async_control.org::*Backtrack][Backtrack:5]]
}
// Backtrack:5 ends here
