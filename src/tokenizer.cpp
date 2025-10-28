#include "tokenizer.hpp"
#include <sstream>

Tokenizer::Tokenizer() {

}

std::vector<std::string> Tokenizer::tokenizeLine(const std::string line) {

    std::vector<std::string> tokens;
    
    std::stringstream ss(line);
    std::string buffer;
    
    while (ss >> buffer) {
        /* here we should also handle the 
        separation of *, **, ***, `, ```*/
        tokens.push_back(buffer);
        //std::cout << buffer << "\n";
    }

    return tokens;
}