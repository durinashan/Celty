/*! \file Endpoint.hh
	An Endpoint that can be set into UDP or HTTP mode
*/
#pragma once
#include <string>
#include <ev++.h>
#include <thread>
namespace Celty {
	class Endpoint {
	public:
		enum EndpointType {
			HTTP,
			UDP,
		};
	private:
		EndpointType     _type;
		ev::io 		     eio;
		ev::sig 	     esig;
		ev::dynamic_loop *loop;
		int 		     sockfd;
		std::thread      t;

		void Runner(void);
	public:
		Endpoint(EndpointType type, std::string listen_addr, std::string port);
		~Endpoint(void);

		void Start(void);
		void Halt(void);

		Endpoint(Endpoint&) = delete;
		Endpoint(Endpoint&&) = delete;
	};
}
