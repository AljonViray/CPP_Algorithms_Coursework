// Code by Aljon Viray (86285526)
// ICS 46 - Homework #6 - main.cpp

#include <iostream>
#include <fstream>
#include <string>
#include <map>
using namespace std;
#include "Timer.h"


/////////////////////  Classes  /////////////////////


template <typename KeyType, typename InfoType>
class TreeNode 
{
	public:
		KeyType key;
		InfoType info;
		TreeNode* left;
		TreeNode* right;

	private:
		static TreeNode* findPred (TreeNode* t)
		{
			if (t->left == nullptr) return nullptr;
			TreeNode* pred = t->left;
			while (pred->right != nullptr)
				pred = pred->right;
			return pred;
		}

		static TreeNode* findPredParent (KeyType predKey, TreeNode* t)
		{
			if (t->left->key == predKey) return t; // The parent of predNode is current
			TreeNode* predParent = t->left;
			while (predParent->right->right != nullptr && predParent->right->key != predKey)
				predParent = predParent->right;
			return predParent;
		}

		static TreeNode* findParent (KeyType key, TreeNode* root)
		{
			if (root->left == nullptr && root->right == nullptr)
				return nullptr;
			if (root->key == key) // Parent is itself
				return root;
			if ( (root->left != nullptr && root->left->key == key) // If the left or right node is correct
				 || (root->right != nullptr && root->right->key == key))
				return root;
		    if (root->key > key)
				return findParent(key, root->left);
			if (root->key < key)
				return findParent(key, root->right);
			return nullptr;
		}

		static void swapKeyAndInfo (TreeNode* node_1, TreeNode* node_2)
		{
			KeyType k = node_2->key;	
			InfoType i = node_2->info;
			node_2->key = node_1->key;  
			node_2->info = node_1->info;
			node_1->key = k; 			
			node_1->info = i;
		}

		static void swapAllData (TreeNode* node_1, TreeNode* node_2)
		{
			KeyType k = node_2->key;		InfoType i = node_2->info;
			TreeNode* l = node_2->left;		TreeNode* r = node_2->right;

			node_2->key = node_1->key;  	node_2->info = node_1->info;
			node_2->left = node_1->left;	node_2->right = node_1->right;

			node_1->key = k; 				node_1->info = i;
			node_1->left = l;				node_1->right = r;
		}


	public:	
		TreeNode (KeyType new_key, InfoType new_info, TreeNode* new_left, TreeNode* new_right) 
			: key(new_key), info(new_info), left(new_left), right(new_right) {}

		static TreeNode* newNode (KeyType k, InfoType i, TreeNode* l, TreeNode* r)
			{ return new TreeNode(k, i, l, r); }

		static void print (ostream& out, TreeNode* t)
		{
			if (t != nullptr)
			{
				out << '[' ;
				print(out, t->left);
				out << '(' << t->key << ',' << t->info << ')';
				print(out, t->right);
				out << ']';
			}
			else out << "nullptr";
		}


		static TreeNode* insert (KeyType key, InfoType info, TreeNode* root)
		// O(log N)
		// T(N) = 5logN + 2
		{
			TreeNode* t = root;
			while (t->key != key) // While we don't find a duplicate or reach the end of tree
			{
				if (key < t->key) // Left Node Insert Attempt
				{
					if (t->left == nullptr) t->left = newNode(key, info, nullptr, nullptr);
					t = t->left; // If cannot insert, move left instead
				}
				else if (key > t->key) // Right Node Insert Attempt
				{
					if (t->right == nullptr) t->right = newNode(key, info, nullptr, nullptr);
					t = t->right; // If cannot insert, move right instead
				}
			}
			return root;
		}

		static TreeNode* find (KeyType key, TreeNode* root)
		// O(log N)
		{
			if (root == nullptr)
				return nullptr;
			if ( root != nullptr && root->key == key)
				return root;
		    if (root->key > key)
				return find(key, root->left);
			if (root->key < key)
				return find(key, root->right);
			return nullptr;
		}


		static void remove (KeyType key, TreeNode* root, TreeNode* to_remove = nullptr, TreeNode* parent = nullptr)
		// O(log N)
		{
			if (to_remove == nullptr && parent == nullptr) {  // Handle two children case recursion
				to_remove = find(key, root);
				parent = findParent(key, root);
			}

			if (to_remove->left == nullptr && to_remove->right == nullptr)  // to_remove has 0 children
			{
				if (parent != nullptr) {  // If parent is not the same as to_remove
					if (parent->left == to_remove) parent->left = nullptr;  // Unlink the Node we will delete
					else parent->right = nullptr;
				}
				delete to_remove;
			}
			
			else if (to_remove->left != nullptr && to_remove->right == nullptr) // to_remove has 1 left child
			{
				TreeNode* child = to_remove->left;
				swapAllData(to_remove, child);
				delete child;
			}

			else if (to_remove->left == nullptr && to_remove->right != nullptr) // to_remove has 1 right child
			{
				TreeNode* child = to_remove->right;
				swapAllData(to_remove, child);
				delete child;
			}
			
			else if (to_remove->left != nullptr && to_remove->right != nullptr) // to_remove has 2 children
			{
				TreeNode* predNode = findPred(to_remove);
				TreeNode* predNodeParent = findPredParent(predNode->key, to_remove);
				swapKeyAndInfo(predNode, to_remove);
				remove(key, root, predNode, predNodeParent);
			}
		}


		static void deleteTree (TreeNode* t)
		{
			if (t != nullptr)
			{
				deleteTree(t->left);
				deleteTree(t->right);
				delete t;
			}
		}
};

//////////////////////////////////////////////////////

template <typename KeyType, typename InfoType>
class BinarySearchTree
{
	public:
		TreeNode<KeyType, InfoType>* root;

		BinarySearchTree()
			{ root = nullptr; }

		void insert (KeyType key, InfoType info)
		// O(log N)
		// T(N) = 5logN + 2
		{
			if (root != nullptr) TreeNode<KeyType, InfoType>::insert(key, info, root); // 5logN + 2
			else root = TreeNode<KeyType, InfoType>::newNode(key, info, nullptr, nullptr); // Simpler than TreeNode::insert
		}

		InfoType find (KeyType key)
		// O(log N)
		// No T(N) because TreeNode::find is recursive
		{
			TreeNode<KeyType, InfoType>* t = TreeNode<KeyType, InfoType>::find(key, root);
			if (t == nullptr) return -1;
			return t->info;
		}

		InfoType& operator[] (KeyType key)
		// O(log N)
		// No T(N) because TreeNode::find is recursive
		{
			TreeNode<KeyType, InfoType>* t = TreeNode<KeyType, InfoType>::find(key, root);
			return t->info;
		}

		void remove (KeyType key)
		// O(log N)
		// No T(N) because TreeNode::remove is recursive
		{
			// If root is not the one to remove, OR if root has children
			if (root->key != key || root->left != nullptr || root->right != nullptr) 
				TreeNode<KeyType, InfoType>::remove(key, root);
			else {
				delete root;	// If deleting the root w/ no children, set it to nullptr manually
				root = nullptr;
			}
		}

		void print (ostream& out)
		{
			if (root != nullptr) {
				TreeNode<KeyType, InfoType>* t = root;
				TreeNode<KeyType, InfoType>::print(out, t);
				cout << endl;
			}
			else cout << "nullptr" << endl;
		}

		void countLengths (map<int, int>& lengths, TreeNode<KeyType, InfoType>* t)
		{
			lengths[t->key.size()]++; // Increment val at this key (length of word), or create new key and increment that val
			if (t->left != nullptr)
				countLengths(lengths, t->left);
			if (t->right != nullptr)
				countLengths(lengths, t->right);
		}

		~BinarySearchTree()
			{ TreeNode<KeyType, InfoType>::deleteTree(root); }
};



//////////////////////  Testing Functions  ///////////////////////////


template <typename KeyType, typename InfoType>
void testInsert (BinarySearchTree<KeyType, InfoType>& tree)
{
	tree.insert("b", 1);
	tree.insert("c", 1);
	tree.insert("a", 1);
	tree.insert("f", 1);
	tree.insert("e", 1);
	tree.insert("d", 1);
	tree.print(cout);
}

template <typename KeyType, typename InfoType>
void testFind (BinarySearchTree<KeyType, InfoType>& tree)
{
	tree.find("a");
	tree.find("b");
	tree.find("c");
	tree.find("d");
	tree.find("e");
}

template <typename KeyType, typename InfoType>
void testRemove (BinarySearchTree<KeyType, InfoType>& tree)
{
	tree.remove("b"); // 2 Children tests, Note: change inserts to both e and c`
	tree.print(cout);
	tree.remove("c");
	tree.print(cout);
	tree.remove("e");
	tree.print(cout);
	tree.remove("a");
	tree.print(cout);
	tree.remove("f");
	tree.print(cout);
	tree.remove("d");
	tree.print(cout);
}


template <typename KeyType, typename InfoType>
void testBST()
{ 
	BinarySearchTree<string, int> tree;
	testInsert(tree);
	testFind(tree);
	testRemove(tree);
}


//////////////  Input File Functions  //////////////


template <typename KeyType, typename InfoType>
void insertAll (const char* filename, BinarySearchTree<KeyType, InfoType>& tree, int partition, int k) 
{
	ifstream infile(filename);
	string word;
	Timer timer;
	double time;
	timer.start();
	for (int i = 0; i < partition; i++) {
		infile >> word;
		tree.insert(word, 1);
	}
	timer.elapsedUserTime(time);
	cout << "insertAll with " << k << "/10 words: " << time << " sec" << endl;
	infile.close();
}


template <typename KeyType, typename InfoType>
void findAll (const char* filename, BinarySearchTree<KeyType, InfoType>& tree, int partition, int k)  
{
	ifstream infile(filename);
	string word;
	Timer timer;
	double time;
	timer.start();
	for (int i = 0; i < partition; i++) {
		infile >> word;
		tree.find(word);
	}
	timer.elapsedUserTime(time);
	cout << "findAll with " << k << "/10 words: " << time << " sec" << endl;
	infile.close();
}


template <typename KeyType, typename InfoType>
void removeAll (const char* filename, BinarySearchTree<KeyType, InfoType>& tree, int partition, int k) 
{
	ifstream infile(filename);
	string word;
	Timer timer;
	double time;
	timer.start();
	for (int i = 0; i < partition; i++) {
		infile >> word;
		tree.remove(word);
	}
	timer.elapsedUserTime(time);
	cout << "removeAll with " << k << "/10 words: " << time << " sec" << endl;
	infile.close();
}


/////////////////////  Main Function  /////////////////////////


int main ()
{
	//testBST();
	BinarySearchTree<string, int> tree;
	const char* file = "random.txt";

	cout << "----------------------------------" << endl;
	/*
	cout << "--- File: " << file << " ---" << endl;
	for (int k = 1; k <= 10; k++) {
		int partition = (k*45392)/10;
		insertAll(file, tree, partition, k);
		findAll(file, tree, partition, k);
		removeAll(file, tree, partition, k);
	}
	cout << endl;
	*/
	insertAll(file, tree, 45392, 10);
	map<int, int> lengths;
	tree.countLengths(lengths, tree.root);
	for (int i = 1; i < lengths.size()+1; i++)
		cout << "Length " << i << ": " << lengths[i] << " words" << endl;

	cout << "----------------------------------" << endl;

	return 0;
}
