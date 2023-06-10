#ifndef __COURSE__

#define __COURSE__

#include <iostream>
#include <vector>
#include "base.h"
#include "faculty.h"

using namespace std;

class Student;

class Course : public Base
{
    friend Student;

public:
    vector<Student *> registeredStudentList;
    Faculty *faculty;
    Course(string name, string id);
    Course(vector<string> record);
    void print();
    void deRegisterAllStudents();
    void removeFaculty();
    int search(string id);

private:
    void registerStudent(Student *s);
    void deRegisterStudent(Student *s);
};
#endif