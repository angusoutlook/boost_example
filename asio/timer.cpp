#include <iostream>
#include <boost/asio.hpp>
#include "TradeCommHeaders.h"

using namespace std;
using namespace boost::asio;
using namespace boost::asio::ip;

void handle1(const boost::system::error_code& error)
{
    if(!error){
        std::cout << "5 s." << endl;
    }
}

void handle2(const boost::system::error_code& error)
{
    if(!error){
        std::cout << "10 s." << endl;
    }
}


int main()
{
    
    io_service      io_srv;
//    tcp::resolver   resolver(io_srv);
//    tcp::resolver::query query("angus.hicp.net","5478");
//    tcp::resolver::iterator     ep_iterator=resolver.resolve(query);

    tcp::socket     cli_socket(io_srv);
//    connect(cli_socket,ep_iterator);
    boost::asio::deadline_timer timer1(io_srv,boost::posix_time::seconds(5));
    timer1.async_wait(handle1);
    boost::asio::deadline_timer timer2(io_srv,boost::posix_time::seconds(10));
    timer2.async_wait(handle2);
    io_srv.run();
    
//    for(;;)
//    {
        boost::array<char,256>      buf;
        boost::system::error_code   error;
        //size_t  len=cli_socket.read_some(boost::asio::buffer(buf),error
//    }
    cout << "hello\n" << endl;

    return 0;

}
