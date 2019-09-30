cmake_minimum_required(VERSION 3.5)

project(cmake_server)

set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -Wall")

include_directories("${PROJECT_SOURCE_DIR}")

file(GLOB
	 CPP_MAIN "${PROJECT_SOURCE_DIR}/*.cpp"
	 CPP_SRC "${PROJECT_SOURCE_DIR}/src/*.cpp")

include_directories("${PROJECT_SOURCE_DIR}/include")

add_executable( cmake_server ${CPP_MAIN} ${CPP_SRC} )
