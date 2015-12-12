/*! \file HTTP.hh
	HTTP helpers
*/
#pragma once
#include <string>
namespace Celty {
class HTTP {
public:
	// See http://www.faqs.org/rfcs/rfc1738.html
	static std::string URLEncodeStr(std::string str);
	static std::string URLDecodeStr(std::string str);
	static std::string RawEncode(const unsigned char* buffer, size_t length);
};
}
