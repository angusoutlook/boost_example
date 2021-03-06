#include <iostream>
#include <boost/asio.hpp>
#include <boost/array.hpp>
#include "TradeCommHeaders.h"

using namespace std;
using namespace boost::asio;

io_service                  io_srv;
ip::tcp::resolver           resolver(io_srv);
ip::tcp::socket             sock(io_srv);
boost::array<char,4096>     buff;

void read_handler(const boost::system::error_code& error,std::size_t bytes_transferred)
{
    if(!error){
        std::cout << std::string(buff.data(),bytes_transferred) << std::endl;
        sock.async_read_some(boost::asio::buffer(buff),read_handler);
    }
}

void write_handler(const boost::system::error_code& error,std::size_t bytes_transferred)
{
    if(!error){
        std::cout << std::string(buff.data(),bytes_transferred) << std::endl;
    }
}

void connect_handler(const boost::system::error_code& error)
{
    if(!error)
    {
        boost::asio::write(sock,boost::asio::buffer("GET / HTTP 1.1\r\nHost: baidu.com\r\n\r\n"));
        sock.async_read_some(boost::asio::buffer(buff),read_handler);
    }
}

void resolve_handler(const boost::system::error_code& error,boost::asio::ip::tcp::resolver::iterator it)
{
    if(!error)
    {
        sock.async_connect(*it,connect_handler);
    }
}

int main()
{
    boost::asio::ip::tcp::resolver::query   query("www.baidu.com","443");
    resolver.async_resolve(query,resolve_handler);
    io_srv.run();
    return 0;

}
