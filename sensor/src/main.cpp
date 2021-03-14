#include "CpuTempService.grpc.pb.h"
#include "CpuUsageService.grpc.pb.h"

#include <iostream>
#include <memory>
#include <string>

#include <grpcpp/grpcpp.h>
#include <grpcpp/health_check_service_interface.h>
#include <grpcpp/ext/proto_server_reflection_plugin.h>

#include <cstdlib>
#include <sstream>
#include <fstream>

#include "SystemCommands.hpp"

using grpc::Server;
using grpc::ServerBuilder;
using grpc::ServerContext;
using grpc::Status;

class CpuTempServiceImpl final : public CpuTempService::Service 
{

Status cpuTemp(ServerContext* context, const CpuTempReq* request, 
	CpuTempResp* response) override
{
	std::string cmd = getTemperatureCommand() + " > tmp.txt";
	std::system(cmd.c_str());

	std::stringstream ss;
	ss << std::ifstream("tmp.txt").rdbuf();
	response->set_cputempdata(ss.str());
	return Status::OK;
}

};

class CpuUsageServiceImpl final : public CpuUsageService::Service 
{

Status cpuUsage(ServerContext* context, const CpuUsageReq* request, 
	CpuUsageResp* response) override
{
	std::system("top -l 1 | grep -E \"^CPU\" > usg.txt");
	std::stringstream ss;
	ss << std::ifstream("usg.txt").rdbuf();
	response->set_cpuusagedata(ss.str());
	return Status::OK;
}

};

void RunService(const std::string& serviceName, const std::string& serviceAddress)
{
	std::unique_ptr<::grpc::Service> service {nullptr};

	//make it smarter
	if(serviceName == "cpu-usage")
	{
		service = std::make_unique<CpuUsageServiceImpl>();
	}
	else if(serviceName == "cpu-temp")
	{
		service = std::make_unique<CpuTempServiceImpl>();
	}
	else
	{
		std::cout << "Unknown service: " << serviceName << ". Aborting.";
		return;
	}

	grpc::EnableDefaultHealthCheckService(true);
	grpc::reflection::InitProtoReflectionServerBuilderPlugin();
	ServerBuilder builder;
	builder.AddListeningPort(serviceAddress, grpc::InsecureServerCredentials());
	builder.RegisterService(service.get());
	std::unique_ptr<Server> server(builder.BuildAndStart());
	std::cout << "Service " << serviceName <<  " listening on " << serviceAddress << std::endl;
	server->Wait();

}

void printHelp()
{
	std::cout 	<< "usage: \n"
				<< "--service=<serviceName>\n"
				<< "\t\tserviceNames: cpu-temp, cpu-usage\n"
				<< "--addr=<serviceAddress>\n"
				<< "\t\te.g. 0.0.0.0:50051\n";
}

int main(int argc, char** argv) 
{
	std::string servicePrefix {"--service"};
	std::string serviceName;

	std::string addressPrefix {"--addr"};
	std::string serviceAddress;

	if (argc > 1)
	{	
		std::string argStr = argv[1];
		if(argStr == "--help" || argStr == "-h")
		{
			printHelp();
			return 0;
		}

		auto startPos = argStr.find(servicePrefix);
		if(startPos != std::string::npos)
		{
			startPos += servicePrefix.size();
			if(argStr[startPos] == '=')
			{
				serviceName = argStr.substr(startPos+1);
			}
		}
		else
		{
			std::cout << "Missing --service argument. Run with --help to check the usage";
		}

		argStr = argv[2];
		startPos = argStr.find(addressPrefix);
		if(startPos != std::string::npos)
		{
			startPos += addressPrefix.size();
			if(argStr[startPos] == '=')
			{
				serviceAddress = argStr.substr(startPos+1);
			}
		}
		else
		{
			std::cout << "Missing --addr argument. Run with --help to check the usage";
		}
	}
	else
	{
		printHelp();
		return 0;
	}

	RunService(serviceName, serviceAddress);
	return 0;
}