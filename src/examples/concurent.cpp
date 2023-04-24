// [[file:../../../async_control.org::*When All - Concurent Async][When All - Concurent Async:1]]
#include <stdexec/execution.hpp>
#include <exec/static_thread_pool.hpp>

int main()
{
// When All - Concurent Async:1 ends here

// [[file:../../../async_control.org::*When All - Concurent Async][When All - Concurent Async:2]]
exec::static_thread_pool pool(3);

auto sched = pool.get_scheduler();

auto fun = [](int i) { return i * i; };

auto work = stdexec::when_all(
    stdexec::on(sched, stdexec::just(0) | stdexec::then(fun)),
    stdexec::on(sched, stdexec::just(1) | stdexec::then(fun)),
    stdexec::on(sched, stdexec::just(2) | stdexec::then(fun)));

auto [i, j, k] = stdexec::sync_wait(std::move(work)).value();

std::printf("%d %d %d\n", i, j, k);
// When All - Concurent Async:2 ends here

// [[file:../../../async_control.org::*When All - Concurent Async][When All - Concurent Async:3]]
}
// When All - Concurent Async:3 ends here
