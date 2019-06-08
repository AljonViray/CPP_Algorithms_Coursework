e// Code by Aljon Viray (86285526)
// ICS 46 - Homework #7 - main.cpp

#include <iostream>
#include <fstream>
#include <string>
using namespace std;
#include "Timer.h"


/////////////////////  Classes  /////////////////////


class Sorter
{
	public:
		string* buf;
		int capacity;

		Sorter (int N)
			:  capacity{N}, buf{new string[N]} {}

		void swap (string& x, string& y)
		{
			string temp = y;
			y = x;
			x = temp;
		}

		virtual void sort() {}

		void print (ostream& out)
		{
			for (int i = 0; i < capacity; i++)
				if (buf[i] != "") cout << i << ": " << buf[i] << endl;
		}

		void insertFromFile (int partition, const char* fileName)
		{
			ifstream infile(fileName);
			string word;
			Timer timer;
			double time;
			timer.start();
			int max = partition*(capacity/10);
			for (int i = 0; i < max; i++) {
				infile >> word;
				buf[i] = word;
			}
			//print(cout); // Before sorting
			sort();
			print(cout); // After sorting
			timer.elapsedUserTime(time);
			cout << "insertAll with " << partition << "/10 words: " << time << " sec" << endl;
			infile.close();
		}

		~Sorter()
			{ delete[] buf; }
};


class InsertionSorter : public Sorter
{
	public:
		using Sorter::Sorter;

		virtual void sort ()
		{
			for (int i = 1; i < capacity-1; i++) {
				for (int j = i; j > 0 && buf[j-1] > buf[j]; j--) {
					if (buf[j] != "" && buf[j-1] != "") // Skip pairs of empty strings, way faster
						swap(buf[j], buf[j-1]);
				}
			}
		}
};


class QuickSorter : public Sorter
{
	public:
		using Sorter::Sorter;

		string median_of_three (string A[], int low, int high)
		{ 
			int mid = low + (high - low)/2;
			if (A[mid] < A[low]) 
				swap(A[low], A[mid]);
			if (A[high] < A[low])
				swap(A[low], A[high]);
			if (A[mid] < A[high])
				swap(A[mid], A[high]);
			return A[high];
		}


		int partition (string A[], int low, int high)
		{
			string pivot = median_of_three(A, low, high);
			int below = low;  int above = high-1;
			while (true) {
				while (A[below] < pivot) 
					below++; 
				while (A[above] > pivot)
					above--;

				if (below < above) // Two in wrong partition
					swap(A[below++], A[above--]);
				else break;
			}
			swap(A[below], A[high]); // Restore pivot to original position?
			return below;
		}

		void insertionSort (int low, int high)
		{
			for (int i = low+1; i <= high; i++) {
				for (int j = i; j > 0 && buf[j-1] > buf[j]; j--) {
					if (buf[j] != "" && buf[j-1] != "") // Skip pairs of empty strings, way faster
						swap(buf[j], buf[j-1]);
				}
			}
		}

		void quicksort (string A[], int low, int high)
		{
			if (high - low < 16) {
				insertionSort(low, high);
			}
			else {
				int i = partition(A, low, high);
				quicksort(A, low, i-1);
				quicksort(A, i+1, high);
			}
		}

		void sort()
			{ quicksort(buf, 0, capacity-1); }
};


class HeapSorter : public Sorter
{
	public:
		using Sorter::Sorter;

		int leftChild (int i)
			{ return 2*i+1; }

		int rightChild (int i)
			{ return 2*i+2; }

		void siftSmallestDown (string A[], int i, int N)
		{
			int child;
			string current = A[i];
			for ( ; leftChild(i) <= N; i = child) {
				child = leftChild(i);
				if (child != N && A[child] < A[child+1])
					child++; // Pick larger of two children
				if (current < A[child])
					A[i] = A[child];
				else break;
			}
			A[i] = current;
		}

		void heapify (string A[], int N)
		{
			for (int start = N/2; start >= 0; start--)
				siftSmallestDown(A, start, N);
		}

		void heapSort (string A[], int N)
		{
			heapify(A, N);
			for (int end = N-1; end > 0; end--)
			{
				swap(A[0], A[end]);
				siftSmallestDown(A, 0, end);
			}
		}

		void sort () // O(N log N)
			{ heapSort(buf, capacity-1); }
};


//////////////////////  Testing Functions  ///////////////////////////


void measureAll (const char* fileName)
{
	// Valgrind Output only
	
	//cout << "Time for InsertSorter: ";
	//InsertionSorter IS(45392);
	//IS.insertFromFile(1, fileName);

	//cout << "Time for QuickSorter: ";
	//QuickSorter QS(45392);
	//QuickSorter QS(20);
	//QS.insertFromFile(10, fileName);

	//cout << "Time for HeapSorter: ";
	//HeapSorter HS(45392);
	//HS.insertFromFile(1, fileName);
	

	// Normal Output only
	for (int i = 1; i <= 10; i++) {
		InsertionSorter IS(45392);
		QuickSorter QS(45392);
		HeapSorter HS(45392);

		cout << "Time for InsertSorter: ";
		IS.insertFromFile(i, fileName);
		cout << "Time for QuickSorter: ";
		QS.insertFromFile(i, fileName);
		cout << "Time for HeapSorter: ";
		HS.insertFromFile(i, fileName);
	}
}
//

/////////////////////  Main Function  /////////////////////////


int main ()
{
	const char* file = "random.txt";
	//const char* file = "words.txt";
	//const char* file = "ten.txt";

	cout << "----------------------------------" << endl;
	cout << "USING FILE: " << file << endl;
	measureAll(file);
	cout << "----------------------------------" << endl;

	return 0;
}
