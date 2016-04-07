#pragma once 

#include <string>
#include "mysql_connection.h" 

#include <time.h>
#include <cppconn/driver.h>
#include <cppconn/exception.h>
#include <cppconn/resultset.h>
#include <cppconn/statement.h> 

using namespace std;

class ReachedAchievement {
private:
	time_t timeAchieved;
public:
	ReachedAchievement();
};

#pragma once
