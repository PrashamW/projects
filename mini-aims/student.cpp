#include <iostream>
#include <vector>
#include "student.h"
#include "course.h"

using namespace std;
Student::Student(string id, string name) : Base(id, name)
{
}
Student::Student(vector<string> record) : Base(record[0], record[1])
{
}
void Student::registerCourse(Course *c)
{

    CourseGrade *cg = new CourseGrade(c);
    myCoursesWithGrades.push_back(cg);
    cg->course->registerStudent(this);
}
void Student::deregisterCourse(Course *c)
{
    c->deRegisterStudent(this);
    int index = search(c->id);
    if (index != -1)
    {
        CourseGrade *temp = myCoursesWithGrades[index];
        myCoursesWithGrades.erase(myCoursesWithGrades.begin() + index);
        delete temp;
    }
}

int Student::search(string id)
{
    for (int i = 0; i < myCoursesWithGrades.size(); i++)
    {
        if (myCoursesWithGrades[i]->course->id == id)
        {
            return i;
        }
    }
    return -1;
}

void Student::deregisterAllCourses()
{
    for (CourseGrade *cg : myCoursesWithGrades)
    {
        cg->course->deRegisterStudent(this);
        delete cg;
    }
    myCoursesWithGrades.clear();
}

void Student::giveGrade(Course *c, string grade)
{
    int index = search(c->id);
    myCoursesWithGrades[index]->giveGrade(grade);
}

void Student::print()
{
    cout << "Student: " << id << " " << name << " " << endl;
    // cout << "Student courses: " << endl;
    // for (CourseGrade *cg : myCoursesWithGrades)
    // {
    //     cg->print();
    // }
    // cout << endl
    //      << endl;
}