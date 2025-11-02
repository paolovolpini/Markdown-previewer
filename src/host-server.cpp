#include "host-server.hpp"

void HttpServer::Log(const std::string &msg) {
	std::cout << "[HttpServer] " << msg << std::endl;
}

void HttpServer::Error(const std::string &msg) {
	std::cerr << "[HttpServer ERROR] " << msg << std::endl;
}

HttpServer::HttpServer() {
	this->socket_fd = socket(AF_INET, SOCK_STREAM, 0);
	if (this->socket_fd < 0) {
		Error("Error in creating socket");
		exit(EXIT_FAILURE);
	}
	this->addr.sin_family = AF_INET;
	this->addr.sin_addr.s_addr = htonl(INADDR_ANY);
	this->addr.sin_port = 0;
	socklen_t temp = sizeof(this->addr);
	if (bind(this->socket_fd, (struct sockaddr *) &this->addr, temp) < 0) {
		Error("Error in binding");
		exit(EXIT_FAILURE);
	}
	if (getsockname(this->socket_fd, (struct sockaddr *) &this->addr, &temp) < 0) {
		Error("Error in getting port number");
		exit(EXIT_FAILURE);
	}
	Log("Socket is ready to listen on port " + std::to_string(ntohs(this->addr.sin_port)));

	/*auto opening of the localhost page*/
	std::string URL;   
    URL = "http://localhost:" + std::to_string(ntohs(this->addr.sin_port));

	pid_t pid = fork();
    if (pid == -1) {
        perror("fork");
        return;
    } else if (pid == 0) {
        execlp("xdg-open", "xdg-open", URL.c_str(), (char *)NULL);
		/*in case of execlp errors, it will pass through*/
        perror("execlp");
        exit(EXIT_FAILURE);
    }
}

void HttpServer::SetPage(const std::string &page) {
	this->content = 
		"HTTP/1.1 200 OK\r\n"
		"Content-Type: text/html\r\n"
        "Content-Length: " + std::to_string(page.size()) + "\r\n"
            "\r\n" + page;
}

void HttpServer::Listen() {
	if (running) return;
	this->running = true;
	if (listen(this->socket_fd, 1) < 0) {
		Error("Error in listening connections");
		exit(EXIT_FAILURE);
	}
	Log("Now listening...");
	while (running) {
		int sock_client = accept(this->socket_fd, nullptr, nullptr);
		if (sock_client < 0) continue;
		// yes i know i didn't put the parenthesis
		// it's just a continue and it's fucking midnight
		// please don't refactor this 
		if (send(sock_client, this->content.c_str(), this->content.size(), 0) < 0) {
			Error("Error in sending page. Skipping client...");
			continue;
		}
		close(sock_client);
	}
}

void HttpServer::Shutdown() {
	if (this->running) {
		this->running = false;
		shutdown(this->socket_fd, SHUT_RDWR);
		close(this->socket_fd);
		Log("Server shutdown");
	}
}

HttpServer::~HttpServer() {
	this->Shutdown();
}
