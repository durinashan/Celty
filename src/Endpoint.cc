/*! \file Endpoint.cc
	An Endpoint that can be set into UDP or HTTP mode
*/
#include <Endpoint.hh>

#include <unistd.h>
#include <fcntl.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <resolv.h>
#include <netdb.h>
#include <arpa/inet.h>

namespace Celty {
	Endpoint::Endpoint(EndpointType type, std::string listen_addr, std::string listen_port) :
		_type(type), listen(listen_addr), port(listen_port) { }

	Endpoint::~Endpoint(void) {
		if(this->loop != nullptr)
			delete this->loop;
	}

	void Endpoint::Runner(void) {
		this->loop = new ev::dynamic_loop(ev::AUTO);
	}

	void Endpoint::Start(void) {
		this->t = std::thread(&Endpoint::Runner, this);
	}

	void Endpoint::Halt(void) {
		this->loop->break_loop(ev::ALL);
		this->t.join();
	}
}
