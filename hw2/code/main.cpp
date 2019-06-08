// Code by Aljon Viray (86285526)
// ICS 46 - Homework #2 - main.cpp

#include <iostream>
#include <fstream>
#include <string>
using namespace std;

#include "Timer.h"
//#include "UnorderedArrayList.cpp"
//#include "UnorderedLinkedList.cpp"


//////////////////////  Classes  ////////////////////////


class UnorderedArrayList
{
	string* buf;
	int capacity;
	int size;

	public:
		// O(1)
		UnorderedArrayList (int max_len)
		{
			capacity = max_len;
			size = 0;
			string* new_array = new string[max_len];
			buf = new_array;
		}

		// O(N)
		void insert (string word)
		{
			if (isFull()) throw("Cannot insert word, array list is full!");

			for (int i = 0; i < capacity; i++)
			{
				if (buf[i] == "")
				{
					buf[i] = word;
					size++;
					break;
				}
			}
		}

		// O(N)
		bool find (string word)
		{
			if (isEmpty()) return false;
			int i = getIndex(word);		// Calling this causes O(N)
			if (i != -1) return true;
			return false;
		}

		// O(N)
		void remove (string word)
		{
			if (isEmpty()) throw("Cannot remove word, array list is empty!");

			int i = getIndex(word);		// Calling this causes O(N)
			if (i != -1) 
			{
				buf[i] = "";
				size--;
			}
		}

		// O(N)
		void print (ostream& out)
		{
			for (int i = 0; i < capacity; i++)
			{
				out << i << ": " << buf[i] << endl;
			}
		}

		// O(1)
		~UnorderedArrayList ()
		{
			delete[] buf;
		}


	private:
		// O(1)
		bool isEmpty ()
		{
			return (size == 0);
		}

		// O(1)
		bool isFull ()
		{
			return (size == capacity);	
		}

		// O(N)
		int getIndex (string word)
		{
			for (int i = 0; i < capacity; i++)
			{
				if (buf[i] == word) return i;
			}
			return -1;
		}
};



class UnorderedLinkedList 
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
		UnorderedLinkedList ()
		{
			head = nullptr;
		}

		// O(1)
		void insert (string word)
		{
			ListNode* temp = head;
			head = new ListNode(word, temp);
		}

		// O(N)
		bool find (string word)
		{
			if (isEmpty()) return false;

			ListNode* temp = head;
			while (temp != nullptr)
			{
				if (temp->info == word) return true;
				else temp = temp->next;
			}
			return false;
		}

		// O(N)
		void remove (string word)
		{
			if (isEmpty()) throw("Cannot remove word, linked list is empty!");

			if (head->info == word)  // For when word to remove is at first head
			{
				ListNode* temp = head->next;
				delete head;
				head = temp;
			}

			else  // For when word is anywhere else
			{
				ListNode* old_ptr;
				ListNode* current = head;
				while (current->info != word && current != nullptr)
				{
					old_ptr = current;
					current = current->next;
				}
				old_ptr->next = current->next;
				delete current;
			}
		}

		// O(N)
		void print (ostream& out)
		{
			ListNode* temp = head;
			while (temp != nullptr)
			{
				out << temp->info << "->";
				temp = temp->next;
			}
			cout << "nullptr" << endl;
		}

		~UnorderedLinkedList ()
		{
			while (head != nullptr)
			{
				ListNode* temp = head->next;
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


/////////////////////  Helper Functions  ///////////////////////

// O(N)
ostream& operator<< (ostream& out, UnorderedArrayList& L)
{
	L.print(out); 
	return out;
}

// O(N)
ostream& operator<< (ostream& out, UnorderedLinkedList& L)
{
	L.print(out); 
	return out;
}

////////////////////////////////////////////////////////////////

// O(N^2)
void insert_all_words (string file_name, UnorderedArrayList& L)
{
	Timer timer;
	double elapsed_time;
	ifstream infile(file_name);
	timer.start();

	string word;
	while (infile >> word)
	{
		L.insert(word);	// This causes O(N^2)
	}
	
	timer.elapsedUserTime(elapsed_time);
	infile.close();
	cout << "Time for insert_all_words: " << elapsed_time << endl;
}

// O(N^2)
void find_all_words (string file_name, UnorderedArrayList& L)
{
	Timer timer;
	double elapsed_time;
	ifstream infile(file_name);
	timer.start();

	string word;
	while (infile >> word)
	{
		L.find(word);	// This causes O(N^2)
	}
	
	timer.elapsedUserTime(elapsed_time);
	infile.close();
	cout << "Time for find_all_words: " << elapsed_time << endl;
}

// O(N^2)
void remove_all_words (string file_name, UnorderedArrayList& L)
{
	Timer timer;
	double elapsed_time;
	ifstream infile(file_name);
	timer.start();

	string word;
	while (infile >> word)
	{
		L.remove(word);	// This causes O(N^2)
	}
	
	timer.elapsedUserTime(elapsed_time);
	infile.close();
	cout << "Time for remove_all_words: " << elapsed_time << endl;
}

/////////////////////////////////////////////////////////////////

// O(N)
void insert_all_words (string file_name, UnorderedLinkedList& L)
{
	Timer timer;
	double elapsed_time;
	ifstream infile(file_name);
	timer.start();

	string word;
	while (infile >> word)
	{
		L.insert(word);
	}
	
	timer.elapsedUserTime(elapsed_time);
	infile.close();
	cout << "Time for insert_all_words: " << elapsed_time << endl;
}

// O(N^2)
void find_all_words (string file_name, UnorderedLinkedList& L)
{
	Timer timer;
	double elapsed_time;
	ifstream infile(file_name);
	timer.start();

	string word;
	while (infile >> word)
	{
		L.find(word);
	}
	
	timer.elapsedUserTime(elapsed_time);
	infile.close();
	cout << "Time for find_all_words: " << elapsed_time << endl;
}

// O(N^2)
void remove_all_words (string file_name, UnorderedLinkedList& L)
{
	Timer timer;
	double elapsed_time;
	ifstream infile(file_name);
	timer.start();

	string word;
	while (infile >> word)
	{
		L.remove(word);
	}
	
	timer.elapsedUserTime(elapsed_time);
	infile.close();
	cout << "Time for remove_all_words: " << elapsed_time << endl;
}


/////////////////////////////////////////////////////////////////

// O(N^2)
void test_UnorderedArrayList_methods (string file_name, UnorderedArrayList& L)
{
	cout << "Testing UnorderedArrayList:" << endl;
	insert_all_words (file_name, L);
	find_all_words (file_name, L);
	remove_all_words (file_name, L);
}

// O(N^2)
void test_UnorderedLinkedList_methods (string file_name, UnorderedLinkedList& L)
{
	cout << "Testing UnorderedLinkedList:" << endl;
	insert_all_words (file_name, L);
	find_all_words (file_name, L);
	remove_all_words (file_name, L);
}


///////////////////////////////////////////////////////////////////

// O(N^2)
int main (int argc, char* argv[])
{
	const char* input_file = argc == 2 ? argv[1] : "random.txt";
	//const char* input_file = argc == 2 ? argv[1] : "test.txt";

	cout << "-------------------------------------------------------" << endl;

	UnorderedArrayList UAL(45500);
	//UnorderedArrayList UAL(101);
	test_UnorderedArrayList_methods (input_file, UAL);

	UnorderedLinkedList ULL;
	test_UnorderedLinkedList_methods (input_file, ULL);

	cout << "-------------------------------------------------------" << endl;

	return 0;
}