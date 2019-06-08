// Code by Aljon Viray (86285526)
// ICS 46 - Homework #1 - MyString.cpp

#include <iostream>
#include <fstream>
using namespace std;

// Class MyString defines a class similar to "std::string"

class MyString
{ 
	// points to the array holding the characters in this MyString object.
 	// Must be allocated in constructor and, maybe in other functions as well
	// This C-string must be null termiated and that is how you know the length
	// strlen() will count the number of characters up to the first null character
	char* buf;

 	// utility method to handle errors, note throws an exception to prevent
 	// access to undefined values.  Always call this method when you detect 
 	// a fatal error from any of your methods.
	// T(N) = 1001
	void error (const char* s)
    {
    	cerr << "Error: " << s << endl;	// 1000 
    	throw 0;						// 1
    }


 	public:

		static int num_allocations;

		// constructs this object from a c-string s (s is an array of char terminated by ‘\0’)
 	    // parameter, s, defaults to the empty string “”
		// T(N) = 9N + 1012
        explicit MyString (const char* s = "")
 	    {
			buf = strdup(s);	// (9N + 1011) + 1
        }

        // copy constructor for a MyString, must make a deep copy of s for this.
		// T(N) = 9N + 1013
 	    MyString (const MyString& s)
 	    {
 	    	buf = strdup(s.buf);	// (9N + 1011) + 2
        }

 	    // assigns this MyString from MyString s, MUST DO DEEP ASSIGNMENT
 	    // remember, both this and s have been previously constructed
 	    // so they each have storage pointed to by buf
		// T(N) = 9N + 2016
 	    MyString& operator= (const MyString& s)
 	    {
 		    delete[] buf;			// 1000
			num_allocations--;		// 1
			buf = strdup(s.buf);	// (9N + 1011) + 2
			return *this;			// 2
 	    }

 	    // return a reference to the char at position index, 
		// 0 is the first element and so on. Note: index must be in bounds
		// T(N) = 3N + 1007
        char& operator[] (const int index)
        {
			if (index > strlen(buf) || index < 0)	// (3N + 2) + 2
			{
				error(buf);							// 1001
			}
			return buf[index];						// 2
        }

		// Returns length of MyString, not counting the null character
		// T(N) = 3N + 3
        int length () const
        {
			return strlen(buf);	// (3N + 2) + 1
        }

        // returns the index of the first occurance of c in this MyString
        // indices range from 0 to length()-1
        // returns -1 if the character c is not in this MyString
		// T(N) = 7N + 5
        int indexOf (char c)
        {
			int len = strlen(buf);			// (3N + 2) + 1
            for (int i = 0; i < len; i++)	// 2N + 1
			{
				if (buf[i] == c) return i;	// 2N
			}
			return -1;						// 1
        }

        // returns the index of the first occurance of pat in this MyString
        // indices range from 0 to length()-1
        // returns -1 or 1 if the MyString pat's buf is not in this MyString
		// T(N) = 31N + 19
        int indexOf (const MyString& pat) const
        {
			char* c = strstr(buf, pat.buf);	// (23N + 12) + 1
        	if (c == '\0')					// 1
			{
				return -1;					// 0
			}

			int i = strlen(buf);			// (3N + 2) + 1
			for (int j = 0; j < i; j++)		// 2N + 1
			{
				if (buf[j] == *c) return j;	// 3N		
			}
			return -1;						// 1
        }

        // True if the two MyStrings contain the same chars in same position
        // e.g, "abc"=="abc" returns true
		// T(N) = 18N + 11
        bool operator== (const MyString& s) const
        {
			int x = strcmp(buf, s.buf);	// (18N + 8) + 1
			if (x == 0) return true;	// 2
			else return false;			// 0 in worst case
        }

        // concatenates this and s to make a new MyString
        // e.g., "abc"+"def" returns "abcdef"
		// T(N) = 54N + 7065
        MyString operator+ (const MyString& s) const
        {
			char* c = str2dup(buf, s.buf);	// (45N + 5049) + 2
			MyString new_s(c);				// (9N + 1012)
			delete[] c;						// 1000
			num_allocations--; 				// 1
			return new_s;					// 1
        }

        // concatenates s onto end of this MyString's buf
        // e.g., s = "abc"; s+="def" now s is "abcdef"
		// T(N) = 45N + 6055
		MyString& operator+= (const MyString& s)
        {
			char* c = str2dup(buf, s.buf);	// (45N + 5049) + 2
			delete[] buf;					// 1000
			num_allocations--;				// 1
			buf = c;						// 1
			return *this;					// 2
        }

        // returns another MyString that is the reverse of this MyString
        // e.g., s = "abc"; s.reverse() returns "cba"
        // T(N) = 22N + 3027
		MyString reverse() const
        {
			char* c = new char[strlen(buf) + 1];	// (3N + 2) + 1002
			num_allocations++;						// 1
			c = strrev(c, buf);						// (10N + 7)) + 1
			MyString s(c);							// (9N + 1012)
			delete[] c;								// 1000
			num_allocations--;						// 1
			return s;								// 1
        }

        // prints out this MyString to the ostream out
		// T(N) = 1000
        void print (ostream& out) const
        {
        	out << buf;	// 1000
        }

        // reads a word from the istream in and this MyString
        // becomes the same as the characters in that word
		// T(N) = 9N + 5016
        void read (istream& in)
        {
			char* x = new char[1024];	// 1001
			num_allocations++;			// 1
			in.getline(x, 1024);		// 1000
			delete[] buf;				// 1000
			num_allocations--;			// 1
			buf = strdup(x);			// (9N + 1011) + 1
			delete[] x;					// 1000
			num_allocations--;			// 1
        }

        // destruct a MyString, must free up any storage allocated within MyString
		// T(N) = 1001
        ~MyString()
        {
			delete[] buf;		// 1000
			num_allocations--;	// 1
        }


	private:

		// Returns the length of a c-string
		// T(N) = 3N + 2
		int strlen (const char* s) const
		{
			int i = 0;					// 1
			while (s[i] != '\0') i++;	// 3N
			return i;					// 1
		}

		// Allocates new array of chars (aka c-string) 
		// Returns a duplicate c-string of s
		// T(N) = 9N + 1011
		char* strdup (const char* s)
		{
			int len = strlen(s) + 1;	// (3N + 2) + 2
			char* dup = new char[len];	// 1000 + 1
			num_allocations++;			// 1
			dup = strcpy(dup, s);		// (8N + 7) + 1
			return dup;					// 1
		}

		// Combines two c-strings and returns resulting c-string
		// T(N) = 45N + 5049
		char* str2dup (char* str1, char* str2) const
		{
			char* c1 = new char[strlen(str1) + 1];	// (3N + 2) + 1002
			char* c2 = new char[strlen(str2) + 1];	// (3N + 2) + 1002
			char* s1 = strcpy(c1, str1);			// (8N + 7) + 1
			char* s2 = strcpy(c2, str2);			// (8N + 7) + 1
			num_allocations += 2;					// 1
			char* c = strcat(s1, s2);				// (23N + 1021) + 1
			delete[] s1;							// 1000
			delete[] s2;							// 1000
			num_allocations -= 2;					// 1
			return c;								// 1
		}

		// Copies and overwrites all characters from source onto dest
		// Note: Assumes each c-string is the same size or dest can accomodate source's size
		// Also adds a null character to the end of dest
		// T(N) = 8N + 7
		char* strcpy (char* dest, const char* source) const
		{
			int len = strlen(source);		// (3N + 2) + 1
			for (int i = 0; i < len; i++)	// 2N + 1
			{
				dest[i] = source[i];		// 3N
			}
			dest[len] = '\0';				// 2
			return dest;					// 1
		}

		// Similar to strcpy() but reverses the order of the characters
		// T(N) = 10N + 7
		char* strrev (char* dest, const char* source) const
		{
			int len = strlen(source);		// (3N + 2) + 1
			for (int i = 0; i < len; i++)	// 2N + 1
			{
				dest[i] = source[len-i-1];	// 5N
			}
			dest[len] = '\0';				// 2
			return dest;					// 1
		}

		// Scans a c-string (str1) and checks if another c-string (str2) is inside it
		// Returns pointer to first occurence of a match
		// T(N) = 23N + 12
		char* strstr (char* str1, char* str2) const
		{
			int len1 = strlen(str1);				// (3N + 2) + 1
			int len2 = strlen(str2);				// (3N + 2) + 1

			for (int i = 0; i < len1; i++)			// 2N + 1
			{
				if (str1[i] == str2[0])				// 3N
				{
					for (int j = 0; j < len2; j++)	// 2N + 1
					{
						if (str1[i] == '\0' || str2[j] == '\0')	// 4N
							break;								// 0 in worst case
						else if (str1[i] != str2[j])			// 3N
							break;								// 0 in worst case
						else if (j == len2-1)					// 2N
							return (str1 + i - len2 + 1);		// 4 in worst case
						i++;									// N
					}
				}
			}
			return '\0';							// 0 in worst case
		}

		// Compares two c-strings, returns an integer value based on how they match
		// If the c-strings are equal, returns 0
		// Returns a value >0 or <0 if not equal (doesn't matter which for our purposes)
		// T(N) = 18N + 8
		int strcmp (const char* str1, const char* str2) const
		{
			if (strlen(str2) > strlen(str1)) return 1;				// (3N + 2) + (3N + 2) + 0 in worst case

			for (int i = 0; i < strlen(str1); i++)					// (3N + 2) + 2N + 1
			{
				if (str1[i] == '\0' || str2[i] == '\0') return -1;	// 4N
				else if (str1[i] != str2[i]) return 1;				// 3N
			}
			return 0;												// 1 in worst case
		}

		// Appends a copy of source to destination, and adds a new null char at the end
		// Returns the resulting c-string
		// T(N) = 23N + 1021
		char* strcat (char* dest, const char* source) const
		{
			int new_size = strlen(dest) + strlen(source) + 1;	// (3N + 2) + (3N + 2) + 2
			char* result = new char[new_size];					// 1001
			num_allocations++;									// 1 

			result = strcpy(result, dest);						// (8N + 7) + 1
			
			int len_dest = strlen(dest);						// (3N + 2) + 1
			for (int i = len_dest; i < new_size; i++)			// 2N + 1 
			{
				result[i] = source[i-len_dest];					// 4N
			}
			return result;										// 1
		}
		
};

int MyString::num_allocations = 0;


// these two I/O methods are complete as long as you define print and read methods correctly
// T(N) = 1001
inline ostream& operator<< (ostream& out, const MyString& str)
{
	str.print(out);	// (1000)
	return out;		// 1
}

// T(N) = 9N + 5017
inline istream& operator>> (istream& in, MyString& str)
{
	str.read(in);	// (9N + 5016)
	return in;		// 1
}


//////////////////////////////// Testing Functions ///////////////////////////////


// T(N) = 9N + 1013
MyString testConstructor ()
{
	MyString s("HELLO");	// (9N + 1012)
	return s;				// 1
}

// T(N) = 9N + 1014
MyString testCopyConstructor (MyString l) //<-- This part makes the copy, (9N + 1013)
{
	return l;	// 1
}

// T(N) = 18N + 3027
void testAssignment (MyString& s)
{
	MyString x("GOODBYE");	// (9N + 1012)
	s = x;					// (9N + 2015)
}

// T(N) = 2
char testIndexingSucceed (MyString& s)
{
	return s[4]; // 2
}

// T(N) = 4004
void testIndexingFail (MyString& s)
{
	try								// 0
		{cout << s[10] << endl;}	// 1001
	catch (int i)					// 1
		{cout << "Failed successfully! (tested out-of-bounds index)" << endl;}	// 1000

	try								// 0
		{cout << s[-1] << endl;}	// 1001
	catch (int i)					// 1
		{cout << "Failed successfully! (tested negative index)" << endl;}	// 1000
}

// T(N) = 3N + 4
int testLength (MyString& s)
{
	return s.length(); // (3N + 3) + 1
}

// T(N) = 7N + 7
bool testIndexOfChar (MyString& s)
{
	if (s.indexOf('B') == 4) return true;	// (7N + 5) + 2
	else return false;						// 0 in worst case
}

// T(N) = 41N + 1032
bool testIndexOfMyString (MyString& s)
{
	MyString x("BYE");					// (9N + 1012)
	if (s.indexOf(x) == 4) return true;	// (31N + 19) + 1
	else return false;					// 0 in worst case
}

// T(N) = 21N + 1020
bool testComparisonSucceed (MyString& s)
{
	MyString x("GOODBYE");	// (9N + 1012)
	return (s == x);		// (12N + 7) + 1
}

// T(N) = 21N + 1020
bool testComparisonFail (MyString& s)
{
	MyString x("BYE");	// (9N + 1012)
	return (s == x);	// (12N + 7) + 1
}

// T(N) = 63N + 8077
MyString testAddition (MyString& s)
{
	MyString x("!!!");	// (9N + 1012)
	return (s + x);		// (54N + 7065)
}

// T(N) = 54N + 7067
void testPlusEquals(MyString& s)
{
	MyString x("?");	// (9N + 1012)
	(s += x);			// (45N + 6055)
}

// T(N) = 22N + 3027
MyString testReverse (MyString& s)
{
	return s.reverse();	// (22N + 3027)
}

// T(N) = 18N + 8030
void testInputAndOutput (MyString& s)
{
	MyString x("HELLO");		// (9N + 1012)
	ifstream in("input.txt");	// 1000
	in >> x;					// (9N + 5017)
	cout << x;					// (1001)
}


///////////////////////////////// Main ////////////////////////////////////


// T(N) = 283N + 58342
int main()
{
    // This try/catch will catch any exception thrown by MyString::error()
    // while calling test functions.
	try 
	{
		cout << "--------------------------------------------------------" << endl;					// 1000

		MyString s = testConstructor();																// (9N + 1013)
		cout << "testConstructor: " << s << " (Should be 'HELLO')" << endl;							// 1000

		testCopyConstructor(s);																		// (9N + 1014)
		cout << "testCopyConstructor: " << s << " (Should be 'HELLO')" << endl;						// 1000

		testAssignment(s);																			// (18N + 3027)
		cout << "testAssignment: " << s << " (Should be 'GOODBYE')" << endl;						// 1000

		cout << "testIndexingSucceed: " << testIndexingSucceed(s) << " (Should be 'B')"  << endl;	// (2) + 1000
		cout << "testIndexingFail: " << endl;														// 1000
		testIndexingFail(s);																		// 4004

		cout << "testLength: " << s << " is of size " << testLength(s) << " (Should be 7)" << endl;	// 1000

		cout << "testIndexOfChar: " << testIndexOfChar(s) << " (1 = Found char at index 4)" << endl; // (7N + 7) + 1000
		
		cout << "testIndexOfMyString: " << testIndexOfMyString(s) 
				<< " (1 = Found pattern starting at index 4)" << endl;								// (41N + 1032) + 1000

		cout << "testComparisonSucceed: " 
				<< testComparisonSucceed(s) << " (1 = MyStrings are equal)" << endl;				// (21N + 1020) + 1000
		cout << "testComparisonFail: " 
				<< testComparisonFail(s) << " (0 = MyStrings are not equal)" << endl;				// (21N + 1020) + 1000

		cout << "testAddition: " << testAddition(s) << " (Should be 'GOODBYE!!!')" << endl;			// (63N + 8077) + 1000

		testPlusEquals(s);																			// (54N + 7067)
		cout << "testPlusEquals: " << s << " (Should be 'GOODBYE?')" << endl;						// 1000

    	cout << "testReverse: " << testReverse(s) << " (Should be '?EYBDOOG')" << endl;				// (22N + 3027) + 1000

		cout << "testInputAndOutput: ";																// 1000
		testInputAndOutput(s);																		// (18N + 8030)
		cout << " (Should be 'GOODBYE')" << endl;													// 1000
		
		cout << "----------------------------------------------------" << endl;						// 1000
    }
                                  
	catch (int i) 																					// 1
	{
    	cout << "Got an exception: " << i << endl;													// 1000
    }
                                                     
cerr << "Net memory allocated at program end: " << MyString::num_allocations << endl;				// 1000
cerr << "(should be zero! positive = memory leak, negative = duplicate delete)\n";					// 1000

return 0;																							// 1
}
