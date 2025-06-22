#include "Preprocessor.h"
#include <algorithm>
#include <cctype>
#include <unordered_set>
#include <sstream>

namespace {
const std::unordered_set<std::string>& getReservedWordsImpl() {
    static const std::unordered_set<std::string> RESERVED_WORDS = {
        // C & C++
        "auto", "break", "case", "char", "const", "continue", "default",
        "do", "double", "else", "enum", "extern", "float", "for", "goto",
        "if", "inline", "int", "long", "register", "restrict", "return",
        "short", "signed", "sizeof", "static", "struct", "switch", "typedef",
        "union", "unsigned", "void", "volatile", "while", "alignas",
        "alignof", "asm", "bool", "catch", "class", "compl", "const_cast",
        "constexpr", "decltype", "delete", "dynamic_cast", "explicit",
        "export", "false", "friend", "mutable", "namespace", "new", "noexcept",
        "nullptr", "operator", "private", "protected", "public", "reinterpret_cast",
        "static_assert", "static_cast", "template", "this", "thread_local",
        "throw", "true", "try", "typeid", "typename", "using", "virtual",
        "wchar_t",

        // Java
        "abstract", "assert", "boolean", "byte", "catch", "char", "class",
        "const", "default", "do", "double", "else", "enum", "extends", "final",
        "finally", "float", "for", "goto", "if", "implements", "import",
        "instanceof", "int", "interface", "long", "native", "new", "null",
        "package", "private", "protected", "public", "return", "short",
        "static", "strictfp", "super", "switch", "synchronized", "this",
        "throw", "throws", "transient", "try", "void", "volatile", "while",
        "true", "false",

        // Python (3.x)
        "False", "None", "True", "and", "as", "assert", "async", "await",
        "break", "class", "continue", "def", "del", "elif", "else", "except",
        "finally", "for", "from", "global", "if", "import", "in", "is",
        "lambda", "nonlocal", "not", "or", "pass", "raise", "return",
        "try", "while", "with", "yield",

        // JavaScript
        "arguments", "await", "break", "case", "catch", "class", "const",
        "continue", "debugger", "default", "delete", "do", "else", "enum",
        "eval", "export", "extends", "false", "finally", "for", "function",
        "if", "implements", "import", "in", "instanceof", "interface", "let",
        "new", "null", "package", "private", "protected", "public", "return",
        "static", "super", "switch", "this", "throw", "throws", "transient",
        "true", "try", "typeof", "var", "void", "volatile", "while", "with",
        "yield"
    };
    return RESERVED_WORDS;
}
}

Preprocessor::Preprocessor() = default;

const std::unordered_set<std::string>& Preprocessor::getReservedWords() {
    return getReservedWordsImpl();
}

std::string Preprocessor::preprocess(const std::string &code) {
    if (code.empty()) {
        return "";
    }

    std::string processed = code;

    // Apply preprocessing steps in order
    processed = removeComments(processed);
    processed = removeStringLiterals(processed);
    processed = removeNumberLiterals(processed);
    processed = normalizeWhitespace(processed);
    processed = normalizeCase(processed);
    processed = normalizeIdentifiers(processed);

    return processed;
}

std::string Preprocessor::removeComments(const std::string &code) {
    std::string result;
    result.reserve(code.size());
    bool inLineComment = false;
    bool inBlockComment = false;
    bool inString = false;
    bool inChar = false;
    bool escape = false;

    for (size_t i = 0; i < code.size(); ++i) {
        char c = code[i];

        // Handle escape sequences
        if (escape) {
            escape = false;
            if (!inLineComment && !inBlockComment) {
                result += c;
            }
            continue;
        }

        if (c == '\\' && (inString || inChar)) {
            escape = true;
            if (!inLineComment && !inBlockComment) {
                result += c;
            }
            continue;
        }

        // Handle string literals
        if (!inLineComment && !inBlockComment) {
            if (c == '"' && !inChar) {
                inString = !inString;
                result += c;
                continue;
            }
            if (c == '\'' && !inString) {
                inChar = !inChar;
                result += c;
                continue;
            }
        }

        // Skip comment processing if we're inside a string
        if (inString || inChar) {
            if (!inLineComment && !inBlockComment) {
                result += c;
            }
            continue;
        }

        // Handle line comments
        if (!inBlockComment && i + 1 < code.size() && c == '/' && code[i + 1] == '/') {
            inLineComment = true;
            i++; // Skip the second '/'
            continue;
        }

        // Handle block comments
        if (!inLineComment && i + 1 < code.size() && c == '/' && code[i + 1] == '*') {
            inBlockComment = true;
            i++; // Skip the '*'
            continue;
        }

        // End line comment
        if (inLineComment && c == '\n') {
            inLineComment = false;
            result += c; // Keep the newline
            continue;
        }

        // End block comment
        if (inBlockComment && i + 1 < code.size() && c == '*' && code[i + 1] == '/') {
            inBlockComment = false;
            i++; // Skip the '/'
            continue;
        }

        // Add character if not in comment
        if (!inLineComment && !inBlockComment) {
            result += c;
        }
    }

    return result;
}

std::string Preprocessor::normalizeWhitespace(const std::string &code) {
    std::string result;
    result.reserve(code.size());
    bool inSpace = false;
    bool atLineStart = true;

    for (char c : code) {
        if (std::isspace(c)) {
            if (c == '\n') {
                // Preserve line breaks
                if (!result.empty() && result.back() != '\n') {
                    result += '\n';
                }
                inSpace = false;
                atLineStart = true;
            } else if (!inSpace && !atLineStart) {
                // Replace multiple spaces with single space, but not at line start
                result += ' ';
                inSpace = true;
            }
        } else {
            result += c;
            inSpace = false;
            atLineStart = false;
        }
    }

    // Remove trailing whitespace
    while (!result.empty() && std::isspace(result.back())) {
        result.pop_back();
    }

    return result;
}

std::string Preprocessor::normalizeIdentifiers(const std::string &code) {
    std::string result;
    std::string currentWord;
    result.reserve(code.size());

    for (char c : code) {
        if (std::isalnum(c) || c == '_') {
            currentWord += c;
        } else {
            if (!currentWord.empty()) {
                // Check if it's a reserved word (case-insensitive)
                std::string lowerWord = currentWord;
                std::transform(lowerWord.begin(), lowerWord.end(), lowerWord.begin(), ::tolower);

                if (!isReservedWord(lowerWord) && !isNumeric(currentWord)) {
                    result += "var";
                } else {
                    result += lowerWord; // Use lowercase version
                }
                currentWord.clear();
            }
            result += c;
        }
    }

    // Handle last word
    if (!currentWord.empty()) {
        std::string lowerWord = currentWord;
        std::transform(lowerWord.begin(), lowerWord.end(), lowerWord.begin(), ::tolower);

        if (!isReservedWord(lowerWord) && !isNumeric(currentWord)) {
            result += "var";
        } else {
            result += lowerWord;
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
        if (escape) {
            escape = false;
            continue;
        }

        if (c == '\\' && (inString || inChar)) {
            escape = true;
            continue;
        }

        if (!inChar && c == '"') {
            if (!inString) {
                inString = true;
                result += "\"str\"";
            } else {
                inString = false;
            }
            continue;
        }

        if (!inString && c == '\'') {
            if (!inChar) {
                inChar = true;
                result += "'c'";
            } else {
                inChar = false;
            }
            continue;
        }

        if (!inString && !inChar) {
            result += c;
        }
    }

    return result;
}

std::string Preprocessor::removeNumberLiterals(const std::string &code) {
    std::string result;
    result.reserve(code.size());
    std::string currentNumber;
    bool inNumber = false;

    for (size_t i = 0; i < code.size(); ++i) {
        char c = code[i];

        if (std::isdigit(c) || (c == '.' && inNumber)) {
            if (!inNumber) {
                inNumber = true;
            }
            currentNumber += c;
        } else if (inNumber && (c == 'f' || c == 'F' || c == 'l' || c == 'L' ||
                                c == 'u' || c == 'U')) {
            // Handle number suffixes
            currentNumber += c;
        } else {
            if (inNumber) {
                result += "num";
                currentNumber.clear();
                inNumber = false;
            }
            result += c;
        }
    }

    // Handle number at end of string
    if (inNumber) {
        result += "num";
    }

    return result;
}

std::string Preprocessor::normalizeCase(const std::string &code) {
    std::string result = code;

    // Only normalize non-string content
    bool inString = false;
    bool inChar = false;
    bool escape = false;

    for (size_t i = 0; i < result.size(); ++i) {
        char c = result[i];

        if (escape) {
            escape = false;
            continue;
        }

        if (c == '\\' && (inString || inChar)) {
            escape = true;
            continue;
        }

        if (!inChar && c == '"') {
            inString = !inString;
            continue;
        }

        if (!inString && c == '\'') {
            inChar = !inChar;
            continue;
        }

        if (!inString && !inChar) {
            result[i] = std::tolower(c);
        }
    }

    return result;
}

bool Preprocessor::isReservedWord(const std::string &word) const {
    return getReservedWords().find(word) != getReservedWords().end();
}

bool Preprocessor::isNumeric(const std::string &str) const {
    if (str.empty()) return false;

    size_t start = 0;
    if (str[0] == '+' || str[0] == '-') {
        start = 1;
        if (str.length() == 1) return false;
    }

    bool hasDigit = false;
    bool hasDot = false;

    for (size_t i = start; i < str.length(); ++i) {
        char c = str[i];
        if (std::isdigit(c)) {
            hasDigit = true;
        } else if (c == '.' && !hasDot) {
            hasDot = true;
        } else if (i == str.length() - 1 &&
                   (c == 'f' || c == 'F' || c == 'l' || c == 'L' ||
                    c == 'u' || c == 'U')) {
            // Number suffix
            continue;
        } else {
            return false;
        }
    }

    return hasDigit;
}

std::string Preprocessor::replaceAll(std::string str, const std::string &from, const std::string &to) const {
    size_t start_pos = 0;
    while ((start_pos = str.find(from, start_pos)) != std::string::npos) {
        str.replace(start_pos, from.length(), to);
        start_pos += to.length();
    }
    return str;
}
