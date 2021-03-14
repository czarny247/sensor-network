# Sensor-Network

## NOTICE: It has been only tested on MacOS (macOS Catalina 10.15.7)

## Setup

### On MacOS

1. Run ./scripts/mac_setup_dependencies.sh (if it won't work, then you probably must chmod +x /scripts/mac_setup_dependencies.sh first)

2. Build: mkdir build && cd build && cmake ..
3. Run (in separate terminals):
	* Sensors:
		* ./sensor-service --service=cpu-temp --addr=0.0.0.0:50051
		* ./sensor-service --service=cpu-usage --addr=0.0.0.0:50052
	* Platform:
		* ./platform-service

	Or alternatively use scripts/mac_run_network.sh

### On Linux

1. Install needed prerequisites: [protobuf](https://grpc.io/docs/protoc-installation/) 
	and [grpc](https://grpc.io/blog/installation/). Since it is a C++ project, 
	please follow the installation guide for this language.

...And rest of steps are similar to MacOS