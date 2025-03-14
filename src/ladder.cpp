#include "ladder.h"
#include <iostream>
#include <fstream>
#include <queue>
#include <set>
#include <vector>
#include <string>
#include <cstdlib>
#include <algorithm>
#include <cmath>

using namespace std;

void error(string word1, string word2, string msg) {
    cout << "Error: " << msg << " (" << word1 << ", " << word2 << ")" << endl;
    exit(1);
}

bool edit_distance_within(const string& str1, const string& str2, int d) {
    int m = str1.size(), n = str2.size();
    if (abs(m - n) > d)
        return false;
    vector<vector<int>> dp(m + 1, vector<int>(n + 1, 0));
    for (int i = 0; i <= m; i++) dp[i][0] = i;
    for (int j = 0; j <= n; j++) dp[0][j] = j;
    for (int i = 1; i <= m; i++) {
        for (int j = 1; j <= n; j++) {
            int cost = (str1[i - 1] == str2[j - 1]) ? 0 : 1;
            dp[i][j] = min({ dp[i - 1][j] + 1,
                             dp[i][j - 1] + 1,
                             dp[i - 1][j - 1] + cost });
        }
        if (*min_element(dp[i].begin(), dp[i].end()) > d)
            return false;
    }
    return dp[m][n] <= d;
}

bool is_adjacent(const string& word1, const string& word2) {
    if (word1 == word2) return true;
    int len1 = word1.length(), len2 = word2.length();
    if (abs(len1 - len2) > 1)
        return false;
    int diff = 0, i = 0, j = 0;
    while (i < len1 && j < len2) {
        if (word1[i] != word2[j]) {
            if (++diff > 1)
                return false;
            if (len1 > len2)
                ++i;
            else if (len2 > len1)
                ++j;
            else { ++i; ++j; }
        } else {
            ++i; ++j;
        }
    }
    if (i < len1 || j < len2)
        ++diff;
    return diff == 1;
}

vector<string> get_neighbors(const string &word, const set<string>& dict) {
    vector<string> neighbors;
    for (int i = 0; i < word.size(); i++) {
        char orig = word[i];
        for (char c = 'a'; c <= 'z'; c++) {
            if (c == orig) continue;
            string candidate = word;
            candidate[i] = c;
            if (dict.find(candidate) != dict.end())
                neighbors.push_back(candidate);
        }
    }
    for (int i = 0; i < word.size(); i++) {
        string candidate = word;
        candidate.erase(i, 1);
        if (dict.find(candidate) != dict.end())
            neighbors.push_back(candidate);
    }
    for (int i = 0; i <= word.size(); i++) {
        for (char c = 'a'; c <= 'z'; c++) {
            string candidate = word;
            candidate.insert(i, 1, c);
            if (dict.find(candidate) != dict.end())
                neighbors.push_back(candidate);
        }
    }
    return neighbors;
}

vector<string> generate_word_ladder(const string& begin_word, const string& end_word, const set<string>& word_list) {
    if (begin_word == end_word) {
        error(begin_word, end_word, "The start and end words must be different.");
        return vector<string>();
    }
    if (word_list.find(end_word) == word_list.end()) {
        error(begin_word, end_word, "The end word is not in the dictionary.");
        return vector<string>();
    }
    set<string> dict = word_list;
    queue<vector<string>> ladder_queue;
    ladder_queue.push({begin_word});
    dict.erase(begin_word);
    while (!ladder_queue.empty()) {
        vector<string> current = ladder_queue.front();
        ladder_queue.pop();
        string last_word = current.back();
        vector<string> neighbors = get_neighbors(last_word, dict);
        for (string candidate : neighbors) {
            dict.erase(candidate);
            vector<string> new_ladder = current;
            new_ladder.push_back(candidate);
            if (candidate == end_word)
                return new_ladder;
            ladder_queue.push(new_ladder);
        }
    }
    return vector<string>();
}

void load_words(set<string>& word_list, const string& file_name) {
    ifstream infile(file_name);
    if (!infile) {
        cout << "Error: unable to open file " << file_name << endl;
        exit(1);
    }
    string token;
    while (infile >> token) {
        word_list.insert(token);
    }
    infile.close();
}

void print_word_ladder(const vector<string>& ladder) {
    if (ladder.empty()) {
        cout << "No word ladder found." << endl;
        return;
    }
    cout << "Word ladder found: ";
    for (size_t i = 0; i < ladder.size(); i++) {
        cout << ladder[i] << " ";
    }
    cout << "\n";
}

#define my_assert(e) { cout << #e << ((e) ? " passed" : " failed") << endl; }

void verify_word_ladder() {
    set<string> word_list;
    load_words(word_list, "words.txt");
    my_assert(generate_word_ladder("cat", "dog", word_list).size() == 4);
    my_assert(generate_word_ladder("marty", "curls", word_list).size() == 6);
    my_assert(generate_word_ladder("code", "data", word_list).size() == 6);
    my_assert(generate_word_ladder("work", "play", word_list).size() == 6);
    my_assert(generate_word_ladder("sleep", "awake", word_list).size() == 8);
    my_assert(generate_word_ladder("car", "cheat", word_list).size() == 4);
}
