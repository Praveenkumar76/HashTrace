#ifndef PREPROCESSOR_H
#define PREPROCESSOR_H

#include <string>
#include <unordered_set>

class Preprocessor {
public:
    Preprocessor();
    std::string preprocess(const std::string &code);

private:
    // Processing steps
    std::string removeComments(const std::string &code);
    std::string normalizeWhitespace(const std::string &code);
    std::string normalizeIdentifiers(const std::string &code);
    std::string removeStringLiterals(const std::string &code);
    std::string removeNumberLiterals(const std::string &code);
    std::string normalizeCase(const std::string &code);

    // Helper functions
    bool isReservedWord(const std::string &word) const;
    std::string replaceAll(std::string str, const std::string &from, const std::string &to) const;

    // Reserved words storage
    static const std::unordered_set<std::string>& getReservedWords();
};

#endif // PREPROCESSOR_H
