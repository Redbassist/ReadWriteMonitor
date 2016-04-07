#include "MyThreadPool.h"

#include <mutex>
#include <algorithm>
#include <condition_variable>

//Method used to call the worker's work method. Is necessary for the calling of different task's methods
/*static int StaticThreadProcess(void * param)
{
	((Worker*)param)->Work();
	return 0;
}*/

Semaphore::Semaphore() :
	_condition(new std::condition_variable()),
	_mutex(new std::mutex()),
	_count(0)
{ }

Semaphore::~Semaphore()
{
	delete _condition;
	delete _mutex;
}

void Semaphore::Wait()
{
	std::unique_lock<std::mutex> lock(*_mutex);
	while (!_count)
		_condition->wait(lock);
	--_count;
}//Wait

void Semaphore::Notify()
{
	std::unique_lock<std::mutex> lock(*_mutex);
	_condition->notify_one();
	++_count;
}//Notify


//Constructor of the Worker. 
Worker::Worker() :
	mtx(new std::mutex()),
	alive(true)
{ 
	//thd = new std::thread(StaticThreadProcess, "hello");
	thd = new std::thread(
		[&]
	{
		while (alive)
		{ 
			while (!tasks.empty())
			{
				mtx->lock();
				Task task = tasks.front(); 
				Parameter p = parameters.front();
				mtx->unlock();

				task(p.param);

				mtx->lock();
				if (!tasks.empty()) {
					tasks.pop();
					parameters.pop();
				}
				mtx->unlock();
			} 
		}
	});
}

//Method is the main work of the worker. 
//Locks the semaphore for each task in it's list and runs that task.
//Unlocks semaphore after each task has been run to give other workers a chance
//to move through their tasks.
/*void Worker::Work() {
	while (alive)
	{
		while (!tasks.empty())
		{
			_semaphore.Wait();

			//Locking the mutex so that nothing else can access the tasks at the same time
			mtx->lock();
			Task task = tasks.front(); //Sets current task to the task at the front of the queue
			Parameter p = parameters.front(); //Takes the parameter from the front of it's queue. Corresponds to task above ^
			mtx->unlock();

			int alive = 1; //This is used to tell if the task needs to be pushed to the back of the queue to be worked again
			task(p.param); //Running the task. This calls the task method.

			//Locking the mutex so that nothing else can access the tasks at the same time
			mtx->lock();
			if (!tasks.empty()) { 
				if (alive == 1) {
					tasks.push(tasks.front()); //Moves current task to the back of the queue
					parameters.push(parameters.front()); //Moves current parameter to the back of the queue
				}
				tasks.pop(); //removes task from front of the queue
				parameters.pop(); //removes parameter from front of the queue
			}

			mtx->unlock();

			_semaphore.Notify();
		}
	}
};*/

//Destructor for the worker, clears the tasks queue, destroys the mtx and waits for the thread to end
Worker::~Worker()
{
	RemoveTasks();
	alive = false; //Worker not alive
	_semaphore.Notify();

	int threadReturnValue; 

	delete thd;
	delete mtx; 
}

//Method not being used currently
int Worker::id()
{
	return 0;
}

//Size of the tasks queue
int Worker::taskCount()
{
	return tasks.size();
}

//Wait for the tasks queue to be empty (ending the worker)
void Worker::Join()
{
	while (tasks.size());
}

//Adding a task and it's parameter to the worker to be worked
void Worker::AddTask(Task task, Parameter p)
{
	mtx->lock();
	tasks.push(task);
	parameters.push(p);
	mtx->unlock();
}

//Removing all tasks in the queue
void Worker::RemoveTasks()
{
	mtx->lock();
	tasks = std::queue<Task>();
	mtx->unlock();
}

//used for creating a singleton of the the threadpool
bool instanceFlag = false;
ThreadPool* instance = NULL;

//used for creating a singleton of the the threadpool
ThreadPool* ThreadPool::GetInstance()
{
	if (!instanceFlag)
	{
		instance = new ThreadPool();
		instanceFlag = true;
		return instance;
	}
	else
	{
		return instance; //Return instance of the threadpool
	}
}

//Constructor of the threadpool 
ThreadPool::ThreadPool(unsigned int threadCount) :
	index(0)
{
	/*if (!threadCount)
		threadCount = SDL_GetCPUCount(); //Returns the number of logical cores <- Not using as this is creating errors. */
	workers = std::vector<Worker>(4);
}

//Destructor of the threadpool. Removes tasks from workers and waits for threads to end
ThreadPool::~ThreadPool()
{
	RemoveTasks();
	Join();
}

//Number of threads running are returned
int ThreadPool::threadCount()
{
	return workers.size();
}

//Total number of tasks in the threadpool
int ThreadPool::taskCount()
{
	int task_count = 0;
	for (auto& worker : workers)
		task_count += worker.taskCount();
	return task_count;
}

//Wait for the threads to finish (0)
void ThreadPool::Join()
{
	while (taskCount());
}

//Add task to the next worker. (Index is kept track of)
void ThreadPool::AddTask(Worker::Task task, Parameter p)
{
	workers[index++].AddTask(task, p);
	if (index >= workers.size())
		index = 0;
}

//Remove all tasks from each of the workers
void ThreadPool::RemoveTasks()
{
	for (auto& worker : workers) //Looping through all the workers
		worker.RemoveTasks();
}