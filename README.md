# JumbleSolver
A Program to solve Jumble Challenge

## Instructions
Clone or download this repo (preferably in your Ubuntu system) 
### Cloning
```
git clone git@github.com:SubMishMar/JumbleSolver.git
```
and inside JumbleSolver Folder build this project using the following commands:
### Building the project
```
cd JumbleSolver
mkdir build
cd build
cmake ..
make
```
### Running the project
Inside the build folder run
```
./JumbleSolver ../data/word_list.txt dog

```
Here `../data/word_list.txt` is the dictionary of words and `dog` is the query string which can be anything you want, like rat, car, anagram, and so on.

### Documentation
```
The process has two important steps
1. Find all sorted sub-strings/anagrams of the queried string
2. Look for those sub-strings/anagrams in the word-list or the dictionary
```
```
Step 1 is to determine all sorted sub-strings/anagram of the queried string
Logic:
consider a sorted string abcde
Step 1a: Looping from behind, find all 2 lettered and complete strings possible
  Looping over     Set of two lettered and complete strings
             d     {de}
             c     {cd, ce cde}
             b     {bc, bd, be, bcde}
             a     {ab, ac, ad, ae, abcde}
The first column above is stored as a character array (vector)
The second column above is stored as an array (vector) of set of strings
The next step is the insert to the above sets more possible alphabetically sorted substrings

Step 1b: Loop over the character array starting from the second element (c)
             append at the front the character for all elements of the previous sets of strings and insert
             the appended strings to the current set of strings
  Looping over     Set of two lettered and complete strings
             c     {cd, ce cde}  + insert{append_at_front(c, {de})} // The set data structure ensures that elements are unique
             b     {bc, bd, be, bcde} + insert{append_at_front(b, {cd, ce, cde}) + append_at_front(b, {de})}
             a     {ab, ac, ad, ae, abcde} + insert(append_at_front(a, {bc, bd, be, bcde}) + append_at_front(a, {cd, ce, cde})
                                           + append_at_front(a, {de}))
The first column above is stored as a character array (vector)
The second column above is stored as an array (vector) of set of strings
Finally the second column is stored as a single set of all alphabetically sorted sub-strings of the query string.
```
```
Step 2 is to look for the matches of the sorted sub strings in the word-list
  For that I store the word list in a dictionary where the key to the word is its alphabetically sorted counterpart
  I use a multimap data structure because a key of sorted characters may point to different words in the word list. 
  
### Complexity Ananlysis
The total time complexity is shared among 3 steps: 
  1. The time taken to find all substrings (O(n^2), where n is the size of the queried string (eg. dog)).
  2. The time taken to store all the words in the program's memory (O(m), where m is the number of words in the word list).
  3. The time taken to look up those substrings in the dictionary (This is done in constant time O(1) for every substring).
  


