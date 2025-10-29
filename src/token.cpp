#include "token.hpp"

Token::Token(std::string input_string) {

    /* this shit does not work pls handle the breaking paragraph logic AAAA
    if (input_string == "\n") {
        this->type = BREAK_PARAGRAPH;
        std::cout << "BREAK\n";
        return;
    }
    */
    /**/
    if (input_string == "```" || input_string == "$$") {
        return;
    }

    if (input_string == "######" || input_string == "#####" ||
        input_string == "####" || input_string == "###" ||
        input_string == "##" || input_string == "#") {
        this->type = TITLE;
        this->title_size = input_string.length();
        return;
    }

    /*
    if ((input_string.substr(0,1) == "*" && input_string.substr(input_string.length()-1, 1) == "*") ||
        (input_string.substr(0,2) == "**" && input_string.substr(input_string.length()-2, 2) == "**") ||
        (input_string.substr(0,3) == "***" && input_string.substr(input_string.length()-3, 3) == "***")) {
        std::cout << "found some!\n";
        this->type = TEXT;
    }*/
    this->type = TEXT;
    this->tokenString = input_string;
    
}

TokenType Token::getTokenType() {
    return this->type;
}

std::string Token::getTokenString() {
    return this->tokenString;
}

short unsigned int Token::getTokenTitleSize() {
    return this->title_size;
}
