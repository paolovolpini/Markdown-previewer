#include <string>
#include <vector>
#include "token.hpp"

class Tokenizer {
    public:
        Tokenizer();
        std::vector<Token> tokenizeLine(std::string &line);
    private:
		void parseAsterisks(std::string &str);
};
