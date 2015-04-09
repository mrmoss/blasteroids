#include "json.hpp"

#include <sstream>

std::string msl::serialize(const msl::json& json)
{
	Json::FastWriter writer;
	return writer.write(json);
}

msl::json msl::deserialize(const std::string& str)
{
	std::istringstream istr(str);
	msl::json json;
	istr>>json;
	return json;
}