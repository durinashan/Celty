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

#include <iostream>

namespace Celty {
Endpoint::Endpoint(EndpointType type, std::string listen_addr, std::string listen_port)
	: loop(ev::AUTO), _type(type), listen(listen_addr), port(listen_port) {}

Endpoint::~Endpoint(void) {}

void Endpoint::Runner(void) {
	this->timer.set(this->loop);
	this->timer.set<Endpoint, &Endpoint::Timeout>(this);
	this->timer.start(4, 4);

	this->ashalt.set(this->loop);
	this->ashalt.set<Endpoint, &Endpoint::AsyncHalt>(this);
	this->ashalt.start();

	// Server Code

	this->loop.run(0);
}

void Endpoint::Start(void) { this->t = std::thread(&Endpoint::Runner, this); }

void Endpoint::Halt(void) {
	this->ashalt.send();
	this->t.join();
}

void Endpoint::Timeout(void) {
	// Disconnect code
}

void Endpoint::AsyncHalt(void) {
	this->ashalt.stop();
	this->timer.stop();
	this->loop.break_loop(ev::ALL);
}
}
