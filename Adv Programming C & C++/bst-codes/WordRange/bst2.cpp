#include "bst.h"
#include <iostream>
BST :: BST()
{
	root = NULL;
}

void BST :: insert(string val)
{
    Node *to_insert = new Node(val); 
    if (root == NULL) {
        root = to_insert; 
		}
    else{
        insert(root,to_insert);
		}
}



int BST::getHeight(Node* node){
	if(node!=NULL) return node->height;
	else return 0;
}

Node* BST :: find(Node* start, string& val)
{
    if (start == NULL || start->key == val) 
        return start;
    if(val < start->key) 
        return find(start->left, val);
    else 
        return find(start->right, val);
}

void BST :: deleteBST(Node* start)
{
    if(start == NULL) 
        return; 
    deleteBST(start->left); 
    deleteBST(start->right);
    delete(start); 
}

string BST :: printInOrder(Node* start)    
{
    if(start == NULL) 
        return ""; 
    string leftpart = printInOrder(start->left);
    string rightpart = printInOrder(start->right);
    string output = start->key;
    if(leftpart.length() != 0) 
        output = leftpart + " " + output; 
    if(rightpart.length() != 0) 
        output = output + " " + rightpart; 
    return output;
}

string BST :: printPreOrder(Node* start)    
{
    if(start == NULL) 
        return ""; 
    string leftpart = printPreOrder(start->left);
    string rightpart = printPreOrder(start->right);
    string output = start->key;
    if(leftpart.length() != 0) 
        output = output + " " + leftpart; 
    if(rightpart.length() != 0) 
        output = output + " " + rightpart; 
    return output;
}

string BST :: printPostOrder(Node* start)    
{
    if(start == NULL) 
        return ""; 
    string leftpart = printPostOrder(start->left);
    string rightpart = printPostOrder(start->right);
    string output = start->key;
    if(rightpart.length() != 0) 
        output = rightpart+" "+output; 
    if(leftpart.length() != 0) 
        output = leftpart+" "+output; 
    return output;
}

// Implement
// 
// 
Node* BST :: insert(Node* start, Node* to_insert)
{
	if(start==NULL) return to_insert;
	if(to_insert->key<start->key){
		start->left = insert(start->left, to_insert);
	}
	else {
		start->right = insert(start->right, to_insert);
	}
	start->height = getHeight(start->left)>getHeight(start->right) ?
		getHeight(start->left)+1:getHeight(start->right)+1;
	return rebalance(start,to_insert->key);
}

int BST::querie(Node* node,string str1,string str2){
  if(!node){
		return 0;
	} else{
		int count=0;
		if(str1<=node->key&&str2>=node->key){
			count++;
		}
		if(str1<node->key){
			count += querie(node->left,str1,str2);
		}
		if(str2>node->key){
			count += querie(node->right,str1,str2);
		} 
		return count;
	}
}

Node* BST::rebalance(Node* node,string& key){
  if(node){
		int diff = getHeight(node->left)-getHeight(node->right);
		if(diff<-1||diff>1){
			if(diff>1){
				if(key<node->left->key) {
					node->left = rotateLeft(node->left);
				}
				return rotateLeft(node);
			} else if(diff<-1){
				if(key>node->right->key) {
					node->right = rotateRight(node->right);
				}
				return rotateRight(node);
			}
		}
		return node;
	} 
	return NULL;
}
Node* BST::rotateRight(Node* node){
	if(node==NULL) return NULL;
	Node* temp = node->left;
	if(temp==NULL) return node;
	node->left = temp->right;
	
	temp->right = node;
	
	node->height = getHeight(node->left)>getHeight(node->right)? 
		getHeight(node->left)+1:getHeight(node->right)+1;
	temp->height = getHeight(temp->left)>getHeight(temp->right)?
		getHeight(temp->left)+1:getHeight(temp->right)+1;
	return temp;
}
Node* BST::rotateLeft(Node* node){
	if(!node) return NULL;
	Node* temp = node->right;
	if(temp==NULL) return node;
	node->right = temp->left;
	
	temp->left = node;
	
	node->height = getHeight(node->left)>getHeight(node->right)? 
		getHeight(node->left)+1:getHeight(node->right)+1;
	temp->height = getHeight(temp->left)>getHeight(temp->right)?
		getHeight(temp->left)+1:getHeight(temp->right)+1;
	return temp;
}