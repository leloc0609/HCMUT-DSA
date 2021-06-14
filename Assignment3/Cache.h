#ifndef CACHE_H
#define CACHE_H

#include "main.h"
class Node
{
public:
	Elem *elem;
	Node* next{nullptr};
public:
	Node(Elem* e) : elem(e){};
	void print(){
		elem->print();
	}
};

class ReplacementPolicy
{
public:
	ReplacementPolicy(){};
	virtual ~ReplacementPolicy(){};
	int	count{0};
	Node* front{nullptr};
	Node* end{nullptr};
public:
	virtual void insert(Elem *e) = 0; //insert e into arr[idx] if idx != -1 else into the position by replacement policy
	virtual void access(Elem *e) = 0;		  //idx is index in the cache of the accessed element
	virtual Elem* remove() = 0;
	virtual void print() = 0;
	bool isFull() { return count == MAXSIZE; }
	bool isEmpty() { return count == 0; }
	//Elem *getValue(int idx) { return arr[idx]; }

};



class FIFO : public ReplacementPolicy
{

public:
	FIFO()	{}
	~FIFO() {}
	void insert(Elem *e) {
		Node *temp= new Node(e);
		if (front == nullptr){
			front = temp;
			end	= temp;
		}
		else
		{
			end->next= temp;
			end= temp;
		}
		count++;
	}
	void access(Elem* e) {
	}
	Elem* remove() { // Pop head
		Node *temp= front;
		front= front->next;
		Elem* popEle= temp->elem;
		delete(temp);
		count--;
		return popEle;
	}
	void print() {
		Node* temp= front;
		while (temp!=NULL){
			temp->elem->print();
			temp= temp->next;
		}
	}
};

class MRU : public ReplacementPolicy
{
public:
	MRU() {}
	~MRU() {}
	void insert(Elem *e) {
		Node *temp= new Node(e);
		if (front == nullptr){
			front = temp;
			end	= temp;
		}
		else
		{
			temp->next= front;
			front= temp;
		}
		count++;
	
	}
	void access(Elem* e) { //MOVE TO FRONT
		Node* temp= front;
		if (e==front->elem){return;}
		while(temp->next->elem!=e){
			temp=temp->next;
		}
		Node* temp2= temp->next;
		temp->next=temp2->next;
		temp2->next= front;
		front= temp2;
	}
	Elem* remove() { 
		--count;
		Node* temp= front;
		front= front->next;
		Elem* popEle= temp->elem;
		return popEle;
	}
	void print() {
		Node* temp= front;
		while (temp!=NULL){
			temp->elem->print();
			temp= temp->next;
		}
	}
};
class LRU : public MRU
{
public:
	Elem* remove() override { 
		--count;
		Node* temp= front;
		while(temp->next->next!=nullptr){
			temp= temp->next;
		}
		Elem* popEle= temp->next->elem;
		temp->next= nullptr;
		return popEle;
	}
};
struct arr{
	Elem* elem{nullptr};
	int count{0};
};
class LFU : public ReplacementPolicy{
public:
	arr* myArr;
	int cap;
public:
	LFU() {
		myArr = new arr[MAXSIZE];
		cap= MAXSIZE;
		count=0;
	}
	~LFU() {}
	void insert(Elem* e) {
		count++;
		int i = count - 1;
		myArr[i].elem = e;
		myArr[i].count = 1;
	
		// Fix the min heap property if it is violated
		while (i != 0 && myArr[parent(i)].count > myArr[i].count)
		{
			swap(&myArr[i], &myArr[parent(i)]);
			i = parent(i);
		}
	}
	void access(Elem* e) {
		int i;
		for (i=0;i<count;i++){
			if (myArr[i].elem==e) break;
		}
		myArr[i].count++;
		MinHeapify(i);
	}
	Elem* remove() { 
		Elem* root = myArr[0].elem;
		myArr[0] = myArr[count-1];
		count--;
		MinHeapify(0);
		return root;
	}
	void print() {
		for(int i =0; i<count; i++){
			myArr[i].elem->print();
		}
	}
	void swap(arr* a, arr* b){
		int temp1 = a->count; 
		Elem* temp2= a->elem;
		a->count=b->count;
		a->elem=b->elem;
		b->count=temp1;
		b->elem=temp2;
	}
	int parent(int i){ return (i-1)/2;}
	int left(int i) { return (2*i + 1); }
    int right(int i) { return (2*i + 2); }

	void MinHeapify(int i)
	{
		int l = left(i);
		int r = right(i);
		int smallest = i;
		if (r < count && myArr[r].count <= myArr[i].count)
			smallest = r;
		if (l < count && myArr[l].count <= myArr[smallest].count)
			smallest = l;
		if (smallest != i)
		{
			swap(&myArr[i], &myArr[smallest]);
			MinHeapify(smallest);
		}
	}
};
class SearchEngine
{
public:
	SearchEngine(){}
	virtual ~SearchEngine(){};
	virtual bool search(int addr) {return true;}; // -1 if not found
	virtual bool insert(Elem* &value) = 0;
	virtual void remove(int addr) = 0;
	virtual void print() = 0;
	virtual Elem* searchE(int addr) = 0;
};

class DBHashing : public SearchEngine
{
public:
	int (*hash1)(int);
	int (*hash2)(int);
	int size;
	Elem** a;
public:
	DBHashing(int (*h1)(int), int (*h2)(int), int s) {
		hash1= h1;
		hash2= h2;
		size= s;
		a= new Elem*[s];
		for(int i=0;i<s;i++){
			a[i]=nullptr;
		}
	}
	~DBHashing() {}
	bool insert(Elem* &value) {
		for(int i=0;i<size*2;i++){
			int index=(hash1(value->addr)+i*hash2(value->addr))%size;
				if (a[index]==nullptr){
					a[index]=value;
					return true;
				}
		}
		return false;
	}
	void remove(int addr) {
		for(int i=0;i<size*2;i++){
			int index=(hash1(addr)+i*hash2(addr))%size;
				if (a[index]!=nullptr){
					if (a[index]->addr==addr){
						a[index]=nullptr;
						return;
					}
				}
		}
	}
	void print() {
		cout<<"Prime memory:"<<endl;
		for(int i=0;i<size;i++){
			if(a[i]!=nullptr){
				a[i]->print();
			}
		}
	}
	bool search(int addr){
		for(int i=0;i<size*2;i++){
			int index=(hash1(addr)+i*hash2(addr))%size;
			if (a[index]==nullptr){
				return true;
			}
		}
		return false;
	}
	Elem* searchE(int addr) {
		for(int i=0;i<size*2;i++){
			int index=(hash1(addr)+i*hash2(addr))%size;
			if (a[index]!=nullptr){
				if (a[index]->addr==addr){
					return a[index];
				}
			}
		}
		return NULL;
	}
};
class myAVL{
	public:
		class Node;
		Node* root{nullptr};
	public:
		class Node{
			private:
				Node* left{nullptr}; 
				Node* right{nullptr};
				Elem* data;
				int balance{0};
				friend class myAVL;
			public:
				Node(Elem* value) {
					data= value; 
				}
				int getAdd(){
					return data->addr;
				}
		};
	public:
		myAVL(){};
		~myAVL(){}
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
					int oldH = getHeight(root->right);
					remove(root->right, tmp->getAdd());
					if (oldH > getHeight(root->right)) {
						balanceLeft_Del(root);
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


class AVL : public SearchEngine
{
public:
	myAVL *newTree{nullptr};
public:
	AVL() {
		newTree = new myAVL();
	}
	~AVL() {
	}
	bool insert(Elem* &value) {
		newTree->insert(value);
		return true;
	}
	void remove(int addr) {
		newTree->remove(addr);
	}
	void print() {
		cout << "Print AVL in inorder:" <<endl;
		newTree->inorder(newTree->root);
		cout << "Print AVL in preorder:" <<endl;
		newTree->preorder(newTree->root);
	}
	Elem* searchE(int addr){
		return newTree->searchE(newTree->root,addr);
	}
};


#endif