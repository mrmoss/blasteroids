#include <iostream>
#include "msl/time.hpp"
#include "msl/webserver.hpp"

bool client_func(const mg_connection& connection,enum mg_event event)
{
	if(event==MG_AUTH)
		return true;
	if(event!=MG_REQUEST)
		return false;

	std::cout<<connection.uri<<std::endl;
	return false;
}

int main()
{
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