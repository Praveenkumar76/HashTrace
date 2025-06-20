#include "Preprocessor.h"
#include <algorithm>
#include <cctype>
#include <unordered_set>

namespace {
const std::unordered_set<std::string>& getReservedWordsImpl() {
    static const std::unordered_set<std::string> RESERVED_WORDS = {
        "if", "else", "for", "while", "do", "switch", "case", "break",
        "continue", "return", "class", "struct", "void", "int", "float",
        "double", "char", "bool", "true", "false", "public", "private",
        "protected", "static", "const", "virtual", "template", "typename"
    };
    return RESERVED_WORDS;
}
}

Preprocessor::Preprocessor() = default;

const std::unordered_set<std::string>& Preprocessor::getReservedWords() {
    return getReservedWordsImpl();
}

std::string Preprocessor::preprocess(const std::string &code) {
    std::string processed = code;
    processed = removeComments(processed);
    processed = normalizeWhitespace(processed);
    processed = normalizeCase(processed);
    processed = removeStringLiterals(processed);
    processed = removeNumberLiterals(processed);
    processed = normalizeIdentifiers(processed);
    return processed;
}

std::string Preprocessor::removeComments(const std::string &code) {
    std::string result;
    result.reserve(code.size());
    bool inLineComment = false;
    bool inBlockComment = false;

    for (size_t i = 0; i < code.size(); ++i) {
        if (!inBlockComment && i+1 < code.size() && code[i] == '/' && code[i+1] == '/') {
            inLineComment = true;
            i++;
            continue;
        }

        if (!inLineComment && i+1 < code.size() && code[i] == '/' && code[i+1] == '*') {
            inBlockComment = true;
            i++;
            continue;
        }

        if (inLineComment && code[i] == '\n') {
            inLineComment = false;
        }

        if (inBlockComment && i+1 < code.size() && code[i] == '*' && code[i+1] == '/') {
            inBlockComment = false;
            i++;
            continue;
        }

        if (!inLineComment && !inBlockComment) {
            result += code[i];
        }
    }

    return result;
}

std::string Preprocessor::normalizeWhitespace(const std::string &code) {
    std::string result;
    result.reserve(code.size());
    bool inSpace = false;

    for (char c : code) {
        if (std::isspace(c)) {
            if (!inSpace && !result.empty()) {
                result += ' ';
                inSpace = true;
            }
        } else {
            result += c;
            inSpace = false;
        }
    }

    if (!result.empty() && result.back() == ' ') {
        result.pop_back();
    }

    return result;
}

std::string Preprocessor::normalizeIdentifiers(const std::string &code) {
    std::string result;
    std::string currentWord;
    result.reserve(code.size());

    for (char c : code) {
        if (isalnum(c) || c == '_') {
            currentWord += c;
        } else {
            if (!currentWord.empty()) {
                if (!isReservedWord(currentWord)) {
                    result += "var";
                } else {
                    result += currentWord;
                }
                currentWord.clear();
            }
            result += c;
        }
    }

    if (!currentWord.empty()) {
        if (!isReservedWord(currentWord)) {
            result += "var";
        } else {
            result += currentWord;
        }
    }

    return result;
}

std::string Preprocessor::removeStringLiterals(const std::string &code) {
    std::string result;
    result.reserve(code.size());
    bool inString = false;
    bool inChar = false;
    bool escape = false;

    for (char c : code) {
        if (!inChar && c == '"' && !escape) {
            inString = !inString;
            if (!inString) result += "\"str\"";
            continue;
        }

        if (!inString && c == '\'' && !escape) {
            inChar = !inChar;
            if (!inChar) result += "'c'";
            continue;
        }

        if (!inString && !inChar) {
            result += c;
        }

        escape = (c == '\\' && !escape);
    }

    return result;
}

std::string Preprocessor::removeNumberLiterals(const std::string &code) {
    std::string result;
    result.reserve(code.size());
    std::string currentNumber;

    for (char c : code) {
        if (isdigit(c) || c == '.') {
            currentNumber += c;
        } else {
            if (!currentNumber.empty()) {
                result += "num";
                currentNumber.clear();
            }
            result += c;
        }
    }

    if (!currentNumber.empty()) {
        result += "num";
    }

    return result;
}

std::string Preprocessor::normalizeCase(const std::string &code) {
    std::string result = code;
    std::transform(result.begin(), result.end(), result.begin(), ::tolower);
    return result;
}

bool Preprocessor::isReservedWord(const std::string &word) const {
    return getReservedWords().find(word) != getReservedWords().end();
}

std::string Preprocessor::replaceAll(std::string str, const std::string &from, const std::string &to) const {
    size_t start_pos = 0;
    while((start_pos = str.find(from, start_pos)) != std::string::npos) {
        str.replace(start_pos, from.length(), to);
        start_pos += to.length();
    }
    return str;
}
