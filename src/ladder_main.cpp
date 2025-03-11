#include "ladder.h"
#include <iostream>
#include <string>
#include <set>
#include <vector>

using namespace std;

int main() {
    set<string> dictionary;
    load_words(dictionary, "words.txt");

    string startWord, endWord;
    cout << "Enter start word: ";
    cin >> startWord;
    cout << "Enter end word: ";
    cin >> endWord;

    vector<string> ladder = generate_word_ladder(startWord, endWord, dictionary);

    if (ladder.empty()) {
        cout << "No word ladder found from \"" << startWord << "\" to \"" << endWord << "\"." << endl;
    } else {
        cout << "Word ladder: ";
        print_word_ladder(ladder);
    }

    return 0;
}
