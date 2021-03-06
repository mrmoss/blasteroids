#include <iostream>
#include <random>
#include <string>
#include <vector>
#include "msl/json.hpp"
#include "msl/time.hpp"
#include "msl/xmlhttp.hpp"

std::string database_address="127.0.0.1:8080";
std::string json_set_request="/json_set/";
std::vector<std::string> asteroid_sprites
{
	"images/asteroid_small.png",
	"images/asteroid_medium.png",
	"images/asteroid_large.png"
};
msl::json simulation;

void print_request(const std::string& host,const std::string& request,const std::string& data);
int random_int(const int min,const int max);
double random_double(const double min,const double max);
void simulation_initialize();
void simulation_update();

int main()
{
	simulation_initialize();

	while(true)
	{
		print_request(database_address,json_set_request,msl::serialize(simulation));
		simulation_update();
		msl::delay_ms(20);
	}

	return 0;
}

void print_request(const std::string& host,const std::string& request,const std::string& data)
{
	std::cout<<"Requesting \""<<request<<"\" from database at "<<host<<"."<<std::endl;

	auto ret=msl::post_request(host,request,data);

	std::cout<<"Received:"<<std::endl;

	std::cout<<"  proto:  ["<<ret.proto<<"]"<<std::endl;
	std::cout<<"  pairs:"<<std::endl;

	for(auto ii:ret.pairs)
		std::cout<<"    ["<<ii.first<<"]:["<<ii.second<<"]"<<std::endl;

	std::cout<<"  data:"<<std::endl;
	std::cout<<"  ["<<ret.data<<"]"<<std::endl;
}

int random_int(const int min,const int max)
{
	std::uniform_int_distribution<int> uniform(min,max);
	std::random_device rand_dev;
	std::mt19937 rand_engine(rand_dev());
	return uniform(rand_engine);
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

	for(int ii=0;ii<20;++ii)
	{
		simulation["asteroids"][ii]["sprite"]=asteroid_sprites[random_int(0,2)];
		simulation["asteroids"][ii]["pos"]["x"]=random_double(100,200);
		simulation["asteroids"][ii]["pos"]["y"]=random_double(100,200);
		simulation["asteroids"][ii]["pos"]["r"]=random_double(0,3.14159265*2);
		simulation["asteroids"][ii]["speed"]["x"]=random_double(-1,1);
		simulation["asteroids"][ii]["speed"]["y"]=random_double(-1,1);
		simulation["asteroids"][ii]["speed"]["r"]=random_double(-0.2,0.2);
	}
}

void simulation_update()
{
	for(auto& asteroid:simulation["asteroids"])
	{
		asteroid["pos"]["x"]=asteroid["pos"]["x"].asDouble()+asteroid["speed"]["x"].asDouble();
		asteroid["pos"]["y"]=asteroid["pos"]["y"].asDouble()+asteroid["speed"]["y"].asDouble();
		asteroid["pos"]["r"]=asteroid["pos"]["r"].asDouble()+asteroid["speed"]["r"].asDouble();

		while(asteroid["pos"]["x"].asDouble()>480)
			asteroid["pos"]["x"]=asteroid["pos"]["x"].asDouble()-480;
		while(asteroid["pos"]["x"].asDouble()<0)
			asteroid["pos"]["x"]=asteroid["pos"]["x"].asDouble()+480;
		while(asteroid["pos"]["y"].asDouble()>480)
			asteroid["pos"]["y"]=asteroid["pos"]["y"].asDouble()-480;
		while(asteroid["pos"]["y"].asDouble()<0)
			asteroid["pos"]["y"]=asteroid["pos"]["y"].asDouble()+480;
		while(asteroid["pos"]["r"].asDouble()>3.14159265*2)
			asteroid["pos"]["r"]=asteroid["pos"]["r"].asDouble()-3.14159265*2;
		while(asteroid["pos"]["r"].asDouble()<0)
			asteroid["pos"]["r"]=asteroid["pos"]["r"].asDouble()+3.14159265*2;
	}
}
