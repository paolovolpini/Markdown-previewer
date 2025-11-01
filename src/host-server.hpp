#pragma once 

#include <iostream>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <atomic>
#include <unistd.h>

class HttpServer {
	public:
		HttpServer();
		void SetPage(const std::string &page);
		void Listen();
		void Shutdown();
		~HttpServer();
	private:
		int socket_fd;
		in_port_t port_num;
		struct sockaddr_in addr;
		std::string content;
		std::atomic<bool> running = false;
		void Log(const std::string &msg);
		void Error(const std::string &msg);
};
