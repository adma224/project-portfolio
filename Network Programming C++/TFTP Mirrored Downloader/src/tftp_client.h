
#include "unp.h"
#include "util.h"
#include <vector>
#include <string>
#include <fstream>
#include <regex>
#include <iostream>
#include <map>
#include <mutex>
#include <chrono>

using namespace std;



multimap<string, unsigned short> get_servers(const char* file_path);

int get_file_size(multimap<string,unsigned short> &server_map, char* file_path);

int get_file(ofstream &ofs, map<string,unsigned short> &server_map, int chunk_size);

int get_chunk(bool & b, const string IP, const unsigned short port,
		const char* filename, unsigned long chunk_size, const int chunk);


