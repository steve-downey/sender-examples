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

install(
  TARGETS main hello
  DESTINATION ${CMAKE_INSTALL_BINDIR}
  )
