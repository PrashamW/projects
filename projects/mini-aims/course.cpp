#include "course.h"
#include "student.h"
using namespace std;

Course::Course(string id, string name) : Base(id, name)
{
    // registeredStudentList = new vector<Student *>();
}
Course::Course(vector<string> record) : Base(record[0], record[1])
{
}

void Course::print()
{
    cout << "Course: " << id << " " << name << "\n";
    // cout << "Registered Student List:\n";
    // for (Student *s : registeredStudentList)
    // {
    //     cout << s->id << " " << s->name << " " << endl;
    // }
}
void Course::registerStudent(Student *s)
{
    registeredStudentList.push_back(s);
}
void Course::deRegisterStudent(Student *s)
{
    int index = search(s->id);
    registeredStudentList.erase(registeredStudentList.begin() + index);
}

int Course::search(string id)
{
    for (int i = 0; i < registeredStudentList.size(); i++)
    {
        if (registeredStudentList[i]->id == id)
        {
            return i;
        }
    }
    return -1;
}

void Course::deRegisterAllStudents()
{
    for (Student *s : registeredStudentList)
    {
        s->deregisterCourse(this);
    }
}

void Course::removeFaculty()
{
    faculty = NULL;
}
