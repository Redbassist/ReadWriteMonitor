#pragma once 

#include <string>
#include "mysql_connection.h" 

#include <cppconn/driver.h>
#include <cppconn/exception.h>
#include <cppconn/resultset.h>
#include <cppconn/statement.h> 

#include "BoundedBuffer.h"
#include "MyThreadPool.h"
#include "ThreadGlobals.h"

using namespace std;

class Achievement {
private: 

public:
	BoundedBuffer* buffer;

	string category;
	int identifier;
	int value;
	bool complete = false;

	string title;
	Achievement(string t, string c, int i, int v); 
	void CompletedAchievement(string Name, BoundedBuffer* b);
};

