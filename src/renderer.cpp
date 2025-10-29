#include "renderer.hpp"
#include "token.hpp"

HtmlRenderer::HtmlRenderer(std::string file_name) {
    this->file_name = file_name.substr(0, file_name.size()-3).append(".html");
}

/* opening tags */
void HtmlRenderer::createFile() {
    this->output_file.open(this->file_name, std::ios::out);
    this->output_file << "<!DOCTYPE html>\n";
    this->output_file << "<html>\n";
    this->output_file << "\n";
}

void HtmlRenderer::writeTokenToFile(std::vector<Token> token_to_write) {
    bool title = false;
    in_paragraph = false;

    for (int i = 0; i < token_to_write.size(); i++) {
        
        switch (token_to_write[i].getTokenType()) {
        case TITLE:
            this->output_file << "<h" << token_to_write[i].getTokenTitleSize() << ">";
            this->stack.push(token_to_write[i]);
            title = true;
            break;

        case TEXT:
            if (this->in_paragraph == false && title == false) {
                this->output_file << "<p>";
                this->in_paragraph = true;
                this->stack.push(token_to_write[i]);
            }

            this->output_file << token_to_write[i].getTokenString() << " ";
            break;

        default:
            break;
        }
    }

    /* handle tag closing using a stack*/
    while (this->stack.size()>0) {
        switch (this->stack.top().getTokenType()) {
        case TITLE:
            this->output_file << "</h" << this->stack.top().getTokenTitleSize() << ">";
            break;
        case TEXT:
            this->output_file << "</p>";
            break;
        default:
            break;
        }
        stack.pop();
    }
    this->output_file << "\n";
    
}

void HtmlRenderer::endHtmlWriting() {
    this->output_file << "</html>";
    this->output_file.close();
}