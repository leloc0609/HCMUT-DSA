#include "main.h"
Cache::Cache(SearchEngine *s, ReplacementPolicy *r) : rp(r), s_engine(s) {}
Cache::~Cache()
{
    delete rp;
    delete s_engine;
}
Data *Cache::read(int addr)
{
        Elem* temp= s_engine->searchE(addr);
        if (temp!=NULL){
            rp->access(temp);
            return temp->data;
        }
        return NULL;
}
Elem *Cache::put(int addr, Data *cont)
{
        Elem* temp= new Elem(addr, cont, true);
        Elem* popOut= nullptr; // Element that pop out
        if (!rp->isFull()){
            rp->insert(temp);
            s_engine->insert(temp);
        }
        else
        {
            popOut= rp->remove();
            s_engine->remove(popOut->addr);
            rp->insert(temp);
            s_engine->insert(temp);
            return popOut;
        }
        return NULL;
}
Elem *Cache::write(int addr, Data *cont)
{
        Elem* temp= s_engine->searchE(addr);
        if (temp!=NULL){
            rp->access(temp);
            temp->data=cont;
            temp->sync=false;
        }
        else 
        {   
                Elem* temp= new Elem(addr, cont, false);
                Elem* popOut= nullptr; // Element that pop out
                if (!rp->isFull()){
                    rp->insert(temp);
                    s_engine->insert(temp);
                }
                else
                {
                    popOut= rp->remove();
                    s_engine->remove(popOut->addr);
                    s_engine->insert(temp);
                    rp->insert(temp);
                    return popOut;
                }
        }
        return NULL;
}
void Cache::printRP()
{
    rp->print();
}
void Cache::printSE()
{
    s_engine->print();
}
