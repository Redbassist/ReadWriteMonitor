#include "BoundedBuffer.H"
#include <iostream>


BoundedBuffer::BoundedBuffer()
{
	front = 0;
	rear = 0;
	count = 0; 
}

string BoundedBuffer::Fetch()
{
	std::unique_lock<std::mutex> l(accessMtx);

	not_empty.wait(l, [this]() {return count != 0; });

	string result = buffer[front];
	front = (front + 1) % capacity;
	--count;

	not_full.notify_one();

	return result;
}


void BoundedBuffer::Deposit(string data)
{
	std::unique_lock<std::mutex> l(accessMtx);

	not_full.wait(l, [this]() {return count != capacity; });

	buffer[rear] = data;
	rear = (rear + 1) % capacity;
	++count;

	not_empty.notify_one();
}
