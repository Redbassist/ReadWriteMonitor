#pragma once

#include <queue>
#include <thread>
#include <vector>
#include <functional>

using namespace std;

namespace std
{
	class mutex;
	class condition_variable;
}//namespace std


struct Parameter {
	void* param;
};

class Semaphore final
{
private:
	std::condition_variable* _condition;
	std::mutex*              _mutex;
	unsigned                 _count;

public:
	Semaphore();
	~Semaphore();

public:
	void Wait();
	void Notify();
};


class Worker {

public:
	typedef std::function<int(void*)> Task;

	Worker();
	~Worker();

	int id();
	int taskCount();
	void Join();
	void AddTask(Task task, Parameter);
	void RemoveTasks();
	void Work();

private:
	queue<Task> tasks;
	queue<Parameter> parameters;
	std::thread*     thd;
	std::mutex*      mtx;
	Semaphore        _semaphore;
	bool alive;
};

class ThreadPool {
private:
	vector<Worker> workers;
	int index;
public:
	static ThreadPool* GetInstance();

	ThreadPool(unsigned int threadCount = 0);
	~ThreadPool();

	int threadCount();
	int taskCount();
	void Join();
	void AddTask(Worker::Task task, Parameter);
	void RemoveTasks();
};