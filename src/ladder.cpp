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
    cout << "Error: " << msg << "\n"
         << "Word 1: " << word1 << "\n"
         << "Word 2: " << word2 << endl;
    exit(1);
}

bool edit_distance_within_helper(const string& str1, int index1, const string& str2, int index2, int remaining) {
    if (remaining < 0) return false;
    if (index1 == str1.size() && index2 == str2.size()) return true;
    if (index1 == str1.size()) return ((int)str2.size() - index2) <= remaining;
    if (index2 == str2.size()) return ((int)str1.size() - index1) <= remaining;
    if (str1[index1] == str2[index2])
        return edit_distance_within_helper(str1, index1 + 1, str2, index2 + 1, remaining);
    else
        return edit_distance_within_helper(str1, index1 + 1, str2, index2, remaining - 1) ||
               edit_distance_within_helper(str1, index1, str2, index2 + 1, remaining - 1) ||
               edit_distance_within_helper(str1, index1 + 1, str2, index2 + 1, remaining - 1);
}

bool edit_distance_within(const string& str1, const string& str2, int d) {
    return edit_distance_within_helper(str1, 0, str2, 0, d);
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

vector<string> generate_word_ladder(const string& begin_word, const string& end_word, const set<string>& word_list) {
    if (begin_word == end_word) {
        error(begin_word, end_word, "Start and end words are the same.");
        return vector<string>();
    }
    if (word_list.find(end_word) == word_list.end()) {
        error(begin_word, end_word, "The end word is not in the dictionary.");
        return vector<string>();
    }
    queue<vector<string>> ladder_queue;
    ladder_queue.push({begin_word});
    set<string> visited;
    visited.insert(begin_word);
    while (!ladder_queue.empty()) {
        vector<string> current_ladder = ladder_queue.front();
        ladder_queue.pop();
        string last_word = current_ladder.back();
        for (const string& candidate : word_list) {
            if (visited.find(candidate) == visited.end() && is_adjacent(last_word, candidate)) {
                visited.insert(candidate);
                vector<string> new_ladder = current_ladder;
                new_ladder.push_back(candidate);
                if (candidate == end_word)
                    return new_ladder;
                ladder_queue.push(new_ladder);
            }
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
