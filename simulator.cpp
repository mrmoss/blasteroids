#include <iostream>
#include "msl/json.hpp"
#include "msl/socket.hpp"
#include "msl/time.hpp"

std::string database_address="127.0.0.1:8080";
std::string json_get_request="/json_get/";
std::string json_set_request="/json_set/";
msl::json simulation;

std::string get_request(const std::string& host,const std::string& name);

int main()
{
	simulation["asteroids"]=Json::arrayValue;

	std::cout<<"Requesting \""<<json_get_request<<"\" from database at "<<database_address<<"."<<std::endl;
	std::cout<<"Received:  "<<get_request(database_address,json_get_request)<<std::endl;

	auto setting=json_set_request+msl::serialize(simulation);
	std::cout<<"Requesting \""<<setting<<"\" from database at "<<database_address<<"."<<std::endl;
	std::cout<<"Received:  "<<get_request(database_address,setting)<<std::endl;

	std::cout<<"Requesting \""<<json_get_request<<"\" from database at "<<database_address<<"."<<std::endl;
	std::cout<<"Received:  "<<get_request(database_address,json_get_request)<<std::endl;

	return 0;
}

std::string get_request(const std::string& host,const std::string& request)
{
	msl::tcp_socket_t client("0.0.0.0:0>"+host);
	client.open();

	if(!client.good())
		return "";

	std::string request_header="GET "+request+" HTTP/1.1\r\n";
	request_header+="Connection: close\r\n";
	request_header+="\r\n";
	client.write(request_header);

	std::string response="";
	uint8_t temp;

	while(client.available()>=0&&client.read(&temp,1)==1)
		response+=temp;

	client.close();

	return response;
}