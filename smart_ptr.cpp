#include <boost/smart_ptr.hpp>
#include <iostream>

using namespace boost;
using namespace std;

int main()
{
	auto_ptr<int> ap (new int(10));
	scoped_ptr<int> sp(ap);
	assert(ap.get()==0);

	ap.reset(new int(30));
	cout << *ap << "," << *sp << endl;

	auto_ptr<int> ap2;
	ap2 = ap;

	assert(ap.get()==0);
	scoped_ptr<int> sp2;
	
	sp2=sp;

	shared_ptr<std::exception> sp1(new std::exception("error"));
//	shared_ptr<std::bad_exception> sp2 = dynamic_pointer_cast<std::bad_exception>(sp1);
//	shared_ptr<std::exception> sp3= static_pointer_cast<std::exception>(sp2);

	assert(sp3==sp1);

	return 0;
}
