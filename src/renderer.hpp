#pragma once

#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <stack>
#include "token.hpp"

class HtmlRenderer {
    public:
        HtmlRenderer(std::string md_file_name);
        void createFile(const std::string &css_path);
        void writeTokenToFile(std::vector<Token> token_to_write);
        void endHtmlWriting();

    private:
        std::string insertCss(const std::string &css_path);
        std::string file_name;
        std::fstream output_file;
        std::stack<Token> stack;
        bool in_paragraph = false;
        bool in_display_code = false;
        bool in_display_math = false;	
};
