#pragma once
#include <string> 
#include <vector>  
#include <mutex>
#include <condition_variable>

using namespace std; 

class BoundedBuffer {

public:
	BoundedBuffer();
	void Deposit(string);
	string Fetch();
private:
	int capacity = 1000;
	string buffer[1000];
	int front;
	int rear;
	int count;

	mutex accessMtx;
	condition_variable not_full;
	condition_variable not_empty;
}; 