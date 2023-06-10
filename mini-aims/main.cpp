#include <iostream>
#include <fstream>
#include <cctype>
#include <algorithm>
#include "student.h"
#include "course.h"
#include "base.h"
#include "faculty.h"
using namespace std;

vector<Student *> studentList;
vector<Course *> courseList;
vector<Faculty *> facultyList;

template <typename T>
void printList(string message, vector<T> list)
{
    cout << message << endl;
    for (int i = 0; i < list.size(); i++)
    {
        list[i]->print();
    }
}

vector<vector<string>> read_file(string fileName)
{
    vector<vector<string>> readFile;
    string line;
    ifstream file(fileName);
    while (!file.eof())
    {
        vector<string> readLine;
        getline(file, line);
        int index = line.find(",");
        string s1 = line.substr(0, index);
        readLine.push_back(s1);
        string s2 = line.substr((index + 1), (line.length() - s1.length() - 1));
        readLine.push_back(s2);
        readFile.push_back(readLine);
    }
    file.close();
    return readFile;
}

// true if the argument is whitespace, false otherwise
bool space(char c)
{
    return isspace(c);
}

// false if the argument is whitespace, true otherwise
bool not_space(char c)
{
    return !isspace(c);
}

vector<string> split(const string &str)
{
    typedef string::const_iterator iter;
    vector<string> ret;
    iter i = str.begin();

    while (i != str.end())
    {
        // ignore leading blanks
        i = find_if(i, str.end(), not_space);
        // find end of next word
        iter j = find_if(i, str.end(), space);
        // copy the characters in [i, j)
        if (i != str.end())
            ret.push_back(string(i, j));
        i = j;
    }
    return ret;
}

void printMenu()
{
    cout << "List of commands (Menu):\n-> listAllStudents => (prints student list.... only admin can use this command)\n-> listCourses => (prints course List.......anyone can use this command)\nFor admin:\n-> addStudent <student id> <student name>\n-> addCourse <course id> <course name>\n-> addFaculty <faculty id> <faculty name>\n-> deleteStudent <student id>\n-> deleteCourse <course id>\n-> deleteFaculty <faculty id>\n-> registerStudent <student id> <course id>\n-> deregisterStudent <student id> <course id>\n2) For faculty:\n-> ListStudents <course id> =>(prints registeredStudentList for that course)\n-> grade <student id> <course id> <grade (A+, A, A-, B+, B, B-, C+, C, C-, F)\n3) For student:\n-> registerCourse <student id> <course id>\n-> deregisterCourse <student id> <course id>\n-> viewCourses <student id>\n-> Menu => (prints list of commands for mini-aims\n-> Exit => (exits mini-aims program\n";
}

Student *findStudent(string id)
{
    for (Student *s : studentList)
    {
        if (id == s->id)
            return s;
    }
    return NULL;
}

Course *findCourse(string id)
{
    for (Course *c : courseList)
    {
        if (id == c->id)
            return c;
    }
    return NULL;
}

Faculty *findFaculty(string id)
{
    for (Faculty *f : facultyList)
    {
        if (id == f->id)
            return f;
    }
    return NULL;
}

int main()
{
    vector<vector<string>> record;
    record = read_file("students.txt");
    for (int i = 0; i < record.size(); i++)
    {
        Student *s = new Student(record[i]);
        studentList.push_back(s);
    }

    record = read_file("courses.txt");
    for (int i = 0; i < record.size(); i++)
    {
        Course *c = new Course(record[i]);
        courseList.push_back(c);
    }

    record = read_file("faculty.txt");
    for (int i = 0; i < record.size(); i++)
    {
        Faculty *f = new Faculty(record[i]);
        facultyList.push_back(f);
    }

    record = read_file("student-course.txt");
    for (int i = 0; i < record.size(); i++)
    {
        Student *s = findStudent(record[i][0]);
        Course *c = findCourse(record[i][1]);
        s->registerCourse(c);
    }

    record = read_file("faculty-course.txt");
    for (int i = 0; i < record.size(); i++)
    {
        Faculty *f = findFaculty(record[i][0]);
        Course *c = findCourse(record[i][1]);
        f->courseRegister(c);
    }

    while (true)
    {
        cout << "Type \"Menu\" for list of commands.\n";
        string commands;
        getline(cin, commands);
        vector<string> command_set = split(commands);
        cout << "-> ";
        if (command_set[0] == "listAllStudents")
        {
            printList("Students List in IITH: ", studentList);
        }
        else if (command_set[0] == "listCourses")
        {
            printList("Courses offered at IITH: ", courseList);
        }
        else if (command_set[0] == "addStudent")
        {
            cout << "Are you the admin? (Y/N): ";
            char c;
            cin >> c;
            if (c == 'Y')
            {
                Student *s = new Student(command_set[1], command_set[2]);
                studentList.push_back(s);
            }
            else
            {
                cout << "Access denied" << endl;
                continue;
            }
        }
        else if (command_set[0] == "addCourse")
        {
            cout << "Are you the admin? (Y/N): ";
            char c;
            cin >> c;
            if (c == 'Y')
            {
                Course *c = new Course(command_set[1], command_set[2]);
                courseList.push_back(c);
                cout << "-> Enter new faculty details for the new added course (Format: <fauclty id> <faculty name>): ";
                getline(cin, commands);
                command_set = split(commands);
                Faculty *f = new Faculty(command_set[0], command_set[1]);
                facultyList.push_back(f);
                f->courseRegister(c);
            }
            else
            {
                cout << "Access denied" << endl;
                continue;
            }
        }
        else if (command_set[0] == "addFaculty")
        {
            cout << "Are you the admin? (Y/N): ";
            char c;
            cin >> c;
            if (c == 'Y')
            {
                Course *c = new Course(command_set[1], command_set[2]);
                courseList.push_back(c);
                cout << "-> Enter new faculty details for the new added course (Format: <fauclty id> <faculty name>): ";
                getline(cin, commands);
                command_set = split(commands);
                Faculty *f = new Faculty(command_set[0], command_set[1]);
                facultyList.push_back(f);
                f->courseRegister(c);
            }
            else
            {
                cout << "Access denied" << endl;
                continue;
            }
        }
        else if (command_set[0] == "registerStudent")
        {
            cout << "Are you the admin? (Y/N): ";
            char c;
            cin >> c;
            Student *s = findStudent(command_set[1]);
            Course *course = findCourse(command_set[2]);
            if (c == 'Y')
            {

                Course *course = findCourse(command_set[2]);
                s->registerCourse(course);
            }
            else
            {
                cout << "Are you a student? (Y/N): ";
                cin >> c;
                if (c == 'Y')
                {
                    cout << "Enter student id: ";
                    string i;
                    cin >> i;
                    if (i == command_set[1])
                    {
                        s->registerCourse(course);
                    }
                    else
                    {
                        cout << "Access denied" << endl;
                        continue;
                    }
                }
            }
        }
        else if (command_set[0] == "deleteStudent")
        {
            cout << "Are you the admin? (Y/N): ";
            char c;
            cin >> c;
            if (c == 'Y')
            {
                Student *s = findStudent(command_set[1]);
                s->deregisterAllCourses();

                int i;
                for (i = 0; i < studentList.size(); i++)
                {
                    if (studentList[i]->id == command_set[1])
                    {
                        break;
                    }
                }
                vector<Student *>::iterator it;
                it = studentList.begin() + i;
                studentList.erase(it);

                delete s;
            }
            else
            {
                cout << "Access denied" << endl;
                continue;
            }
        }
        else if (command_set[0] == "deleteCourse")
        {
            cout << "Are you the admin? (Y/N): ";
            char c;
            cin >> c;
            if (c == 'Y')
            {
                Course *c = findCourse(command_set[1]);
                c->deRegisterAllStudents();

                int i;
                for (i = 0; i < courseList.size(); i++)
                {
                    if (courseList[i]->id == command_set[1])
                    {
                        break;
                    }
                }
                vector<Course *>::iterator it;
                it = courseList.begin() + i;
                courseList.erase(it);

                Faculty *f = c->faculty;
                f->removeCourse();

                delete c;
            }
            else
            {
                cout << "Access denied" << endl;
                continue;
            }
        }
        else if (command_set[0] == "deleteFaculty")
        {
            cout << "Are you the admin? (Y/N): ";
            char c;
            cin >> c;
            if (c == 'Y')
            {
                Faculty *f = findFaculty(command_set[1]);
                Course *c = f->course;
                c->removeFaculty();

                int i;
                for (i = 0; i < facultyList.size(); i++)
                {
                    if (facultyList[i]->id == command_set[1])
                    {
                        break;
                    }
                }
                vector<Faculty *>::iterator it;
                it = facultyList.begin() + i;
                facultyList.erase(it);

                delete f;
            }
            else
            {
                cout << "Access denied" << endl;
                continue;
            }
        }
        else if (command_set[0] == "deregisterStudent")
        {
            cout << "Are you the admin? (Y/N): ";
            char c;
            cin >> c;
            Student *s = findStudent(command_set[1]);
            Course *course = findCourse(command_set[2]);
            if (c == 'Y')
            {

                Course *course = findCourse(command_set[2]);
                s->deregisterCourse(course);
            }
            else
            {
                cout << "Are you a student? (Y/N): ";
                cin >> c;
                if (c == 'Y')
                {
                    cout << "Enter student id: ";
                    string i;
                    cin >> i;
                    if (i == command_set[1])
                    {
                        s->deregisterCourse(course);
                    }
                    else
                    {
                        cout << "Access denied" << endl;
                        continue;
                    }
                }
            }
        }
        else if (command_set[0] == "ListStudents")
        {
            Course *course = findCourse(command_set[1]);
            printList("Registered students for this course: ", course->registeredStudentList);
        }
        else if (command_set[0] == "grade")
        {
            Student *s = findStudent(command_set[1]);
            Course *course = findCourse(command_set[2]);
            cout << "Are you the appropriate faculty? (Y/N): ";
            char c;
            cin >> c;
            if (c == 'Y')
            {
                string i;
                cout << "Enter faculty id: ";
                cin >> i;
                if (course->faculty == findFaculty(i))
                {
                    s->giveGrade(course, command_set[3]);
                }
                else
                {
                    cout << "Access denied" << endl;
                    continue;
                }
            }
            else
            {
                cout << "Access denied" << endl;
                continue;
            }
        }
        else if (command_set[0] == "registerCourse")
        {
            Student *s = findStudent(command_set[1]);
            Course *course = findCourse(command_set[2]);
            char c;
            cout << "Are you a student? (Y/N): ";
            cin >> c;
            if (c == 'Y')
            {
                cout << "Enter student id: ";
                string i;
                cin >> i;
                if (i == command_set[1])
                {
                    s->registerCourse(course);
                }
                else
                {
                    cout << "Access denied" << endl;
                    continue;
                }
            }
            else
            {
                cout << "Access denied\n";
                continue;
            }
        }
        else if (command_set[0] == "deregisterCourse")
        {
            Student *s = findStudent(command_set[1]);
            Course *course = findCourse(command_set[2]);
            char c;
            cout << "Are you a student? (Y/N): ";
            cin >> c;
            if (c == 'Y')
            {
                cout << "Enter student id: ";
                string i;
                cin >> i;
                if (i == command_set[1])
                {
                    s->deregisterCourse(course);
                }
                else
                {
                    cout << "Access denied" << endl;
                    continue;
                }
            }
            else
            {
                cout << "Access denied\n";
                continue;
            }
        }
        else if (command_set[0] == "viewCourses")
        {
            cout << "Enter student id: ";
            string i;
            cin >> i;
            Student *s = findStudent(command_set[1]);
            if (i == command_set[1])
            {
                printList("Student courses with grades: ", s->myCoursesWithGrades);
            }
            else
            {
                cout << "Access denied\n";
                continue;
            }
        }
        else if (command_set[0] == "Menu")
        {
            printMenu();
        }
        else if (command_set[0] == "Exit")
        {
            break;
        }
        else
        {
            cout << "Command not recognised\n";
        }

        continue;
    }
}

/*
List of commands:
     listAllStudents => (prints student list.... only admin can use this command)
     listCourses => (prints course List.......anyone can use this command)
1) For admin:
     addStudent <student id> <student name>
     addCourse <course id> <course name>
     addFaculty <faculty id> <faculty name>
     deleteStudent <student id>
     deleteCourse <course id>
     deleteFaculty <faculty id>
     registerStudent <student id> <course id>
     deregisterStudent <student id> <course id>

2) For faculty:
     ListStudents <course id> =>(prints registeredStudentList for that course)
     grade <student id> <course id> <grade (A+, A, A-, B+, B, B-, C+, C, C-, F)
3) For student:
     registerCourse <student id> <course id>
     deregisterCourse <student id> <course id>
     viewCourses <student id>

*/