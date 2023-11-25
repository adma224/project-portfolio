#include "queens.h"

void Queens::print(FILE* out){
	int count =0;
	for(auto iterator=queens.begin();iterator!=queens.end();iterator++){
		fprintf(out,"%i %i",iterator->first,iterator->second);
		if(count!=size){
			fprintf(out," ");
			count++;
		}
	}
}
void Queens::clear(){
	queens.clear();
	while(!heap.empty()) heap.pop();
	size=0;
}
bool Queens::add(int column, int row){
	if(check(column,row)){
	       	queens[column] = row;
		return true;
	}
	else return false;
}
int Queens::next(int column){
	int count = column;
	if(column>size) return 0;
	for(auto find = queens.find(count); find!=queens.end(); find = queens.find(count)) count++;
	if(count>size) return 0;
	else return count;
}
bool Queens::run_queens(){
	int column = next(1);
	int row;
	bool valid;
	while(queens.size()!=size){
		if(heap.empty()||heap.top()!=column){
		       	heap.push(column);
		}
		auto find = queens.find(column);
		if(find!=queens.end()){
			row = find->second+1;
			queens.erase(column);
		} else row=1;
		valid=false;
		while(row<=size){
			valid=check(column,row);
			if(valid){
				break;
			}
			row++;
		}
		if(valid){
			queens.insert(pair<int,int>(column,row));
			column = next(column+1);
		} else{
			heap.pop();
			if(heap.empty()){
			       	return false;
			} else{
				column = heap.top();
				if(column==0){
					return false;
				}
			}
		}
	}
	return true;
}
bool Queens::check(int column, int row){
	int row_diff, column_diff;
	for(auto iterator=queens.begin();iterator!=queens.end();iterator++){
		if(iterator->first==column&&iterator->second==row) continue;
		if(iterator->second==row) return false;
		row_diff = abs(row-iterator->second);
		column_diff = abs(column-iterator->first);
		if(row_diff==column_diff) return false;
	}
	return true;
}






