include_guard(GLOBAL)

set(CMAKE_C_COMPILER gcc-10)
set(CMAKE_CXX_COMPILER g++-10)

set(CMAKE_CXX_STANDARD 20 CACHE STRING "C++ standard.")
set(CXX_STANDARD_REQUIRED 20 CACHE STRING "C++ standard.")

set(CMAKE_CXX_FLAGS
  "-Wall -Wextra "
CACHE STRING "CXX_FLAGS" FORCE)

set(CMAKE_CXX_FLAGS_DEBUG "-O0 -fno-inline -g3" CACHE STRING "C++ DEBUG Flags" FORCE)
set(CMAKE_CXX_FLAGS_RELEASE "-Ofast -g0 -DNDEBUG" CACHE STRING "C++ Release Flags" FORCE)
set(CMAKE_CXX_FLAGS_RELWITHDEBINFO "-O3 -g -DNDEBUG" CACHE STRING "C++ RelWithDebInfo Flags" FORCE)
set(CMAKE_CXX_FLAGS_TSAN "-O3 -g -DNDEBUG -fsanitize=thread" CACHE STRING "C++ TSAN Flags" FORCE)
set(CMAKE_CXX_FLAGS_ASAN "-O3 -g -DNDEBUG -fsanitize=address -fsanitize=undefined -fsanitize=leak" CACHE STRING "C++ ASAN Flags" FORCE)
