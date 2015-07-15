/*
Name: Joo Wan Ro
ID: jr3fs
Lab Section: 104
Filename: wordPuzzle.cpp
Date: 10/23/14 
*/

// Big theta analysis for Open Addressing -- Quadratic Probing:
// The worst-case for search and find are big theta(n), but on average 
// (or amortized) they are big theta(1), i.e., constant time.  The worse 
// case is beta theta(n) because a collision may occur every time the hash 
// table is probed. However, with a good hash function (to evenly distribute 
// the data as much as possible) and with a moderate load factor, e.g. 0.5, for 
// leaving enough space to minimize a heavily occupied hash table, it will not 
// be the case that probing will have to occur to the point that it 
// has to look at every spot in the table, since there will be plenty of empty
// spots and the data will be more evenly distributed.  For linear probing,
// a rough number of expected probes is approximately 1 / (1 - lamda)^2.

#include "hashTable.h"
#include "timer.h"
#include <iostream>
#include <fstream>
#include <string>
#include <stdlib.h>
#include <stdio.h>
using namespace std;

// Forward declarations
void countDictionary(char* dictionaryFile, int &hashTableSize, int &prefixTableSize);
void genHashTable(char* dictionaryFile, hashTable &hashtable);
void readInTable (char* wordGridFile, int &rows, int &cols);
char* getWordInTable (int startRow, int startCol, int dir, int len,
                      int numRows, int numCols);
void printOutput(int row, int col, int dir, const string &strVal);
bool checkStrLen(int row, int col, int dir, signed int strLen, int rows, int cols);

// We create a 2-D array of some big size, and assume that the table
// read in will be less than this size (a valid assumption for lab 6)
#define MAXROWS 500
#define MAXCOLS 500
char table[MAXROWS][MAXCOLS];

int main(int argc, char* argv[]) {

  // Check if the program has received two files
  // If not, then terminate
  if (argc != 3) {
    cout << "Number of entered files: " << (argc - 1) << endl;
    cout << "This program takes in two files!" << endl;
    exit(1);
  }

  // Code section below are used to generate the main hash table as well as a hash
  // table to hold the prefixes of valid words.  This secondary table also includes
  // the valid word itself.
  // -------------------------------------------------------------------------------
  // Rename file names for code readability
  char* dictionaryFile = argv[1];
  char* wordGridFile = argv[2];

  // Initialize a variable to hold the hash table size
  int hashTableSize = 0;
  int prefixTableSize = 0;

  // First pass of the dictionary file: count the total number of valid words
  countDictionary(dictionaryFile, hashTableSize, prefixTableSize);
  
  // Initialize a hashtable
  hashTable hashtable(hashTableSize, prefixTableSize);

  // Second pass of the dictionary file: hash each valid string into the table
  genHashTable(dictionaryFile, hashtable);
  // -------------------------------------------------------------------------------

  // Test code: print out the hashtable
  // hashtable.printHashTable();

  // Variable initialization
  // -------------------------------------------------------------------------------
  // Setup timer
  timer t;
  // For-loop index variables
  int i, j, k, l, m;
  // Initialize two ints to hold rows and columns from word grid file
  int rows, cols;
  // Initialize a string to hold the current word searched
  string curWord;
  // Initialize a int for number of words found
  int wordNum = 0;
  // Initialize a signed int for the length of curWord string
  int curLen;

  // For optimization -- store the output strings into a vector to print out at the end
  vector<string> outputStrings;
  // This vector stores row, col, and dir information for each output string
  vector<int> outputInfo;
  // -------------------------------------------------------------------------------

  // 3 Variables below are used for hash function accumulation to speed up the
  // the hash function process for subsequent strings in a given search direction.
  // -------------------------------------------------------------------------------
  // Initialize an unsigned int for the current hash value
  unsigned int curHash;
  // Initialize an unsigned int for the current prefix hash value
  unsigned int curPrefix;
  // Initialize an int to hold the ascii value of next character to add onto curHash
  int curAscii;
  // -------------------------------------------------------------------------------

  // Generate word grid
  readInTable(wordGridFile, rows, cols);

  // This is where the hash tables are now in usage
  // -------------------------------------------------------------------------------
  // Start timer
  t.start();

  // Search all words in the grid and search hash table
  // For each row
  for (i = 0; i < rows; ++i) {
    // For each col
    for (j = 0; j < cols; ++j) {
      // For each dir
      for (k = 0; k < 8; ++k) {

	// Check the bounds of the current row/col position
	switch (k) {
	// For north
	case 0:
	  if ( (i - 2) < 0 ) {continue;} break;
	// For north-east
	case 1:
	  if ( (i - 2) < 0 || (j + 2) >= cols ) {continue;} break;
	// For east
	case 2:
	  if ( (j + 2) >= cols ) {continue;} break;	
	// For south-east
	case 3:
	  if ( (i + 2) >= rows || (j + 2) >= cols ) {continue;} break;
	// For south
	case 4: 
	  if ( (i + 2) >= rows ) {continue;} break;
	// For south-west
	case 5:
	  if ( (i + 2) >= rows || (j - 2) < 0 ) {continue;} break; 
	// For west
	case 6:
	  if ( (j - 2) < 0 ) {continue;} break;	
	// For north-west
	case 7:
	  if ( (i - 2) < 0 || (j - 2) < 0 ) {continue;} break; 
	}
        
	// Retrieve initial word from word grid
	curWord = getWordInTable(i, j, k, 3, rows, cols);

	// Get initial hash key
	curHash = hashtable.getInitKey(curWord, 1);

	// Get initial prefix hash key
	curPrefix = hashtable.getInitKey(curWord, 0);

	// Check hash table
	if (hashtable.find(curWord, curHash, 1)) {
	  ++wordNum;
	  printOutput(i, j, k, curWord); 
	}

	// Check prefix table
	if (!hashtable.find(curWord, curPrefix, 0)) continue;

	// Convert string length from unsigned to signed so I can perform computations with it
	curLen = (signed int) curWord.length();

	// Check if the next word is a valid string to be searched
	if (!checkStrLen(i, j, k, curLen, rows, cols)) continue;

        // For each length after 3
        for (l = 4, m = 3; ; ++l, ++m) {

	  // Retrieve next word from word grid
	  curWord = getWordInTable(i, j, k, l, rows, cols);

	  // Get the fourth character to add onto curHash
	  curAscii = (int) curWord[m];

	  // Increment curLen
	  ++curLen;

	  // Get next hash key
	  curHash = hashtable.getNextKey(curHash, curAscii, 1);

	  // Get next prefix hash key
	  curPrefix = hashtable.getNextKey(curPrefix, curAscii, 0);

	  // Check hash table
	  if (hashtable.find(curWord, curHash, 1)) {
	    ++wordNum;
	    
	    // Store the valid word into outputStrings vector
	    outputInfo.push_back(i);
	    outputInfo.push_back(j);
	    outputInfo.push_back(k);
	    // Then add curWord to outputStrings vector
	    outputStrings.push_back(curWord);
	  }

	  // Check prefix table
	  if (!hashtable.find(curWord, curPrefix, 0)) break;

	  // Check if the next word is a valid string to be searched
	  if (!checkStrLen(i, j, k, curLen, rows, cols)) break;
	}
      }
    }
  }

  // Stop timer
  t.stop();
  // -------------------------------------------------------------------------------

  // Print out the results
  int a, b;
  for (a = 0, b = 0; a < outputStrings.size(); ++a, ++b) {
    i = outputInfo[b];
    j = outputInfo[++b];
    k = outputInfo[++b];
    printOutput(i, j, k, outputStrings[a]);
  }

  cout << wordNum << " words found" << endl; 
  cout << "Found all words in " << t.getTime() << " seconds" << endl;

  // In-lab code
  cout << (int) (t.getTime() * 1000) << endl;

  return 0;
}

// This function is used in the first linear pass of the dictionary file in order
// to count the number of words, which are used to generate the hash table sizes.
void countDictionary(char* dictionaryFile, int &hashTableSize, int &prefixTableSize) {
  // Initialize a C++ string to hold the line of data
  string line;

  // Initialize a bool as a flag for word validity
  bool validWord = true;

  // Initialize an int for the numerical ASCII value of the character
  int ascii;

  // Set up the input file stream
  ifstream file(dictionaryFile);

  // If the file is not successfully opened, return false
  if (!file.is_open()) {
    cout << "Dictionary file was not opened successfully!" << endl;
    exit(1);
  }

  // There are no header information; simply read from start to end
  while (getline(file, line)) {

    // Make sure each character is a letter
    for (int i = 0; i < line.length(); ++i) {
      
      ascii = (int) line[i];
      
      if (!( ( (65 <= ascii) && (ascii <= 90) ) || ( (97 <= ascii) && ( ascii <= 122) ) )) {
	validWord = false;
	// Break out of for-loop
	break;
      }
    }

    // If the word is valid, increment hashTableSize and count for prefixTableSize
    if (validWord) {
      ++hashTableSize; 
      if (line.length() == 3) {
	++prefixTableSize;
      } else {
	prefixTableSize += (line.length() - 2);
      }
    }

    // Reset validWord flag
    validWord = true;
  }
}

// This function is used in the second linear pass of the dictionary file where the words
// are now actually inserted into the hash tables.
void genHashTable(char* dictionaryFile, hashTable &hashtable) {
  // Initialize a C++ string to hold the line of data
  string line;

  // Initialize a bool as a flag for word validity
  bool validWord = true;

  // Initialize numerical ASCII value of the character in question
  int ascii;

  // int for tempKey
  unsigned int tempKey;

  // Set up the input file stream
  ifstream file(dictionaryFile);

  // There are no header information; simply read from start to end
  while (getline(file, line)) {

    // Check if the word is at least 3 letters
    if (line.length() < 3) {
      validWord = false;
    } else {
    // Make sure each character is a letter
      for (int i = 0; i < line.length(); ++i) {
      
	ascii = (int) line[i];
      
	if (!( ( (65 <= ascii) && (ascii <= 90) ) || ( (97 <= ascii) && ( ascii <= 122) ) )) {
	  validWord = false;
	  // Break out of for-loop
	  break;
	}
      }
    }

    // If the word is valid, insert into the hash table
    if (validWord) {

      // Fill main hash table of valid words
      hashtable.insert(line, 1);

      // Insert each possible prefix of a word
      for (int i = 3; i <= line.length(); ++i) {

	// Obtain the key so we can find if this prefix already exists in the pre-fix
	// hash table, since there are duplicates
	tempKey = hashtable.getInitKey(line.substr(0, i), 0);
	
	// Check here if the pre-fix is already in the table.  If not, then insert.
	if (hashtable.find(line.substr(0, i), tempKey, 0)) {
	  continue;
	} else {
	  hashtable.insert(line.substr(0, i), 0);
	}

      } 
      
    }
    // Reset validWord flag
    validWord = true;
  }
}

/** This function will read in a grid file, as per the format in the
 * CS 2150 lab 6 document, into a global table[][] array.  It uses C++
 * file streams, and thus requires the the <fstream> #include header.
 *
 * @param filename The file name to read in -- it's assumed to be in
 *                 the file format described in the lab document.
 * @param rows The number of rows as specified in the input file;
 *             as this is a reference, it is set by the function.
 * @param cols The number of columnss as specified in the input file;
 *             as this is a reference, it is set by the function.
 */
void readInTable (char* wordGridFile, int &rows, int &cols) {
    // a C++ string to hold the line of data that is read in
    string line;

    // set up the file stream to read in the file (takes in a C-style
    // char* string, not a C++ string object)
    ifstream file(wordGridFile);

    // If the file is not successfully opened, return false
    if (!file.is_open()) {
      cout << "Word grid file was not opened successfully!" << endl;
      exit(1);
    }

    // the first line is the number of rows: read it in
    file >> rows;
    // cout << "There are " << rows << " rows." << endl;
    getline (file,line); // eats up the return at the end of the line

    // the second line is the number of cols: read it in and parse it
    file >> cols;
    // cout << "There are " << cols << " cols." << endl;
    getline (file,line); // eats up the return at the end of the line

    // the third and last line is the data: read it in
    getline (file,line);

    // close the file
    file.close();

    // convert the string read in to the 2-D grid format into the
    // table[][] array.  In the process, we'll print the table to the
    // screen as well.
    int pos = 0; // the current position in the input data
    for ( int r = 0; r < rows; r++ ) {
        for ( int c = 0; c < cols; c++ ) {
            table[r][c] = line[pos++];
            //cout << table[r][c];
        }
        //cout << endl;
    }
}

/** This function will retrieve a word in a grid of letters in a given
 * direction.  If the end of the grid is encountered before the length
 * of the desired string is reached, then a shorter string will be
 * returned.  The data is retrieved from a global char table[][]
 * array, which is assumed to be defined (and in scope).  NOTE: The
 * return value is a static char[][] variable (for efficiency
 * reasons), so a successive return value will overwrite a previous
 * return value.
 *
 * @return A char* containing the letters in the provided direction
 *         (NOTE: it is returned in a static char array).
 * @param startRow The starting (row,col) position to find the word.
 * @param startCol The starting (row,col) position to find the word.
 * @param dir The direction to move: 0 is north (upwards), 1 is
 *            northeast, and it rotates around clockwise until it
 *            reaches 7 for northwest.
 * @param len The length of the string to return (assuming the edge of
 *            the grid is not reached).
 * @param numRows The number of rows in the global char table[][]
 *                array .
 * @param numCols The number of columns in the global char table[][]
 *                array.
 */
char* getWordInTable (int startRow, int startCol, int dir, int len,
                      int numRows, int numCols) {
    // the static-ness of this variable prevents it from being
    // re-declared upon each function invocataion.  It also prevents it
    // from being deallocated between invocations.  It's probably not
    // good programming practice, but it's an efficient means to return
    // a value.
    static char output[256];
    // make sure the length is not greater than the array size.
    if ( len >= 255 )
        len = 255;
    // the position in the output array, the current row, and the
    // current column
    int pos = 0, r = startRow, c = startCol;
    // iterate once for each character in the output
    for ( int i = 0; i < len; i++ ) {
        // if the current row or column is out of bounds, then break
        if ( (c >= numCols) || (r >= numRows) || (r < 0) || (c < 0) )
            break;
        // set the next character in the output array to the next letter
        // in the table
        output[pos++] = table[r][c];
        // move in the direction specified by the parameter
        switch (dir) { // assumes table[0][0] is in the upper-left
            case 0:
                r--;
                break; // north
            case 1:
                r--;
                c++;
                break; // north-east
            case 2:
                c++;
                break; // east
            case 3:
                r++;
                c++;
                break; // south-east
            case 4:
                r++;
                break; // south
            case 5:
                r++;
                c--;
                break; // south-west
            case 6:
                c--;
                break; // west
            case 7:
                r--;
                c--;
                break; // north-west
        }
    }
    // set the next character to zero (end-of-string)
    output[pos] = 0;
    // return the string (a C-style char* string, not a C++ string
    // object)
    return output;
}

// This method helps with printing out the output according to direction
void printOutput(int row, int col, int dir, const string &strVal) {
  // Print output according to direction
  switch (dir) {  
      case 0:
	cout << "N " << "(" << row << ", " << col << "):\t" << strVal << endl;
	break;

      case 1:
	cout << "NE" << "(" << row << ", " << col << "):\t" << strVal << endl;
	break;

      case 2:
	cout << "E " << "(" << row << ", " << col << "):\t" << strVal << endl;
	break;

      case 3:
	cout << "SE" << "(" << row << ", " << col << "):\t" << strVal << endl;
	break;

      case 4:
	cout << "S " << "(" << row << ", " << col << "):\t" << strVal << endl;
	break;

      case 5:
	cout << "SW" << "(" << row << ", " << col << "):\t" << strVal << endl;
	break;

      case 6:
	cout << "W " << "(" << row << ", " << col << "):\t" << strVal << endl;
	break;

      case 7:
	cout << "NW" << "(" << row << ", " << col << "):\t" << strVal << endl;
	break;
  }
}

// This method helps determine the validity of string length for next search iteration
bool checkStrLen(int i, int j, int k, signed int curLen, int rows, int cols) {
  // Check to see if this is the last string to be searched in current dir
  switch (k) {
    // For north
  case 0:
    if ( (i - curLen) < 0 ) {return false;}
    break;
    // For north-east
  case 1:
    if ( (i - curLen) < 0 || (j + curLen) >= cols ) {return false;}
    break;
    // For east
  case 2:
    if ( (j + curLen) >= cols ) {return false;}
    break;
    // For south-east
  case 3:
    if ( (i + curLen) >= rows || (j + curLen) >= cols ) {return false;}
    break;
    // For south 
  case 4:
    if ( (i + curLen) >= rows ) {return false;}
    break;
    // For south-west
  case 5:
    if ( (i + curLen) >= rows || (j - curLen) < 0 ) {return false;}
    break;
    // For west
  case 6:
    if ( (j - curLen) < 0  ) {return false;}
    break;
    // For north-west
  case 7:
    if ( (i - curLen) < 0 || (j - curLen) < 0 ) {return false;}
    break;
  }
  return true;
}
