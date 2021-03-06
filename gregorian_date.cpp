#define BOOST_DATE_TIME_SOURCE
#include <libs/date_time/src/gregorian/greg_names.hpp>
#include <libs/date_time/src/gregorian/date_generators.cpp>
#include <libs/date_time/src/gregorian/greg_month.cpp>
#include <libs/date_time/src/gregorian/greg_weekday.cpp>
#include <libs/date_time/src/gregorian/gregorian_types.cpp>
#include <boost/date_time/gregorian/gregorian.hpp>

#define BOOST_DATE_TIME_POSIX_TIME_STD_CONFIG
#include <boost/date_time/posix_time/posix_time.hpp>


#include <iostream>
using namespace std;
using namespace boost;
using namespace boost::gregorian;
using namespace boost::posix_time;

int main()
{
	date d(2015,11,20);
	
	cout << to_simple_string(d) << endl;
	cout << to_iso_string(d) << endl;
	cout << to_iso_extended_string(d) << endl;

	cout << d << endl;

	date d1(2000,01,01);
	date d2(2001,12,21);

	cout << d2 - d1 << endl;

	time_duration td(1,60,60,1000*1000*6+1000);
	time_duration td1(1,10,30,1000);
	time_duration td2 = duration_from_string("1:10:30:00101");
	
	cout << td << endl;
	cout << td1 << endl;
	cout << td2 << endl;

	ptime p(date(2010,3,20),hours(12)+minutes(20));
	
	cout << p << endl;

	return 0;
}

