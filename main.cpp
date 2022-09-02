#include <iostream>
#include <fstream>

#include <algorithm>
#include <string>
#include <utility>
#include <vector>
#include <set>
#include <map>

/// The process has two important parts
/// Find all sorted sub-strings/anagrams of the queried string
/// Look for those sub-strings/anagrams in the word-list or the dictionary

class JumbleSolver {
private:
    std::string query_string_; /// Variable to hold the query string passed as second parameter
    std::string word_list_filename_; /// Variable to hold filename of the dictionary/word-list
    std::multimap<std::string, std::string> mp_sorted_unsorted_dictionary; /// A multimap to hold all the words in the
                                                                           /// dictionary/word-list such that if we query a sorted
                                                                           /// string of characters (key), it will return
                                                                           /// all available words which use those
                                                                           /// characters.
                                                                           /// multimap is used because, for eg.
                                                                           /// "dog" and "god" are held behind the same key
                                                                           /// "dgo", one key may hold several values

public:
    /// Class constructor
    JumbleSolver(std::string word_list_filename, std::string query_string) {
        word_list_filename_ = std::move(word_list_filename);
        query_string_ = std::move(query_string);
        std::transform(query_string_.begin(), query_string_.end(), query_string_.begin(),
                       ::tolower); /// transform the query string to lower-case

        /// Load all words from the txt file into the multimap dictionary
        loadWordsFromDictionary();

        /// Solve Jumble
        solveJumble();
    }

    /// Function to sort the characters in a string
    static std::string sortString(std::string word) {
        std::sort(word.begin(), word.end());
        return word;
    }

    /// The following Function determines all alphabetically sorted sub-strings of the queried string s
    /// Logic:
    /// consider a sorted string abcde
    /// Step 1: Looping from behind, find all 2 lettered and complete strings possible
    ///  Looping over     Set of two lettered and complete strings
    ///             d     {de}
    ///             c     {cd, ce cde}
    ///             b     {bc, bd, be, bcde}
    ///             a     {ab, ac, ad, ae, abcde}
    /// The first column above is stored as a character array (vector)
    /// The second column above is stored as an array (vector) of set of strings
    /// The next step is the insert to the above sets more possible alphabetically sorted substrings

    /// Step 2: Loop over the character array starting from the second element (c)
    ///             append at the front the character for all elements of the previous sets of strings and insert
    ///             the appended strings to the current set of strings
    ///  Looping over     Set of two lettered and complete strings
    ///             d     {de}
    ///             c     {cd, ce cde}  + insert{append_at_front(c, {de})} // The set data structure ensures that elements are unique
    ///             b     {bc, bd, be, bcde} + insert{append_at_front(b, {cd, ce, cde}) + append_at_front(b, {de})}
    ///             a     {ab, ac, ad, ae, abcde} + insert(append_at_front(a, {bc, bd, be, bcde}) + append_at_front(a, {cd, ce, cde})
    ///                                        + append_at_front(a, {de}))
    /// The first column above is stored as a character array (vector)
    /// The second column above is stored as an array (vector) of set of strings
    /// Finally the second column is stored as a single set of all alphabetically sorted sub-strings of the query string
    static std::set<std::string> getAllSubStrings(std::string s) {
        int input_string_size = s.size();

        std::vector<std::set<std::string>> vec_string_set;
        std::vector<std::string> character;

        /// Step 1
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

        /// Step 2
        for(int i = 1; i < character.size(); i++) {
            for(int j = i-1; j >=0; j--){
                std::set<std::string>  vec_string_set_prev = vec_string_set[j];
                for (const auto& k : vec_string_set_prev) {
                    vec_string_set[i].insert(character[i]+k);
                }
            }
        }

        /// Putting all sub-strings together
        std::cout << "--- All Substrings ---" << std::endl;
        std::set<std::string> all_substrings;
        for(const auto& i : vec_string_set) {
            for(const auto& j : i){
                all_substrings.insert(j);
                std::cout << j << " ";
            }
        }
        std::cout << std::endl << std::endl;

        return all_substrings;
    }

    /// Function to load all words from the dictionary and store it in a dictionary data structure
    /// Each word is stored behind a key. The key is formed by a string which equals the sorted characters of the word.
    /// Multimap data structure is used so that a key can point to different words, for eg. "dgo" is key to "dog" and "god".
    void loadWordsFromDictionary() {
        std::string word_in_dictionary;
        std::ifstream all_words_filehandler(word_list_filename_);

        if (all_words_filehandler) {
            while (getline(all_words_filehandler, word_in_dictionary)) {
                std::string word_in_dictionary_sorted = sortString(word_in_dictionary);
                mp_sorted_unsorted_dictionary.insert({word_in_dictionary_sorted, word_in_dictionary});
            }
            all_words_filehandler.close();
        } else {
            std::cout << "Error in the word file path" << std::endl;
        }
    }

    /// Output all sub-anagrams stored in the dictionary data-structure
    /// Logic is to search the sorted query substring in the dictionary
    /// Since we use the map data structure we do not have to loop through it, we just need to find() it.
    void outputSubAnagramFoundInDictionary(const std::string& sorted_query_substring) {
        auto it = mp_sorted_unsorted_dictionary.find(sorted_query_substring);
        /// If the iterator to the queried sorted substring is found then display it
        if(it!=mp_sorted_unsorted_dictionary.end()) {
            /// Iterates through the key and returns iterators to all the values stored at that key
            auto iterator = mp_sorted_unsorted_dictionary.equal_range(sorted_query_substring);
            for (auto itr = iterator.first; itr != iterator.second; ++itr) {
                /// We want to display those values which are not equal to the queried string
                /// For eg. We do not want to display "dog" if we entered "dog" as the queried string.
                if(itr->second != query_string_) {
                    std::cout << itr->second << std::endl;
                }
            }
        }
    }

    /// Function to solve Jumble
    void solveJumble() {
        std::string query_string_sorted;
        query_string_sorted = sortString(query_string_);
        std::set<std::string> all_sub_strings_of_query_string = getAllSubStrings(query_string_sorted);

        std::cout << "---- Solver Output ----" << std::endl;
        for(auto i : all_sub_strings_of_query_string) {
            outputSubAnagramFoundInDictionary(i);
        }
    }
};

int main(int argc, char** argv) {
    std::cout << "---- Inputs ----" << std::endl;
    std::cout << "Word List File Name: " << argv[1] << std::endl;
    std::cout << "Query String: " << argv[2] << std::endl<<std::endl;
    std::string word_file_name = argv[1];
    std::string queried_string = argv[2];
    /// Create an object of the Jumble Solve Class and pass the word list filename and the queried string as the
    /// arguments to its constructor.
    JumbleSolver JS(word_file_name, queried_string);
    return 0;
}
