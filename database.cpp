#include <fstream>
#include <iostream>
#include <mutex>
#include <string>
#include <vector>
#include "msl/json.hpp"
#include "msl/string.hpp"
#include "msl/time.hpp"
#include "msl/webserver.hpp"

std::string host_address="0.0.0.0:8080";
std::string host_webroot="web";
std::mutex database_lock;
msl::json database;
std::string backup_filename="database.json";
size_t backup_interval_ms=5000;
size_t restart_interval_ms=1000;

bool client_func(const mg_connection& connection,enum mg_event event);
std::vector<std::string> get_paths(std::string str);

int main()
{
	while(true)
	{
		msl::webserver_t server(client_func,host_address,host_webroot);
		server.open();

		std::cout<<"Attempting to host database on address "<<server.address()<<
			" from folder \""<<server.webroot()<<"\"."<<std::endl;

		if(!server.good())
		{
			std::cout<<"Error starting database."<<std::endl;
			msl::delay_ms(restart_interval_ms);
			continue;
		}

		std::cout<<"Database started."<<std::endl;

		while(server.good())
		{
			msl::delay_ms(backup_interval_ms);

			std::ofstream backup(backup_filename);

			database_lock.lock();
			auto database_copy=database;
			database_lock.unlock();

			if(!(backup<<msl::serialize(database_copy)))
			{
				std::cout<<"Could not write backup file named \""<<backup_filename<<"\"."<<std::endl;
				backup.close();
				continue;
			}

			std::cout<<"Wrote backup file named \""<<backup_filename<<"\"."<<std::endl;

			backup.close();
		}

		std::cout<<"Database stopped."<<std::endl;
	}

	return 0;
}

bool client_func(const mg_connection& connection,enum mg_event event)
{
	if(event==MG_AUTH)
		return true;
	if(event!=MG_REQUEST)
		return false;

	std::string request=connection.uri;
	std::string json_set_prefix="/json_set/";
	std::string json_get_prefix="/json_get";

	std::cout<<"Request:  "<<request<<std::endl;

	try
	{
		if(msl::starts_with(request,json_set_prefix))
		{
			std::cout<<"  Set JSON Request."<<std::endl;

			std::string json_set_str=request.substr(json_set_prefix.size(),
				request.size()-json_set_prefix.size());
			msl::json json=msl::deserialize(json_set_str);

			std::cout<<"  Parsing JSON Object:"<<std::endl;

			for(auto ii:json.getMemberNames())
			{
				switch(json[ii].type())
				{
					case Json::arrayValue:
						std::cout<<"    Adding \""<<ii<<"\" as array."<<std::endl;
						break;
					case Json::objectValue:
						std::cout<<"    Adding \""<<ii<<"\" as object."<<std::endl;
						break;
					case Json::nullValue:
						std::cout<<"    Adding \""<<ii<<"\" as null."<<std::endl;
						break;
					case Json::intValue:
						std::cout<<"    Adding \""<<ii<<"\" as int."<<std::endl;
						break;
					case Json::uintValue:
						std::cout<<"    Adding \""<<ii<<"\" as uint."<<std::endl;
						break;
					case Json::realValue:
						std::cout<<"    Adding \""<<ii<<"\" as real."<<std::endl;
						break;
					case Json::stringValue:
						std::cout<<"    Adding \""<<ii<<"\" as string."<<std::endl;
						break;
					case Json::booleanValue:
						std::cout<<"    Adding \""<<ii<<"\" as bool."<<std::endl;
						break;
					default:
						std::cout<<"    Skipping \""<<ii<<"\" with invalid type."<<std::endl;
						continue;
						break;
				}

				database_lock.lock();
				database[ii]=json[ii];
				database_lock.unlock();
			}

			return true;
		}
		else if(msl::starts_with(request,json_get_prefix))
		{
			std::cout<<"  Get JSON Request."<<std::endl;

			std::string json_get_str=request.substr(json_get_prefix.size(),
				request.size()-json_get_prefix.size());
			std::vector<std::string> paths=get_paths(json_get_str);

			std::cout<<"  Parsing JSON Path:"<<std::endl;

			database_lock.lock();
			auto json_get_object=database;
			database_lock.unlock();

			for(auto path:paths)
			{
				json_get_object=json_get_object[path];

				switch(json_get_object.type())
				{
					case Json::arrayValue:
						std::cout<<"    Getting \""<<path<<"\" as array."<<std::endl;
						break;
					case Json::objectValue:
						std::cout<<"    Getting \""<<path<<"\" as object."<<std::endl;
						break;
					case Json::nullValue:
						std::cout<<"    Getting \""<<path<<"\" as null."<<std::endl;
						break;
					case Json::intValue:
						std::cout<<"    Getting \""<<path<<"\" as int."<<std::endl;
						break;
					case Json::uintValue:
						std::cout<<"    Getting \""<<path<<"\" as uint."<<std::endl;
						break;
					case Json::realValue:
						std::cout<<"    Getting \""<<path<<"\" as real."<<std::endl;
						break;
					case Json::stringValue:
						std::cout<<"    Getting \""<<path<<"\" as string."<<std::endl;
						break;
					case Json::booleanValue:
						std::cout<<"    Getting \""<<path<<"\" as bool."<<std::endl;
						break;
					default:
						std::cout<<"    Exiting \""<<path<<"\" with invalid type."<<std::endl;
						return false;
				}
			}

			msl::client_reply(connection,msl::serialize(json_get_object),"application/javascript");
			return true;
		}
	}
	catch(std::exception& e)
	{
		std::cout<<"  Error:  "<<e.what()<<std::endl;
	}
	catch(...)
	{
		std::cout<<"  Error:  Unknown error occured."<<std::endl;
	}

	return false;
}

std::vector<std::string> get_paths(std::string str)
{
	str+='/';
	std::vector<std::string> paths;
	std::string temp="";

	for(size_t ii=0;ii<str.size();++ii)
	{
		if(str[ii]=='/')
		{
			if(temp.size()>0)
				paths.push_back(temp);

			temp="";
		}
		else
		{
			temp+=str[ii];
		}
	}

	return paths;
}