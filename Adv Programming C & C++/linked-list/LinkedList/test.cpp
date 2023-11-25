#include "linkedlist.h"

int main(){
    LinkedList list;
    list.insert(10);
    list.insert(20);
    list.insert(20);
    list.insert(10);
    
    //list.dedup();
    printf("%s\n",list.print().c_str());
    

    list.delLast(10);
    list.delLast(20);
    list.delLast(10);
    list.delLast(20);

    printf("%s\n",list.print().c_str());

    //printf("Is it a palindrome?  %s\n",(list.palindrome()?"true":"false"));

    list.deleteList();

    return 0;
}
    
