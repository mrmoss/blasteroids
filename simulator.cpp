#include <iostream>
#include <sstream>
#include <utility>
#include <vector>
#include "msl/json.hpp"
#include "msl/socket.hpp"
#include "msl/time.hpp"

#include "msl/mongoose/mongoose.h"

std::string database_address="127.0.0.1:8080";
std::string json_get_request="/json_get/";
std::string json_set_request="/json_set/";
msl::json simulation;

struct http_response
{
	std::string proto;
	std::vector<std::pair<std::string,std::string>> pairs;
	std::string data;
};

http_response parse_http_response(std::string response);
http_response get_request(const std::string& host,const std::string& name);

void print_request(const std::string& host,const std::string& request)
{
	std::cout<<"Requesting \""<<request<<"\" from database at "<<host<<"."<<std::endl;

	auto ret=get_request(host,request);

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

http_response get_request(const std::string& host,const std::string& request)
{
	http_response ret{"",{},""};
	msl::tcp_socket_t client("0.0.0.0:0>"+host);
	client.open();

	if(!client.good())
		return ret;

	std::string request_header="GET "+request+" HTTP/1.1\r\n";
	request_header+="Connection: close\r\n";
	request_header+="\r\n";
	client.write(request_header);

	std::string response="";
	uint8_t temp;

	while(client.available()>=0&&client.read(&temp,1)==1)
		response+=temp;

	client.close();

	return parse_http_response(response);
}

http_response parse_http_response(std::string response)
{
	http_response ret{"",{},""};
	std::string line;
	std::vector<std::string> lines;
	size_t data_size=0;

	while(response.size()>0)
	{
		if(response.size()==1)
		{
			line+=response[0];
			response.erase(0,1);
			lines.push_back(line);
			line="";
		}
		else if(response[0]=='\r'&&response.size()>1&&response[1]=='\n')
		{
			lines.push_back(line+"\r\n");
			response.erase(0,2);
			line="";
		}
		else
		{
			line+=response[0];
			response.erase(0,1);
		}
	}

	if(lines.size()>0)
	{
		if(lines[0].size()>1)
			lines[0]=lines[0].substr(0,lines[0].size()-2);

		ret.proto=lines[0];
		lines.erase(lines.begin());

		while(lines.size()>0&&lines[0]!="\r\n")
		{
			if(lines[0].size()>1)
				lines[0]=lines[0].substr(0,lines[0].size()-2);

			std::string key;

			while(lines[0].size()>0&&lines[0][0]!=':')
			{
				key+=lines[0][0];
				lines[0].erase(0,1);
			}

			if(lines[0].size()>0&&lines[0][0]==':')
				lines[0].erase(0,1);

			while(lines[0].size()>0&&isspace(lines[0][0])!=0)
				lines[0].erase(0,1);

			std::string value=lines[0];

			ret.pairs.push_back({key,value});

			if(key=="Content-Length")
			{
				bool is_number=true;

				for(auto ii:value)
				{
					if(isdigit(ii)==0)
					{
						is_number=false;
						break;
					}
				}

				if(is_number)
					data_size=std::stoi(value);
			}

			lines.erase(lines.begin());
		}

		if(lines[0].size()>0&&lines[0]=="\r\n")
			lines.erase(lines.begin());

		while(lines.size()>0)
		{
			while(lines[0].size()>0&&ret.data.size()<data_size)
			{
				ret.data+=lines[0][0];
				lines[0].erase(0,1);
			}

			lines.erase(lines.begin());

			if(ret.data.size()>=data_size)
				break;
		}
	}

	return ret;
}