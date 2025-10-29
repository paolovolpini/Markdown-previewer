#include "tokenizer.hpp"
#include "token.hpp"
#include <sstream>

Tokenizer::Tokenizer() {

}

std::vector<Token> Tokenizer::tokenizeLine(const std::string line) {

    std::vector<Token> tokens;
    
    std::stringstream ss(line);
    
    std::string buffer;
    
    while (ss >> buffer) {
        /* here we should also handle the 
        separation of *, **, ***, `, ```*/
        tokens.push_back(Token(buffer));

        //std::cout << buffer << "\n";
    }
    
    return tokens;
}