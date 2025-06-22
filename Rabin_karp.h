#ifndef RABIN_KARP_H
#define RABIN_KARP_H

#include <string>
#include <vector>
#include <unordered_set>
#include <unordered_map>

class RabinKarp {
public:
    static constexpr long long BASE = 256;
    static constexpr long long MOD = 1000000007;

    double computeSimilarity(const std::string &text1, const std::string &text2, int k = 5);
    std::vector<std::pair<size_t, size_t>> findMatches(const std::string &text1, const std::string &text2, int k = 5);

private:
    std::vector<long long> generateHashes(const std::string &text, int k) const;
    std::unordered_set<long long> generateUniqueHashes(const std::string &text, int k) const;
};

#endif // RABIN_KARP_H
