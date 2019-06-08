// Code by Aljon Viray (86285526)
// ICS 46 - Homework #4 - test_queue.cpp

#include <iostream>
#include <fstream>
#include <string>
#include <exception>
using namespace std;


//////////////////  Classes  //////////////////

class ContainerException : public exception { public: virtual string print() {}; };

class ContainerOverflow  : public ContainerException 
{
	string error_msg;
	public:
		ContainerOverflow (const char* msg) { error_msg = msg; }
		string print() { return error_msg; }
};

class ContainerUnderflow : public ContainerException 
{
	string error_msg;
	public:
		ContainerUnderflow (const char* msg) { error_msg = msg; }
		string print() { return error_msg; }
};

/////////////////////////////////////////

class Queue
{
	public:
		virtual void enq (string word) {}
		virtual string deq() {}
		virtual string front() {}
		virtual void print() {}
		virtual bool isEmpty() {}
		virtual bool isFull() {}
		virtual ~Queue() {}
};

////////////////////////////////////////

class ArrayQueue : public Queue
{
	string* buf;
	int capacity;
	int head = 0;
	int rear = 0;

	public:
		// O(1)
		ArrayQueue (int max_len)
		{
			capacity = max_len+1; // Queues need +1
			buf = new string[capacity];
		}

		// O(1)
		void enq (string word)
		{
			if (isFull()) throw ContainerOverflow("Cannot enq into full queue!");
			buf[rear] = word;
			rear = (rear+1) % capacity;	// Rear moves right once, but also accounts for
		}								// Circular buffer logic (after capacity is index 0)

		// O(1)
		string deq()
		{
			if (isEmpty()) throw ContainerUnderflow("Cannot deq from empty stack!");
			string word = buf[head];
			head = (head+1) % capacity;	// Same logic is above, circular buffer logic
			return word; 					// Note: word is not removed, just inaccesible now.
		}									// It will be overwritten later with enq().

		// O(1)
		string front()
		{
			if (isEmpty()) throw ContainerUnderflow("Cannot get front from empty stack!");
			return buf[head];
		}

		// O(N)
		void print()
		{
			for (int i = 0; i < rear; i++) 
				cout << i << ": " << buf[i] << endl;
		}

		// O(1)
		~ArrayQueue()
			{ delete[] buf; }

		// O(1)
		bool isEmpty()
			{ return rear == head; }

		// O(1)
		bool isFull()
			{ return (rear+1) % capacity == head; }
};

///////////////////////////////////////////

class LinkedQueue : public Queue
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
	ListNode* tail;
	
	public:
		// O(1)
		LinkedQueue()
			{ head = tail = nullptr; }

		// O(1)
		void enq (string word)
		{
			if (isFull()) throw("Cannot enq into full queue!");  // Note: Never full, never called

			ListNode* new_node = new ListNode(word, nullptr);
			if (tail == nullptr)
				head = new_node;
			else
				tail->next = new_node;
			tail = new_node;
		}

		// O(1)
		string deq()
		{
			if (isEmpty()) 
			{
				tail = nullptr;	// Remove dangling pointer
				throw ContainerUnderflow("Cannot deq from empty queue!");
			}

			ListNode* next = head->next;
			string word = head->info;
			delete head;
			head = next;
			return word;
		}

		// O(1)
		string front()
		{
			if (isEmpty()) throw ContainerUnderflow("Cannot get top from empty stack!");
			return head->info; 
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
		~LinkedQueue()
		{
			ListNode* temp;
			while (head != nullptr)
			{
				temp = head->next;
				delete head;
				head = temp;
			}
		}

		// O(1)
		bool isEmpty()
			{ return head == nullptr; }

		// O(1)
		bool isFull()
			{ return false; } // Linked Lists are never full
};


/////////////////////  Testing Functions  ///////////////////////


// O(N)
void fillAll (Queue& Q, string file)
{
	ifstream infile(file);
	if (infile.is_open() == false)
	{
		cout << "Input file '" << file << "' does not exist, aborting fillAll." << endl;
		return;
	}
	string word;
	while (infile >> word)
	{
		try
			{ Q.enq(word); }
		catch (ContainerOverflow error)
		{ 
			cout << error.print() << endl;
			break;
		}
	}
	//Q.print();
	infile.close();
}

// O(N)
void emptyAll (Queue& Q, string file)
{
	ofstream outfile(file);
	string word;
	do
	{
		try
			{ outfile << Q.deq() << endl; }
		catch (ContainerUnderflow error) 
		{
			cout << error.print() << endl;
			break;
		}	
	}
	while (Q.isEmpty() == false);
	//Q.print();
	outfile.close();
}

// O(N)
void test_Queue (Queue& Q, string infile, string outfile)
{
	fillAll(Q, infile);
	emptyAll(Q, outfile);
}


///////////////////////  Main Function  ///////////////////////


// O(N)
int main (int argc, char* argv[])
{
	if (argc != 4)
	{
		cout << "ERROR: test_queue must be called with exactly 3 parameters {input, output1, output2}" << endl;
		return 0;
	}
	const char* infile = argv[1];
	const char* array_outfile = argv[2];
	const char* linked_outfile = argv[3];

	cout << "-------------------------------------------------------" << endl;

	ArrayQueue AQ(45500);
	cout << "---Testing ArrayQueue---" << endl;
	//fillAll(AQ, infile); // For testing ContainerOverflow
	test_Queue(AQ, infile, array_outfile);
	//emptyAll(AQ, array_outfile); // For testing ContainerUnderflow
	cout << "---ArrayQueue DONE!---" << endl;

	LinkedQueue LQ;
	cout << "---Testing LinkedQueue---" << endl;
	//fillAll(LQ, infile); // For testing ContainerOverflow
	test_Queue(LQ, infile, linked_outfile);
	//emptyAll(LQ, linked_outfile); // For testing ContainerUnderflow
	cout << "---LinkedQueue DONE!---" << endl;

	cout << "-------------------------------------------------------" << endl;

	return 0;
} 
