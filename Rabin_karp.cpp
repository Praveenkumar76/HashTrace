// Rabin_karp.cpp
#include "Rabin_karp.h"
#include <cmath>

std::vector<long long> RabinKarp::generateHashes(const std::string &text, int k) const
{
    std::vector<long long> hashes;
    if (text.length() < static_cast<size_t>(k)) return hashes;

    long long hash = 0;
    long long power = 1;

    // Precompute power = base^(k-1) % mod
    for (int i = 0; i < k - 1; ++i) {
        power = (power * base) % mod;
    }

    // Initial window hash
    for (int i = 0; i < k; ++i) {
        hash = (hash * base + text[i]) % mod;
    }
    hashes.push_back(hash);

    // Rolling window
    for (size_t i = k; i < text.length(); ++i) {
        // Remove leftmost character
        hash = (hash - text[i - k] * power % mod + mod) % mod;
        // Add new character
        hash = (hash * base + text[i]) % mod;
        hashes.push_back(hash);
    }

    return hashes;
}

std::unordered_set<long long> RabinKarp::generateUniqueHashes(const std::string &text, int k) const
{
    auto hashes = generateHashes(text, k);
    return {hashes.begin(), hashes.end()};
}

double RabinKarp::computeSimilarity(const std::string &text1, const std::string &text2, int k)
{
    if (text1.empty() || text2.empty() || k <= 0) return 0.0;

    auto set1 = generateUniqueHashes(text1, k);
    auto set2 = generateUniqueHashes(text2, k);

    if (set1.empty() && set2.empty()) return 1.0;
    if (set1.empty() || set2.empty()) return 0.0;

    size_t intersection = 0;
    for (const auto &h : set1) {
        if (set2.count(h)) intersection++;
    }

    size_t unionSize = set1.size() + set2.size() - intersection;
    return static_cast<double>(intersection) / unionSize;
}
