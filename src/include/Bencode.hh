/*! \file bencode.hh
	Bencode parsing implementation

*/
#pragma once
#include <map>
#include <vector>
#include <string>
#include <utility>
#include <iostream>
namespace Celty {
	enum bennode_type {
		INTEGER,
		STRING,
		LIST,
		DICTIONARY
	};

	class bennode_base {
	public:
		bennode_type _type;
	};


	class bennode_integer : public bennode_base {
	public:
		long _value;
		bennode_type _type;
		bennode_integer(long value)
			: _value(value), _type(INTEGER) { }
	};



	class bennode_string : public bennode_base {
	public:
		std::string _value;
		bennode_type _type;
		bennode_string(const char* value)
			: _value(value), _type(STRING) { }
		bennode_string(std::string value)
			: _value(value), _type(STRING) { }
	};



	class bennode_list : public bennode_base {
	public:
		std::vector<bennode_base> _value;
		bennode_type _type;

		bennode_list(void) : _type(LIST) { }
		bennode_list(std::vector<bennode_base> value)
			: _value(value), _type(LIST) { }

		void append(bennode_base& item) {
			_value.push_back(item);
		}
	};



	class bennode_dictionary : public bennode_base {
	public:
		std::map<std::string, bennode_base> _value;
		bennode_type _type;

		bennode_dictionary(void) : _type(DICTIONARY) { }
		bennode_dictionary(std::map<std::string, bennode_base> value)
			: _value(value), _type(DICTIONARY) { }

		void append(std::string key, bennode_base& value) {
			_value.insert(std::make_pair(key, value));
		}
	};



	std::vector<bennode_base> ParseBencode(std::string input);

}
