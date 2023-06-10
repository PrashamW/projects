#include <iostream>
#include "course.h"
#include "courseGrade.h"
#include "base.h"
using namespace std;

CourseGrade::CourseGrade(Course *c) : Base(c->id, c->name)
{
    course = c;
    grade = "Not Graded.";
}

void CourseGrade::print()
{
    course->print();
    cout << "Grade: " << grade << endl;
}
void CourseGrade::giveGrade(string g)
{
    grade = g;
}