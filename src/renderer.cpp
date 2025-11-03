#include "renderer.hpp"
#include "token.hpp"
#include <istream>

HtmlRenderer::HtmlRenderer(std::string file_name) {
    this->file_name = file_name.substr(0, file_name.size()-3).append(".html");
}

void HtmlRenderer::elaborateAsterisks() {
	std::ifstream reader(this->file_name, std::ios::in);
	std::string line;
	std::vector<std::string> lines;
	while(getline(reader, line)) {
		std::cout << "LINE ELABORATED: " << line << std::endl;
		std::stack<std::pair<std::size_t, int>> ast_stack;
		std::size_t index = line.find('*', 0);
		std::size_t prev = index;
		ast_stack.push({index, 1});
		while((index = line.find('*', prev+1)) != std::string::npos) {	
			if(index == prev+1) {
				ast_stack.top().second++;
			}
			else {
				ast_stack.push({index, 1});
			}
			prev = index;
		}
		while(!ast_stack.empty()) {
			auto pair1 = ast_stack.top();
			ast_stack.pop();
			if(ast_stack.empty() || line.at(pair1.first-1) == ' ') continue;
			auto pair2 = ast_stack.top();
			ast_stack.pop();
			std::cout << "pair1: " << pair1.first << " " << pair1.second << "\npair2: " << pair2.first << " " << pair2.second << std::endl; 
			if(line.at(pair2.first+1) == ' ') continue;
			int ast_num = pair1.second > pair2.second ? pair2.second : pair1.second;
			if(ast_num == 1) {
				line.replace(pair1.first, ast_num, "</i>");
				line.replace(pair2.first+(pair2.second-ast_num), ast_num, "<i>");
			}
			else if(ast_num > 1 && ast_num % 2 == 1) {
				line.replace(pair1.first, ast_num, "</i></b>");
				line.replace(pair2.first+(pair2.second - ast_num), ast_num, "<b><i>");
			}
			else if (ast_num > 1 && ast_num % 2 == 0) {
				line.replace(pair1.first, ast_num, "</b>");
				line.replace(pair2.first+(pair2.second - ast_num), ast_num, "<b>");
			}
		}
		std::cout << "Elab: " << line << std::endl;
		lines.push_back(line);
	}
	reader.close();
	std::ofstream writer(this->file_name, std::ios::trunc);
	for(const auto& line : lines) {
		std::cout << "writing this: " << line << std::endl;
		writer << line << "\n";
	}
	writer.close();
}

/* opening tags */
void HtmlRenderer::createFile() {
    this->output_file.open(this->file_name, std::ios::out | std::ios::in);
    this->output_file << "<!DOCTYPE html>\n<html>\n\n";
}

void HtmlRenderer::writeTokenToFile(std::vector<Token> token_to_write) {
    int title = 0;
	std::size_t index_bold = 0, index_italic = 0;
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
	elaborateAsterisks(); 

}
