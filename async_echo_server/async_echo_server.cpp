// async_echo_server.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"

#include "talk_to_client.h"

boost::asio::ip::tcp::acceptor acceptor(service, boost::asio::ip::tcp::endpoint(boost::asio::ip::tcp::v4(), 8001));

void handle_accept(talk_to_client::ptr client, const talk_to_client::error_code & err) {
	client->start();
	talk_to_client::ptr new_client = talk_to_client::new_();
	acceptor.async_accept(new_client->sock(), boost::bind(handle_accept, new_client, _1));

}

int _tmain(int argc, _TCHAR* argv[])
{
	talk_to_client::ptr client = talk_to_client::new_();
	acceptor.async_accept(client->sock(), boost::bind(handle_accept, client, _1));
	service.run();

	boost::this_thread::sleep(boost::posix_time::milliseconds(5000));

	return 0;
}

