#ifndef RECCHECK
// For debugging
#include <iostream>
// For std::remove
#include <algorithm> 
#include <map>
#include <set>
#include <string> 
#include <vector> 
#endif

#include "wordle.h"
#include "dict-eng.h"
using namespace std;


// Add prototypes of helper functions here

// Brainstorm: we are gonna loop through the input once and document all the blank spots
// Then, we get all the floatings letters 
// In our form_word_one we are going to try all the possible spots for the floats 
// In our form_word_two we are going to try all A-Z for the remaining empty spots
// Note that we are going to pass our dictionary and the target string set all the way down to form_word_two so that we can check all the possibilities 
// We would have to do this by passing by reference
size_t fact(size_t n); 
void form_word_one(set<string>& target, const set<string>& dict, size_t count, size_t full, string input, char the_char, size_t char_idx, vector<size_t> positions, string floats, size_t loop_times); 
void form_word_two(set<string>& target, const set<string>& dict, size_t count, size_t full, size_t pos, string input, vector<size_t> remaining); 
void check_the_word(set<string>& target, const set<string>& dict, string input);

// Definition of primary wordle function
std::set<std::string> wordle(
    const std::string& in,
    const std::string& floating,
    const std::set<std::string>& dict)
{
    // Add your code here
    set<string> target; 
    vector<size_t> blanks; 
    string the_string = in; 
    string the_floats = floating; 
    //size_t float_count = floating.length() + 1;
    for(size_t i = 0; i < in.length(); i++){
        if(in[i] == '-'){
            blanks.push_back(i); 
        }
    }
    //Getting all the blanks
    if(floating.empty()){
        form_word_two(target, dict, 0, 26^(blanks.size()), 0, the_string, blanks); 
        return target; 
    }
    size_t max = fact(blanks.size()) / fact(blanks.size() - floating.length()); 
    form_word_one(target, dict, 0, max, the_string, the_floats[0], 0, blanks, the_floats, blanks.size()); 
    return target; 

}

// Define any helper functions here

size_t fact(size_t n) {
    if (n == 0 || n == 1)
    return 1;
    else
    return n * fact(n - 1);
}

void form_word_one(set<string>& target, const set<string>& dict, size_t count, size_t full, string input, char the_char, size_t char_idx, vector<size_t> positions, string floats, size_t loop_times){

    if(count == full){
        return; 
    }
    for(size_t i = 0; i < loop_times; i++){
        input[positions[i]] = the_char; 
        //cout << "loop one now the input is: " << input << endl; 
        if(char_idx == floats.length() -1){
            vector<size_t> remaining; 
            for(size_t j = 0; j < input.length(); j++){
                if(input[j] == '-'){
                    remaining.push_back(j); 
                }
            }
            size_t the_max = 26^(remaining.size()); 
            //cout << "im in here going into function two, and the input is: " << input << endl; 
            form_word_two(target, dict, 0, the_max, 0, input, remaining); 
        }
        else {
            vector<size_t> new_positions = positions;
            new_positions.erase (new_positions.begin() + i); 
            form_word_one(target, dict, count+1, full, input, floats[char_idx+1], char_idx+1, new_positions, floats, loop_times - 1); //place the next float char
        }
        input[positions[i]] = '-'; 
    }
}

void form_word_two(set<string>& target, const set<string>& dict, size_t count, size_t full, size_t pos, string input, vector<size_t> remaining){

    //target.insert(input);
    //return; 
    if(count == full){
        return; 
    }
    if(remaining.empty()){
        check_the_word(target, dict, input);
        return; 
    }
    for(size_t i = 0; i < 26; i++){
        input[remaining[pos]] = 'a' + i; 
        if(pos == remaining.size() - 1){
            check_the_word(target, dict, input); 
        }
        else{
            form_word_two(target, dict, count+1, full, pos+1, input, remaining); 
        }
        input[remaining[pos]] = '-'; 
    }

}

void check_the_word(set<string>& target, const set<string>& dict, string input){

    if(dict.find(input) != dict.end()){
        target.insert(input);
    }
    else{
        return; 
    }
}

