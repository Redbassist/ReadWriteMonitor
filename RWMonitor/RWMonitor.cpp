// RWMonitor.cpp : Defines the entry point for the console application.
//

#include <iostream>
#include <string>
#include <vector>
#include <boost\regex.hpp>
#include "mysql_connection.h" 

#include <cppconn/driver.h>
#include <cppconn/exception.h>
#include <cppconn/resultset.h>
#include <cppconn/statement.h>
#include "RWMonitor.h"

#include "SFML/Graphics.hpp" 
#include "Player.h"
#include "BoundedBuffer.h"
#include "Monitor.h"
#include "Achievement.h"
#include "AchievementWatcher.h" 
#include "MyThreadPool.h"
#include "ThreadGlobals.h"

using namespace sf;

using namespace std;

vector<Achievement*> achievements;

int LoadAchievements(void* p) {
	try {
		sql::Driver *driver;
		sql::Connection *con;
		sql::Statement *stmt;
		sql::ResultSet *res;

		// Create a connection 
		driver = get_driver_instance();
		con = driver->connect("tcp://127.0.0.1:3306", "root", "guinness");
		// Connect to the MySQL test database 
		con->setSchema("gamedb");

		while (nr > 0) {
			//cant access till free
		}
		nr++;
		rw++;
		mutexR.lock();
		stmt = con->createStatement();
		res = stmt->executeQuery("SELECT * FROM achievements");
		while (res->next()) {
			cout << res->getString(2).c_str() << endl;
			//const char * name = res->getString(2).c_str();
			cout << sizeof(res->getString(2).c_str()) << endl;
			Achievement* temp = new Achievement(string(res->getString(2).c_str(), sizeof(res->getString(2))), "", res->getInt(1), res->getInt(4));
			achievements.push_back(temp);
			cout << "Achievement Loaded" << endl;
		}
		mutexR.unlock();
		nr--;
		if (nr == 0) {
			rw--;
		}

		delete res;
		delete stmt;
		delete con;

	}
	catch (sql::SQLException &e) {
		cout << "# ERR: SQLException in " << __FILE__;
		cout << "# ERR: " << e.what();
		cout << " (MySQL error code: " << e.getErrorCode();
		cout << ", SQLState: " << e.getSQLState() << " )" << endl;
	}

	cout << endl;
	return 0;
}

int main()
{
	nr = 0;
	rw = 0;
	int screenWidth = 1280;
	int screenHeight = 720;
	sf::RenderWindow window(sf::VideoMode(screenWidth, screenHeight), "SFML window");
	window.setFramerateLimit(60);

	Player* player = new Player(Vector2f(600, 600));
	BoundedBuffer* buffer = new BoundedBuffer();
	Monitor monitor = Monitor(player, buffer);

	Parameter p;
	p.param = NULL;
	
	ThreadPool::GetInstance()->AddTask(LoadAchievements, p);

	//LoadAchievements();  
	
	AchievementWatcher aw = AchievementWatcher(&achievements, player);

	while (window.isOpen())
	{
		player->Update();
		monitor.MonitorPlayer();
		aw.ApplyAchievement(buffer);
		window.clear();
		player->Draw(window);
		monitor.DrawMonitorMessages(window);
		window.display();
	}

	return EXIT_SUCCESS;

	/*system("pause");*/
}