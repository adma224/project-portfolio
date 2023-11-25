#ifndef BST_H
#define BST_H

#include <string>
#include <vector>

using namespace std;

class Node
{
	public:
    string key = "";
    Node *left, *right;
    int height;
    Node()
    {
        left = right = NULL; 
    }

    Node(string val) 
    {
        key = val;
        left = right = NULL; 
        height = 1;
    }
};

class BST
{
	private:
		Node *root; 
	public:
		BST(); 
    void insert(string); 
    Node* insert(Node*, Node*); 
    Node* find(string& str) {return find(root,str);} 
    Node* find(Node*, string&) ; 
		int getHeight(Node*);
		void deleteBST(){deleteBST(root);} 
    void deleteBST(Node* start); 
		
		int querie(Node*,string,string) ; // Recursive call for finding range
    
		Node* rebalance(Node*,string&); // Check subtree heights and call rotate functions
    Node* rotateLeft(Node*); // Rotate left at Node*
    Node* rotateRight(Node*); // Rotate right at Node*
		
		int range(string str1,string str2) {return querie(root,str1,str2);} // Base caller
    
		string printInOrder(){return printInOrder(root);} 
		string printInOrder(Node* start); 
		string printPreOrder(){return printPreOrder(root);} 
		string printPreOrder(Node* start); 
		string printPostOrder(){return printPostOrder(root);} 
		string printPostOrder(Node* start); 

};

#endif