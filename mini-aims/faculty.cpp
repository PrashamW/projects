#include <iostream>
#include "base.h"
#include "faculty.h"
#include "course.h"
using namespace std;
Faculty::Faculty(string id, string name) : Base(id, name)
{
}
Faculty::Faculty(vector<string> record) : Base(record[0], record[1])
{
}
void Faculty::print()
{
    cout << "Faculty: " << id << " " << name << " ............";
    course->print();
}
void Faculty::removeCourse()
{
    course = NULL;
}
void Faculty::courseRegister(Course *c)
{
    course = c;
    course->faculty = this;
}
