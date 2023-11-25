// $Id: commands.cpp,v 1.18 2019-10-08 13:55:31-07 - - $

#include "commands.h"
#include "debug.h"
#include <map>

command_hash cmd_hash {
   {"cat"   , fn_cat   },
   {"cd"    , fn_cd    },
   {"echo"  , fn_echo  },
   {"exit"  , fn_exit  },
   {"ls"    , fn_ls    },
   {"lsr"   , fn_lsr   },
   {"make"  , fn_make  },
   {"mkdir" , fn_mkdir },
   {"prompt", fn_prompt},
   {"pwd"   , fn_pwd   },
   {"rm"    , fn_rm    },
};

command_fn find_command_fn (const string& cmd) {
   // Note: value_type is pair<const key_type, mapped_type>
   // So: iterator->first is key_type (string)
   // So: iterator->second is mapped_type (command_fn)
   DEBUGF ('c', "[" << cmd << "]");
   const auto result = cmd_hash.find (cmd);
   if (result == cmd_hash.end()) {
      throw command_error (cmd + ": no such function");
   }
   return result->second;
}

command_error::command_error (const string& what):
            runtime_error (what) {
}

int exit_status_message() {
   int status = exec::status();
   cout << exec::execname() << ": exit(" << status << ")" << endl;
   return status;
}

void fn_cat (inode_state& state, const wordvec& words){
   if (words.size() == 1)
      throw command_error (words[0] + ": Missing operand");
   directory* dir = dynamic_cast<directory*>
      (state.get_cwd()->get_contents().get());
   map<string,inode_ptr> dirents = dir->get_dirents();
   auto find = dirents.find(words[1]);
   if(find!=dirents.end()){
      wordvec data = find->second->get_contents()->readfile();
      for(unsigned int rider=0;rider<data.size();rider++){
         cout << data[rider] << " ";
      }
      cout << endl;
   }else{
      throw command_error (words[0]+ ": " + words[1] +
      ": No such file or directory");
   }
   DEBUGF ('c', state);
   DEBUGF ('c', words);
}

void fn_cd (inode_state& state, const wordvec& words){
   if(words.size()==1) {
      state.go_home();
   } 
   else state.cd_path(words[1]);
   DEBUGF ('c', state);
   DEBUGF ('c', words);
}

void fn_echo (inode_state& state, const wordvec& words){
   DEBUGF ('c', state);
   DEBUGF ('c', words);
   cout << word_range (words.cbegin() + 1, words.cend()) << endl;
}


void fn_exit (inode_state& state, const wordvec& words){
   DEBUGF ('c', state);
   DEBUGF ('c', words);
   throw ysh_exit();
}

void fn_ls (inode_state& state, const wordvec& words){
   inode_ptr ptr;   
   if (words.size() == 1 || words[1] == "."){
      ptr = state.get_cwd();
      cout << state.get_path()<<":" << endl;
   } else if (words[1] == "/"){
      ptr = state.get_root();
      cout << "/" << endl;
   } else if (words[1] == ".."){
      ptr = state.get_cwd()->get_contents()
      ->get_dirents()[".."];
   } else{
      wordvec path = split (words[1], "/");
      ptr = state.get_root()->search(path);
      cout << words[1] << ":" <<endl;
   }
   if (ptr && ptr->is_dir) {
   directory* dir = dynamic_cast<directory*>
      (ptr.get()->get_contents().get());
   map<string,inode_ptr> dirents = dir->get_dirents();
   int size;
   for (auto rider = dirents.begin(); rider != dirents.end(); rider++){
      size = rider->second->get_inode_nr();
      for(unsigned int spaces=0;
            spaces<(8-to_string(size).size());spaces++){
         cout << " ";
      }
      cout << rider->second->get_inode_nr();
      size = rider->second->get_contents()->size();
      for(unsigned int spaces=0;
            spaces<(8-to_string(size).size());spaces++){
         cout << " ";
      }
      cout << size << "  " << rider->first;
      cout << endl;
   }
   }else if (ptr != nullptr && !ptr->is_dir){
      throw command_error (words[0]+ ": " + words[1] +
      ": Is a file");
   }
   else{
      throw command_error (words[0]+ ": " + words[1] +
      ": No such file or directory");
   }
   DEBUGF ('c', state);
   DEBUGF ('c', words);
}

void fn_lsr (inode_state& state, const wordvec& words){
   DEBUGF ('c', state);
   DEBUGF ('c', words);
}

void fn_make (inode_state& state, const wordvec& words){
   if(words.size()==1) return;
   directory* dir = dynamic_cast<directory*>
      (state.get_cwd().get()->get_contents().get());
   map<string,inode_ptr> dirents = dir->get_dirents();

   auto dirent = dirents.find(words[1]);
   inode_ptr ptr;
   
   if(dirent!=dirents.end()){
      ptr = dirent->second;
   } else{
      ptr = dir->mkfile(words[1]);
   }
   plain_file* file = dynamic_cast<plain_file*>
      (ptr.get()->get_contents().get());
   file->writefile(words);
   
   DEBUGF ('c', state);
   DEBUGF ('c', words);
}

void fn_mkdir (inode_state& state, const wordvec& words){
   if(words.size()==1) return;
   directory* cwd = dynamic_cast<directory*>
      (state.get_cwd().get()->get_contents().get());
   map<string,inode_ptr> dirents = cwd->get_dirents();

   auto find = dirents.find(words[1]);
   inode_ptr ptr;
   
   if(find!=dirents.end()){
      cout << "mkdir: cannot create directory "
         <<words[1]<<": File exists"<<endl;      
   } else{
      ptr = cwd->mkdir(words[1]);
      directory* dir = dynamic_cast<directory*>
         (ptr.get()->get_contents().get());
      dir->insert(".",ptr);
      dir->insert("..",state.get_cwd());
   }
   DEBUGF ('c', state);
   DEBUGF ('c', words);
}

void fn_prompt (inode_state& state, const wordvec& words){
   if(words.size()==1) return;
   state.inode_set_prompt(words);
   DEBUGF ('c', state);
   DEBUGF ('c', words);
}

void fn_pwd (inode_state& state, const wordvec& words){
   cout << state.get_path() << endl;
   DEBUGF ('c', state);
   DEBUGF ('c', words);
}
void fn_rm (inode_state& state, const wordvec& words){
   if (words.size() == 1)
      throw command_error (words[0] + ": Missing operand");
   else if (words[1] == "/")
      throw command_error (words[0] + ": Cannot delete root");
   inode_ptr result;
   wordvec path = split (words[1], "/");
   if (words[1].at(0) == '/')
      result = state.get_root()->search(path);
   else
      result = state.get_cwd()->search(path);

   if (result == nullptr)
      throw command_error (words[0]+ ": " + words[1] +
      ": No such file or directory");
   else if (result == state.get_cwd())
      throw command_error (words[0]+ ": " + words[1] +
      ": Cannot remove current working directory");
   else{
      if (result->is_dir){
         if (result->get_contents()->size() > 2)
            throw command_error (words[0] + ": " + words[1] +
            ": Directory is not empty");
         else{
            result->get_contents()->get_dirents()[".."]->
            get_contents()->remove(path
            [path.size() - 1]);
         }
      }else{
         if (path.size() > 1){
            string temp = path[path.size() - 1];
            path.erase(path.end() - 1);
            result = state.get_root()->search(path);
            result->get_contents()->remove(temp);
         }
         else{
            state.get_cwd()->get_contents()->
            remove(path[0]);
         }
      }
   }
   DEBUGF ('c', state);
   DEBUGF ('c', words);
}

void fn_rmr (inode_state& state, const wordvec& words){
   DEBUGF ('c', state);
   DEBUGF ('c', words);
}

