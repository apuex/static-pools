cmake_minimum_required(VERSION 2.8...3.20)

project(static-pools CXX)

option(ENABLE_TEST "Enable building tests" ON)
option(ENABLE_VALGRIND "Enable the valgrind memory checker for the tests." OFF)

if(MSVC)
ADD_DEFINITIONS(-D_WIN32_WINNT=0x0502)
ADD_DEFINITIONS(-D_CRT_SECURE_NO_WARNINGS)
ADD_DEFINITIONS(-D_UNICODE)
ADD_DEFINITIONS(-DUNICODE)
endif()

include(CheckCXXCompilerFlag)
if(("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang") OR ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU"))
  list(APPEND CUSTOM_CXX_COMPILER_FLAGS
    -pedantic
    -fPIC
    )
elseif("${CMAKE_CXX_COMPILER_ID}" STREQUAL "MSVC")
  list(APPEND CUSTOM_CXX_COMPILER_FLAGS
    /EHsc
    /MD
    /utf-8
    )
endif()

foreach(CXX_COMPILER_FLAG ${CUSTOM_CXX_COMPILER_FLAGS})
  # remove problematic characters
  string(REGEX REPLACE "[^a-zA-Z0-9]" "" current_variable ${CXX_COMPILER_FLAG})
  CHECK_CXX_COMPILER_FLAG(${CXX_COMPILER_FLAG} "FLAG_SUPPORTED_${current_variable}")
  if(FLAG_SUPPORTED_${current_variable})
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${CXX_COMPILER_FLAG}")
  endif()
endforeach()

include(CheckIncludeFileCXX)
CHECK_INCLUDE_FILE_CXX(cstdint  HAVE_CSTDINT)
if(HAVE_CSTDINT)
  ADD_DEFINITIONS(-DHAVE_CSTDINT=1)
endif()

include_directories(
  include
  src 
  ${INCLUDE_DIRECTORIES}
  ${CMAKE_INSTALL_PREFIX}/include
)

link_directories(
  ${LINK_DIRECTORIES}
  ${CMAKE_INSTALL_PREFIX}/lib
)

set(unit_tests
  allocator-binary-tree-test
  ring-buffer-test
)

if(ENABLE_TEST)
  include(CTest)
  enable_testing()

  #"check" target that automatically builds everything and runs the tests
  add_custom_target(check
    COMMAND ${CMAKE_CTEST_COMMAND} --output-on-failure
    DEPENDS ${unit_tests}
    )
endif()

if (ENABLE_VALGRIND)
  find_program(MEMORYCHECK_COMMAND valgrind)
  if ("${MEMORYCHECK_COMMAND}" MATCHES "MEMORYCHECK_COMMAND-NOTFOUND")
    message(WARNING "Valgrind couldn't be found.")
    unset(MEMORYCHECK_COMMAND)
  else()
    set(MEMORYCHECK_COMMAND_OPTIONS --trace-children=yes --leak-check=full)
  endif()
endif()

foreach(unit_test ${unit_tests})
  add_executable("${unit_test}" "tests/${unit_test}.cpp")
  target_link_libraries("${unit_test}")
  if(MEMORYCHECK_COMMAND)
    add_test(NAME "${unit_test}"
      COMMAND "${MEMORYCHECK_COMMAND}" ${MEMORYCHECK_COMMAND_OPTIONS} "${CMAKE_CURRENT_BINARY_DIR}/${unit_test}")
  else()
    add_test(NAME "${unit_test}"
      COMMAND "./${unit_test}")
  endif()
endforeach()

add_dependencies(check ${unit_tests})

