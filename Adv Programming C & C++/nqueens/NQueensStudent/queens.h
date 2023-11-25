#include <map>
#include <fstream>
#include <iostream>
#include <bitset>
#include <stack>
#include <cmath>

using namespace std;

class Queens{
	private:
		map <int,int> queens;
   stack<int> heap;
		bool check(int,int);
    int next(int);
		int size;
	public:
		Queens()=default;
		void set_size(int n){size=n;};
		int get_size(){return size;};
		bool run_queens();
		void clear();
		bool add(int,int);
    		void print(FILE*);
};

