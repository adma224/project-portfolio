#include <sstream>
#include <memory>
#include <list>
#include <map>
#include "util.h"
#include <queue>

using namespace std;

class Graph;



class vertex{
  enum Color{WHITE, RED, BLACK};
  friend class Graph;
  private:
    struct link {
      string relation;
      vertex* node;
      link(string& relation_, vertex* node_)
        :relation(relation_),node(node_){}
    };
    Color color;
  public:
    using link_ptr = shared_ptr<link>;
  private:
    list<link_ptr> links;
    string name;
    void explore(queue<pair<link_ptr,string>>&,string&);
    bool connect(string&,vertex*);
    bool contains(string&);
  public:
    vertex(string name_) : name(name_){ color=Color::WHITE;}
    string getname(){return name;}
};

class Graph{
  friend class vertex;
  
  private:
    map<string,vertex*> members;
  public:
    using link_ptr = shared_ptr<vertex::link>;
    size_t size(){return members.size();}
    bool insert(wordvec&);
    bool clear();
    string degrees(string&, string&);
};


