/*! \file Bencode.cc
	Implements bencode parsing
*/
#include <Bencode.hh>
#include <cctype>
namespace Celty {
	std::vector<bennode_base> ParseBencode(std::string input) {
		int c;
		int index = 0;
		std::vector<bennode_base> items;

		while((c = input[index++]) != NULL) {
			if(isalpha(c)) {
				switch(c) {
					case 'i': {
						// Integers
						std::string tmp;
						while((c = input[index++]) != 'e' && isdigit(c))
							tmp += c;
						items.push_back(bennode_integer(std::stol(tmp)));
						break;
					} case 'l': {
						// Lists, seeing as ParseBencode essentially returns
						// a bennode_list anyway, we can kinda just call this
						// recursively, but first we need to truncate it
						break;
					} case 'd': {
						// Dictionary Parsing
						break;
					} default: {
						// The fuck is this?
					}
				}
			} else if(isdigit(c)) {
				// So It looks to be a string
				std::string tmp;
				while((c = input[index++]) != ':')
					tmp += c;
				items.push_back(bennode_string(tmp));
			} else {
				// The fuck is that?
			}
		}

		return items;
	}
	std::ostream& operator<<(std::ostream& os, const bennode_list& bn_d) {
		for(auto& val : bn_d._value)
			os << val << "\n";
		return os;
	}
	std::ostream& operator<<(std::ostream& os, const bennode_dictionary& bn_d) {
		for(auto& val : bn_d._value)
			os << "[" << val.first << "] " << val.second << "\n";
		return os;
	}
	std::ostream& operator<<(std::ostream& os, const bennode_string& bn_s) {
		os << bn_s._value << "\n";
		return os;
	}
	std::ostream& operator<<(std::ostream& os, const bennode_integer& bn_i) {
		os << bn_i._value << "\n";
		return os;
	}
	std::ostream& operator<<(std::ostream& os, const bennode_base& bn_b) {
		switch(bn_b._type) {
			case INTEGER: {
				os << dynamic_cast<bennode_integer>(bn_b);
				break;
			} case STRING: {
				os << dynamic_cast<bennode_string>(bn_b);
				break;
			} case LIST: {
				os << dynamic_cast<bennode_list>(bn_b);
				break;
			} case DICTIONARY: {
				os << dynamic_cast<bennode_dictionary>(bn_b);
				break;
			}
		}
		return os;
	}
}
