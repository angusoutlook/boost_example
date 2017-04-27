#include <boost/timer.hpp>
#include <iostream>
using namespace boost;
using namespace std;

int main()
{
	timer t;
	cout << "max timespan:" << t.elapsed_max()/3600/24/356 << "y" << endl;
	cout << "min timespan:" << t.elapsed_min() << "s" << endl;

	cout << " now time elapsed:" << t.elapsed() << "s" << endl;
	cout << "clock per seconds:" << CLOCKS_PER_SEC << endl;

	return 0;
}
