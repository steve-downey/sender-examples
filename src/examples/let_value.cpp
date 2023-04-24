// [[file:../../../async_control.org::*Dynamic Choice of Sender][Dynamic Choice of Sender:1]]
#include <stdexec/execution.hpp>
#include <exec/static_thread_pool.hpp>
#include <string>

int main()
{
// Dynamic Choice of Sender:1 ends here

// [[file:../../../async_control.org::*Dynamic Choice of Sender][Dynamic Choice of Sender:2]]
exec::static_thread_pool pool(3);

auto sched = pool.get_scheduler();

auto fun = [](int i) -> stdexec::sender auto {
  using namespace std::string_literals;
  if ((i % 2) == 0) {
    return stdexec::just("even"s);
  } else {
    return stdexec::just("odd"s);
  }
};

auto work = stdexec::when_all(
    stdexec::on(sched, stdexec::just(0) | stdexec::let_value(fun)),
    stdexec::on(sched, stdexec::just(1) | stdexec::let_value(fun)),
    stdexec::on(sched, stdexec::just(2) | stdexec::let_value(fun))

auto [i, j, k] = stdexec::sync_wait(std::move(work)).value();

std::printf("%s %s %s", i.c_str(), j.c_str(), k.c_str());
// Dynamic Choice of Sender:2 ends here

// [[file:../../../async_control.org::*Dynamic Choice of Sender][Dynamic Choice of Sender:3]]
}
// Dynamic Choice of Sender:3 ends here
