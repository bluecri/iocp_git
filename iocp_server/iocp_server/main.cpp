#include "stdafx.h"


#include <stdlib.h>
#include <iostream>
#include <sstream>
#include <stdexcept>

#include <mysql.h>

#define EXAMPLE_HOST NULL	//localhost
#define EXAMPLE_USER "clientUser7"
#define EXAMPLE_PASS "clientUser7"
#define EXAMPLE_DB "world"
#define EXAMPLE_PORT 3306

#include "SendRequestSessionConcurrentQueue.h"
#include "ClientSessionManager.h"
#include "ServerSessionManager.h"
#include "ClientSession.h"
#include "ServerSession.h"
#include "PlayerManager.h"

using namespace std;

int main(int argc, const char **argv)
{
	//iocp



	// Global
	GSendRequestSessionQueue = new SendRequestSessionConcurrentQueue();
	GClientSessionManager = new ClientSessionManager();
	GServerSessionManager = new ServerSessionManager();

	GClientSessionManager->ReturnSession(new ClientSession());
	GPlayerManager = new PlayerManager();

	return 0;
}

/*
MYSQL mysql;
mysql_init(&mysql);
if (!mysql_real_connect(&mysql, EXAMPLE_HOST, EXAMPLE_USER, EXAMPLE_PASS, EXAMPLE_DB, EXAMPLE_PORT, NULL, 0)) {
cout << "error" << endl;
}
else {
cout << "success" << endl;
}
mysql_close(&mysql);

system("pause");
*/