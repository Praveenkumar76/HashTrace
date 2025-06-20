#ifndef RABIN_KARP_H
#define RABIN_KARP_H

#include <vector>
#include <string>
#include <unordered_set>
#include <stdexcept>

class RabinKarp {
public:
    /**
     * Computes similarity between two texts using Rabin-Karp fingerprinting
     * @param text1 First text to compare
     * @param text2 Second text to compare
     * @param k Length of k-grams to use (default 5)
     * @return Similarity score between 0.0 (no similarity) and 1.0 (identical)
     * @throws std::invalid_argument if k <= 0
     */
    double computeSimilarity(const std::string &text1, const std::string &text2, int k = 5);

private:
    // Large prime number to reduce collision probability
    static constexpr long long BASE = 256;
    static constexpr long long MOD = 1e9 + 7;

    /**
     * Generates all rolling hashes for the given text
     * @param text Input text to hash
     * @param k Length of k-grams
     * @return Vector of hash values for each k-gram
     */
    std::vector<long long> generateHashes(const std::string &text, int k) const;

    /**
     * Generates unique hash set for the given text
     * @param text Input text to hash
     * @param k Length of k-grams
     * @return Set of unique hash values
     */
    std::unordered_set<long long> generateUniqueHashes(const std::string &text, int k) const;
};

#endif // RABIN_KARP_H
