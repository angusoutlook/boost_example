#pragma once


//#define BOOST_ASIO_ENABLE_HANDLER_TRACKING

#include <boost/enable_shared_from_this.hpp>
#include <boost/noncopyable.hpp>
#include <boost/asio.hpp>
#include <boost/bind.hpp>
#include <string>
#include <iostream>


#define MEM_FN(x)		boost::bind(&self_type::x, shared_from_this())
#define MEM_FN1(x,y)	boost::bind(&self_type::x, shared_from_this(), y)
#define MEM_FN2(x,y,z)	boost::bind(&self_type::x, shared_from_this(), y, z)

boost::asio::io_service service;

class talk_to_server:public boost::enable_shared_from_this<talk_to_server>,boost::noncopyable {

	typedef talk_to_server self_type;
	talk_to_server(const std::string &message) :sock_(service), started_(true), message_(message) {}
	void start(boost::asio::ip::tcp::endpoint ep) {
		sock_.async_connect(ep, MEM_FN1(on_connect, _1));
	}
public:

	typedef boost::system::error_code error_code;
	typedef boost::shared_ptr<talk_to_server> ptr;
	static ptr start(boost::asio::ip::tcp::endpoint ep, const std::string & message) {
		ptr new_(new talk_to_server(message));
		new_->start(ep);
		return new_;
	}

	void stop() {
		if (!started_) return;
		started_ = false;
		sock_.close();
	}

	void do_read() {
		boost::asio::async_read(sock_, boost::asio::buffer(read_buffer_), MEM_FN2(read_complete, _1, _2), MEM_FN2(on_read, _1, _2));
	}

	void do_write(const std::string &msg) {
		if (!started()) return;
		std::copy(msg.begin(), msg.end(), write_buffer_);
		//std::cout << "do_write write the msg :" << msg << "with " << msg.length() << "bytes" << std::endl;
		sock_.async_write_some(boost::asio::buffer(write_buffer_,msg.size()), MEM_FN2(on_write, _1, _2));
		//boost::asio::async_write(sock_, boost::asio::buffer(write_buffer_), MEM_FN2(on_write, _1, _2));
	}

	size_t read_complete(const boost::system::error_code &err, size_t bytes) {
		static int read_complete_hits = 0;
		read_complete_hits++;
		if (err) return 0;
		bool found = std::find(read_buffer_, read_buffer_ + bytes, '\n') < read_buffer_ + bytes;
		std::cout << (found ? "true" : "false") << "read_complete hits is " << read_complete_hits << "times, " << std::endl;
		return found ? 0 : 1;
	}

	void on_connect(const boost::system::error_code &err) {
		if (!err) {
			do_write(message_ + "\n");
		}
		else {
			std::cout << "connect to 127.0.0.1 8001 fialed! error_code is : " << err << std::endl;
			stop();
		}
	}

	void on_read(const boost::system::error_code &err, size_t bytes) {
		if (!err) {
			std::string copy(read_buffer_, bytes - 1);
			std::cout << "server echoed our " << message_ << ": " << (copy == message_ ? "OK":"FAIL") << std::endl;
		}

		
		stop();
	}

	void on_write(const boost::system::error_code & err, size_t bytes) {
		if (!err) {
			std::cout << "on_write get the msg write bytes is  :" << bytes << std::endl;
			std::cout << "the msg is " << write_buffer_ << std::endl;
		}
		
		do_read();
	}

	bool started() {
		return started_;
	}

private:
	boost::asio::ip::tcp::socket sock_;
	enum {max_msg = 1024};
	char read_buffer_[max_msg];
	char write_buffer_[max_msg];
	bool started_;
	std::string message_;


};

