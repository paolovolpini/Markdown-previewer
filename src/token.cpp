#include "token.hpp"

Token::Token(std::string input_string) {
    
    if (input_string == "```" || input_string == "$$") {
        this->type == DISPLAY_SYMBOL;
        return;
    }

    if (input_string == "######" || input_string == "#####" ||
        input_string == "####" || input_string == "###" ||
        input_string == "##" || input_string == "#") {
        this->type == TITLE_SYMBOL;
        
        return;
    }
    /*
    if ((input_string.substr(0,1) == "*" && input_string.substr(input_string.length()-1, 1) == "*") ||
        (input_string.substr(0,2) == "**" && input_string.substr(input_string.length()-2, 2) == "**") ||
        (input_string.substr(0,3) == "***" && input_string.substr(input_string.length()-3, 3) == "***")) {
        std::cout << "found some!\n";
        this->type = TEXT;
    }*/
    
    
}

TokenType Token::getTokenType() {
    return this->type;
}