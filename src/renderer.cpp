#include "renderer.hpp"
#include "token.hpp"
#include <istream>

HtmlRenderer::HtmlRenderer(std::string file_name) {
    this->file_name = file_name.substr(0, file_name.size()-3).append(".html");
}

/* opening tags */
void HtmlRenderer::createFile() {
    this->output_file.open(this->file_name, std::ios::out | std::ios::trunc);
    this->output_file << "<!DOCTYPE html>\n<html>\n\n";
}

void HtmlRenderer::writeTokenToFile(std::vector<Token> token_to_write) {
    int title = 0;
	for (int i = 0; i < (int)token_to_write.size(); i++) {
        
        switch (token_to_write[i].getTokenType()) {
        case TITLE:
            this->output_file << "<h" << token_to_write[i].getTokenTitleSize() << ">";
            //this->stack.push(token_to_write[i]);
            title = token_to_write[i].getTokenTitleSize();
            break;

        case TEXT: {
            std::string to_write = token_to_write[i].getTokenString();
            if (this->in_display_code) {
                this->output_file << to_write;
                break;
            }

            if (this->in_paragraph == false && title == false) {
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
                this->output_file << "</p>";
                this->in_paragraph = false;
            } 
            break;

        case DISPLAY_CODE:
            std::cout << token_to_write[i].getTokenString();
            if (this->in_display_code == false) {
                this->output_file << "<pre>\n\t<code>";
                this->in_display_code = true;
            } else {
                this->output_file << "\t</code>\n</pre>";
                this->in_display_code = false;
            }
            break;

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
