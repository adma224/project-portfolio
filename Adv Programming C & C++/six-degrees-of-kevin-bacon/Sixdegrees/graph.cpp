#include "graph.h"
#include <vector>
#include <string>
#include <sstream>
#include <iostream>

using wordvec = vector<string>;
using namespace std;

bool vertex::connect(string& name, vertex* node){
  try{
    link_ptr that_ptr = make_shared<link>(name,node);
    links.push_back(that_ptr);
    link_ptr this_ptr = make_shared<link>(name,this);
    node->links.push_back(this_ptr);
    return true;
  } catch(int e){
    return false;
  }
}
bool vertex::contains(string& name){
  list<link_ptr>::iterator itor;
  itor = links.begin();
  while(itor != links.end()){
    if(itor->get()->relation == name) return true;
    ++itor;
  }
  return false;
}
bool Graph::insert(wordvec& words){
  try{
    size_t size = words.size();
    if(size<=1) return false;
    string title = words[0];
    map<string,vertex*>::iterator itor;
    vertex* add;
    for(size_t i=1; i<size; i++){
      itor = members.find(words[i]);
      if(itor == members.end()){
        add = new vertex(words[i]);
        members.insert(pair<string,vertex*>(words[i],add));
      }
    }
    for(size_t i=1; i<size; i++){
      for(size_t j=i+1; j<size; j++){
        vertex* first = members[words[i]];
        vertex* second = members[words[j]];
        if(!first->contains(second->name)&&!second->contains(first->name)){
          if(!first->connect(title,second)) return false;
        }
      }
    }
    return true;
  } catch(int e){
    return false;
  }
}

bool Graph::clear(){
  map<string,vertex*>::iterator itor;
  itor = members.begin();
  while(itor!=members.end()){
    delete(itor->second);
    ++itor;
  }
}

void vertex::explore(queue<pair<link_ptr,string>>& process,string& last){
  for(auto itor:links){
    if(itor->node->color == Color::WHITE){ 
      itor->node->color = Color::RED;
      string insert = "-("+itor->relation+")- "+itor->node->name+" ";
      process.push(pair<link_ptr,string>(itor,last+insert));
    }
  }
  color = Color::BLACK;
}

string Graph::degrees(string& first, string& second){
  vertex* curr = members[first];
  vertex* goal = members[second];
  if(!curr||!goal) return "Not present";
  
  queue<pair<link_ptr,string>> process;
  pair<link_ptr,string> curr_pair;
  
  string output = "Not present";
  string last = first+" ";
  
  link_ptr ptr = make_shared<vertex::link>(last,curr);
  process.push(pair<vertex::link_ptr,string>(ptr,last));
  
  do {
    try{
      if(curr->name==goal->name){
        output = last;
        output.pop_back();
        break;
      }
      curr_pair = process.front();
      process.pop();
      last = curr_pair.second;
      curr = curr_pair.first->node;
      curr->explore(process, last);
    } catch(int e){
      break;
    }
  } while(!process.empty()||curr->name==goal->name);
  
  for(auto i:members){
    if(i.second) i.second->color = vertex::Color::WHITE;
  }
  
  return output;
}
