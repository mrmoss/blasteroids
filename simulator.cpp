#include <iostream>
#include <random>
#include "msl/json.hpp"
#include "msl/time.hpp"
#include "msl/xmlhttp.hpp"

std::string database_address="127.0.0.1:8080";
std::string json_get_request="/json_get/";
std::string json_set_request="/json_set/";
msl::json simulation;

void print_request(const std::string& host,const std::string& request);
double random_double(const double min,const double max);
void simulation_initialize();
void simulation_update();

int main()
{
	simulation_initialize();

	while(true)
	{
		auto setting=json_set_request+msl::serialize(simulation);
		print_request(database_address,setting);
		simulation_update();
		msl::delay_ms(20);
	}

	//print_request(database_address,json_get_request);

	return 0;
}

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

double random_double(const double min,const double max)
{
	std::uniform_real_distribution<double> uniform(min,max);
	std::random_device rand_dev;
	std::mt19937 rand_engine(rand_dev());
	return uniform(rand_engine);
}

void simulation_initialize()
{
	simulation["asteroids"]=Json::arrayValue;

	for(int ii=0;ii<10;++ii)
	{
		simulation["asteroids"][ii]["pos"]["x"]=random_double(-1000,1000);
		simulation["asteroids"][ii]["pos"]["y"]=random_double(-1000,1000);
		simulation["asteroids"][ii]["speed"]["x"]=random_double(-1,1);
		simulation["asteroids"][ii]["speed"]["y"]=random_double(-1,1);
	}
}

void simulation_update()
{
	for(auto& asteroid:simulation["asteroids"])
	{
		asteroid["pos"]["x"]=asteroid["pos"]["x"].asDouble()+asteroid["speed"]["x"].asDouble();
		asteroid["pos"]["y"]=asteroid["pos"]["y"].asDouble()+asteroid["speed"]["x"].asDouble();
	}
}