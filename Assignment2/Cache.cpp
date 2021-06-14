#include "main.h"

Data* Cache::read(int addr) {
	return myQ->search(addr);
}
Elem* Cache::put(int addr, Data* cont) {
	return myQ->enQueue(addr, cont, true);
}
Elem* Cache::write(int addr, Data* cont) {
	Elem* t= myQ->searchE(addr);
    if (t!=NULL){
        myQ->change(t,cont);
    }
    else{
        myQ->enQueue(addr, cont, false);
    }
    return NULL;
}
void Cache::print() {
    myQ->printQ();
}
void Cache::preOrder() {
	myQ->preorder();
}
void Cache::inOrder() {
    myQ->inorder();
}
