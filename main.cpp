#include <iostream>
#include <fstream>

#include <algorithm>
#include <string>
#include <utility>
#include <vector>
#include <set>

class JumbleSolver {
private:
    std::string query_string_;
    std::string word_list_filename_;

public:
    JumbleSolver(std::string word_list_filename, std::string query_string) {
        word_list_filename_ = std::move(word_list_filename);
        query_string_ = std::move(query_string);
        std::transform(query_string_.begin(), query_string_.end(), query_string_.begin(), ::tolower);
        solveJumble();
    }

    static std::string sortString(std::string word) {
        std::sort(word.begin(), word.end());
        return word;
    }

    static std::set<std::string> getAllSubStrings(std::string s) {
        int input_string_size = s.size();

        std::vector<std::set<std::string>> vec_string_set;
        std::vector<std::string> character;

        for (int i = input_string_size-2; i >= 0; --i) {
            character.emplace_back(std::string(1, s[i]));
            std::set<std::string> strings_i;
            std::string full_string = std::string(1, s[i]);
            for (int j = i+1; j < input_string_size; ++j) {
                strings_i.insert(std::string(1, s[i])+std::string(1, s[j]));
                full_string +=  std::string(1, s[j]);
            }
            strings_i.insert(full_string);
            vec_string_set.push_back(strings_i);
        }

        for(int i = 1; i < character.size(); i++) {
            for(int j = i-1; j >=0; j--){
                std::set<std::string>  vec_string_set_prev = vec_string_set[j];
                for (const auto& k : vec_string_set_prev) {
                    vec_string_set[i].insert(character[i]+k);
                }
            }
        }

        std::set<std::string> all_substrings;
        for(const auto& i : vec_string_set) {
            for(const auto& j : i){
                all_substrings.insert(j);
            }
        }
        return all_substrings;
    }

    void printSubAnagramInDictionary(const std::string& string_after_sorting) {
        std::string checkPerWord;
        std::ifstream all_words(word_list_filename_);

        if (all_words) {
            while (getline(all_words,
                           checkPerWord)) {
                std::string Ch = sortString(checkPerWord);
                if (Ch == string_after_sorting) {
                    if(checkPerWord != query_string_){
                        std::cout << checkPerWord << std::endl <<std::endl;
                    }
                }
            }
            all_words.close();
        } else {
            std::cout << "Error in the word file path" << std::endl;
        }
    }

    void solveJumble() {
        std::string userEnteredAfterSorting;

        userEnteredAfterSorting = sortString(query_string_);

        std::set<std::string> all_sub_strings = getAllSubStrings(userEnteredAfterSorting);

        for(auto i : all_sub_strings){
            printSubAnagramInDictionary(i);
        }
    }
};

int main(int argc, char** argv) {
    std::cout << "---- Inputs ----" << std::endl;
    std::cout << "Word List File Name: " << argv[1] << std::endl;
    std::cout << "Query String: " << argv[2] << std::endl;
    std::cout << "---- Solver Output ----" << std::endl;
    std::string word_file_name = argv[1];
    std::string jumbled_string = argv[2];
    JumbleSolver JS(word_file_name, jumbled_string);
    return 0;
}
