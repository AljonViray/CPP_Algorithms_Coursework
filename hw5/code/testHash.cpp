// Code by Aljon Viray (86285526)
// ICS 46 - Homework #5 - testHash.cpp

#include <iostream>
#include <fstream>
#include <string>
#include <cmath>
using namespace std;
#include "Timer.h"


/////////////////////  Classes  /////////////////////


struct Hasher
	{ virtual int hash(string s, int N) = 0; };

struct GeneralStringHasher: public Hasher
{	
	int hash (string key, int N) // O(1)
	{
		const unsigned shift = 6;
		const unsigned zero = 0;
		unsigned mask = ~zero >> (32-shift);  // low 6 bits on
		unsigned result = 0;
		int len = min(int(key.size()), 6);
		for (int i = 0; i < len; i++)
			result = (result << shift) | (key[i] & mask);
		return result % N;
	}
};

struct SumHasher: public Hasher
{	
	int hash (string key, int N) // O(N)
	{
		int result = 0;
		for (int i = 0; i < key.size(); i++)
			result += key[i];
		return abs(result) % N;
	}
};

struct ProdHasher: public Hasher
{	
	int hash (string key, int N) // O(N)
	{
		int result = 1;
		for (int i = 0; i < key.size(); i++)
			result *= key[i];
		return abs(result) % N;
	}
};

//////////////////////////////////////////////////////

class ChainedHashTable 
{
	private:
			struct ListNode 
			{
				string key;
				int value;
				ListNode* next;
		
				ListNode (string new_key, int new_value, ListNode* new_next) 
					: key(new_key), value(new_value), next(new_next) {}
			};

			ListNode** table;
			int capacity;
			Hasher& hasher;

			void copyTable (ListNode** old_table, ChainedHashTable& new_table, int capacity)
			{
				for (int i = 0; i < capacity; i++) {
					ListNode* current = old_table[i];
					while (current != nullptr) {  // Copy through the LinkedList
						new_table.insert(current->key, current->value);
						current = current->next;
					}
				}
			}


	public:
			int* chain_lengths;
			int max_chain = 0;
			int min_chain= 0;
			float avg_chain = 0;
			float std_dev = 0;

			ChainedHashTable (int max_len, Hasher& myHasher) // O(N)
				: hasher{myHasher}, capacity{max_len}
			{ 
				table = new ListNode*[max_len];
				for (int i = 0; i < capacity; i++)
					table[i] = nullptr;
				chain_lengths = new int[capacity];
			}
			 

			ChainedHashTable (ChainedHashTable& myTable) // O(N)
				: hasher{myTable.hasher}, capacity{myTable.capacity}
			{ 
				table = new ListNode*[myTable.capacity];
				for (int i = 0; i < capacity; i++)
					table[i] = nullptr;
				chain_lengths = new int[capacity];
				copyTable(myTable.table, *this, capacity); // Copy contents of the old table to new one
			}


			~ChainedHashTable() // O(N)
			{
				for (int i = 0; i < capacity; i++) {
					ListNode* temp;
					while (table[i] != nullptr)
					{
						temp = table[i]->next;
						delete table[i];
						table[i] = temp;
					}
				}
				delete[] table;
				delete[] chain_lengths;
			}


			int& operator[] (string word) // Best: O(1), Worst: O(L) // L = LinkedList size
			{
				int i = hasher.hash(word, capacity);
				ListNode* current = table[i];
				while (current != nullptr) {
					if (current->key == word) return current->value;
					current = current->next;
				}
				return table[i]->value;
			}

			
			void insert (string word, int val = 1) // Best: O(1), Worst: O(L) // L = LinkedList size
			{
				int i = hasher.hash(word, capacity);
				if (table[i] == nullptr) // If it's the first key at hash #
					table[i] = new ListNode(word, val, nullptr);

				else if (table[i] != nullptr) {	// Check if it's a duplicate key
					ListNode* current = table[i];
					while (current != nullptr) {
						if (current->key == word) { 
							current->value = val;
							return;
						}
						current = current->next;
					}
					table[i] = new ListNode(word, val, table[i]); // If it's a brand new key
				}
			}


			int find (string word) // Best: O(1), Worst: O(L) // L = LinkedList size
			{
				int i = hasher.hash(word, capacity);
				if (table[i]->key == word) return table[i]->value; // If at head of LinkedList

				else { // If further into LinkedList
					ListNode* saved;
					ListNode* current = table[i];
					while (current != nullptr){
						saved = current;
						current = current->next;
						if (current->key == word) return current->value;
					}
					return -1; // If reached nullptr and can't find the word
				}
			}


			void remove (string word) // Best: O(1), Worst: O(L) // L = LinkedList size
			{
				int i = hasher.hash(word, capacity);
				ListNode* saved;

				if (table[i]->key == word) { // If the first ListNode at hash index is correct
					saved = table[i]->next;
					delete table[i];
					table[i] = saved;
				}

				else { // If anywhere else in hash index
					ListNode* current = table[i];
					while (current != nullptr) {
						saved = current;
						current = current->next;
						if (current->key == word) {
							saved->next = current->next;
							delete current;
							break;
						}
					}
				}
			}


			void print() // O(N)
			{
				for (int i = 0; i < capacity; i++) {
					if (table[i] == nullptr) continue; // Skip if nullptr

					cout << i << ": (" << table[i]->key << ", " << table[i]->value << ")->"; // Print head
					if (table[i]->next == nullptr) {
						cout << "nullptr" << endl; 
						continue;
					}
					
					ListNode* current = table[i]->next; // Print LinkedList items after head
					while (current != nullptr) {
						cout << "(" << current->key << ", " << current->value << ")->";
						current = current->next;
					}
					cout << "nullptr" << endl;
				}
			}

			
			void calculateChainLengths() // O(N)
			{
				// Max/Min Chain Lengths
				min_chain = max_chain = 0;
				int count;
				for (int i = 0; i < capacity; i++) {
					count = 0;
					if (table[i] != nullptr) {
						ListNode* current = table[i]->next;
						while (current != nullptr) {
							count++;
							current = current->next;
						}
					}
					chain_lengths[i] = count;
					if (count < min_chain) min_chain = count;
					else if (count > max_chain) max_chain = count;
				}

				// Avg Chain Length
				avg_chain = 0;
				for (int i = 0; i < capacity; i++) {
					avg_chain += chain_lengths[i];
				}
				avg_chain = avg_chain/capacity;

				cout << "min = " << min_chain << "; max = " << max_chain << "; avg = " << avg_chain;
			}


			void calculateStdDev() // O(N)
			{
				double var_sum = 0;
				for (int i = 0; i < capacity; i++)
					var_sum += pow((chain_lengths[i] - avg_chain), 2);
				float variance = var_sum/capacity;
				std_dev = sqrt(variance);
				cout << "; std_dev = " << std_dev << endl;
			}
};


//////////////////////  Testing Functions  ///////////////////////////


void testConstructor (Hasher& hasher)
	{ ChainedHashTable table(5000, hasher); }

void testCopyConstructor (Hasher& hasher)
{ 
	ChainedHashTable table(5000, hasher);
	table.insert("word", 10);
	ChainedHashTable copy(table);
}

void testDestructor (Hasher& hasher)
{
	ChainedHashTable table(5000, hasher); // Destructs on its own
	table.insert("word", 10);
}

void testIndexing (Hasher& hasher)
{
	ChainedHashTable table(5000, hasher);
	table.insert("word");
	cout << table["word"] << endl;
	table["word"]++;
	cout << table["word"] << endl;
}

void testInsert (ChainedHashTable& table, Hasher& hasher)
{
	table.insert("one", 1);
	table.insert("two", 2);
	table.insert("three", 3);
	table.insert("one", 11);
	cout << table["one"] << table["two"] << table["three"] << endl;
	table.print();
}

void testFind (ChainedHashTable& table, Hasher& hasher)
{
	cout << table.find("one") << endl;
	cout << table.find("two") << endl;
	cout << table.find("three") << endl;
}

void testRemove (ChainedHashTable& table, Hasher& hasher)
{
	table.remove("one");
	table.remove("two");
	table.remove("three");
}

void testHash (const char* inputFileName, Hasher& hasher)
{ 
	testConstructor(hasher);
	testCopyConstructor(hasher);
	testDestructor(hasher);
	testIndexing(hasher);

	ChainedHashTable table(5000, hasher);
	testInsert(table, hasher);
	testFind(table, hasher);
	testRemove(table, hasher);
}


//////////////  Input File Functions  //////////////


void insertAll (const char* filename, ChainedHashTable& table, int partition, int k) 
// Best: O(N), Worst: O(N*L)
{
	ifstream infile(filename);
	string word;
	Timer timer;
	double time;
	timer.start();
	for (int i = 0; i < partition; i++) {
		infile >> word;
		table.insert(word);
	}
	timer.elapsedUserTime(time);
	cout << "insertAll with " << k << "/10 words: " << time << " sec" << endl;
	infile.close();
	//table.print();
}


void findAll (const char* filename, ChainedHashTable& table, int partition, int k) 
// Best: O(N), Worst: O(N*L)
{
	ifstream infile(filename);
	string word;
	Timer timer;
	double time;
	timer.start();
	for (int i = 0; i < partition; i++) {
		infile >> word;
		table.find(word);
	}
	timer.elapsedUserTime(time);
	cout << "findAll with " << k << "/10 words: " << time << " sec" << endl;
	infile.close();
}


void removeAll (const char* filename, ChainedHashTable& table, int partition, int k) 
// Best: O(N), Worst: O(N*L)
{
	ifstream infile(filename);
	string word;
	Timer timer;
	double time;
	timer.start();
	for (int i = 0; i < partition; i++) {
		infile >> word;
		table.remove(word);
	}
	timer.elapsedUserTime(time);
	cout << "removeAll with " << k << "/10 words: " << time << " sec" << endl;
	infile.close();
}


/////////////////////  Main Function  /////////////////////////


int main ()
{
	GeneralStringHasher hasher;
	ChainedHashTable table(5000, hasher);
	const char* file = "random.txt";
	//const char* file = "words.txt";
	//testHash(file, hasher);

	cout << "----------------------------------" << endl;

	// Parts 1 & 2 //
	cout << "---" << file << "---" << endl;
	for (int k = 1; k < 11; k++) {
		int partition = (k*45392)/10;
		insertAll(file, table, partition, k);
		findAll(file, table, partition, k);
		removeAll(file, table, partition, k);
	}
	cout << endl;

	// Parts 3 & 4 //
	cout << "---Hash Function 1 Statistics---" << endl;
	insertAll(file, table, 45392, 10);
	table.calculateChainLengths();
	table.calculateStdDev();
	findAll(file, table, 45392, 10);
	removeAll(file, table, 45392, 10);

	cout << endl << "---Hash Function 2 Statistics---" << endl;
	SumHasher hasher_2;
	ChainedHashTable table_2(5000, hasher_2);
	insertAll(file, table_2, 45392, 10);
	table_2.calculateChainLengths();
	table_2.calculateStdDev();
	findAll(file, table_2, 45392, 10);
	removeAll(file, table_2, 45392, 10);

	cout << endl << "---Hash Function 3 Statistics---" << endl;
	ProdHasher hasher_3;
	ChainedHashTable table_3(5000, hasher_3);
	insertAll(file, table_3, 45392, 10);
	table_3.calculateChainLengths();
	table_3.calculateStdDev();
	findAll(file, table_3, 45392, 10);
	removeAll(file, table_3, 45392, 10);

	cout << "----------------------------------" << endl;

	return 0;
}
