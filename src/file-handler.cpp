#include <iostream>
#include <fstream>
#include "tokenizer.hpp"
#include "renderer.hpp"

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
    while (!input_file.eof()) {
        getline(input_file, lineBuffer);
        //std::cout << lineBuffer << "\n";
        tokenLineBuffer = tokenizer.tokenizeLine(lineBuffer);
        renderer.writeTokenToFile(tokenLineBuffer);
    }
    
    renderer.endHtmlWriting();
    input_file.close();

}