#include "markov.h"

#include <fstream>
#include <sstream>
#include <cstdlib>
using namespace std;

static void splitPrefix(const string& prefix, string outWords[], int order) {
    istringstream iss(prefix);
    for (int i = 0; i < order; i++) {
        if (!(iss >> outWords[i])) outWords[i] = "";
    }
}

string joinWords(const string words[], int startIndex, int count) {
    string result = "";
    for (int i = 0; i < count; i++) {
        result += words[startIndex + i];
        if (i != count - 1) result += " ";
    }
    return result;
}

int readWordsFromFile(string filename, string words[], int maxWords) {
    ifstream inFile(filename);
    if (!inFile.is_open()) return -1;

    int count = 0;
    while (count < maxWords && (inFile >> words[count])) {
        count++;
    }

    inFile.close();
    return count;
}

int buildMarkovChain(const string words[], int numWords, int order,
                     string prefixes[], string suffixes[],
                     int maxChainSize) {
    int count = 0;

    for (int i = 0; i <= numWords - order - 1; i++) {
        if (count >= maxChainSize) break;

        string prefix = joinWords(words, i, order);
        string suffix = words[i + order];

        prefixes[count] = prefix;
        suffixes[count] = suffix;
        count++;
    }

    return count;
}

string getRandomSuffix(const string prefixes[], const string suffixes[],
                       int chainSize, string currentPrefix) {
    int matchCount = 0;

    for (int i = 0; i < chainSize; i++) {
        if (prefixes[i] == currentPrefix) matchCount++;
    }

    if (matchCount == 0) return "";

    int pick = rand() % matchCount;

    int seen = 0;
    for (int i = 0; i < chainSize; i++) {
        if (prefixes[i] == currentPrefix) {
            if (seen == pick) return suffixes[i];
            seen++;
        }
    }

    return "";
}

string getRandomPrefix(const string prefixes[], int chainSize) {
    int index = rand() % chainSize;
    return prefixes[index];
}

string generateText(const string prefixes[], const string suffixes[],
                    int chainSize, int order, int numWords) {
    if (chainSize <= 0 || order <= 0 || numWords <= 0) return "";

    string currentPrefix = getRandomPrefix(prefixes, chainSize);
    string result = currentPrefix;

    string window[3];
    splitPrefix(currentPrefix, window, order);

    for (int i = 0; i < numWords; i++) {
        string nextWord = getRandomSuffix(prefixes, suffixes, chainSize, currentPrefix);
        if (nextWord == "") break;

        result += " " + nextWord;

        if (order == 1) {
            currentPrefix = nextWord;
        } else {
            for (int k = 0; k < order - 1; k++) {
                window[k] = window[k + 1];
            }
            window[order - 1] = nextWord;
            currentPrefix = joinWords(window, 0, order);
        }
    }

    return result;
}