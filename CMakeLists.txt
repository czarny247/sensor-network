cmake_minimum_required(VERSION 3.7 FATAL_ERROR)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

project(sensors-xberry)

find_package(gRPC CONFIG REQUIRED)
find_package(Protobuf REQUIRED)

execute_process(COMMAND
	protoc -I ${CMAKE_SOURCE_DIR}/proto 
		--grpc_out=. 
		--plugin=protoc-gen-grpc=/usr/local/Cellar/grpc/1.36.2/bin/grpc_cpp_plugin 
		${CMAKE_SOURCE_DIR}/proto/CpuTempService.proto
		${CMAKE_SOURCE_DIR}/proto/CpuUsageService.proto)

execute_process(COMMAND
	protoc -I ${CMAKE_SOURCE_DIR}/proto 
		--cpp_out=. 
		${CMAKE_SOURCE_DIR}/proto/CpuTempService.proto
		${CMAKE_SOURCE_DIR}/proto/CpuUsageService.proto)

add_executable(sensors-xberry-platform 
	${CMAKE_SOURCE_DIR}/platform/src/main.cpp
	${PROJECT_BINARY_DIR}/CpuTempService.pb.cc
	${PROJECT_BINARY_DIR}/CpuTempService.grpc.pb.cc
	${PROJECT_BINARY_DIR}/CpuUsageService.pb.cc
	${PROJECT_BINARY_DIR}/CpuUsageService.grpc.pb.cc)

target_include_directories(sensors-xberry-platform 
	PRIVATE
	    ${Protobuf_INCLUDE_DIRS}
		${CMAKE_SOURCE_DIR}/platform/src
		${PROJECT_BINARY_DIR})
target_link_libraries(sensors-xberry-platform
	gRPC::grpc++
	gRPC::grpc++_reflection
    protobuf::libprotobuf)

add_executable(sensors-xberry-sensor 
	${CMAKE_SOURCE_DIR}/sensor/src/main.cpp
	${PROJECT_BINARY_DIR}/CpuTempService.pb.cc
	${PROJECT_BINARY_DIR}/CpuTempService.grpc.pb.cc
	${PROJECT_BINARY_DIR}/CpuUsageService.pb.cc
	${PROJECT_BINARY_DIR}/CpuUsageService.grpc.pb.cc)

target_include_directories(sensors-xberry-sensor 
	PRIVATE
		${Protobuf_INCLUDE_DIRS}
		${CMAKE_SOURCE_DIR}/sensor/src
		${PROJECT_BINARY_DIR})
target_link_libraries(sensors-xberry-sensor 
	gRPC::grpc++
	gRPC::grpc++_reflection
    protobuf::libprotobuf)

add_executable(sensors-xberry-client 
	${CMAKE_SOURCE_DIR}/client/src/main.cpp
	${PROJECT_BINARY_DIR}/CpuTempService.pb.cc
	${PROJECT_BINARY_DIR}/CpuTempService.grpc.pb.cc
	${PROJECT_BINARY_DIR}/CpuUsageService.pb.cc
	${PROJECT_BINARY_DIR}/CpuUsageService.grpc.pb.cc)

target_include_directories(sensors-xberry-client 
	PRIVATE
		${Protobuf_INCLUDE_DIRS}
		${CMAKE_SOURCE_DIR}/client/src
		${PROJECT_BINARY_DIR})
target_link_libraries(sensors-xberry-client
	gRPC::grpc++
	gRPC::grpc++_reflection
    protobuf::libprotobuf)

#add installation on mac - brew install osx-cpu-temp
#to use on linux / bsd - ?
#brew install protobuf - already in grpc
#brew install grpc
#find path to grpc plugin
#add configuration file to set cmd based on host os

