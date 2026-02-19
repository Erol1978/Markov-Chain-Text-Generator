#include "markov.h"

#include <iostream>
#include <cstdlib>
#include <ctime>
using namespace std;

int main() {
    srand((unsigned)time(0));

    string filename;
    int order;
    int numWords;

    cout << "Enter input filename: ";
    cin >> filename;

    cout << "Enter order (1, 2, or 3): ";
    cin >> order;

    cout << "Enter number of words to generate: ";
    cin >> numWords;

    if (order < 1 || order > 3) {
        cout << "Error: order must be 1, 2, or 3.\n";
        return 0;
    }

    const int MAX_WORDS = 100000;
    const int MAX_CHAIN = 100000;

    static string words[MAX_WORDS];
    static string prefixes[MAX_CHAIN];
    static string suffixes[MAX_CHAIN];

    int count = readWordsFromFile(filename, words, MAX_WORDS);
    if (count == -1) {
        cout << "Error: could not open file.\n";
        return 0;
    }

    if (count <= order) {
        cout << "Error: file does not have enough words.\n";
        return 0;
    }

    int chainSize = buildMarkovChain(words, count, order, prefixes, suffixes, MAX_CHAIN);
    if (chainSize <= 0) {
        cout << "Error: chain is empty.\n";
        return 0;
    }

    cout << "\nGenerated text:\n";
    string output = generateText(prefixes, suffixes, chainSize, order, numWords);
    cout << output << "\n";

    return 0;
}