#include <iostream>
#include <memory>
#include <string>
#include <chrono>
#include <thread>
#include <vector>

#include <grpcpp/grpcpp.h>

#include "CpuTempService.grpc.pb.h"
#include "CpuUsageService.grpc.pb.h"

using grpc::Channel;
using grpc::ClientContext;
using grpc::Status;

class Platform 
{
public:
	Platform(std::vector<std::shared_ptr<Channel> > channels)
		: cpuTempServiceStub_(CpuTempService::NewStub(channels[0])) 
		, cpuUsageServiceStub_(CpuUsageService::NewStub(channels[1]))
	{}

	std::string getCpuTemp()
	{
		CpuTempReq req;
		CpuTempResp resp;
		ClientContext context;

		Status status = cpuTempServiceStub_->cpuTemp(&context, req, &resp);

    	if (status.ok()) {
    	  return resp.cputempdata();
    	} 
    	else 
    	{
    	  std::cout << status.error_code() << ": " << status.error_message()
    	            << std::endl;
    	  return "RPC failed";
    	}
	}

	std::string getCpuUsage()
	{
		CpuUsageReq req;
		CpuUsageResp resp;
		ClientContext context;

		Status status = cpuUsageServiceStub_->cpuUsage(&context, req, &resp);

    	if (status.ok()) {
    	  return resp.cpuusagedata();
    	} 
    	else 
    	{
    	  std::cout << status.error_code() << ": " << status.error_message()
    	            << std::endl;
    	  return "RPC failed";
    	}
	}

private:
	//add another node
	//make spawning the processes from platform
  	std::unique_ptr<CpuTempService::Stub> cpuTempServiceStub_;
  	std::unique_ptr<CpuUsageService::Stub> cpuUsageServiceStub_;
};

int main(int argc, char** argv) 
{

  	Platform pl({
  		grpc::CreateChannel("localhost:50051", grpc::InsecureChannelCredentials()),
  		grpc::CreateChannel("localhost:50052", grpc::InsecureChannelCredentials())
  		});
  	while(true)
  	{
		std::string resp = pl.getCpuTemp();
		std::cout << "Platform received: " << resp << std::endl;

		//make it on separate thread
		resp = pl.getCpuUsage();
		std::cout << "Platform received: " << resp << std::endl;

		std::this_thread::sleep_for(std::chrono::milliseconds(600));
  	}
	return 0;
}