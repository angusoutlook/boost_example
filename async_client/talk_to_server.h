#pragma once

//#define BOOST_ASIO_ENABLE_HANDLER_TRACKING

#include <boost/asio.hpp>
#include <boost/thread.hpp>
#include <string>
#include <vector>
#include <iostream>

#define MEM_FN(x)		boost::bind(&self_type::x,shared_from_this())
#define MEM_FN1(x,y)	boost::bind(&self_type::x,shared_from_this(),y)
#define MEM_FN2(x,y,z)	boost::bind(&self_type::x,shared_from_this(),y,z)

boost::asio::io_service service;

class talk_to_server :public boost::enable_shared_from_this<talk_to_server>, boost::noncopyable{

	typedef talk_to_server self_type;

	talk_to_server(const std::string &username) :sock_(service), started_(true), username_(username), timer_(service){}

	void start(boost::asio::ip::tcp::endpoint ep) {
		sock_.async_connect(ep, MEM_FN1(on_connect, _1));
	}

public:

	static boost::shared_ptr<talk_to_server> start(boost::asio::ip::tcp::endpoint ep, const std::string &username){
		boost::shared_ptr<talk_to_server> new_(new talk_to_server(username));
		new_->start(ep);
		return new_;
	}

	void stop() {
		if (!started_) {
			return;
		}
		started_ = false;
		sock_.close();
	}

	bool started() {
		return started_;
	}

	void on_connect(const boost::system::error_code &err) {
		if (!err) do_write("login " + username_ + "\n");
		else stop();
	}

	void on_read(const boost::system::error_code &err, size_t bytes) {
		if (err) stop();
		if (!started()) return;
		std::string msg(read_buffer_, bytes);
		if (msg.find("login ") == 0) on_login();
		else if (msg.find("ping") == 0) on_ping(msg);
		else if (msg.find("clients ") == 0) on_clients(msg);
	}

	void on_write(const boost::system::error_code &err, size_t bytes){ do_read(); }

	void on_login() {
		do_ask_clients();
	}

	void on_ping(const std::string &msg) {
		std::istringstream in(msg);
		std::string answer;
		in >> answer >> answer;
		if (answer == "client_list_changed") do_ask_clients();
		else postpone_ping();
	}

	void on_clients(const std::string &msg){
		std::string clients = msg.substr(8);
		std::cout << username_ << ", new clients list: " << clients;
		postpone_ping();
	}

	void do_ping(){
		do_write("ping\n");

	}

	void postpone_ping(){
		timer_.expires_from_now(boost::posix_time::millisec(rand() % 7000));
		timer_.async_wait(MEM_FN(do_ping));
	}

	void do_ask_clients(){ do_write("ask_clients\n"); }


	void do_read(){
		boost::asio::async_read(sock_, boost::asio::buffer(read_buffer_), MEM_FN2(read_complete, _1, _2), MEM_FN2(on_read, _1, _2));

	}

	void do_write(const std::string &msg){
		if (!started()) return;
		std::copy(msg.begin(), msg.end(), write_buffer_);
		sock_.async_write_some(boost::asio::buffer(write_buffer_, msg.size()), MEM_FN2(on_write, _1, _2));
	}
	

private:

	size_t read_complete(const boost::system::error_code &err, size_t bytes) {
		if (err) return 0;
		bool found = std::find(read_buffer_, read_buffer_ + bytes, '\n') < read_buffer_ + bytes;
		return found ? 0 : 1;

	}

	boost::asio::ip::tcp::socket sock_;
	enum {max_msg = 1024};
	char read_buffer_[max_msg];
	char write_buffer_[max_msg];
	bool started_;
	std::string username_;
	boost::asio::deadline_timer timer_;
};

