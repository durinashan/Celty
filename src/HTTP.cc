/*! \file HTTP.cc
	HTTP Helper implementation
*/
#include <HTTP.hh>
#include <string>
namespace Celty {
static char hex[16] = {'0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'A', 'B', 'C', 'D', 'E', 'F'};

std::string HTTP::URLEncodeStr(std::string str) {
	std::string tmp;

	return tmp;
}

std::string HTTP::URLDecodeStr(std::string str) {
	std::string tmp;

	return tmp;
}

std::string HTTP::RawEncode(const unsigned char *buffer, size_t length) {
	std::string tmp;
	size_t i;
	for (i = 0; i < length; i++) {
		tmp += "%";
		tmp += hex[((buffer[i] >> 4) & 0xF)];
		tmp += hex[(buffer[i] & 0xF)];
	}
	return tmp;
}
}
