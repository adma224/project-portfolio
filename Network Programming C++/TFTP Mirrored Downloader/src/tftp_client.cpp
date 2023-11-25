#include "tftp_client.h"

int main (int argc, char** argv) {
	/*
		Check validity of program arguments
	*/
	if(argc<4) err_sys("Usage: ./myclient <server-info.txt> <num-chunk> <filename-path>");
	string arg1(argv[2]);
	if (!all_of(arg1.begin(), arg1.end(),::isdigit))
		return err_sys("Invalid number of chunks.");
	int num_chunks = atoi(argv[2]);
	/*
		Read path file and create a map of IP addresses and ports
	*/
	for(int curr_file = 0; curr_file < argc-3; curr_file++){
		char* file_request = argv[3+curr_file];

		multimap<string,unsigned short> server_map = get_servers(argv[1]);
		if(server_map.empty()) 
		return err_sys("Error reading server_info file.");
		/*
			Send a TFTP RRQ to servers to get file size
		*/
		int file_size = get_file_size(server_map, file_request);
		if(file_size<0) {
			err_sys("File not found.");
			continue;
		}
		//cout << "Filesize:" << file_size << "\n";
		int chunk_size = file_size/num_chunks;
		//cout << "Chunk size:"<< chunk_size <<"B. Number of Chunks: "<<num_chunks  << ".\n";
		/*
			Iterate through IP addresses and ports creating a sub-process for each request
		*/
		vector<thread> threads;
		auto it = server_map.begin();
		
		for(int curr_chunk = 0; curr_chunk < num_chunks; curr_chunk++){
			if (file_size == 0){
				num_chunks = 0;
				break;
			}
			try {		
				/*
					Create copies of client info and start a thread to handle client request
				*/
				string IP = it->first;
				unsigned short port = it->second;
				bool b = false;
				//cout << "Downloading " << file_request << ": " 
				//		<< curr_chunk*(file_size/(num_chunks))<< "B. Source: "<<IP<<":"<<port<<"\n";
				get_chunk(ref(b), ref(IP), port, file_request, chunk_size, curr_chunk);
				//thread tftp_get(get_chunk, ref(b), ref(IP), port, file_request, chunk_size, curr_chunk);
				//threads.push_back(move(tftp_get));

				/*
					Move iterator and restart iteration if it reaches the end
				*/
				if(b){
					it++;
				} else{
					auto it_ = it++;
					server_map.erase(it_);
				}
				if (it == server_map.end()) it = server_map.begin();
				if (server_map.empty()) {
					return err_sys("Downloading failed.");
					continue;
				}
			}catch (int e){
				err_sys("Client had to abort.");
			}
		}
		/*
			Join the threads if they are joinable
		*/
		for (thread &th : threads){
			if (th.joinable())
			    th.join();
		}

		string split(file_request);
		vector<string> path;
		regex reg {R"(/)"};
		sregex_token_iterator p(split.begin(), split.end(), reg, -1);  
		sregex_token_iterator end; 
		while (p != end) {
			path.push_back(*p++);
		}
		string output = path.back();
		ofstream ofs(output.c_str());
		/*
			Traverse thorugh the temporary chunk files and merge them into final file
		*/
		string file = "output.txt";
		for(int i=0; i<num_chunks; i++){
			string output_chunk = file.c_str();
			output_chunk += to_string(i) + ".temp";
			ifstream ifs(output_chunk.c_str());
		    ofs << ifs.rdbuf();
			ifs.close();
			remove (output_chunk.c_str());
		}
		ofs.close();
	}
	
	return EXIT_SUCCESS;
}

multimap<string,unsigned short> get_servers(const char* file_path){
	/*
		Initialize input file stream and map
	*/
	ifstream ifs;
	ifs.open (file_path);
	multimap<string,unsigned short> server_map;
	if(ifs.good()){
		/*
			Traverse through lines in the server info file and save each address
		*/
		string line;
		vector<string> address;
		int i=0;
		while(getline(ifs, line)){
			stringstream split(line);
			string word;
			while(split >> word){
				address.push_back(word);
			}
			if (address.size()<2){
				cerr << "Wrong number of arguments in line " << i++ << ".\n";
				continue;
			}
			if (!all_of(address[1].begin(), address[1].end(),::isdigit)){
				cerr << "Wrong port number in line " << i++ << ".\n";
				continue;
			}
			string IP = address[0];
			unsigned short port = htons(stoi(address[1],nullptr,10));
			server_map.insert(pair<string, unsigned short>(IP, port));
			i++;
		}
	}
	return server_map;
}

int get_file_size(multimap<string,unsigned short> &server_map, char* file_path){
	/*
		Prepare a socket for file size request
	*/
	int sockfd;
	if ((sockfd = socket(AF_INET, SOCK_DGRAM, 0))<0)
		return -1;
	struct sockaddr_in clianddr;
	bzero(&clianddr, sizeof(clianddr));
	clianddr.sin_family = AF_INET;
	clianddr.sin_addr.s_addr = htonl(INADDR_ANY);
	bind(sockfd, (SA*) &clianddr, sizeof(clianddr));
	char buffer[MAXLINE+1];
	unsigned long filesize  = -1;
	int n;
	int path_length = strlen(file_path);
	strncpy(buffer, file_path, path_length);
	/*
		Iterate through addresses until one of them responds will a file size
	*/
	for(auto it = server_map.begin(); it!= server_map.end() || !server_map.empty(); it++){
		struct sockaddr_in servaddr;
		bzero(&servaddr, sizeof(servaddr));
		string IP = it->first;
		if (inet_pton(AF_INET, &IP[0], &servaddr.sin_addr) <= 0)
			continue;
		servaddr.sin_family = AF_INET;
		servaddr.sin_port = it->second;
		/*
			Prepare RRQ message
		*/
		unsigned short out_op = 1;
		memcpy(buffer, reinterpret_cast<void*>(&out_op), 2);
		strncpy(buffer+2, file_path, path_length);
		buffer[2+path_length] = '\0';
		buffer[3+path_length] = 'F';
		buffer[4+path_length] = '\0';
		/*
			Send and receive from bound socket
		*/
		if((n=tftp_sendto(sockfd, buffer,  5+path_length, (SA*) &servaddr, sizeof(servaddr)))>0)
			n=tftp_read(sockfd, buffer, MAXLINE);
		if(n<0){
			auto it_ = it++;
			server_map.erase(it_);
			if(server_map.empty()||it==server_map.end()){
				close(sockfd);
				return -1;
			} else{
				continue;
			}
		}
		buffer[n] = 0;
		/*
			Extract response message
		*/
		unsigned short* in_op = reinterpret_cast<unsigned short*>(buffer);
		unsigned short* block = reinterpret_cast<unsigned short*>(buffer+sizeof(short));
		if ((*in_op == 3)&&(*block==1)){
			memcpy(&filesize, reinterpret_cast<void*>(buffer+sizeof(unsigned short)*2), 
				sizeof(unsigned long));
			close(sockfd);
			return filesize;
		}			
	}
	close(sockfd);
	cerr << "Filesize unavailable\n";
	return -1;
}

int get_chunk(bool & b, const string IP, const unsigned short port,
		const char* filename, unsigned long chunk_size, const int chunk){	

	string split(filename);
	vector<string> path;
	regex reg {R"(/)"};  
	sregex_token_iterator p(split.begin(), split.end(), reg, -1);  
	sregex_token_iterator end; 
	while (p != end) {
		path.push_back(*p++);
	}
	string output_chunk = path.back();
	output_chunk += to_string(chunk) + ".temp";
	ofstream out(output_chunk, ofstream::out);
	/*
		Prepare RRQ request
	*/
	char buffer[MAXLINE+1];
	unsigned short in_op = 1;
	unsigned short chunk_num = chunk;
	int path_length = strlen(filename);
	memcpy(buffer, reinterpret_cast<void*>(&in_op), 2);
	strncpy(buffer+2, filename, path_length);
	buffer[path_length+2] = '\0';
	buffer[path_length+3] = 'C';
	memcpy(buffer+path_length+4, reinterpret_cast<void*>(&chunk_num),  2);
	memcpy(buffer+path_length+6, reinterpret_cast<void*>(&chunk_size), 8);
	buffer[path_length+13] = '\0';
	
	/*
		Prepare socket to send and receive messages
	*/
	int sockfd;
	struct sockaddr_in servaddr, clianddr;
	if ((sockfd = socket(AF_INET, SOCK_DGRAM, 0))<0)
		return -1;
	bzero(&servaddr, sizeof(servaddr));
	bzero(&clianddr, sizeof(clianddr));
	if (inet_pton(AF_INET, &IP[0], &servaddr.sin_addr) <= 0)
		return -1;
	servaddr.sin_family = AF_INET;
	clianddr.sin_family = AF_INET;
	clianddr.sin_addr.s_addr = htonl(INADDR_ANY);
	servaddr.sin_port = port;
	/*
		Send and listen for incoming packets
	*/
	int n;
	//int slice = 0;
	bind(sockfd, (SA*) &clianddr, sizeof(clianddr));
	//cout << "\nRequesting chunk " << chunk <<":"<<to_string(chunk_size) <<"\n";
	if (tftp_sendto(sockfd, buffer, path_length+15, (SA*) &servaddr, sizeof(servaddr))<0) 
		return -1;
	while ((n = tftp_read(sockfd, buffer, 516))>0) {
		//cout << "Chunk: "<< chunk << ". Slice: " << slice++ << ". Size:" << n << "B\n";
		buffer[n] = 0;
		out << buffer+4;
		if(n<516) break;
	}
	//cout << "Finished chunk "<< chunk <<":"<<to_string(chunk_size) <<"\n" ;
	close(sockfd);
	out.close();
	if (n>0) b = true;
	return 0;
}