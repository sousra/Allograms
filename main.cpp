
#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <map>
#include <algorithm>

std::vector<std::string> stringAnagrams(const std::string& sourceStr, const std::map<std::string, int>& dict);

//! argv[1] - input.txt, argv[2] - dictionary.txt
int main(int argc, char** argv)
{
    if (argc < 3) {
        std::cerr << "No arguments for main!\n";
    }

    // Create dictionary
    std::map<std::string, int> dict {};
    std::ifstream dictFile(argv[2]);
    if (!dictFile) {
        std::cerr << "File open error!\n";
    }
    std::string word;
    int weight;
    while (dictFile >> word) {
        if (word.empty()) { continue; }
        word.pop_back(); // delete ','
        dictFile >> weight;
        dict.insert(std::make_pair(word, weight));
    }
    dictFile.close();

    std::ifstream inputFile(argv[1]);
    if (!inputFile) {
        std::cerr << "File open error!\n";
    }
    std::string sentence;
    while (std::getline(inputFile, sentence)) {
        if (sentence.empty()) {
            continue;
        }
        std::vector<std::string> anagrams = stringAnagrams(sentence, dict);
        std::cout << "Sentence - " << sentence << std::endl;
        std::cout << "All anagrams:\n";
        for (auto& i : anagrams) {
            std::cout << i << '\n';
        }
        std::cout << "\n\n";
    }
    inputFile.close();
    return 0;
}

std::vector<std::string> stringAnagrams(const std::string& sourceStr, const std::map<std::string, int>& dict)
{
    // split sourceStr into words
    std::string word {};
    std::vector<std::string> wordsVec {};
    for (char i : sourceStr) {
        if (std::isspace(i)) {
            wordsVec.push_back(word);
            word.clear();
        }
        else {
            word.push_back(i);
        }
    }
    if (!word.empty()) {
        wordsVec.push_back(word);
    }

    // find anagrams for sourseStr words
    for (auto& w : wordsVec) {
        bool inDict = false;
        int weight = -1;
        for (auto& i : dict) {
            if (std::is_permutation(w.begin(), w.end(), i.first.begin()) && i.second > weight) {
                w = i.first;
                weight = i.second;
                inDict = true;
            }
        }
        if (!inDict) {
            throw std::logic_error("No such word in dictionary!");
        }
    }

    // find anagrams for sourceStr
    std::sort(wordsVec.begin(), wordsVec.end());
    std::vector<std::string> anagrams {};
    bool isEnd = false;
    while (!isEnd) {
        std::string anagram {};
        for (auto& w : wordsVec) {
            anagram.append(w + ' ');
        }
        anagram.pop_back(); // delete space from end
        anagrams.push_back(anagram);
        isEnd = !std::next_permutation(wordsVec.begin(), wordsVec.end());
    }

    return anagrams;
}

/*
dera liwl hatt ralet kobo I
tras nca ese ouy shti
wedrar ti saeelp
ym hree lsvei fendri
 */