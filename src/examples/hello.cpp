// [[file:../../../async_control.org::*Hello Async World][Hello Async World:1]]
#include <stdexec/execution.hpp>
#include <exec/static_thread_pool.hpp>
#include <iostream>

int main() {
  exec::static_thread_pool pool(8);

  stdexec::scheduler auto sch = pool.get_scheduler();

  stdexec::sender auto begin = stdexec::schedule(sch);
  stdexec::sender auto hi    = stdexec::then(begin, [] {
    std::cout << "Hello world! Have an int.";
    return 13;
  });

  auto add_42 = stdexec::then(hi, [](int arg) { return arg + 42; });

  auto [i] = stdexec::sync_wait(add_42).value();

  return i;
}
// Hello Async World:1 ends here
