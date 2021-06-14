#include <iostream>

using namespace std;

class List{
private:
    class Node{
    private:
        int data;
        Node* next;
        friend class List;
    public:
        Node(int d) : data(d),next(nullptr){};
    };
private:
    Node* head;
    int count;
public:
    List(){
        head = nullptr;
        count = 0;
    }
    void insertBack(int d);             //  Insert to back
    void insertFront(int d);            //  Insert to front  
    void insertAtIndex(int d,int t);    //  Insert at a index 
    void popFront();
    void popBack();
    void popAtIndex(int t);
    void print();
};

//  Insert to back
void List :: insertBack(int d){
    Node* temp = new Node(d);
    if (count == 0){
        head = temp;
    }
    else{
        Node* cursor = head;
        while(cursor->next != nullptr){
            cursor = cursor->next;
        }
        cursor->next = temp;
    }
    count++;
    return;
}

//  Insert to front
void List :: insertFront(int d){
    Node* temp = new Node(d);
    if (count == 0){
        head = temp;
    }
    else{
        temp->next = head;
        head = temp;
    }
    count++;
    return;
}

//  Insert at a index 
void List :: insertAtIndex(int d, int t){
    if (t == 0){
        insertFront(d);
    }
    else if (t < count){
        Node* temp = new Node(d);
        Node* cursor = head;
        int index = 0;
        while (index != t-1){
            cursor = cursor->next;
            ++index;
        }
        temp->next = cursor->next;
        cursor->next = temp;
    }
    else{
        insertBack(d); // if the wanted to be inerted index is larger than the number of elements in the list, insert to the back
    }
}

// Print
void List :: print(){
    Node* temp = head;
    while(temp != nullptr){
        cout << temp->data << endl;
        temp = temp->next;
    }
}

// Pop at front
void List :: popFront(){
    Node* temp = head;
    head = head->next;
    delete(temp);
}

// Pop at back
void List :: popBack(){
    Node* cursor = head;
        while(cursor->next->next != nullptr){
            cursor = cursor->next;
        }
    Node* temp = cursor->next;
    cursor->next = nullptr;
    delete(temp);
}

// Pop at index
void List :: popAtIndex(int t){
    if (t == 0){
        popFront();
    }
    else if (t < count){
        
        Node* cursor = head;
        int index = 0;
        while (index != t-1){
            cursor = cursor->next;
            ++index;
        }
        Node* temp = cursor->next;
        cursor->next= temp->next;
        delete(temp);
    }
    else{
        popBack(); // if the wanted to be pop index is larger than the number of elements in the list, pop back
    }
}
int main(){
    List *myList = new List();
    myList->insertBack(1);
    myList->insertBack(2);
    myList->insertBack(3);
    myList->popAtIndex(1);
    myList->print(); 
}