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
