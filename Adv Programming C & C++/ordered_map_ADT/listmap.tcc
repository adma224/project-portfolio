// $Id: listmap.tcc,v 1.15 2019-10-30 12:44:53-07 - - $

#include "listmap.h"
#include "debug.h"

//
/////////////////////////////////////////////////////////////////
// Operations on listmap.
/////////////////////////////////////////////////////////////////
//

//
// listmap::~listmap()
//
template <typename key_t, typename mapped_t, class less_t>
listmap<key_t,mapped_t,less_t>::~listmap() {
   DEBUGF ('l', reinterpret_cast<const void*> (this));
   while(!empty()){
     erase(begin());
   }
}

//
// iterator listmap::insert (const value_type&)
//
template <typename key_t, typename mapped_t, class less_t>
typename listmap<key_t,mapped_t,less_t>::iterator
listmap<key_t,mapped_t,less_t>::insert (const value_type& pair) {
   DEBUGF ('l', &pair << "->" << pair);
   if(empty()){
      node* insert = new node (anchor(), anchor(), pair);
      anchor()->next = insert;
      anchor()->prev = insert;
      iterator itor (insert);
      return itor;
   } else{
      iterator itor;
      for(itor=begin();itor!=end();++itor){
         if(itor->first==pair.first){
            itor->second = pair.second;
            return itor;
         }
         if(less(pair.first,itor->first)){
            node* where = itor.where;
            node* insert = new node
               (where,where->prev,pair);
            where->prev->next=insert;
            where->prev = insert;
            return --itor;
         }
      }
      node* insert = new node(anchor(),anchor()->prev,pair);
      anchor()->prev->next = insert;
      anchor()->prev = insert;
      iterator ret (insert);
      return ret;
   }
}

//
// listmap::find(const key_type&)
//
template <typename key_t, typename mapped_t, class less_t>
typename listmap<key_t,mapped_t,less_t>::iterator
listmap<key_t,mapped_t,less_t>::find (const key_type& that) {
   DEBUGF ('l', that);
   iterator itor;
   for(itor=begin();itor!=end();++itor){
      if(that==itor->first){
      return itor;
      }
   }
   return itor;
}

//
// iterator listmap::erase (iterator position)
//
template <typename key_t, typename mapped_t, class less_t>
typename listmap<key_t,mapped_t,less_t>::iterator
listmap<key_t,mapped_t,less_t>::erase (iterator position) {
   DEBUGF ('l', &*position);
   node* where = position.where;
   if(where&&where!=anchor()){
      where->prev->next = where->next;
      where->next->prev = where->prev;
      delete where;
   }
   return position;
}


