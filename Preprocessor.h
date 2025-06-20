// Preprocess.h
#ifndef PREPROCESSOR_H
#define PREPROCESSOR_H

#include <string>
#include <vector>

class Preprocessor {
public:
    std::string preprocess(const std::string &code);

private:
    std::string removeComments(const std::string &code);
    std::string normalizeWhitespace(const std::string &code);
    std::string normalizeVariables(const std::string &code);
    std::string removeLiterals(const std::string &code);
};

#endif // PREPROCESS_H
