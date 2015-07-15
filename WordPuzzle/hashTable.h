/*
Name: Joo Wan Ro
ID: jr3fs
Lab Section: 104
Filename: hashTable.h
Date: 10/23/14
*/

#ifndef HASHTABLE_H
#define HASHTABLE_H

#include <vector>
#include <cmath>
#include <iostream>
using namespace std;

class hashTable {

 public: 
  // Constructor
  hashTable(int hashTableSize, unsigned int prefixTableSize);

  // Destructor
  ~hashTable();

  // Find function: takes in a string to find
  bool find(const string &strVal, unsigned int key, int tableIndicator);

  // Public insert function: takes in a string to insert
  void insert(const string &strVal, int tableIndicator);

  // Retrieve current hash key
  unsigned int getInitKey(const string &strVal, int tableIndicator);

  // Retrieve next hash key
  unsigned int getNextKey(unsigned int key, int curAscii, int tableIndicator);

  // Prints out the whole hashtable
  void printHashTable();

 private:
  // Hash function: takes in a string and returns a key
  unsigned int hash(const string &strVal, int tableIndicator);

  // FOR SECOND HASH FUNCTION:
  // Second hash function: takes in a string and returns a key
  unsigned int secondHash(const string &strVal, int tableIndicator);

  /* ORIGINAL INSERT FUNCTION, which uses quadratic probing
  // [For insert] Quadratic probing function to deal with collisions
  unsigned int insertQuadProbe(unsigned int key, int index, const string &strVal, int tableIndicator);
  */

  unsigned int insertDoubleHash(unsigned int key, const string &strVal, int tableIndicator);

  /* ORIGINAL FIND FUNCTION, which uses quadratic probing
  // [For find] Quadratic probing function to deal with collisions
  bool findQuadProbe(unsigned int originalKey, unsigned int key, int index, const string &strVal, int tableIndicator);
  */

  // FOR SECOND HASH FUNCTION:
  // [For find] Quadratic probing function to deal with collisions
  bool findDoubleHash(unsigned int originalKey, unsigned int key, const string &strVal, int tableIndicator);

  // Private insert function: takes in a key and a value, string
  void insert(unsigned int key, const string &strVal, int tableIndicator);

  // a vector of strings for the hashtable (OPEN ADDRESSING)
  vector<string> hashtable;

  // a vector of strings for the hashtable that holds prefixes
  vector<string> prefixtable;
  
  // a vector to hold all the next square root of the probe value
  vector<long long> quadtable;

  // a vector to hold all the ascii * prime multiplier values
  vector<int> asciitable;

  // FOR SECOND HASH FUNCTION:
  // a second vector to hold all the ascii * prime multiplier values
  vector<int> asciitable2;
  
  // Constant variables
  const double LOAD_FACTOR;
  const int PRIME_MULTIPLIER;
  const int PREDICTED_MAX_COLLISIONS;
  // An int for the hash table size and prefix table size
  int tableSize;
  unsigned int prefixSize;
  // An int for the numerical ASCII value of the character
  int ascii;
  // An int for the key value to return
  unsigned int key;

  // FOR SECOND HASH FUNCTION:
  // Second prime multiplier added to implement a second hash function
  const int PRIME_MULTIPLIER2;
  unsigned int doubleHashIndex;
  unsigned int retKey;
};

// Non-member functions below
// checkprime and getNextPrime functions obtained from primenumber.cpp file
bool checkprime(unsigned int p);
int getNextPrime (unsigned int n);

#endif
