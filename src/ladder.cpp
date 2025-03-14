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
#include <unordered_set>
#include <unordered_map>

using namespace std;

void error(string word1, string word2, string msg) {
    cout << "Error: " << msg << " (" << word1 << ", " << word2 << ")" << endl;
}

bool edit_distance_within(const std::string& str1, const std::string& str2, int d) {
    int m = str1.size(), n = str2.size();
    if (abs(m - n) > d)
        return false;
    
    vector<vector<int>> dp(m + 1, vector<int>(n + 1, 0));
    for (int i = 0; i <= m; i++) {
        dp[i][0] = i;
    }
    for (int j = 0; j <= n; j++) {
        dp[0][j] = j;
    }
    
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

vector<string> get_neighbors(const string& word, const unordered_set<string>& dict) {
    vector<string> neighbors;
    neighbors.reserve(100);
    
    string temp = word;
    for (int i = 0; i < word.size(); i++) {
        char original = temp[i];
        for (char c = 'a'; c <= 'z'; c++) {
            if (c == original) continue;
            temp[i] = c;
            if (dict.count(temp) > 0) {
                neighbors.push_back(temp);
            }
        }
        temp[i] = original;
    }
    
    for (int i = 0; i < word.size(); i++) {
        temp = word;
        temp.erase(i, 1);
        if (dict.count(temp) > 0) {
            neighbors.push_back(temp);
        }
    }
    
    for (int i = 0; i <= word.size(); i++) {
        temp = word;
        temp.insert(i, 1, 'a');
        for (char c = 'a'; c <= 'z'; c++) {
            temp[i] = c;
            if (dict.count(temp) > 0) {
                neighbors.push_back(temp);
            }
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
    
    unordered_set<string> dict(word_list.begin(), word_list.end());
    
    unordered_set<string> forward_visited = {begin_word};
    unordered_set<string> backward_visited = {end_word};
    unordered_map<string, string> forward_parent = {{begin_word, ""}};
    unordered_map<string, string> backward_parent = {{end_word, ""}};
    
    while (!forward_visited.empty() && !backward_visited.empty()) {
        bool forward = forward_visited.size() <= backward_visited.size();
        unordered_set<string>& current_visited = forward ? forward_visited : backward_visited;
        unordered_set<string>& other_visited = forward ? backward_visited : forward_visited;
        unordered_map<string, string>& current_parent = forward ? forward_parent : backward_parent;
        
        unordered_set<string> next_visited;
        
        for (const string& word : current_visited) {
            vector<string> neighbors = get_neighbors(word, dict);
            
            for (const string& neighbor : neighbors) {
                if (other_visited.count(neighbor) > 0) {
                    vector<string> path;
                    string current = word;
                    
                    while (!current.empty()) {
                        if (forward) path.push_back(current);
                        else path.insert(path.begin(), current);
                        current = forward_parent[current];
                    }
                    
                    if (forward) path.push_back(neighbor);
                    else path.insert(path.begin(), neighbor);
                    
                    current = neighbor;
                    while (!backward_parent[current].empty()) {
                        current = backward_parent[current];
                        if (forward) path.push_back(current);
                        else path.insert(path.begin(), current);
                    }
                    
                    return path;
                }
                
                if (current_parent.count(neighbor) == 0) {
                    next_visited.insert(neighbor);
                    current_parent[neighbor] = word;
                }
            }
        }
        
        current_visited = std::move(next_visited);
    }
    
    return vector<string>();
}

void load_words(set<string>& word_list, const string& file_name) {
    ifstream infile(file_name);
    if (!infile) {
        cout << "Error: unable to open file " << file_name << endl;
        return;
    }
    string token;
    while (infile >> token) {
        transform(token.begin(), token.end(), token.begin(), ::tolower);
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