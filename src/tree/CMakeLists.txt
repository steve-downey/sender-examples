add_library(tree STATIC "")

target_sources(
  tree
  PRIVATE
  tree.cpp)

include(GNUInstallDirs)

target_include_directories(tree PUBLIC
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/../>
  $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}/${CMAKE_LOWER_PROJECT_NAME}>  # <prefix>/include/tree
  )

install(
  TARGETS tree
  EXPORT ${TARGETS_EXPORT_NAME}
  DESTINATION ${CMAKE_INSTALL_LIBDIR}
  )

string(TOLOWER ${CMAKE_PROJECT_NAME} CMAKE_LOWER_PROJECT_NAME)

install(
  DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/
  DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/${CMAKE_LOWER_PROJECT_NAME}
  FILES_MATCHING PATTERN "*.h"
  )


## Tests
add_executable(tree_test "")

target_sources(
  tree_test
  PRIVATE
  tree.t.cpp)

target_link_libraries(tree_test tree)
target_link_libraries(tree_test gtest)
target_link_libraries(tree_test gtest_main)

include(GoogleTest)
gtest_discover_tests(tree_test)
