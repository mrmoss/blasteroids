#ifndef MSL_C11_JSON_HPP
#define MSL_C11_JSON_HPP

#include "rapidjson/document.h"

namespace msl
{
	typedef rapidjson::Document json;

	json string_to_json(const std::string& str);
	std::string json_to_string(const json& json);
}

#endif