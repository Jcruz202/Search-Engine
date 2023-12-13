//
//  Project 2 - Search Project
//  Course: CS 251, fall 2022. Tues 2pm lab
//  System: Replit
//  Author: Juan Miguel Cruz
//  Net Id: Jcruz85
// 

//for my creative component i did stop words. I didn't use the function for this program reason because it will change the index size and it wont pass the zybooks test case
// I made a set of different stop words and checks if that stop word exist and erase it from the index if it does
#pragma once

#include <iostream>
#include <sstream>
#include <fstream>
#include <algorithm>
#include <bits/stdc++.h>
#include <string>
#include <set>
#include <map>
using namespace std;
//--------------------------------------------------------------------------------------------------
// This function cleanStrings all the string
// It cleanStrings all the text from the body text
string cleanToken(string s) {

  for(size_t i=0; i < s.size(); i++){ // checks if there are any punctuation in the beginning of any text
    if(ispunct(s.at(0))){             // It removes any punctions in the beginning of the text
        s.erase(0, 1); 
    }
    if((s.size() != 0) && ispunct(s.at(s.size()-1))){ // checks if there are any punctuation at the end of any text
        s.erase(s.size()-1, 1);           // It removes any punctions at the end of any text
    }
  }
  
  transform(s.begin(), s.end(), s.begin(), ::tolower); // makes all the characters of any string to a lower case letter
  for(size_t i=0; i < s.size(); i++){
    if(isalpha(s[i])){ //checks if the token is not empty
      return s;
    }
  }
  return "";
}
//--------------------------------------------------------------------------------------------------
// this function gets all the cleanString tokens and store it to a set
set<string> gatherTokens(string text) {
  set<string> tokens;
  
  stringstream ss(text); 
  string sentence;
  //tokenizing the text with spaces
  while(ss >> sentence){ // it gathers all the text, cleanStringing it, and storing it to the set
   string cleanString = cleanToken(sentence); // cleanStrings all the token using the cleanString token function
    if(!cleanString.empty()){ // checks if the token is not empty and if its not empty store it to the set
      tokens.insert(cleanString);
      }
  }
  return tokens;
}
//--------------------------------------------------------------------------------------------------
// build index makes an inverted index where the keys are the tokenized words
// the value of the map is the urls of the word
int buildIndex(string filename, map<string, set<string> >& index) {
  ifstream infile;
  string readString, setHolder, bodyText;
  set<string> bodySet;
  int count = 0;
  
  infile.open(filename);// opens the file
  if(!infile.is_open()){  //checks if the file opens
    cout << "Error handling for invalid file: ";
  }
  
  while(!infile.eof()){
    getline(infile, readString); //  gets the first line of the file which is the url
    setHolder = readString;
    if(setHolder.empty()){ // checks if the url is empty and if it is then do nothing
      continue;
    }
    count++; // counts the number of the url
    // cout << "*" << setHolder << "*" << endl;
    getline(infile, readString); // gets the second line of the file which is the body text
    bodyText = readString;
    
    bodySet = gatherTokens(bodyText); // store all the tokens into a set

    for(auto word : bodySet){ // store all the set of words and the url connected to those words in the map
      index[cleanToken(word)].insert(setHolder);
    }
  }  
    return count; // returns the count number of the url
}
//--------------------------------------------------------------------------------------------------
//stopWords functions checks if the sentence input is a stop words
//if the word is a stop word then it erase it from the index map
void stopWords(map<string, set<string> >& index, string sentence){
  set<string> determiners = {"a", "an", "another", "any", "certain", "each", "every", "her", "his", "its", "its",     "my", "no", "our", "some", "that", "the", "their", "this"};
  set<string> conjunctions = {"and", "but", "or", "yet", "for", "nor", "so"};
  set<string> prepositions = { "as", "aboard", "about", "above", "across", "after", "against", "along", "around", "at", "before", "behind", "below", "beneath", "beside", "between", "beyond", "but", "by", "down", "during", "except", "following", "for", "from", "in", "inside", "into", "like", "minus", "near", "next", "of", "off", "on", "onto", "opposite", "out", "outside" ,"over", "past", "plus", "round", "since", "since", "than", "through", "to", "toward", "under", "underneath", "unlike", "until", "up", "upon", "with", "without"};

  stringstream ss(sentence);
  while(ss >> sentence){
    if(determiners.count(sentence) == 1){ //checks if the word entered exist in the determiners set
      index.erase(sentence);
    }
    if(conjunctions.count(sentence) == 1){ //checks if the word entered exist in the conjuntions set
      index.erase(sentence);
    }
    if(prepositions.count(sentence) == 1){ //checks if the word entered exist in the prepositions set
      index.erase(sentence); //it erases that word from the index map
    }
  }
}
//--------------------------------------------------------------------------------------------------
//this funtion uses the inverted index that was built on the 3rd milestone
// it also uses the algorithm library for multiple words search
set<string> findQueryMatches(map<string, set<string> >& index, string sentence) {
  set<string> result; 
  set<string> set1;
  set<string> set2;
  string holder;
  
  stringstream ss(sentence); //gets the sentence input by words
  // stopWords(index, sentence); // this is my creative component
  while(ss >> sentence){
    // stopWords(index, sentence); // this is my creative component
    if(sentence.at(0) == '+'){ // checks if the first letter of the word is a plus char, cleans the token if it is and store the url for it in set two
      holder = cleanToken(sentence);
      set2 = index.at(holder); 
      result.clear();
      set_intersection(set1.begin(), set1.end(), set2.begin(), set2.end(), inserter(result, result.end())); // using the set intersection function, it gets the url where the first word and the second word appears in the url and store it to the result set
    }
    else if(sentence.at(0) == '-'){ // checks if the first letter of the word is a minus char, cleans the token if it is and store the url for it in set two
      holder = cleanToken(sentence);
      set2 = index.at(holder);
      result.clear();
      set_difference(set1.begin(), set1.end(), set2.begin(), set2.end(), inserter(result, result.end())); // using the set differnce function, it gets the url where the first word appears but not the second word. It stores the result to the result set
    }
    else{ // if it has no + or - in the front it stores the url to set1 and uses the set union funciton for multiple words without + or - in the front of the word
      set1 = index.at(sentence); 
      set_union(set1.begin(), set1.end(), set1.begin(), set1.end(), inserter(result, result.end()));
    }
  }
  return result; 
}
//--------------------------------------------------------------------------------------------------
// searchEngine function have all the previous functions created
// it ask the user to enter a word they would like to search
// it will show the user all the urls that has the word(s) that they enter
void searchEngine(string filename) {
  map<string, set<string> > index;
  string sentence;
  set<string> result;

  cout << "Stand by while building index..." << endl;
  cout << "Indexed " << buildIndex(filename, index) 
       << " pages containing " << index.size() << " unique terms" << endl;
  
  cout << "Enter query sentence (press enter to quit): ";
  getline(cin , sentence);
  
  while(true){
    result = findQueryMatches(index, sentence);
    cout << "Found " << result.size() << " matching pages" << endl;
    for (auto it = result.begin(); it != result.end(); ++it){ //this prints the set of the url
      cout << *it << endl;
    }
    cout << "\nEnter query sentence (press enter to quit): ";
    getline(cin , sentence);
    if(sentence == ""){ // it ends the program when the sentence input is enter
      cout << "Thank you for searching!" << endl;
      exit(0);
    }
  }
  cout << endl;
}