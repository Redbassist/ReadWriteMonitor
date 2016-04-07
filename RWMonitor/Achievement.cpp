#include "Achievement.h"

int DatabaseUpdate(void *data)
{
	Achievement* tempA = static_cast<Achievement*>(data);
	if (!tempA->complete) {
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

			stmt = con->createStatement();

			string temp;

			while (rw > 0) {
				cout << "Writer locked" << endl;
 			}

			rw++;

			temp.append(" REPLACE INTO gamedb.reachedachievement VALUES(");
			temp.append("0, NOW(), ");
			temp.append("'" + tempA->title + "', " + to_string(tempA->identifier) + ") ");

			rw--;
			tempA->buffer->Deposit("Achievement: " + tempA->title + " achieved!");
			tempA->complete = true;
			res = stmt->executeQuery(temp.c_str());

			delete res;
			delete stmt;
			delete con;

		}
		catch (sql::SQLException &e) {
			/*cout << "# ERR: SQLException in " << __FILE__;
			cout << "# ERR: " << e.what();
			cout << " (MySQL error code: " << e.getErrorCode();
			cout << ", SQLState: " << e.getSQLState() << " )" << endl;*/
		}
	}
	return 0;
}

Achievement::Achievement(string t, string c, int i, int v)
{
	title = t;
	category = "Unknown";
	identifier = i;
	value = v;
}

void Achievement::CompletedAchievement(string Name, BoundedBuffer* b)
{
	buffer = b;

	Parameter p;
	p.param = (void*)this;
	ThreadPool::GetInstance()->AddTask(DatabaseUpdate, p);

	if (!complete) {
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

			stmt = con->createStatement();

			string temp;

			temp.append(" REPLACE INTO gamedb.reachedachievement VALUES(");
			temp.append("0, NOW(), ");
			temp.append("'" + Name + "', " + to_string(identifier) + ") ");

			complete = true;
			res = stmt->executeQuery(temp.c_str());

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
	}
}