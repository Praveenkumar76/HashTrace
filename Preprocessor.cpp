// Preprocess.cpp
#include "Preprocessor.h"
#include <regex>
#include <sstream>
#include <algorithm>

std::string Preprocessor::preprocess(const std::string &code)
{
    std::string processed = removeComments(code);
    processed = normalizeWhitespace(processed);
    processed = normalizeVariables(processed);
    processed = removeLiterals(processed);
    return processed;
}

std::string Preprocessor::removeComments(const std::string &code)
{
    // Remove single-line comments
    std::string noSingleLine = std::regex_replace(code, std::regex(R"(\/\/.*)"), "");
    // Remove multi-line comments
    return std::regex_replace(noSingleLine, std::regex(R"(\/\*[\s\S]*?\*\/)"), "");
}

std::string Preprocessor::normalizeWhitespace(const std::string &code)
{
    std::string result;
    result.reserve(code.size());
    bool inSpace = false;

    for (char c : code) {
        if (std::isspace(c)) {
            if (!inSpace) {
                result += ' ';
                inSpace = true;
            }
        } else {
            result += c;
            inSpace = false;
        }
    }

    return result;
}

std::string Preprocessor::normalizeVariables(const std::string &code)
{
    // This is a simplified version - consider using a proper parser for real projects
    std::regex varDecl(R"((\b(int|float|double|char|bool|auto)\s+)([a-zA-Z_][a-zA-Z0-9_]*))");
    return std::regex_replace(code, varDecl, "$1var");
}

std::string Preprocessor::removeLiterals(const std::string &code)
{
    // Replace string literals with "str"
    std::regex strLiteral(R"("([^"\\]|\\.)*")");
    std::string noStrings = std::regex_replace(code, strLiteral, "\"str\"");

    // Replace numeric literals with "num"
    std::regex numLiteral(R"(\b\d+\.?\d*\b)");
    return std::regex_replace(noStrings, numLiteral, "num");
}
