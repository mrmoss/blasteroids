#include <iostream>
#include "msl/json.hpp"
#include "msl/time.hpp"
#include "msl/xmlhttp.hpp"

std::string database_address="127.0.0.1:8080";
std::string json_get_request="/json_get/";
std::string json_set_request="/json_set/";
msl::json simulation;

void print_request(const std::string& host,const std::string& request)
{
	std::cout<<"Requesting \""<<request<<"\" from database at "<<host<<"."<<std::endl;

	auto ret=msl::get_request(host,request);

	std::cout<<"Received:"<<std::endl;

	std::cout<<"  proto:  ["<<ret.proto<<"]"<<std::endl;
	std::cout<<"  pairs:"<<std::endl;

	for(auto ii:ret.pairs)
		std::cout<<"    ["<<ii.first<<"]:["<<ii.second<<"]"<<std::endl;

	std::cout<<"  data:"<<std::endl;
	std::cout<<"  ["<<ret.data<<"]"<<std::endl;
}

int main()
{
	simulation["asteroids"]=Json::arrayValue;

	print_request(database_address,json_get_request);

	auto setting=json_set_request+msl::serialize(simulation);
	print_request(database_address,setting);

	print_request(database_address,json_get_request);

	return 0;
}