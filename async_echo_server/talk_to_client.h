
#pragma once

//#define BOOST_ASIO_ENABLE_HANDLER_TRACKING

#include <boost/enable_shared_from_this.hpp>
#include <boost/noncopyable.hpp>
#include <boost/asio.hpp>
#include <boost/bind.hpp>
#include <string>
#include <iostream>
#include <boost/thread.hpp>

boost::asio::io_service service;

#define MEM_FN(x)		boost::bind(&self_type::x, shared_from_this())
#define MEM_FN1(x,y)	boost::bind(&self_type::x, shared_from_this(), y)
#define MEM_FN2(x,y,z)	boost::bind(&self_type::x, shared_from_this(), y, z)

class talk_to_client :public boost::enable_shared_from_this<talk_to_client>,boost::noncopyable{

	typedef talk_to_client self_type;
	talk_to_client() :sock_(service), started_(false) {}

public:
	typedef boost::system::error_code error_code;
	typedef boost::shared_ptr<talk_to_client> ptr;

	void start() {
		started_ = true;
		std::cout << "talk_to_client started\n" << std::endl;
		do_read();
	}

	static ptr new_() {
		ptr new_(new talk_to_client);
		std::cout << "new talk to client \n" << std::endl;
		return new_;
	}

	void stop() {
		if (!started_) return;
		started_ = false;
		sock_.close();
	}

	boost::asio::ip::tcp::socket & sock(){
		return sock_; 
	}

	void do_read() {
		std::cout << "in do_read" << std::endl;
		boost::asio::async_read(sock_, boost::asio::buffer(read_buffer_), MEM_FN2(read_complete, _1, _2), MEM_FN2(on_read, _1, _2));
	}

	void do_write(const std::string &msg) {
		if (!started()) return;
		std::copy(msg.begin(), msg.end(), write_buffer_);
		std::cout << "do_write write the msg with " << msg << std::endl;
		sock_.async_write_some(boost::asio::buffer(write_buffer_,msg.size()), MEM_FN2(on_write, _1, _2));
		//boost::asio::async_write(sock_, boost::asio::buffer(write_buffer_), MEM_FN2(on_write, _1, _2));
	}

	size_t read_complete(const boost::system::error_code &err, size_t bytes) {
		static int read_complete_hits = 0;
		read_complete_hits++;
		if (err) return 0;
		bool found = std::find(read_buffer_, read_buffer_ + bytes, '\n') < read_buffer_ + bytes;
		std::cout << "in read_complete, " << (found ? "true" : "false") << read_complete_hits << "times " << std::endl;
		return found ? 0 : 1;
	}

	void on_read(const boost::system::error_code & err, size_t bytes) {
		if (!err) {
			std::string msg(read_buffer_, bytes);
			std::cout << "on_read get the read_buffer_ is :" << read_buffer_ << std::endl;
			do_write(msg + '\n');
		}
		stop();
	}

	void on_write(const boost::system::error_code &err, size_t bytes) {
		if (!err) {
			std::string msg(write_buffer_, bytes);
			std::cout << "on write wrtie the write_buffer_ with :" << bytes << "bytes" << std::endl;
		}
		do_read();
	}

	bool started() {
		return started_;
	}

private:
	boost::asio::ip::tcp::socket sock_;
	bool started_;
	enum { max_msg = 1024 };
	char read_buffer_[max_msg];
	char write_buffer_[max_msg];


};

