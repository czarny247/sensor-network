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

if (${CMAKE_SYSTEM_NAME} MATCHES "Darwin")
	set(TEMPERATURE_COMMAND "osx-cpu-temp")
endif()

if (${CMAKE_SYSTEM_NAME} MATCHES "Linux")
	set(TEMPERATURE_COMMAND "${CMAKE_SOURCE_DIR}/scripts/linux_temp.sh")
endif()

configure_file(sensor/src/SystemCommands.hpp.in sensor/src/SystemCommands.hpp)

add_executable(platform-service 
	${CMAKE_SOURCE_DIR}/platform/src/main.cpp
	${PROJECT_BINARY_DIR}/CpuTempService.pb.cc
	${PROJECT_BINARY_DIR}/CpuTempService.grpc.pb.cc
	${PROJECT_BINARY_DIR}/CpuUsageService.pb.cc
	${PROJECT_BINARY_DIR}/CpuUsageService.grpc.pb.cc)

target_include_directories(platform-service
	PRIVATE
	    ${Protobuf_INCLUDE_DIRS}
		${CMAKE_SOURCE_DIR}/platform/src
		${PROJECT_BINARY_DIR})
target_link_libraries(platform-service
	gRPC::grpc++
	gRPC::grpc++_reflection
    protobuf::libprotobuf)

add_executable(sensor-service
	${CMAKE_SOURCE_DIR}/sensor/src/main.cpp
	${PROJECT_BINARY_DIR}/CpuTempService.pb.cc
	${PROJECT_BINARY_DIR}/CpuTempService.grpc.pb.cc
	${PROJECT_BINARY_DIR}/CpuUsageService.pb.cc
	${PROJECT_BINARY_DIR}/CpuUsageService.grpc.pb.cc)

target_include_directories(sensor-service
	PRIVATE
		${Protobuf_INCLUDE_DIRS}
		${CMAKE_SOURCE_DIR}/sensor/src
		${PROJECT_BINARY_DIR}
		${PROJECT_BINARY_DIR}/sensor/src)
target_link_libraries(sensor-service
	gRPC::grpc++
	gRPC::grpc++_reflection
    protobuf::libprotobuf)