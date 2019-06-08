// Code by Aljon Viray (86285526)
// ICS 46 - Homework #4 - test_stack.cpp

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

////////////////////////////////

class Stack
{
	public:
		virtual void push (string word) {}
		virtual string pop() {}
		virtual string top() {}
		virtual void print() {}
		virtual ~Stack() {}
		virtual bool isEmpty() {}
		virtual bool isFull() {}
};

////////////////////////////////

class ArrayStack : public Stack
{
	string* buf;
	int capacity;
	int size = 0;

	public:
		// O(1)
		ArrayStack (int max_len)
		{
			capacity = max_len;
			buf = new string[max_len];
		}

		// O(1)
		void push (string word)
		{
			if (isFull()) throw ContainerOverflow("Cannot push onto full stack!");
			buf[size++] = word;	// Set word at current, then increment size
		}

		// O(1)
		string pop()
		{
			if (isEmpty()) throw ContainerUnderflow("Cannot pop from empty stack!");
			return buf[--size]; // Decrement size, then return the word there
		}						// Note: word still there but is no longer accessed by size

		// O(1)
		string top()
		{
			if (isEmpty()) throw ContainerUnderflow("Cannot get top from empty stack!");
			return buf[size-1];
		}

		// O(N)
		void print()
		{
			for (int i = 0; i < size; i++) 
				cout << i << ": " << buf[i] << endl;
		}

		// O(1)
		~ArrayStack()
			{ delete[] buf; }

		// O(1)
		bool isEmpty()
			{ return (size <= 0); }

		// O(1)
		bool isFull()
			{ return (size >= capacity); }
};

//////////////////////////////////

class LinkedStack : public Stack
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
		LinkedStack()
			{ head = nullptr; }

		// O(1)
		void push (string word)
			{ head = new ListNode(word, head); }

		// O(1)
		string pop()
		{
			if (isEmpty()) throw ContainerUnderflow("Cannot pop from empty stack!");
			ListNode* next = head->next;
			string word = head->info;
			delete head;
			head = next;
			return word;
		}

		// O(1)
		string top()
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
		~LinkedStack()
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
			{ return (head == nullptr); }

		// O(1)
		bool isFull()
			{ return false; } // Linked Lists are never full
};


/////////////////////  Testing Functions  ///////////////////////


// O(N)
void fillAll (Stack& S, string file)
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
			{ S.push(word); }
		catch (ContainerOverflow error)
		{ 
			cout << error.print() << endl;
			break;
		}
	}
	//S.print();
	infile.close();
}

// O(N)
void emptyAll (Stack& S, string file)
{
	ofstream outfile(file);
	string word;
	do
	{
		try
			{ outfile << S.pop() << endl; }
		catch (ContainerUnderflow error) 
		{
			cout << error.print() << endl;
			break;
		}	
	}
	while (S.isEmpty() == false);
	//S.print();
	outfile.close();
}

// O(N)
void test_Stack (Stack& S, string infile, string outfile)
{
	fillAll(S, infile);
	emptyAll(S, outfile);
}


///////////////////////  isBalanced()  ///////////////////////


// O(N)
bool isBalanced (LinkedStack& LS, string line)
{
	if (line == "") return true;

	int size = line.size();
	for (int i = 0; i < size; i++)
	{
		if (line[i] == '(' || line[i] == '{' || line[i] == '<' || line[i] == '[')
		{
			cout << line[i];
			LS.push( string(1, line[i]) );
		}
		else if (line[i] == ')' || line[i] == '}' || line[i] == '>' || line[i] == ']')
		{
			if (LS.isEmpty()) return false;	// Stack already empty, not enough opening brackets
			cout << line[i];
			string c = LS.pop();

			if (line[i] == ')' && c != "(") return false;
			else if (line[i] == '}' && c != "{") return false;
			else if (line[i] == '>' && c != "<") return false;
			else if (line[i] == ']' && c != "[") return false;
		}
	}
	if (LS.isEmpty() == false) return false; // Stack was not emptied, not enough closing brackets

	return true; // Perfectly balanced, as all things should be.
}


///////////////////////  Main Function  ///////////////////////


// O(N)
int main (int argc, char* argv[])
{
	if (argc != 4)
	{
		cout << "ERROR: Must call test_stack with exactly 3 parameters, {input, output1, output2}" << endl;
		return 0;
	}
	const char* infile = argv[1];
	const char* array_outfile = argv[2];
	const char* linked_outfile = argv[3];

	cout << "-------------------------------------------------------" << endl;

	ArrayStack AS(45500);
	cout << "---Testing ArrayStack---" << endl;
	//fillAll  (AS, infile); // For testing ContainerOverflow
	test_Stack (AS, infile, array_outfile);
	//emptyAll (AS, array_outfile); // For testing ContainerUnderflow
	cout << "---ArrayStack DONE!---" << endl;

	LinkedStack LS;
	cout << "---Testing LinkedStack---" << endl;
	//fillAll  (LS, infile); // For testing ContainerOverflow
	test_Stack (LS, infile, "LinkedStackOutput.txt");
	//emptyAll (LS, linked_outfile); // For testing ContainerUnderflow
	cout << "---LinkedStack DONE!---" << endl;

	cout << endl << "---Testing isBalanced()---" << endl;
	//bool b = isBalanced (LS, "({(())})((([({})])))(((((<>([{()}])(<>))))))()");	// Correct, returns true
	//bool b = isBalanced (LS, "({(<>)})((([({})])))(((((()([{()}])(())))))");	// Not enough, returns false
	//bool b = isBalanced (LS, "({(<>)})((([({})])))((((([]([{<>}])(()))))))()])");	// Too many, returns false
	bool b = isBalanced (LS, "({(())])((([({})>)))(((((<>([{()}>)(<>))))))(>");	// Wrong brackets, returns false

	if (b == false) cout << " <-- Incorrect Bracket!" << endl 
		<< "^^^The above string IS NOT balanced!^^^" << endl;
	else cout << endl << "^^^The above string IS balanced!^^^" << endl;
	cout << "---isBalanced() DONE!---" << endl;

	cout << "-------------------------------------------------------" << endl;

	return 0;
} 
