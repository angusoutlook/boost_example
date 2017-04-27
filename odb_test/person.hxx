#ifndef __PERSON_HXX
#define __PERSON_HXX



#include <string>

#include <odb/core.hxx>

#pragma db object

class person
{
public:
    person(){}
    person(const std::string& first,
        const std::string& last,
        unsigned short age):first_(first),last_(last),age_(age)
    {
    }

    const std::string&
    first() const
    {
        return first_;
    }

    unsigned long 
    id()
    {
        return id_;
    }

    const std::string& last() const;
    
    unsigned short 
    age() const
    {
        return age_;
    }


    void age(unsigned short age)
    {
        age_=age;
    }

private:
    friend class odb::access;

#pragma db id auto
    unsigned long id_;

    std::string first_;
    std::string last_;
    unsigned short age_;
};

#endif
