// Rabin_karp.h
#ifndef RABIN_KARP_H
#define RABIN_KARP_H

#include <vector>
#include <string>
#include <unordered_set>
#include <algorithm>

class RabinKarp {
public:
    double computeSimilarity(const std::string &text1, const std::string &text2, int k);

private:
    static const int base = 256;
    static const long long mod = 1e9 + 7;

    std::vector<long long> generateHashes(const std::string &text, int k) const;
    std::unordered_set<long long> generateUniqueHashes(const std::string &text, int k) const;
};

#endif // RABIN_KARP_H
