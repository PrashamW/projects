#ifndef __BASE__

#define __BASE__
#include <iostream>
using namespace std;
class Base
{
public:
    string name;
    string id;
    Base(string i, string n);
    virtual void print();
};

#endif