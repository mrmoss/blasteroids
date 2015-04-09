#include <iostream>
#include "msl/json.hpp"
#include "msl/time.hpp"
#include "msl/webserver.hpp"

bool client_func(const mg_connection& connection,enum mg_event event)
{
	if(event==MG_AUTH)
		return true;
	if(event!=MG_REQUEST)
		return false;

	try
	{
		std::cout<<connection.uri<<std::endl;
	}
	catch(std::exception& e)
	{
		std::cout<<"\terror:  "<<e.what()<<std::endl;
	}
	catch(...)
	{
		std::cout<<"\terror:  Unknown error occured."<<std::endl;
	}

	return false;
}

int main()
{
	auto json=msl::string_to_json("{\"project\":\"rapidjson\",\"stars\":10}");
	json["stars"].SetInt(json["stars"].GetInt()+1);
	std::cout<<msl::json_to_string(json)<<std::endl;

	while(true)
	{
		msl::webserver_t server(client_func,"0.0.0.0:8080","web");

		server.open();

		if(!server.good())
		{
			std::cout<<":("<<std::endl;
			return 0;
		}

		std::cout<<":)"<<std::endl;

		while(server.good())
			msl::delay_ms(10);

		std::cout<<"T_T"<<std::endl;
	}

	return 0;
}