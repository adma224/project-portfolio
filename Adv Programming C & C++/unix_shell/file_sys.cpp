// $Id: file_sys.cpp,v 1.7 2019-07-09 14:05:44-07 - - $

#include <iostream>
#include <stdexcept>
#include <unordered_map>

using namespace std;

#include "debug.h"
#include "file_sys.h"

int inode::next_inode_nr {1};

struct file_type_hash {
   size_t operator() (file_type type) const {
      return static_cast<size_t> (type);
   }
};

ostream& operator<< (ostream& out, file_type type) {
   static unordered_map<file_type,string,file_type_hash> hash {
      {file_type::PLAIN_TYPE, "PLAIN_TYPE"},
      {file_type::DIRECTORY_TYPE, "DIRECTORY_TYPE"},
   };
   return out << hash[type];
}

inode_state::inode_state() {
  root = make_shared<inode> (file_type::DIRECTORY_TYPE);
  cwd=root;
  directory* dir = dynamic_cast<directory*>
    (cwd.get()->get_contents().get());
  root->is_dir=true;
  dir->insert(".",root);
  dir->insert("..",root);
  DEBUGF ('i', "root = " << root << ", cwd = " << cwd
          << ", prompt = \"" << prompt() << "\"");
}

base_file_ptr inode::get_contents(){
  return contents;
}

void inode::set_contents(base_file_ptr ptr){
  contents = ptr;
}

inode_ptr inode_state::get_root(){
  return root;
}
inode_ptr inode_state::get_cwd(){
  return cwd;
}
void directory::insert(string filename,inode_ptr ptr){
  dirents.insert(pair<string,inode_ptr>(filename,ptr));
}

const string& inode_state::prompt() const { return prompt_; }

void inode_state::inode_set_prompt(const wordvec& words){
  prompt_="";
  for(unsigned int curr_word=1;curr_word<words.size();curr_word++){
    
      prompt_+=words[curr_word]+" ";
    }
}

string inode_state::get_path(){
  stack<string> temp;
  string str="/";
  while(!path.empty()){
    temp.push(path.top());
    path.pop();
  }
  while(!temp.empty()){
    path.push(temp.top());
    str+=path.top();
    temp.pop();
    if(!temp.empty()) str+="/";
  }
  return str;
}

void inode_state::go_home(){
  cwd = root;
  while(!path.empty()) path.pop();
}

void inode_state::cd_path(string dirname){
  directory* dir = dynamic_cast<directory*>
    (cwd.get()->get_contents().get());
  map<string,inode_ptr> dirents = dir->get_dirents();
  auto find = dirents.find(dirname);
  if(find!=dirents.end()){
    cwd = find->second;
    if(dirname.compare(".")==0) path.top();
    else if(dirname.compare("..")==0){
      path.pop();
    }
    else{
      path.push(dirname);
    }
  } else{
    cout<<"cd: "<<dirname<<": No such file or directory"<<endl;
  }
  
}

ostream& operator<< (ostream& out, const inode_state& state) {
   out << "inode_state: root = " << state.root
       << ", cwd = " << state.cwd;
   return out;
}

inode::inode(file_type type): inode_nr (next_inode_nr++) {
   switch (type) {
      case file_type::PLAIN_TYPE:
           contents = make_shared<plain_file>();
           break;
      case file_type::DIRECTORY_TYPE:
           contents = make_shared<directory>();
           break;
   }
   DEBUGF ('i', "inode " << inode_nr << ", type = " << type);
}

int inode::get_inode_nr() const {
   DEBUGF ('i', "inode = " << inode_nr);
   return inode_nr;
}


file_error::file_error (const string& what):
            runtime_error (what) {
}

const wordvec& base_file::readfile() const {
   throw file_error ("is a " + error_file_type());
}

void base_file::writefile (const wordvec&) {
   throw file_error ("is a " + error_file_type());
}

void base_file::remove (const string&) {
   throw file_error ("is a " + error_file_type());
}

inode_ptr base_file::mkdir (const string&) {
   throw file_error ("is a " + error_file_type());
}

inode_ptr base_file::mkfile (const string&) {
   throw file_error ("is a " + error_file_type());
}

map<string,inode_ptr> base_file::get_dirents(){
   throw file_error ("is a plain file");
}


size_t plain_file::size() const {
   size_t size =0;
   string curr_word;
   for(auto rider = data.begin(); rider != data.end(); rider++){
    curr_word = *rider;
    size += curr_word.size();
    size++;
   }
   DEBUGF ('i', "size = " << size);
   return size;
}

const wordvec& plain_file::readfile() const {
   DEBUGF ('i', data);
   return data;
}

void plain_file::writefile (const wordvec& words) {
   DEBUGF ('i', words);

   data.clear();
   auto rider = words.begin();
   rider++;
   rider++;
   while(rider!=words.end()){
    data.push_back(*rider);
    rider++;
   }
}

size_t directory::size() const {
   size_t size = dirents.size();
   DEBUGF ('i', "size = " << size);
   return size;
}

void directory::remove (const string& filename) {
  dirents.erase(filename);
   DEBUGF ('i', filename);
}

inode_ptr directory::mkdir (const string& dirname) {
  DEBUGF ('i', dirname);
  inode_ptr ptr = make_shared<inode>(file_type::DIRECTORY_TYPE);
  ptr->set_name(dirname);
  string str = dirname;
  insert(str,ptr);
  ptr->is_dir=true;
  return ptr;
}

inode_ptr directory::mkfile (const string& filename) {
  DEBUGF ('i', filename);
  inode_ptr ptr = make_shared<inode>(file_type::PLAIN_TYPE);
  ptr->set_name(filename);
  string str = filename;
  insert(str,ptr);
  return ptr;
}

map<string,inode_ptr> directory::get_dirents(){
  return dirents;
}


inode_ptr inode::search(const wordvec& path_vec) const{
   wordvec temp = path_vec;
   auto place = contents->get_dirents().begin();
   while (place != contents->get_dirents().end()){
      if (place->first != "." && place->first != ".."){
         if (place->second->name == temp[0]){
            if (place->second->is_dir && temp.size() > 1){
               temp.erase(temp.begin());
               return place->second->search(temp);
            }else
               return place->second;
         }
      }
      else{
         if (place->second->name == temp[0])
            return place->second;
      }
      ++place;
   }
   return nullptr;
}
