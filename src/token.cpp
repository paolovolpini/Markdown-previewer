#include "token.hpp"

Token::Token(std::string input_string) {

    /* this shit does not work pls handle the breaking paragraph logic AAAA
    if (input_string == "\n") {
        this->type = BREAK_PARAGRAPH;
        std::cout << "BREAK\n";
        return;
    }
    */
    /*a string of size == 0 means that from the getline function was found only a '\n' character. 
    the newline symbol is always deleted, giving a string of size 0.
    this case is the only instance of a string of this size, so we can use this criteria to say if something
    is a newline or not*/
    if (input_string.size() == 0) {
        this->type = BREAK_PARAGRAPH;
        return;
    }

    if (input_string.substr(0,3) == "```") {
        this->type = DISPLAY_CODE;
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
        this->type = ;
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
