#ifndef __STUDENT__

#define __STUDENT__

#include <iostream>
#include "base.h"
#include "courseGrade.h"

using namespace std;

class Course;

class Student : public Base
{
public:
    vector<CourseGrade *> myCoursesWithGrades;
    Student(string id, string name);
    Student(vector<string> record);
    void registerCourse(Course *c);
    void deregisterCourse(Course *c);
    void deregisterAllCourses();
    void giveGrade(Course *c, string grade);
    void print();
    int search(string id);
};

#endif