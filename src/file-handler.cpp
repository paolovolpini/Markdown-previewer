#include <iostream>
#include <fstream>
#include "tokenizer.hpp"

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

    input_file.clear();

    input_file.open(argv[1]);
    if (!input_file.is_open()) {
        std::cerr << "error: file was not properly opened, probably does not exists." << std::endl;
        return(EXIT_FAILURE);
    } 

    /* stuff */
    Tokenizer tokenizer;
    std::string lineBuffer;
    getline(input_file, lineBuffer);
    std::cout << lineBuffer << "\n";
    tokenizer.tokenizeLine(lineBuffer);

    /* should we use a data structure to pass the lines to the tokenizer? */
    input_file.close();

}