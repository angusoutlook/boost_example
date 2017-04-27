#include <vector>
#include <iostream>
#include <boost/asio.hpp>

int main()
{

	std::vector<int> vint;
	boost::shared_ptr<vint> g_ptr;

	std::cout << g_ptr.unique();




}

