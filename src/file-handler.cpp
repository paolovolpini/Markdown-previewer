#include <iostream>
#include <fstream>
#include <filesystem>
#include <future>
#include <exception>
#include "tokenizer.hpp"
#include "renderer.hpp"
#include "host-server.hpp"

typedef struct {
    std::string file_name;
    std::string css_path;
} CommandArgs;


void printHelp(const std::string &prog_name) {
    std::cout << "Usage: " << prog_name << " <markdown_file> [options]\n"
    "Options available are:\n\t-c: custom CSS file path" << std::endl;
}
void parseArguments(int argc, char *argv[], CommandArgs &args) {
    if(argc < 2) {
        printHelp(std::string(argv[0]));
        throw std::exception();
    }
    for(int i = 1; i < argc; i++) {
        std::string temp(argv[i]);
        if(temp.at(0) == '-') {
            // process the flags 
            switch(temp.at(1)) {
                case 'c':
                    args.css_path = std::string(argv[i+1]);
                    break;
                case 'h':
                    printHelp(std::string(argv[0]));
                    break;
                default:
                    std::cerr << "Unknown option '-" << temp.at(1) << "'. Try '-h' for help" << std::endl;
                    throw std::exception();
            }
            i++;
        }
        else {
            // file was given. Just take the first file and go on 
            if(temp.substr(temp.length()-3, 3) != ".md") {
                std::cerr << "File " << temp << " is not a markdown file" << std::endl;
                throw std::exception();
            }
            
            if(args.file_name.empty()) {
                args.file_name = temp;
            }
        }
    }
}

int main(int argc, char *argv[]) {

    std::fstream input_file;
    CommandArgs args = {};
    try {
        parseArguments(argc, argv, args);
    }
    catch (const std::exception&) {
        return 15;
    }

    input_file.open(args.file_name);
    if(input_file.bad()) {
        std::cerr << "Could not open file " << args.file_name << "." << std::endl;
        return 16;
    }    
    /* creating both the tokenizer object and html-renderer object */
    Tokenizer tokenizer;
    std::string lineBuffer;
    HtmlRenderer renderer(args.file_name);
    std::vector<Token> tokenLineBuffer;
    
    renderer.createFile(args.css_path);

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
	std::string html_source = args.file_name.substr(0, args.file_name.size()-3).append(".html");
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
