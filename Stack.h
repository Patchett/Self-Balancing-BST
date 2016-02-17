#ifndef STACK_H
#define STACK_H

#include <iostream>
#include "List.h"
using namespace std;

template <class Arbitrary>
class Stack : private List<Arbitrary> {
public:
        List<Arbitrary> :: IsEmpty;
        List<Arbitrary> :: NumElements;
        List<Arbitrary> :: Write;

        Arbitrary * Pop (void);
        Arbitrary * Push (Arbitrary *);
        Arbitrary * Top (void) const;
};

#include "Stack.c"

#endif
