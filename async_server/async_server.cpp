// async_server.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include "talk_to_client.h"

boost::asio::ip::tcp::acceptor acceptor(service, boost::asio::ip::tcp::endpoint(boost::asio::ip::tcp::v4(), 8001));
boost::thread_group threads;


void handle_accept(boost::shared_ptr<talk_to_client> client, const boost::system::error_code &err)
{
	client->start();
	boost::shared_ptr<talk_to_client> new_client = talk_to_client::new_();
	acceptor.async_accept(new_client->sock(), boost::bind(handle_accept, new_client, _1));

}

void listen_thread()
{
	service.run();
}

void start_listen(int thread_count){
	for (int i = 0; i < thread_count; ++i){
		threads.create_thread(listen_thread);
	}
}
int _tmain(int argc, _TCHAR* argv[])
{
	boost::shared_ptr<talk_to_client> client = talk_to_client::new_();
	acceptor.async_accept(client->sock(), boost::bind(handle_accept, client, _1));
	start_listen(100);
	threads.join_all();

	return 0;
}
