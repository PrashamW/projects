#ifndef __FACULTY__

#define __FACULTY__

#include <iostream>
#include <vector>
#include "base.h"
using namespace std;

class Course;

class Faculty : public Base
{
public:
    Course *course;
    Faculty(string id, string name);
    Faculty(vector<string> record);
    void print();
    void courseRegister(Course *c);
    void removeCourse();
};
#endif
