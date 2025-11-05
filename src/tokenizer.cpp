#include "tokenizer.hpp"
#include "token.hpp"
#include <sstream>
#include <stack>

Tokenizer::Tokenizer() {

}


// if we tokenize by spaces, how are newlines tokenized?
// stringstream by default ignores newlines

/*
std::vector<Token> Tokenizer::tokenizeLine(const std::string &line) {

    std::vector<Token> tokens;
    
    std::stringstream ss(line);
    
    std::string buffer;
    // this ignores EVERY newline
	
    while (ss >> buffer) {
        here we should also handle the 
        separation of *, **, ***, `, ```
        tokens.push_back(Token(buffer));
        //std::cout << buffer << "\n";
    }
    
	
    return tokens;
}*/

void Tokenizer::parseAsterisks(std::string &str) {
	std::stack<std::pair<std::size_t, int>> ast_stack;
	std::size_t index = str.find('*', 0);
	std::size_t prev = index;
	ast_stack.push({index, 1});
	while ((index = str.find('*', prev+1)) != std::string::npos) {	
		if (index == prev+1) {
			ast_stack.top().second++;
		}
		else {
			ast_stack.push({index, 1});
		}
		prev = index;
	}

	while (!ast_stack.empty()) {
		auto pair1 = ast_stack.top();
		ast_stack.pop();
		if (ast_stack.empty() || str.at(pair1.first-1) == ' ') continue;

		auto pair2 = ast_stack.top();
		ast_stack.pop();
		if (str.at(pair2.first+1) == ' ') continue;
		int ast_num = pair1.second > pair2.second ? pair2.second : pair1.second;
		if (ast_num == 1) {
			str.replace(pair1.first, ast_num, "</it>");
			str.replace(pair2.first+(pair2.second-ast_num), ast_num, "<it>");
		}
		if (ast_num > 1 && ast_num % 2 == 1) {
			str.replace(pair1.first, ast_num, "</b></it>");
			str.replace(pair2.first+(pair2.second - ast_num), ast_num, "<it><b>");
		}
		else {
			str.replace(pair1.first, ast_num, "</b>");
			str.replace(pair2.first+(pair2.second - ast_num), ast_num, "<b>");
		}
	}		
}

void Tokenizer::parseInlineSymbol(std::string &str, const char symbol, const std::string tag_name) {
	std::stack<std::pair<std::size_t, int>> ast_stack;
	std::size_t index = str.find(symbol, 0);
	std::size_t prev = index;
	ast_stack.push({index, 1});

	while ((index = str.find(symbol, prev+1)) != std::string::npos) {	
		ast_stack.push({index, 1});
		prev = index;
	}
	while (!ast_stack.empty()) {
		auto pair1 = ast_stack.top();
		ast_stack.pop();
		if (ast_stack.empty()) continue; // needed to ignore not closed symbols

		auto pair2 = ast_stack.top();
		ast_stack.pop();
		int ast_num = pair1.second > pair2.second ? pair2.second : pair1.second;
		str.replace(pair1.first, ast_num, "</"+tag_name+">");
		str.replace(pair2.first+(pair2.second-ast_num), ast_num, "<"+tag_name+">");
	}		
}

void Tokenizer::parseInlineSymbol(std::string &str, const char symbol, const std::string opening_tag, const std::string closing_tag) {
	std::stack<std::pair<std::size_t, int>> ast_stack;
	std::size_t index = str.find(symbol, 0);
	std::size_t prev = index;
	ast_stack.push({index, 1});

	while ((index = str.find(symbol, prev+1)) != std::string::npos) {	
		ast_stack.push({index, 1});
		prev = index;
	}
	while (!ast_stack.empty()) {
		auto pair1 = ast_stack.top();
		ast_stack.pop();
		if (ast_stack.empty()) continue; // needed to ignore not closed symbols

		auto pair2 = ast_stack.top();
		ast_stack.pop();
		int ast_num = pair1.second > pair2.second ? pair2.second : pair1.second;
		str.replace(pair1.first, ast_num, closing_tag);
		str.replace(pair2.first+(pair2.second-ast_num), ast_num, opening_tag);
	}		
}

// I would like to propose this 
std::vector<Token> Tokenizer::tokenizeLine(std::string &line) {
	/* our line might not contain a single word which means that it's just a newline
	 * stringstream ignores every space, thus not pushing newlines
	 * in order to fix this, we need to split the string only by ' '
	 **/
	parseAsterisks(line);

	if (line.substr(0,2) != "$$") {
		parseInlineSymbol(line, '$', "\\(", "\\)");
	}
	
	if (line.substr(0,3) != "```") {
		parseInlineSymbol(line, '`', "code");
	}
	
	std::vector<Token> tokens;
	std::size_t prev = 0;
	std::size_t found = line.find(' ');

	// this is f*cking GREAT :) -dam
	while (found != std::string::npos) {
		std::string token = line.substr(prev, found-prev);
		std::cout << "Token pushed:" << token << std::endl;
		tokens.push_back(Token(token));
		prev = found;
		found = line.find(' ', prev+1);
	}

	std::string remainder = line.substr(prev);
	tokens.push_back(Token(remainder));
	return tokens;
}
