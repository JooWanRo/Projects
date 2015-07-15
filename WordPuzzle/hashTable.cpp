/*
Name: Joo Wan Ro
ID: jr3fs
Lab Section: 104
Filename: hashTable.cpp
Date: 10/23/14
*/

#include "hashTable.h"

// Default Constructor
hashTable::hashTable(int hashTableSize, unsigned int prefixTableSize) 
  : LOAD_FACTOR(0.3), PRIME_MULTIPLIER(101), PRIME_MULTIPLIER2(53), PREDICTED_MAX_COLLISIONS(150000) {
  
  // First set the table size according to the load factor
  hashTableSize = (int) (hashTableSize/LOAD_FACTOR);

  prefixTableSize = (int) (prefixTableSize/LOAD_FACTOR);

  // Find the next prime number
  hashTableSize = getNextPrime(hashTableSize);

  prefixTableSize = getNextPrime(prefixTableSize);

  // Size the hash table appropriately
  hashtable.resize(hashTableSize);

  prefixtable.resize(prefixTableSize);

  // Fill up the quad table
  for (double i = 1; i <= PREDICTED_MAX_COLLISIONS; ++i) {
    quadtable.push_back((long long) pow(i, 2.0));
  }

  // Size the ascii table appropriately
  asciitable.resize(123);

  // Size the ascii table appropriately
  asciitable2.resize(123);

  // Fill up the ascii table
  for (int i = 65; i <= 90; ++i) {
    asciitable.at(i) = i * PRIME_MULTIPLIER;
  }
  for (int i = 97; i <= 122; ++i) {
    asciitable.at(i) = i * PRIME_MULTIPLIER;
  }

  // FOR SECOND HASH FUNCTION:
  // Fill up the second ascii table
  for (int i = 65; i <= 90; ++i) {
    asciitable2.at(i) = i * PRIME_MULTIPLIER2;
  }
  for (int i = 97; i <= 122; ++i) {
    asciitable2.at(i) = i * PRIME_MULTIPLIER2;
  }

  // Initialize hash table sizes and the current hash key
  tableSize = hashTableSize;
  prefixSize = prefixTableSize;
  key = 0;

  // FOR SECOND HASH FUNCTION:
  doubleHashIndex = 1;
  retKey = key;
}

// Destructor
hashTable::~hashTable() {}

/* ORIGINAL FIND FUNCTION, which used quadratic probing

// Find function: takes in a string to find
bool hashTable::find(const string &strVal, unsigned int key, int tableIndicator) {

  // Check which hash table we are working with
  if (tableIndicator == 1) {
    // Check if the element is found; if not, then call QuadProbe
    if (hashtable[key] == strVal) return true;
    else return findQuadProbe(key, key, 0, strVal, tableIndicator);
  } else {
    // Check if the element is found; if not, then call QuadProbe
    if (prefixtable[key] == strVal) return true;
    else return findQuadProbe(key, key, 0, strVal, tableIndicator);
  }

  // Dummy return value
  return true;
}

*/

// Find function via second hash function: takes in a string to find
bool hashTable::find(const string &strVal, unsigned int key, int tableIndicator) {

  // Check which hash table we are working with
  if (tableIndicator == 1) {
    // Check if the element is found; if not, then call QuadProbe
    if (hashtable[key] == strVal) return true;
    else return findDoubleHash(key, key, strVal, tableIndicator);
  } else {
    // Check if the element is found; if not, then call QuadProbe
    if (prefixtable[key] == strVal) return true;
    else return findDoubleHash(key, key, strVal, tableIndicator);
  }

  // Dummy return value
  return true;
}

// Public insert function: takes in a string to insert
void hashTable::insert(const string &strVal, int tableIndicator) {

  // Call the private insert function using the hash function for key
  insert(hash(strVal, tableIndicator), strVal, tableIndicator);

}

// Retrieve current initial hash key
unsigned int hashTable::getInitKey(const string &strVal, int tableIndicator) {
  return hash(strVal, tableIndicator);
}

// Retrieve next hash key
unsigned int hashTable::getNextKey(unsigned int key, int curAscii, int tableIndicator) {

  // Hash function
  key += asciitable.at(curAscii);
 
  key <<= 1;

  // Check which hash table we are working with
  if (tableIndicator == 1) {
    return (key % tableSize);
  } else {
    return (key % prefixSize);
  }
}

// Prints out the whole hashtable (this is a test code method!)
void hashTable::printHashTable() {
  // Print number of collisions
  // cout << "# of Max Collisions: " << collisionNum << endl;
  // cout << "# of Total Collisions: " << collisionTotal << endl;

  // Print number of collisions
  // cout << "# of Max Collisions2: " << collisionNum2 << endl;
  // cout << "# of Total Collisions2: " << collisionTotal2 << endl;

  //  cout << endl;

  /*  cout << endl;
  cout << "hash table: " << endl;
  // Go through each element and print them
  for (int i = 0; i < hashtable.size(); ++i) {
    cout << "Index " << i << ": " << hashtable[i] << endl;
  }
  */

  // Go through each element and print them
  for (int i = 0; i < prefixtable.size(); ++i) {
    cout << "Index " << i << ": " << prefixtable[i] << endl;
  }

  cout << endl;

  /*
  cout << "quad table: " << endl;
  // Go through each element and print them
  for (int i = 0; i < quadtable.size(); ++i) {
    cout << "Index " << i << ": " << quadtable[i] << endl;
  }
  */

  /*
  cout << endl;
  cout << "ascii table: " << endl;
  // Go through each eleemnt and print them
  for (int i = 0; i < asciitable.size(); ++i) {
    cout << "Index " << i << ": " << asciitable[i] << endl;
  }
  */
}

// Hash function: takes in a string and returns a key
unsigned int hashTable::hash(const string &strVal, int tableIndicator) { 

  // Reset key
  key = 0;

  // Multiply each character with the prime multiplier and accumulate
  for (int i = 0; i < strVal.length(); ++i) {
    
    ascii = (int) strVal[i];

    key += asciitable.at(ascii);

    key <<= 1;

  }
  
  // Check which hash table we are working with
  if (tableIndicator == 1) {
    return (key % tableSize);
  } else {
    return (key % prefixSize);
  }
}

// Second hash function: takes in a string and returns a key
unsigned int hashTable::secondHash(const string &strVal, int tableIndicator) { 

  // Reset key
  key = 0;

  // Multiply every other character with the prime multiplier and accumulate
  for (int i = 0; i < strVal.length(); ++i) {
    
    ascii = (int) strVal[i];

    key += asciitable2.at(ascii);

    key >>= 3;

  }
  
  // Check which hash table we are working with
  if (tableIndicator == 1) {
    return (key % tableSize);
  } else {
    return (key % prefixSize);
  }
}

/* ORIGINAL INSERTQUADPROBE FUNCTION, which used quadratic probing

// [For insert] Quadratic probing function to deal with collisions
unsigned int hashTable::insertQuadProbe(unsigned int key, int index, const string &strVal, int tableIndicator) {
  
  // Test code: increment
//  if (tableIndicator == 1) {
//    ++collisionTotal;
//    ++collisionTemp;
//  } else {
//    ++collisionTotal2;
//    ++collisionTemp2;
//  }

  // Compute the next value of key via quadratic probing
  // Return the key if only and if the current spot is empty
  // Check which hash table we are working with
  if (tableIndicator == 1) {

   while (1) {
     key = (key + quadtable[index]) % tableSize;
     
     if (hashtable[key] == "") return key;
    }
    
  } else {

    while (1) {
      key = (key + quadtable[index]) % prefixSize;

      if (prefixtable[key] == "") return key;
    }

  }

}

*/

// [For insert] Double hashing function to deal with collisions
unsigned int hashTable::insertDoubleHash(unsigned int key, const string &strVal, int tableIndicator) {
  
  // Test code: increment
//  if (tableIndicator == 1) {
//    ++collisionTotal;
//    ++collisionTemp;
//  } else {
//    ++collisionTotal2;
//    ++collisionTemp2;
//  }

  doubleHashIndex = 1;
  retKey = key;

  // Compute the next value of key via quadratic probing
  // Return the key if only and if the current spot is empty
  // Check which hash table we are working with
  if (tableIndicator == 1) {

   while (1) {
     // Compute the next value of key via double hashing
     retKey = (key + (doubleHashIndex * secondHash(strVal, tableIndicator))) % tableSize;
	
     // Increment doubleHashIndex
     ++doubleHashIndex;
     
     if (hashtable[retKey] == "") return retKey;
    }
    
  } else {

    while (1) {
      // Compute the next value of key via double hashing
      retKey = (key + (doubleHashIndex * secondHash(strVal, tableIndicator))) % tableSize;
	
      // Increment doubleHashIndex
      ++doubleHashIndex;

      if (prefixtable[retKey] == "") return retKey;
    }

  }

}

/* ORIGINAL FINDQUADPROBE FUNCTION, which used quadratic probing

// [For find] Quadratic probing function to deal with collisions
bool hashTable::findQuadProbe(unsigned int originalKey, unsigned int key, int index, const string &strVal, int tableIndicator) {

  // Check which hash table we are working with
  if (tableIndicator == 1) {

    while (1) {
      // Check first if it is empty, if so, then it is not in the table
      if (hashtable[key] == "") {
	return false;
      } else if (hashtable[key] == strVal) {
	return true;
      } else {
	// Compute the next value of key via quadratic probing
	key = (key + quadtable[index]) % tableSize;

	// Searched the whole table
	if (key == originalKey) return false;
      } 
    }

  } else {
    
    while (1) {
      // Check first if it is empty, if so, then it is not in the table
      if (prefixtable[key] == "") {
	return false;
      } else if (prefixtable[key] == strVal) {
	return true;
      } else {
	// Compute the next value of key via quadratic probing
	key = (key + quadtable[index]) % prefixSize;

	// Searched the whole table
	if (key == originalKey) return false;
      } 
    }

  }
}

*/

// [For find] Double hashing function to deal with collisions
bool hashTable::findDoubleHash(unsigned int originalKey, unsigned int key, const string &strVal, int tableIndicator) {

  doubleHashIndex = 1;
  retKey = key;

  // Check which hash table we are working with
  if (tableIndicator == 1) {

    while (1) {
      // Check first if it is empty, if so, then it is not in the table
      if (hashtable[retKey] == "") {
	return false;
      } else if (hashtable[retKey] == strVal) {
	return true;
      } else {
	// Compute the next value of key via double hashing
	retKey = (key + (doubleHashIndex * secondHash(strVal, tableIndicator))) % tableSize;
	
	// Increment doubleHashIndex
	++doubleHashIndex;
	
	// Searched the whole table
	if (retKey == originalKey) return false;
      } 
    }

  } else {
    
    while (1) {
      // Check first if it is empty, if so, then it is not in the table
      if (prefixtable[retKey] == "") {
	return false;
      } else if (prefixtable[retKey] == strVal) {
	return true;
      } else {
	// Compute the next value of key via double hashing
	retKey = (key + (doubleHashIndex * secondHash(strVal, tableIndicator))) % tableSize;
	
	// Increment doubleHashIndex
	++doubleHashIndex;

	// Searched the whole table
	if (retKey == originalKey) return false;
      } 
    }

  }
}

// Private insert function: takes in a key and a value, string
void hashTable::insert(unsigned int key, const string &strVal, int tableIndicator) {

  // Check which hash table we are working with
  if (tableIndicator == 1) {
    // First check if this initial key is occupied
    if (hashtable[key] != "") {
      // If so, call quadratic probing
      key = insertDoubleHash(key, strVal, tableIndicator);
    }
  } else {
    // First check if this initial key is occupied
    if (prefixtable[key] != "") {
      // If so, call quadratic probing
      key = insertDoubleHash(key, strVal, tableIndicator);
    } 
  }

  // At this point, we now have a valid key to work with
  // Check which hash table we are working with
  if (tableIndicator == 1) {
    // Insert strVal into hashtable
    hashtable[key] = strVal;
  } else {
    // Insert strVal into prefixtable
    prefixtable[key] = strVal;
  }
}

// checkprime and getNextPrime functions obtained from primenumber.cpp file
bool checkprime(unsigned int p) {
    if ( p <= 1 ) // 0 and 1 are not primes; the are both special cases
        return false;
    if ( p == 2 ) // 2 is prime
        return true;
    if ( p % 2 == 0 ) // even numbers other than 2 are not prime
        return false;
    for ( int i = 3; i*i <= p; i += 2 ) // only go up to the sqrt of p
        if ( p % i == 0 )
            return false;
    return true;
}

int getNextPrime (unsigned int n) {
    while ( !checkprime(++n) );
    return n; // all your primes are belong to us
}
