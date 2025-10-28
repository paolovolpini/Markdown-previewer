#include "token.hpp"

Token::Token(std::string input_string) {

    /**/
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

    if (input_string == "*" || input_string == "***" ||
        input_string == "***" || input_string == "`" ) {
        this->type == INLINE_SYMBOL;
        return;
    }

    this->type = TEXT;
}