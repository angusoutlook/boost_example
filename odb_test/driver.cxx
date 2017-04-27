// file      : hello/driver.cxx
// copyright : not copyrighted - public domain

#include <memory>   // std::auto_ptr
#include <iostream>
#include <iomanip>

#include <odb/database.hxx>
#include <odb/transaction.hxx>

//#include "database.hxx" // create_database

#include <odb/mysql/database.hxx>


#include "person.hxx"
#include "person-odb.hxx"
#include <time.h>
#include <sys/unistd.h>
#include <linux/types.h>
using namespace std;

class auto_timer
{
private:
    struct timespec start,end;
public:
    auto_timer()
    {
        start.tv_sec=0,start.tv_nsec=0;
        end.tv_sec=0,end.tv_nsec=0;
        clock_gettime(CLOCK_REALTIME,&start);
        cout << "start elapse time!" << endl;
    }
    ~auto_timer()
    {
        clock_gettime(CLOCK_REALTIME,&end);
        if(end.tv_nsec<start.tv_nsec){
            end.tv_sec--;
            end.tv_nsec+=1000000000;
        }
        cout<<"Elapsed Time is "<< end.tv_sec-start.tv_sec << "seconds," \
            <<  setprecision(10) <<  (double)((end.tv_nsec-start.tv_nsec)/1000000.0) << "ms" << endl;
    }
};

using namespace std;
using namespace odb::core;

int
main (int argc, char* argv[])
{
  try
  {
    auto_ptr<database> db (new odb::mysql::database(argc, argv));
    unsigned long john_id, jane_id,joe_id;

    // Create a few persistent person objects.
    {
      auto_timer t1;
      person john ("John", "Doe", 33);
      person jane ("Jane", "Doe", 32);
      person joe ("Joe", "Dirt", 30);

      transaction t (db->begin ());

      // Make objects persistent and save their ids for later use.
      john_id = db->persist (john);
      jane_id = db->persist (jane);
      joe_id  = db->persist (joe);

      t.commit ();
    }

    {
      auto_timer t2;
      transaction t(db->begin());

      typedef odb::result<person> result;
      typedef odb::query<person>  query;
        
      result r(db->query<person>(query::age>30));

      result::iterator i;
      for(i=r.begin();i!=r.end();++i)
      {
        cout << "Hello, " << i->first() << "(" << i->id() << ")!" << endl;
      }
      t.commit();
    }

    {
        auto_timer t3;
        transaction t(db->begin());
        auto_ptr<person> joe (db->load<person>(joe_id));
        joe->age(joe->age()+30);
        db->update(*joe);

        t.commit();
    }
  }
  catch (const odb::exception& e)
  {
    cerr << e.what () << endl;
    return 1;
  }
}
