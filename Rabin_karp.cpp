#include "Rabin_karp.h"
#include <cmath>
#include <algorithm>
#include <stdexcept>

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

std::vector<std::pair<size_t, size_t>> RabinKarp::findMatches(const std::string &text1, const std::string &text2, int k) {
    std::vector<std::pair<size_t, size_t>> matches;
    if (k <= 0 || text1.empty() || text2.empty()) return matches;

    // Ensure k doesn't exceed text lengths
    const size_t min_len = std::min(text1.length(), text2.length());
    if (static_cast<size_t>(k) > min_len) {
        k = static_cast<int>(min_len);
    }

    auto hashes1 = generateHashes(text1, k);
    auto hashes2 = generateHashes(text2, k);

    if (hashes1.empty() || hashes2.empty()) return matches;

    // Create a map of hash values to their positions in text1
    std::unordered_map<long long, std::vector<size_t>> hashPositions;
    for (size_t i = 0; i < hashes1.size(); ++i) {
        hashPositions[hashes1[i]].push_back(i);
    }

    // Find matches in text2
    for (size_t j = 0; j < hashes2.size(); ++j) {
        auto it = hashPositions.find(hashes2[j]);
        if (it != hashPositions.end()) {
            // Add all positions where this hash occurs in text1
            for (size_t pos1 : it->second) {
                matches.emplace_back(pos1, j);
            }
        }
    }

    // Sort matches by position in first text
    std::sort(matches.begin(), matches.end());

    return matches;
}

std::vector<long long> RabinKarp::generateHashes(const std::string &text, int k) const
{
    std::vector<long long> hashes;
    const size_t text_len = text.length();

    if (text_len < static_cast<size_t>(k) || k <= 0) {
        return hashes;
    }

    long long hash = 0;
    long long power = 1;

    // Precompute power = BASE^(k-1) % MOD
    for (int i = 0; i < k - 1; ++i) {
        power = (power * BASE) % MOD;
    }

    // Compute initial window hash
    for (int i = 0; i < k; ++i) {
        hash = (hash * BASE + static_cast<unsigned char>(text[i])) % MOD;
    }
    hashes.push_back(hash);

    // Rolling hash for remaining windows
    for (size_t i = k; i < text_len; ++i) {
        // Remove leftmost character
        long long leftChar = static_cast<unsigned char>(text[i - k]);
        hash = (hash - (leftChar * power) % MOD + MOD) % MOD;

        // Add new character
        hash = (hash * BASE + static_cast<unsigned char>(text[i])) % MOD;
        hashes.push_back(hash);
    }

    return hashes;
}

std::unordered_set<long long> RabinKarp::generateUniqueHashes(const std::string &text, int k) const
{
    auto hashes = generateHashes(text, k);
    return {hashes.begin(), hashes.end()};
}
