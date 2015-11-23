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

		while((c = input[index++]) != '\0') {
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
				int _c = std::stoi(c);
				index += 2; // Skip over the index and
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
}
