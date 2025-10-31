#include "tokenizer.hpp"
#include "token.hpp"
#include <sstream>

Tokenizer::Tokenizer() {

}


// if we tokenize by spaces, how are newlines tokenized?
// stringstream by default ignores newlines

/*
std::vector<Token> Tokenizer::tokenizeLine(const std::string &line) {

    std::vector<Token> tokens;
    
    std::stringstream ss(line);
    
    std::string buffer;
    // this ignores EVERY newline
	
    while (ss >> buffer) {
        here we should also handle the 
        separation of *, **, ***, `, ```
        tokens.push_back(Token(buffer));
        //std::cout << buffer << "\n";
    }
    
	
    return tokens;
}*/

// I would like to propose this 
std::vector<Token> Tokenizer::tokenizeLine(const std::string &line) {
	/* our line might not contain a single word which means that it's just a newline
	 * stringstream ignores every space, thus not pushing newlines
	 * in order to fix this, we need to split the string only by ' '
	 **/
	std::vector<Token> tokens;
	std::size_t prev = 0;
	std::size_t found = line.find(' ');

	// this is f*cking GREAT :) -dam
	while(found != std::string::npos) {
		std::string token = line.substr(prev, found-prev);
		std::cout << "Token pushed:" << token << std::endl;
		tokens.push_back(Token(token));
		prev = found;
		found = line.find(' ', prev+1);
	}

	std::string remainder = line.substr(prev);
	tokens.push_back(Token(remainder));
	return tokens;
}
