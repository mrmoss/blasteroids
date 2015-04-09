#include "json.hpp"

#include "rapidjson/writer.h"
#include "rapidjson/stringbuffer.h"

msl::json msl::string_to_json(const std::string& str)
{
	msl::json json;
	json.Parse(str.c_str());
	return json;
}

std::string msl::json_to_string(const msl::json& json)
{
	rapidjson::StringBuffer buffer;
	rapidjson::Writer<rapidjson::StringBuffer> writer(buffer);
	json.Accept(writer);
	return std::string(buffer.GetString());
}