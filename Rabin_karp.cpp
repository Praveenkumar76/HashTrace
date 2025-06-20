#include "Rabin_karp.h"
#include <cmath>
#include <algorithm>

double RabinKarp::computeSimilarity(const std::string &text1, const std::string &text2, int k)
{
    if (k <= 0) {
        throw std::invalid_argument("k-gram size must be positive");
    }

    if (text1.empty() && text2.empty()) return 1.0;
    if (text1.empty() || text2.empty()) return 0.0;

    // Use minimum length to prevent generating more hashes than necessary
    const size_t min_len = std::min(text1.length(), text2.length());
    if (static_cast<size_t>(k) > min_len) {
        k = static_cast<int>(min_len);
    }

    auto set1 = generateUniqueHashes(text1, k);
    auto set2 = generateUniqueHashes(text2, k);

    if (set1.empty() && set2.empty()) return 1.0;
    if (set1.empty() || set2.empty()) return 0.0;

    // Compute Jaccard similarity coefficient
    size_t intersection = 0;
    for (const auto &h : set1) {
        if (set2.count(h)) intersection++;
    }

    size_t union_size = set1.size() + set2.size() - intersection;
    return static_cast<double>(intersection) / union_size;
}

std::vector<long long> RabinKarp::generateHashes(const std::string &text, int k) const
{
    std::vector<long long> hashes;
    const size_t text_len = text.length();

    if (text_len < static_cast<size_t>(k)) {
        return hashes;
    }

    long long hash = 0;
    long long power = 1;

    // Precompute power = BASE^(k-1) % MOD
    for (int i = 0; i < k - 1; ++i) {
        power = (power * BASE) % MOD;
    }

    // Initial window hash
    for (int i = 0; i < k; ++i) {
        hash = (hash * BASE + text[i]) % MOD;
    }
    hashes.push_back(hash);

    // Rolling window
    for (size_t i = k; i < text_len; ++i) {
        // Remove leftmost character
        hash = (hash - text[i - k] * power % MOD + MOD) % MOD;
        // Add new character
        hash = (hash * BASE + text[i]) % MOD;
        hashes.push_back(hash);
    }

    return hashes;
}

std::unordered_set<long long> RabinKarp::generateUniqueHashes(const std::string &text, int k) const
{
    auto hashes = generateHashes(text, k);
    return {hashes.begin(), hashes.end()};
}
