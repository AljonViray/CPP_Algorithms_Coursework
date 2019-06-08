// Code by Aljon Viray (86285526)
// ICS 46 - Homework #3 - main.cpp

#include <iostream>
#include <fstream>
#include <string>
using namespace std;
#include "Timer.h"


//////////////////////  Classes  ////////////////////////


class SortedList
{
	public:
		virtual void insert (string word) {}
		virtual bool find (string word) {}
		virtual void remove (string word) {}
		virtual void print() {}
};


class SortedArrayList : public SortedList
{
	string* buf;
	int capacity;
	int size = 0;

	public:
		// O(1)
		SortedArrayList (int max_len)
		{
			capacity = max_len;
			buf = new string[max_len];
		}

		// O(N)
		void insert (string word)
		{
			if (isFull()) cout << ("Note: cannot insert word, array list is full!");

			else if (isEmpty())		// For very first word
			{
				buf[0] = word;
				size++;
			}

			else
			{
				int i = binary_search(word, 0, size-1);	// Would cause O(log N) but...
				copy_down(i);	// ...this overrides to O(N)
				buf[i] = word;
				size++;
			}
		}

		// O(log N)
		bool find (string word)
		{
			if (isEmpty()) return false;

			if (buf[binary_search(word, 0, size-1)] == word) return true;
			else return false;
		}

		// O(N)
		void remove (string word)
		{
			if (isEmpty()) cout << ("Note: Cannot remove word, array list is empty!");

			int i = binary_search(word, 0, size-1);
			buf[i] = "";
			copy_up(i);		// Causes O(N)
			buf[capacity-1] = ""; // Removes duplicate at end of array from copy_up
			size--;
		}

		// O(N)
		void print ()
		{
			for (int i = 0; i < capacity; i++)
				{cout << i << ": " << buf[i] << endl;}
		}

		// O(1)
		~SortedArrayList ()
		{
			delete[] buf;
		}


	private:
		// O(1)
		bool isEmpty()
		{
			return (size == 0);
		}

		// O(1)
		bool isFull()
		{
			return (size == capacity);	
		}

		// Returns index of where word is OR should go
		// O(log N)
		int binary_search (string word, int min, int max)
		{
			int mid = min + ((max - min)/2);

			if (min > max || buf[mid] == word) // If found word OR at correct position to insert 
					return mid;
			
			if (buf[mid] > word) 
					return binary_search(word, min, mid-1);	// Cut off right half of search area
			
			else if (buf[mid] < word) 
					return binary_search(word, mid+1, max);	// Cut off left half of search area
		}

		// Starting at CAPACITY, copies everything from left pos to right pos. Stops at INDEX.
		// O(N)
		void copy_down (int index)
		{
			for (int i = capacity-1; i > index; i--)
				{buf[i] = buf[i-1];}
		}

		// Starting at INDEX, copies everything from left pos to right pos. Stops at SIZE.
		// O(N)
		void copy_up (int index)
		{
			for (int i = index; i < capacity-1; i++)
				{buf[i] = buf[i+1];}
		}
};


///////////////////////////////////////////


class SortedLinkedList : public SortedList
{
	struct ListNode 
	{
		string info;
		ListNode* next;
		
		ListNode (string new_info, ListNode* new_next) 
			: info(new_info), next(new_next) {}
		
		// O(N)
		static void print (ostream& out, ListNode* L)
		{
			if (L)
			{
				out << L->info << endl;
				print(out, L->next);	// Recursion causes it to be O(N)
			}
		}
	};
	ListNode* head;
	
	public:
		// O(1)
		SortedLinkedList ()
		{
			head = nullptr;
		}

		// O(N)
		void insert (string word)
		{
			if (head == nullptr || head->info > word)	// If this is the 1st node to insert OR initial head > word
			{
				ListNode* temp = head;
				head = new ListNode(word, temp);
			}

			else	// If need to insert anywhere else
			{
				ListNode* temp = head;
				ListNode* old;
				while (temp != nullptr && temp->info < word)
				{
					old = temp;
					temp = temp->next;
				}
				old->next = new ListNode(word, temp);	// Connect old to new node, connect new node to temp
			}
		}

		// O(N)
		bool find (string word)
		{
			if (isEmpty()) return false;

			ListNode* temp = head;
			while (temp != nullptr)
			{
				if (temp->info == word) return true;
				else if (temp->info > word) break;	// Moved past a word that would be greater than what we want
				else temp = temp->next;
			}
			return false;
		}

		// O(N)
		void remove (string word)
		{
			if (isEmpty()) cout << ("Note: cannot remove word, linked list is empty!");

			if (head->info == word)  // For when word to remove is at initial head
			{
				ListNode* temp = head->next;
				delete head;
				head = temp;
			}

			else  // For when word is anywhere else
			{
				ListNode* temp = head;
				ListNode* old;
				while (temp->info != word && temp != nullptr)
				{
					if (temp->info > word) return;	// If past where the word would be, give up
					old = temp;
					temp = temp->next;
				}
				old->next = temp->next;
				delete temp;
			}
		}

		// O(N)
		void print()
		{
			ListNode* temp = head;
			while (temp != nullptr)
			{
				cout << temp->info << "->";
				temp = temp->next;
			}
			cout << "nullptr" << endl;
		}

		// O(N)
		~SortedLinkedList()
		{
			ListNode* temp;
			while (head != nullptr)
			{
				temp = head->next;
				delete head;
				head = temp;
			}
		}

	private:
		// O(1)
		bool isEmpty ()
		{
			return (head == nullptr);
		}

		// O(1)
		bool isFull ()
		{
			return false; // Linked Lists are never full
		}
};


/////////////////////  Testing Functions  ///////////////////////


// O(N^2)
void insert_all_words (string file_name, SortedList& L)
{
	Timer timer;
	double elapsed_time;
	ifstream infile(file_name);
	timer.start();

	string word;
	while (infile >> word)
	{
		L.insert(word);	// This causes O(N^2) from SortedArrayList
		//L.print();
	}
	//L.print();

	timer.elapsedUserTime(elapsed_time);
	infile.close();
	cout << "Time for insert_all_words: " << elapsed_time << endl;
}


// O(N^2)
void find_all_words (string file_name, SortedList& L)
{
	Timer timer;
	double elapsed_time;
	ifstream infile(file_name);
	timer.start();

	string word;
	while (infile >> word)
	{
		L.find(word);	// This causes O(N^2) from SortedLinkedLIst
	}
	
	timer.elapsedUserTime(elapsed_time);
	infile.close();
	cout << "Time for find_all_words: " << elapsed_time << endl;
}


// O(N^2)
void remove_all_words (string file_name, SortedList& L)
{
	Timer timer;
	double elapsed_time;
	ifstream infile(file_name);
	timer.start();

	string word;
	while (infile >> word)
	{
		L.remove(word);	// This causes O(N^2) from SortedArrayList
		//L.print();
	}
	//L.print();

	timer.elapsedUserTime(elapsed_time);
	infile.close();
	cout << "Time for remove_all_words: " << elapsed_time << endl;
}


// O(N^2)
void test_SortedList (string file_name, SortedList& L, string version)
{
	cout << "Testing " << version << ":" << endl;
	insert_all_words (file_name, L);
	find_all_words   (file_name, L);
	remove_all_words (file_name, L);
}


///////////////////////////////////////////////////////////////////


// O(N^2)
int main (int argc, char* argv[])
{
	const char* input_file = argc == 2 ? argv[1] : "random.txt";
	//const char* input_file = argc == 2 ? argv[1] : "test.txt";
	//const char* input_file = argc == 2 ? argv[1] : "mini_test.txt";

	cout << "-------------------------------------------------------" << endl;

	SortedArrayList SAL(45500);
	//SortedArrayList SAL(100);
	//SortedArrayList SAL(10);
	test_SortedList (input_file, SAL, "SortedArrayList");

	SortedLinkedList SLL;
	test_SortedList (input_file, SLL, "SortedLinkedList");

	cout << "-------------------------------------------------------" << endl;

	return 0;
} 
