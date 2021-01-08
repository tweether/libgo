cmake_minimum_required(VERSION 2.8)

set(projName channel)

if(BUILD_DYNAMIC)
    set(libgo_lib ${SHARED_T})
else()
    set(libgo_lib ${STATIC_T})
endif()

add_executable(${projName} main.cc)
add_dependencies(${projName} ${libgo_lib})
target_include_directories(${projName} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/../libgo)
target_link_libraries(${projName} PRIVATE ${libgo_lib} pthread)
