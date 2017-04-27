// file      : hello/driver.cxx
// copyright : not copyrighted - public domain

#ifndef AUTO_TIMER_H
#define AUTO_TIMER_H

#include <iostream>
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
#endif
