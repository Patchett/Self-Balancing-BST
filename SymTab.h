#ifndef SYMTAB_H
#define SYMTAB_H

#include <iostream>
#include "Tree.h"
using namespace std;

template <class Whatever>
class SymTab : private Tree<Whatever> {
public:
        Tree<Whatever> :: Insert;
        Tree<Whatever> :: Lookup;
        Tree<Whatever> :: Remove;
        Tree<Whatever> :: Write;
        Tree<Whatever> :: Set_Debug_On;
        Tree<Whatever> :: Set_Debug_Off;
};

#endif
