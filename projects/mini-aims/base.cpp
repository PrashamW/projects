#include <iostream>
#include "base.h"

using namespace std;

Base::Base(string i, string n)
{
    name = n;
    id = i;
}

void Base::print()
{
    cout << name << " " << id << endl;
}