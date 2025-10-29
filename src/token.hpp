#pragma once
#include <iostream>

enum TokenType {
    TEXT,
    INLINE_SYMBOL,
    DISPLAY_SYMBOL,
    TITLE_SYMBOL
};

class Token { 
    public:
        Token(std::string input_string);
        TokenType getTokenType();

    private:
        TokenType type;
        std::string tokenString;
        std::size_t title_size;
};