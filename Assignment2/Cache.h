#ifndef CACHE_H
#define CACHE_H

#include "main.h"

class AVL{
	public:
		class Node;
		Node* root;
	public:
		class Node{
			private:
				Node* left{nullptr}; 
				Node*right{nullptr};
				Elem* data;
				int balance{0};
				friend class AVL;
			public:
				Node(Elem* value) {
					data= value; 
				}
				int getAdd(){
					return data->addr;
				}
		};
	public:
		AVL(){root= nullptr;};
		~AVL(){}
		int getHeight(Node* node){
				if (node== nullptr) return 0;
				int lh= this-> getHeight(node->left);
				int rh= this-> getHeight(node->right);
				return (lh>rh? lh : rh) +1;
		}
		
		void insert(Node* &root, Elem* &value){
			if (root== nullptr){
				root= new Node(value);
			}
			else{
				if (value->addr < root->getAdd()) {
					int oldH = getHeight(root->left);
					this->insert(root->left, value);
					if (oldH < getHeight(root->left)) {
						if (root->balance == -1)
							balanceLeft(root);
						else if (root->balance == 0)
							root->balance = -1;
						else root->balance = 0;
					}

				}
				else {
					int oldH = getHeight(root->right);
					this->insert(root->right, value);
					if (oldH < getHeight(root->right)) {
						if (root->balance == 1)
							balanceRight(root);
						else if (root->balance == 0)
							root->balance = 1;
						else root->balance = 0;
					}
				}
			}
		}
		void rotL(Node*& root) {
			Node* tmp = root->right;
			root->right = tmp->left;
			tmp->left = root;
			root = tmp;
		}
		void rotR(Node*& root) {
			Node* tmp = root->left;
			root->left = tmp->right;
			tmp->right = root;
			root = tmp;
		}

		bool balanceLeft(Node*& root) {
			if (root->left->balance == -1) {
				root->balance = 0;
				rotR(root);
				root->balance = 0;
				return 0;
			}
			else {
				if (root->left->right->balance == -1) {
					root->balance = 1;
					root->left->balance = 0;
				}
				else if (root->left->right->balance == 0) {
					root->balance = 0;
					root->left->balance = 0;
				}
				else {
					root->balance = 0;
					root->left->balance = -1;
				}
				root->left->right->balance = 0;
				rotL(root->left);
				rotR(root);
				return 1;
			}
		}
		bool balanceRight(Node*& root) {
			if (root->right->balance == 1) {
				root->balance = 0;
				rotL(root);
				root->balance = 0;
				return 0;
			}
			else {
				if (root->right->left->balance == 1) {
					root->right->balance = 0;
					root->balance = -1;
				}
				else if (root->right->left->balance == 0) {
					root->balance = 0;
					root->right->balance = 0;
				}
				else {
					root->balance = 0;
					root->right->balance = 1;
				}
				root->right->left->balance = 0;
				rotR(root->right);
				rotL(root);
				return 1;
			}
		}

		bool balanceLeft_Del(Node*& root) {
			if (root->balance == 1) {
				root->balance = 0;
				return 0;
			}
			else if (root->balance == 0) {
				root->balance = -1;
				return 0;
			}
			else {
				if (root->left->balance == 1) {
					if (root->left->right->balance == -1) {
						root->balance = 1;
						root->left->balance = 0;

					}
					else if (root->left->right->balance == 0) {
						root->balance = 0;
						root->left->balance = 0;
					}
					else {
						root->balance = 0;
						root->left->balance = -1;
					}
					root->left->right->balance = 0;
					rotL(root->left);
					rotR(root);
				}
				else {
					if (root->left->balance == 0) {
						root->left->balance = 1;
					}
					else { // LH
						root->balance = 0;
						root->left->balance = 0;
					}
					rotR(root);
				}
				return 1;
			}
		}
		bool balanceRight_Del(Node*& root) {
			if (root->balance == -1) {
				root->balance = 0;
				return 0;
			}
			else if (root->balance == 0) {
				root->balance = 1;
				return 0;
			}
			else {
				if (root->right->balance == -1) {
					if (root->right->left->balance == 1) {
						root->balance = -1;
						root->right->balance = 0;

					}
					else if (root->right->left->balance == 0) {
						root->balance = 0;
						root->right->balance = 0;
					}
					else {
						root->balance = 0;
						root->right->balance = 1;
					}
					root->right->left->balance = 0;
					rotR(root->right);
					rotL(root);
				}
				else {
					if (root->right->balance == 0) {
						root->right->balance = -1;
					}
					else { // RH
						root->balance = 0;
						root->right->balance = 0;
					}
					rotL(root);
				}
				return 1;
			}
		}

		void remove(Node*& root, int value) {
			if (root == nullptr) return;
			if (value < root->getAdd()) {
				int oldH = getHeight(root->left);
				remove(root->left, value);
				if (oldH > getHeight(root->left)) {
					balanceRight_Del(root);
				}
			}
			else if (value > root->getAdd()) {
				int oldH = getHeight(root->right);
				remove(root->right, value);
				if (oldH > getHeight(root->right)) {
					balanceLeft_Del(root);
				}
			}
			else {
				if (root->left == nullptr) {
					Node* tmp = root;
					root = root->right;
					delete tmp;
					tmp = nullptr;
				}
				else if (root->right == nullptr) {
					Node* tmp = root;
					root = root->left;
					delete tmp;
					tmp = nullptr;
				}
				else {
					Node* tmp = root->right;
					while (tmp->left != nullptr) {
						tmp = tmp->left;
					}
					root->data = tmp->data;
					int oldH = getHeight(root->left);
					remove(root->right, tmp->getAdd());
					if (oldH > getHeight(root->right)) {
						balanceRight_Del(root);
					}
				}
			}
		}

		void insert(Elem* &value)
		{
			insert(this->root, value);
		}

		void inorder(Node* root){
			if (root != NULL) {
				inorder(root->left);
				root->data->print();
				inorder(root->right);
			}
		}

		void preorder(Node*root){
			if (root!=NULL){
    		root->data->print();
			preorder(root->left);
			preorder(root->right);
			}
		}
		void remove(int addr){
			remove(this->root, addr);
		}
		
		Data* search(Node* root, int addr){
			if (root==nullptr) return NULL;
			if (root->getAdd()==addr) return root->data->data;
			else if (root->getAdd()<addr)
				return search(root->right, addr);
			else if (root->getAdd()>addr)
				return search(root->left, addr);
			return NULL;
		}

		Data* search(int addr){
			return search(this->root, addr);
			
		}

		Elem* searchE(Node* root, int addr){
			if (root==nullptr) return NULL;
			if (root->getAdd()==addr) return root->data;
			else if (root->getAdd()<addr)
				return searchE(root->right, addr);
			else if (root->getAdd()>addr)
				return searchE(root->left, addr);
			return NULL;
		}
};


class Queue{
	private:
		class Node;
	private:
		AVL*tree;
		Node* front{nullptr};
		Node* end{nullptr};
		int size{0};
		int maxSize{0};
	private:
		class Node{
			private:
				Node* next{nullptr};
				Elem* element;
				friend class Queue;
			public:	
				Node(int addr,Data* cont, bool b){
					element= new Elem(addr,cont,b);
				}
		};
	public:
		Queue(int s){
			tree =new AVL();
			maxSize = s;
		}
		Elem* enQueue(int addr,Data* cont, bool b){
			Node* temp= new Node(addr,cont,b);
			if (front==nullptr){
				front= temp;
			}
			else
				end->next= temp;
			end= temp;
			end->next= front;
			++size;
			tree->insert(temp->element);
			if (size> maxSize){
				size=maxSize;
				Elem* t= deQueue();
				tree->remove(t->addr);
				return t;
			}
			return NULL;
		}
		Elem* deQueue(){
			Elem* temp;
			if (front == end) {
				temp = front->element;
				delete front;
				front = NULL;
				end = NULL;
				size=0;
			}
			else 
			{
				Node* temp1= front;
				temp= front->element;
				front = front->next;
				end->next = front;
				delete temp1;
			}
			return temp;
		}
		Data* search(int addr){
			return tree->search(tree->root, addr);
		}
		Elem* searchE(int addr){
			return tree->searchE(tree->root,addr);
		}
		void change(Elem* &temp, Data* cont){
			temp->data= cont;
			temp->sync= false;
		}
		void printQ(){
			Node* temp= front;
			while (temp->next!= front){
				temp->element->print();
				temp= temp->next;
			}
			temp->element->print();
			
		}
		void inorder(){
			tree->inorder(tree->root);
		}
		void preorder(){
			tree->preorder(tree->root);
		}
};


class Cache {
	private:
		Queue *myQ;
	public:
		Cache(int s) {
			myQ= new Queue(s);
		}
		~Cache() {
			
		}
		Data* read(int addr);
		Elem* put(int addr, Data* cont);
		Elem* write(int addr, Data* cont);
		void print();
		void preOrder();
		void inOrder();	
};


#endif