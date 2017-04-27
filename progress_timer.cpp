#include <boost/progress.hpp>
#include <iostream>
#include <sstream>

using namespace std;
using namespace boost;

int main()
{
	int i;
	boost::progress_timer t;
	for(i=0;i<100000000;i++)
	{
	}
	
	cout << t.elapsed() << endl;

	stringstream ss;
	{
		progress_timer t(ss);
	}

	cout << ss.str() << endl;
	
	return 0;
}
