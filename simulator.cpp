#include <iostream>
#include "msl/json.hpp"
#include "msl/socket.hpp"
#include "msl/time.hpp"

std::string get_request(const std::string& host,const std::string& name);

int main()
{
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