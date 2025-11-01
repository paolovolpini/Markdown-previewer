#include <iostream>
#include <fstream>
#include <filesystem>
#include <future>
#include "tokenizer.hpp"
#include "renderer.hpp"
#include "host-server.hpp"
int main(int argc, char *argv[]) {

    std::fstream input_file;
    
    /* i think we could print the actual syntax here. or maybe we could have support for a -help flag*/
    if (argc < 2) {
        std::cerr << "not enough arguments." << std::endl;
        return(EXIT_FAILURE);
    }

    /* checking for file extension */
    std::string file_path(argv[1]);
    if (file_path.length() < 3) {
        std::cerr << "input file is not a .md file." << std::endl;
        return(EXIT_FAILURE);
    }

    if (!(file_path.substr(file_path.length()-3, 3) == ".md")) {
        std::cerr << "input file is not a .md file." << std::endl;
        return(EXIT_FAILURE);
    }
    
    input_file.open(argv[1]);
    if (!input_file.is_open()) {
        std::cerr << "error: file was not properly opened, probably does not exists." << std::endl;
        return(EXIT_FAILURE);
    } 

    /* creating both the tokenizer object and html-renderer object */
    Tokenizer tokenizer;
    std::string lineBuffer;
    HtmlRenderer renderer(file_path);
    std::vector<Token> tokenLineBuffer;
    
    renderer.createFile();

    /* reads line per line*/
    while (getline(input_file, lineBuffer)) {
        if(input_file.eof()) break;
        //std::cout << lineBuffer << "\n";
        tokenLineBuffer = tokenizer.tokenizeLine(lineBuffer);
        renderer.writeTokenToFile(tokenLineBuffer);
    }
    
    renderer.endHtmlWriting();
    input_file.close();
	// testing server....
	
	// since we save automatically into an html
	// i am testing by reading the html as a string 
	std::string html_source = file_path.substr(0, file_path.size()-3).append(".html");
	auto size = std::filesystem::file_size(html_source);
	std::string content(size, '\0');
	std::ifstream in(html_source);
	in.read(&content[0], size);

	// start server
	HttpServer server;
	server.SetPage(content);
	auto server_future = std::async(std::launch::async, [&]() 
		{ server.Listen(); }
	); // lambda function that takes server passed by reference and calls Listen
	std::cout << "Server is up! Press anything to stop...\n";
	std::cin.get();
	server.Shutdown();
	server_future.wait();
	
	return 0;
}
