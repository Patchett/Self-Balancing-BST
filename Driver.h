/****************************************************************************

                                                        Ryan Bridges
                                                        February 29, 2014

File Name:      Driver.h
Description:    Defines the UCSDStudent class
 ****************************************************************************/
#ifndef DRIVER_H
#define DRIVER_H

#include <string.h>
#include <iostream>
using namespace std;

class UCSDStudent
{
    friend ostream &operator <<(ostream &, const UCSDStudent &);
    char name[20];
    long studentnum;
public:

    /**
     * UCSDStudent constructor initializes variables for a new UCSDStudent
     */
    UCSDStudent (char *nm, long val = 0) : studentnum (val)
    {
        strcpy (name, nm);
    }

    /**
     * operator const char * overloads the const char * operator by returning
     * the name field
     */
    operator const char *(void) const
    {
        return name;
    }

    /**
     * operator == overloads the == operator so it works on UCSDStudents
     * @param vvv a reference to the student to compare to
     */
    long operator == (const UCSDStudent &vvv) const
    {
        return ! strcmp (name, vvv.name);
    }

    /**
     * operator > overloads the > operator to work on UCSDStudents
     * vvv the student to compare to
     */
    long operator > (const UCSDStudent &vvv) const
    {
        return (strcmp (name, vvv.name) > 0) ? 1 : 0;
    }
};

#endif
