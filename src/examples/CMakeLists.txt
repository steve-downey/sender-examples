include(GNUInstallDirs)

add_executable(main "")

target_sources(
  main
  PRIVATE
  main.cpp)

target_link_libraries(main scratch)

add_executable(hello "")

target_sources(
  hello
  PRIVATE
  hello.cpp)

target_link_libraries(hello PRIVATE stdexec)

add_executable(concurent "")

target_sources(
  concurent
  PRIVATE
  concurent.cpp)

target_link_libraries(concurent PRIVATE stdexec)

add_executable(let_value "")

target_sources(
  let_value
  PRIVATE
  let_value.cpp)

target_link_libraries(let_value PRIVATE stdexec)

add_executable(decision "")

target_sources(
  decision
  PRIVATE
  decision.cpp)

target_link_libraries(decision PRIVATE stdexec)

add_executable(factorial "")

target_sources(
  factorial
  PRIVATE
  factorial.cpp)

target_link_libraries(factorial PRIVATE stdexec)

add_executable(fibonacci "")

target_sources(
  fibonacci
  PRIVATE
  fibonacci.cpp)

target_link_libraries(fibonacci PRIVATE stdexec)

add_executable(fold "")

target_sources(
  fold
  PRIVATE
  fold.cpp)

target_link_libraries(fold PRIVATE stdexec)

install(
  TARGETS main hello concurent let_value decision factorial fibonacci
  DESTINATION ${CMAKE_INSTALL_BINDIR}
  )

add_executable(backtrack "")

target_sources(
  backtrack
  PRIVATE
  backtrack.cpp)

target_link_libraries(backtrack PRIVATE stdexec tree)

add_executable(loop "")

target_sources(
  loop
  PRIVATE
  loop.cpp)

target_link_libraries(loop PRIVATE stdexec)


install(
  TARGETS
  main
  hello
  concurent
  let_value
  decision
  factorial
  fibonacci
  fold
  backtrack
  loop
  DESTINATION ${CMAKE_INSTALL_BINDIR}
  )


## Tests
add_executable(async_test "")

target_sources(
  async_test
  PRIVATE
  async.t.cpp)

target_link_libraries(async_test stdexec)
target_link_libraries(async_test gtest)
target_link_libraries(async_test gtest_main)

include(GoogleTest)
gtest_discover_tests(async_test)
