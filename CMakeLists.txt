cmake_minimum_required(VERSION 3.0)
project(ocv_extract_pngs)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_HOME_DIRECTORY}/bin)

find_package(OpenCV REQUIRED)
find_package(Boost 1.58 COMPONENTS filesystem REQUIRED)
find_package(Threads)

include_directories(${Boost_INCLUDE_DIR})

set(SOURCE_FILES src/main.cpp)

add_executable(ocv_extract_pngs ${SOURCE_FILES})

target_link_libraries(ocv_extract_pngs ${CMAKE_THREAD_LIBS_INIT} ${Boost_LIBRARIES} ${OpenCV_LIBS})

