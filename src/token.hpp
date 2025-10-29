#pragma once
#include <iostream>

enum TokenType {
    TEXT,
    BOLD,
    ITALIC,
    BOLD_ITALIC,
    TITLE,
    INLINE_MATH,
    DISPLAY_MATH,
    BREAK_PARAGRAPH
};

class Token { 
    public:
        Token(std::string input_string);
        TokenType getTokenType();
        std::string getTokenString();
        short unsigned int getTokenTitleSize();

    private:
        TokenType type;
        std::string tokenString;
        short unsigned int title_size;
};