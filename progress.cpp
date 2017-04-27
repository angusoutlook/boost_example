#include <boost/progress.hpp>
#include <iostream>
#include <fstream>
#include <vector>

using namespace boost;
using namespace std;

int main()
{
	vector<string> v(100);
	ofstream fs("test.txt");

	progress_display pd(v.size());

	vector<string>::iterator pos;

	for(pos=v.begin();pos!=v.end();++pos)
	{
		sleep(1);
		fs << *pos << endl;
		++pd;
	}
}
