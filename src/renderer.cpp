#include "renderer.hpp"
#include "token.hpp"
#include <istream>


HtmlRenderer::HtmlRenderer(std::string file_name) {
    this->file_name = file_name.substr(0, file_name.size()-3).append(".html");
}

std::string HtmlRenderer::insertCss(const std::string &css_path) {
    if(css_path.empty()) return "";
    std::fstream css_file(css_path);
    std::stringstream buffer;
    buffer << css_file.rdbuf();
    return "<style>\n" + buffer.str() + "\n</style>\n";    
}

/* opening tags */
void HtmlRenderer::createFile(const std::string &css_path) {
    this->output_file.open(this->file_name, std::ios::out | std::ios::trunc);
    this->output_file << "<!DOCTYPE html>\n<html>\n<script src=\"https://cdn.jsdelivr.net/npm/mathjax@3/es5/tex-mml-chtml.js\"></script>\n";
    this->output_file << "<html lang=\"it\">\n<head>\n" << insertCss(css_path) << "\t<meta charset=\"utf-8\">\t<title> your preview </title>\n</head>\n";
}


void HtmlRenderer::writeTokenToFile(std::vector<Token> token_to_write) {
    int title = 0;

    if (!((token_to_write[0].getTokenType() == LIST) ||
        (token_to_write[0].getTokenType() == ORDERED_LIST)) && (this->in_list == true)) {
            if (this->is_list_ordered) {
                this->output_file << "</ol>\n";
                this->is_list_ordered = false;
            } else {
                this->output_file << "</ul>\n";
            }  
            this->in_list = false; 
    }

	for (int i = 0; i < (int)token_to_write.size(); i++) {

        
        switch (token_to_write[i].getTokenType()) {
        case TITLE:
            this->output_file << "<h" << token_to_write[i].getTokenTitleSize() << ">";
            //this->stack.push(token_to_write[i]);
            title = token_to_write[i].getTokenTitleSize();
            break;

        case TEXT: {
            std::string to_write = token_to_write[i].getTokenString();
            if (this->in_display_code || this->in_display_math) {
                this->output_file << to_write;
                break;
            }

            if (this->in_paragraph == false && title == false && this->in_list == false) {
                this->output_file << "<p>";
                this->in_paragraph = true;
                //this->stack.push(token_to_write[i]);
            }
            if(to_write.back() == '\\') {
				to_write.back() = '\0';
				to_write.append("\n<br>");
			} 
			
			this->output_file << to_write;
            break;
		}

        case BREAK_PARAGRAPH:
            if (this->in_paragraph == true) {
                this->output_file << "</p>\n";
                this->in_paragraph = false;
            } 
            break;

        case DISPLAY_CODE:
            std::cout << token_to_write[i].getTokenString();
            if (this->in_display_code == false) {
                this->output_file << "<div>\n";
                this->output_file << "<pre>\n\t<code>";
                this->in_display_code = true;
            } else {
                this->output_file << "\t</code>\n</pre>\n</div>\n";
                this->in_display_code = false;
            }
            break;

        case DISPLAY_MATH:
            std::cout << token_to_write[i].getTokenString();
            if (this->in_display_math == false) {
                this->output_file << "<div align=\"center\"\">\n";
                this->output_file << "\t \\(";
                this->in_display_math = true;
            } else {
                this->output_file << "\t \\)\n";
                this->output_file << "</div>\n";
                this->output_file << "<br>\n";
                this->in_display_math = false;
            }
            break;

        case LIST:
            std::cout << token_to_write[i].getTokenString();
            if (this->in_list == false) {
                this->output_file << "<ul>\n";
                this->in_list = true;
            } 
            this->output_file << "\t<li> ";
        
        case ORDERED_LIST:
            std::cout << token_to_write[i].getTokenString();
            if (this->in_list == false) {
                this->output_file << "<ol>\n";
                this->in_list = true;
                this->is_list_ordered = true;
            } 
            this->output_file << "\t<li> ";
        
        default:
            break;
        }


    }
	/*
     handle tag closing using a stack
    while (this->stack.size() > 0) {
        switch (this->stack.top().getTokenType()) {
        case TITLE:
            this->output_file << "</h" << this->stack.top().getTokenTitleSize() << ">";
            break;
        default:
            break;
        }
        stack.pop();
    }
	*/
	if(title) {
		this->output_file << "</h" << title << ">";
	}

    this->output_file << " ";
    
    if(this->in_list) {
		this->output_file << "</li>\n";
	}

    if (this->in_display_code) {
        this->output_file << "\n";
    }
}

void HtmlRenderer::endHtmlWriting() {
    if (this->in_paragraph == true) {
        this->output_file << "</p>\n";
        this->in_paragraph = false;
    } else if (this->in_display_code == true) {
        this->output_file << "\t</code>\n</pre>";
        this->in_display_code = false;
    }	
    this->output_file << "</html>";
	this->output_file.close();
}
