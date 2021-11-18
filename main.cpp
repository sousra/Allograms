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

std::vector<std::string> stringAnagrams(const std::string& sentence, const std::map<std::string, int>& dict)
{
    // split sentence into words
    std::string word {};
    std::vector<std::string> sentenceVec {};
    for (char i : sentence)
    {
        if (std::isspace(i)) {
            sentenceVec.push_back(word);
            word.clear();
        }
        else {
            word.push_back(i);
        }
    }
    if (!word.empty()) {
        sentenceVec.push_back(word);
    }

    // find anagrams for each word in sentense
    std::vector<std::multimap<int, std::string, std::greater<int>>> wordsAnagrams {};
    for (const auto& w : sentenceVec)
    {
        std::multimap<int, std::string, std::greater<int>> buf {};
        for (auto& el : dict)
        {
            if (w.size() == el.first.size() && std::is_permutation(w.begin(), w.end(), el.first.begin())) {
                buf.insert(std::make_pair(el.second, el.first));
            }
        }
        if (buf.empty()) {
            throw std::logic_error("No such word in dictionary!");
        }
        wordsAnagrams.push_back(buf);
    }

    // find all combinations of words
    std::multimap<int, std::vector<std::string>, std::greater<int>> buf {};
    std::vector<std::multimap<int, std::string, std::greater<int>>::iterator> itVec {}; // contain wordsAnagrams elements iterators
    for (auto& i : wordsAnagrams) {
        itVec.push_back(i.begin());
    }
    bool isEnd = false;
    do {
        std::vector<std::string> strVec {};
        int strWeight = 0;
        for (auto it : itVec) {
            strVec.push_back(it->second);
            strWeight += it->first;
        }
        buf.insert(std::make_pair(strWeight, strVec));

        for (int i = wordsAnagrams.size() - 1; i >= 0; --i)
        {
            if (std::distance(wordsAnagrams[i].begin(), itVec[i]) != wordsAnagrams[i].size() - 1) {
                ++itVec[i];
                break;
            }
            else {
                if (i == 0) {
                    isEnd = true;
                    break;
                }
                for (int j = wordsAnagrams.size() - 1; j >= i; --j) {
                    itVec[j] = wordsAnagrams[j].begin();
                }
            }
        }
    } while (!isEnd);

    // find anagrams for sentence
    std::vector<std::string> sentenceAnagrams {};
    for (auto& p : buf)
    {
        std::sort(p.second.begin(), p.second.end());
        do {
            std::string anagram {};
            for (auto& w : p.second) {
                anagram.append(w + ' ');
            }
            anagram.pop_back(); // delete space from end
            sentenceAnagrams.push_back(anagram);
        } while (std::next_permutation(p.second.begin(), p.second.end()));
    }
    return sentenceAnagrams;
}