#pragma once

#include <boost/asio.hpp>
#include <boost/thread.hpp>
#include <string>
#include <iostream>
#include <vector>

#define BOOST_ASIO_ENABLE_HANDLER_TRACKING

class talk_to_client;
boost::asio::io_service service;
std::vector<boost::shared_ptr<talk_to_client>> clients;
boost::recursive_mutex clients_cs;


class talk_to_client :public boost::enable_shared_from_this<talk_to_client>, boost::noncopyable
{
public:
	talk_to_client():sock_(service),started_(false),timer_(service),clients_changed_(false){}

public:
	typedef boost::system::error_code error_code;
	typedef boost::shared_ptr<talk_to_client> ptr;

	void start(){
		started_ = true;
		clients.push_back(shared_from_this());
		last_ping = boost::posix_time::microsec_clock::local_time();
		do_read();
	}

	static ptr new_(){ ptr new_(new talk_to_client); return new_; }

	void stop(){
		if (!started_) return;
		started_ = false;
		sock_.close();
		ptr self = shared_from_this();
		auto it = std::find(clients.begin(), clients.end(), self);
		clients.erase(it);
		set_clients_changed();
	}

	bool started(){ return started_; }
	boost::asio::ip::tcp::socket & sock() { return sock_; }
	std::string username() const { return username_; }

	void set_clients_changed(){ clients_changed_ = true; }

	void on_read(const error_code &err, size_t bytes)
	{
		if (err) stop();
		if (!started()) return;
		std::string msg(read_buffer_, bytes);
		std::cout << "on_read reading:" << msg << "," << bytes << "bytes" << std::endl;
		if (msg.find("login ") == 0) on_login(msg);
		else if (msg.find("ping") == 0) on_ping();
		else if (msg.find("ask_clients") == 0) on_clients();
	}

	void on_write(const boost::system::error_code & error, size_t bytes){ do_read(); }

	void on_login(const std::string &msg){
		std::istringstream in(msg);
		in >> username_ >> username_;
		do_write("login ok\n");
		update_clients_changed();
	}

	void update_clients_changed()
	{
		std::vector<boost::shared_ptr<talk_to_client>> copy;
		{
			boost::recursive_mutex::scoped_lock lk(clients_cs);
			copy = clients;
		}
		for (auto b = copy.begin(), e = copy.end(); b != e; ++b)
			(*b)->set_clients_changed();
	}

	void on_ping(){
		do_write(clients_changed_ ? "ping client_list_changed\n" : "ping ok\n");
		clients_changed_ = false;
	}

	void on_clients(){
		std::string msg;
		for (auto b = clients.begin(), e = clients.end(); b != e; ++b)
		{
			msg += (*b)->username() + " ";
		}
			do_write("clients " + msg + "\n");
	}

	void do_ping(){ do_write("ping\n"); }
	void do_ask_clients(){ do_write("ask_clients\n"); }
	void do_write(const error_code & error, size_t bytes) { do_read(); }
	void do_read(){
		boost::asio::async_read(sock_, boost::asio::buffer(read_buffer_), boost::bind(&talk_to_client::read_complete,shared_from_this(),_1, _2), boost::bind(&talk_to_client::on_read,shared_from_this(), _1, _2));
		post_check_ping();
	}

	void do_write(const std::string &msg){
		if (!started()) return;
		std::copy(msg.begin(), msg.end(), write_buffer_);
		sock_.async_write_some(boost::asio::buffer(write_buffer_, msg.size()), boost::bind(&talk_to_client::on_write, shared_from_this(), _1, _2));

	}

	size_t read_complete(const boost::system::error_code & error, size_t bytes){
		if (error) return 0;
		bool found = std::find(read_buffer_, read_buffer_ + bytes, '\n') < read_buffer_ + bytes;
		return found ? 0 : 1;
	}

	void on_check_ping(){
		boost::posix_time::ptime now = boost::posix_time::microsec_clock::local_time();
		if ((now - last_ping).total_milliseconds()>5000)
			stop();
		last_ping = boost::posix_time::microsec_clock::local_time();
	}

	void post_check_ping(){
		timer_.expires_from_now(boost::posix_time::millisec(5000));
		timer_.async_wait(boost::bind(&talk_to_client::on_check_ping,shared_from_this()));
	}

private:
	boost::asio::ip::tcp::socket sock_;
	enum { max_msg = 1024 };
	char read_buffer_[max_msg];
	char write_buffer_[max_msg];
	bool started_;
	std::string username_;
	boost::asio::deadline_timer timer_;
	boost::posix_time::ptime last_ping;
	bool clients_changed_;

};

