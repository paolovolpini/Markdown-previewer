#include <string>
#include <vector>
#include "token.hpp"

class Tokenizer {
    public:
        Tokenizer();
        std::vector<Token> tokenizeLine(const std::string &line);
    private:
};
