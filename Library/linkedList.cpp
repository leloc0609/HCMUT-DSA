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
    void insertAtIndex(int d,int i);    //  Insert at a index 
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

//  Insert to back
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

int main(){
    List *myList = new List();
    myList->insertFront(1);
    myList->insertBack(0);
    myList->insertBack(1);
    myList->print(); 
}