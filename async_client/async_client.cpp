// async_client.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include "talk_to_server.h"


int _tmain(int argc, _TCHAR* argv[])
{
	boost::asio::ip::tcp::endpoint ep(boost::asio::ip::address::from_string("127.0.0.1"), 8001);
	char * messages[] = { "alice", "angus", "charles", 0
	};
	for (char** message = messages; *message; ++message)  {
		talk_to_server::start(ep, *message);
		boost::this_thread::sleep(boost::posix_time::millisec(100));

	}

	service.run();

	boost::this_thread::sleep(boost::posix_time::milliseconds(5000));

	getchar();

	return 0;
}

