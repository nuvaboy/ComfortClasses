cmake_minimum_required(VERSION 3.7)
project(ComfortClasses VERSION 1.0.1)

# Download and unpack googletest at configure time
configure_file(CMakeLists.txt.in googletest-download/CMakeLists.txt)
execute_process(COMMAND ${CMAKE_COMMAND} -G "${CMAKE_GENERATOR}" .
  RESULT_VARIABLE result
  WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/googletest-download )
if(result)
  message(FATAL_ERROR "CMake step for googletest failed: ${result}")
endif()
execute_process(COMMAND ${CMAKE_COMMAND} --build .
  RESULT_VARIABLE result
  WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/googletest-download )
if(result)
  message(FATAL_ERROR "Build step for googletest failed: ${result}")
endif()

# Prevent overriding the parent project's compiler/linker
# settings on Windows
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)

# Add googletest directly to our build. This defines
# the gtest and gtest_main targets.
add_subdirectory(${CMAKE_CURRENT_BINARY_DIR}/googletest-src
                 ${CMAKE_CURRENT_BINARY_DIR}/googletest-build
                 EXCLUDE_FROM_ALL)

# The gtest/gtest_main targets carry header search path
# dependencies automatically when using CMake 2.8.11 or
# later. Otherwise we have to add them here ourselves.
if (CMAKE_VERSION VERSION_LESS 2.8.11)
  include_directories("${gtest_SOURCE_DIR}/include")
endif()

# Now simply link against gtest or gtest_main as needed. Eg
#add_executable(example example.cpp)
#target_link_libraries(example gtest_main)
#add_test(NAME example_test COMMAND example)

add_subdirectory(src)

add_executable(demo demo/CCStringDemo.cpp)
add_dependencies(demo ComfortClasses)
target_link_libraries(demo ComfortClasses)

include(CTest)
enable_testing()

add_executable(testCCString test/CCStringTest.cpp)
add_dependencies(testCCString ComfortClasses gtest_main)
target_link_libraries(testCCString ComfortClasses gtest_main)
add_test(NAME test_CCString COMMAND testCCString)

add_executable(testCCDecimal test/CCDecimalTest.cpp)
add_dependencies(testCCDecimal ComfortClasses gtest_main)
target_link_libraries(testCCDecimal ComfortClasses gtest_main)
add_test(NAME test_CCDecimal COMMAND testCCDecimal)


#set(CPACK_PROJECT_NAME ${PROJECT_NAME})
#set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
#include(CPack)
