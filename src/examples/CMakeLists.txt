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

install(
  TARGETS main hello concurent let_value
  DESTINATION ${CMAKE_INSTALL_BINDIR}
  )
