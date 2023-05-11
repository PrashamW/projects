#ifndef __COURSEGRADE__

#define __COURSEGRADE__

#include <iostream>
#include "course.h"
using namespace std;

class CourseGrade : public Base
{
public:
    Course *course;
    string grade;
    CourseGrade(Course *c);
    void giveGrade(string g);
    void print();
};
#endif