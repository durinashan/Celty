/*! \file Endpoint.cc
	An Endpoint that can be set into UDP or HTTP mode
*/
#include <Endpoint.hh>
#include <HTTP.hh>

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
	: _type(type), loop(ev::AUTO), listen(listen_addr), port(listen_port) {}

Endpoint::~Endpoint(void) {}

void Endpoint::Runner(void) {
	// Setup socket
	if(this->_type != UDP) {
		this->timer.set(this->loop);
		this->timer.set<Endpoint, &Endpoint::Timeout>(this);
		this->timer.start(4, 4);
	}

	this->eio.set(this->loop);
	this->eio.set<Endpoint, &Endpoint::EVIORead>(this);
	this->eio.start(this->sockfd, ev::READ);

	this->ashalt.set(this->loop);
	this->ashalt.set<Endpoint, &Endpoint::AsyncHalt>(this);
	this->ashalt.start();

	this->loop.run(0);
}

void Endpoint::Start(void) { this->t = std::thread(&Endpoint::Runner, this); }

void Endpoint::Halt(void) {
	this->ashalt.send();
	this->t.join();
}

void Endpoint::Timeout(void) {
	// Disconnect code (Only triggered if non UDP)
}

void Endpoint::EVIORead(ev::io& watcher, int revent) {

}

void Endpoint::AsyncHalt(void) {
	this->ashalt.stop();
	if(this->_type != UDP) {
		this->timer.stop();
	}
	this->eio.stop();
	this->loop.break_loop(ev::ALL);
}
}
