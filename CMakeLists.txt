cmake_minimum_required(VERSION 3.25.1)

project(PrintIp VERSION 0.0.1)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wno-long-long -pedantic")

add_executable(print_ip main.cpp)

target_include_directories(
    print_ip
    PRIVATE
    ${CMAKE_CURRENT_BINARY_DIR}
)


set_target_properties(print_ip PROPERTIES
    CXX_STANDARD 17
    CXX_STANDARD_REQUIRED ON
)

FIND_PACKAGE(Doxygen)

IF(DOXYGEN_FOUND)
    SET(DOXYGEN_INPUT ${CMAKE_SOURCE_DIR})
    CONFIGURE_FILE(${CMAKE_SOURCE_DIR}/doxygen.conf.in ${CMAKE_SOURCE_DIR}/Doxyfile)
    ADD_CUSTOM_TARGET(doc COMMAND ${DOXYGEN_EXECUTABLE} ${CMAKE_SOURCE_DIR}/Doxyfile)
ELSE(DOXYGEN_FOUND)    
    MESSAGE(STATUS "WARNING: Doxygen not found - Reference manual will not be created")
ENDIF(DOXYGEN_FOUND)
