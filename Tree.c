/****************************************************************************

                                                        Ryan Bridges
                                                        February 29, 2014

File Name:      Tree.c
Description:    Implements a binary tree with recursion
 ****************************************************************************/
#include <cstdlib>
#include <string>
#include "Tree.h"
using namespace std;

// messages
static const char AND[]        = " and ";
static const char COMPARE[]    = " - Comparing ";
static const char DEALLOCATE[] = " - Deallocating]\n";
static const char INSERT[]     = " - Inserting ";
static const char REPLACE[]    = " - Replacing ";
static const char UPDATE[]     = " - Updating ";

template <class Whatever>
int Tree<Whatever>::debug = 0;

#ifndef TRUE
#define TRUE 1
#endif

#ifndef FALSE
#define FALSE 0
#endif

#define THRESHOLD 2

template <class Whatever>
ostream &operator << (ostream &, const TNode<Whatever> &);

template <class Whatever>
struct TNode
{
    long balance;
    Whatever data;
    long height;
    TNode<Whatever> *left;
    long &occupancy;
    TNode<Whatever> *right;
    unsigned long &tree_count;

    /**
     * Tnode Constructor - Initializes necessary variables for a TNode
     * and increments occupancy
     */
    TNode(const Whatever &element, Tree<Whatever> &theTree)
        : balance(0), data(element), height(0), left(0),
          occupancy(theTree.occupancy), right(0),
          tree_count(theTree.tree_count)
    {
        occupancy++;
    }

    /**
     * Tnode Copy Constructor - Initializes necessary variables for a TNode
     * and increments occupancy
     */
    TNode(const Whatever &element, TNode<Whatever> &parentTNode)
        : balance(0), data(element), height(0), left(0),
          occupancy(parentTNode.occupancy), right(0),
          tree_count(parentTNode.tree_count)
    {
        occupancy++;
    }

    /**
     * TNode destructor decreases the occupancy
     */
    ~TNode(void)
    {
        occupancy--;
    }

    /**
     * delete_AllTNodes recursively deletes all TNodes in the tree by
     * recursively calling the constructor
     */
    void delete_AllTNodes(void)
    {
        if (left) // Go left
        {
            left->delete_AllTNodes();
        }
        if (right) // Go right
        {
            right->delete_AllTNodes();
        }
        delete this; // Delete the TNode
        return;
    }

    unsigned long Insert(const Whatever &element,
                         TNode<Whatever>  *&PointerInParent);

    /**
     * ReplaceAndRemoveMax - Finds the maximum element in the sub tree
     * @param targetTNode     The TNode that needs to be replaced
     * @param PointerInParent A pointer from the parent to this node
     */
    void ReplaceAndRemoveMax(TNode<Whatever> &targetTNode,
                             TNode<Whatever>  *&PointerInParent)
    {
        if (Tree<Whatever>::debug)
        {
            cerr << TREE << tree_count << COMPARE <<
                 (const char *) (data) << "]\n";

        }

        if (right) // Traverse to the right
        {
            right->ReplaceAndRemoveMax(targetTNode, right);
            SetHeightAndBalance(PointerInParent); // Rebalance the tree
        }

        else
        {
            if (Tree<Whatever>::debug)
            {
                cerr << TREE << tree_count << REPLACE <<
                     (const char *) (data) << "]\n";
            }
            PointerInParent = left; // Set PIP
            // Overwrite the data in the target node
            // Overwrite the data in the node that needs to be replaced
            targetTNode.data = this->data;
            delete this;
        }
    }

    /**
     * Removes a TNode from the tree
     * @param elementTNode - the current TNode containing the data
     *                       to be deleted
     * @param PointerInParent - Pointer from the parent to the current node
     * @param fromSHB - Tells us where Remove was called from
     * @return - true/false based on success of the remove
     */
    unsigned long Remove(TNode<Whatever> &elementTNode,
                         TNode<Whatever>  *&PointerInParent,
                         long fromSHB = FALSE)
    {
        // Tells us whether or not the remove was successful
        bool status = FALSE;
        if (Tree<Whatever>::debug)
        {
            cerr << TREE << tree_count << COMPARE <<
                 (const char *) (elementTNode.data) << AND <<
                 (const char *) (data) << "]\n";

        }

        // We found the node we want to delete
        if (elementTNode.data == data)
        {
            status = TRUE;
            // Store that data from the deleted node
            elementTNode.data = data;
            if (right && !left) // Node has only a right child
            {
                PointerInParent = right;
                delete this;
            }

            else if (!right && left) // Node has only a left child
            {
                PointerInParent = left;
                delete this;
            }

            else if (!right && !left) // Node has no children
            {
                PointerInParent = NULL;
                delete this;
            }

            else if (right && left) // Node has 2 children
            {
                left->ReplaceAndRemoveMax(*this, left);
                if (status && !fromSHB)
                    SetHeightAndBalance(PointerInParent);
            }
        }

        else if (elementTNode.data > data) // go right
        {
            if (right)
            {
                status = right->Remove(elementTNode, right);
                if (status && !fromSHB) // Rebalance the tree
                    SetHeightAndBalance(PointerInParent);
            }
        }
        else if (!(elementTNode.data > data)) // go right
        {
            if (left)
            {
                status = left->Remove(elementTNode, left);
                if (status && !fromSHB) // Rebalance the tree
                    SetHeightAndBalance(PointerInParent);

            }
        }
        return status;
    }

    /**
     * SetHeightAndBalance Sets the height and balance of the node
     *                     pointed to by PIP
     * @param PointerInParent Pointer to the node we want to set the height
     *                        and balance of from the parent
     */
    void SetHeightAndBalance(TNode<Whatever>  *&PointerInParent)
    {
        long heightLeft = -1;
        long heightRight = -1;

        if (Tree<Whatever>::debug)
        {
            cerr << TREE << tree_count << UPDATE <<
                 (const char *) (PointerInParent->data) << "]\n";
        }
        if (left) // Stores the height of the left child
            heightLeft = left->height;
        if (right) // Stores the height of the right child
            heightRight = right->height;
        balance = heightLeft - heightRight; // Sets the balance

        // Sets the height
        if (heightLeft > heightRight)
            height = heightLeft + 1;
        else if (heightRight > heightLeft)
            height = heightRight + 1;

        // If the node is a leaf
        else if (heightRight == -1 && heightLeft == -1)
            height = 0;

        // If both children have equal height
        else if (heightRight == heightLeft)
            height = heightRight + 1;

        // Checks if the tree needs to be rebalanced
        if (abs(balance) > THRESHOLD)
        {
            TNode<Whatever> current(data, *this); // Creates a temp node
            // Removes the node to be rebalanced
            Remove(*this, PointerInParent, TRUE);
            // Reinserts the node
            PointerInParent->Insert(current.data, PointerInParent);
        }
    }

    /**
    * Write_AllTNodes Writes out all Nodes in the tree
    * @param  stream Stream to write to
    * @return        Returns the stream
    */
    ostream &Write_AllTNodes(ostream &stream) const
    {
        if (left)
            left->Write_AllTNodes(stream);
        stream << *this;
        if (right)
            right->Write_AllTNodes(stream);

        return stream;
    }
};

/**
 * Sets the debug option on
 */
template <class Whatever>
void Tree<Whatever> ::Set_Debug_On(void)
{
    Tree<Whatever>::debug = 1;
}

/**
 * Sets the debug option off
 */
template <class Whatever>
void Tree<Whatever> ::Set_Debug_Off(void)
{
    Tree<Whatever>::debug = 0;
}

/**
 * Insert - Inserts a node into the tree
 * @param element the data to insert
 * @param PointerInParent Pointer from the parent to the current node
 */
template <class Whatever>
unsigned long TNode<Whatever> ::Insert(const Whatever &element,
                                       TNode<Whatever>  *&PointerInParent)
{
    if (Tree<Whatever>::debug)
    {
        cerr << TREE << tree_count << COMPARE << (const char *) (element)
             << AND << (const char *) (data) << "]\n";
    }

    if (element == data) // Duplicate insert
    {
        return true;
    }

    else if (element > data) // Go right
    {
        if (right)
        {
            right->Insert(element, right);
        }

        else // Insert at the right
        {
            if (Tree<Whatever>::debug)
            {
                cerr << TREE << tree_count << INSERT <<
                     (const char *) (element) << "]\n";
            }
            right = new TNode<Whatever> (element, *this);
        }
    }

    else // Go left
    {
        if (left)
        {
            left->Insert(element, left);
        }

        else // Insert at the left
        {
            if (Tree<Whatever>::debug)
            {
                cerr << TREE << tree_count << INSERT <<
                     (const char *) (element) << "]\n";
            }
            left = new TNode<Whatever> (element, *this);
        }
    }
    SetHeightAndBalance(PointerInParent); // Update the height and balance
    return FALSE;

}

template <class Whatever>
ostream &operator << (ostream &stream, const TNode<Whatever> &nnn)
{
    stream << "at height:  :" << nnn.height << " with balance:  "
           << nnn.balance << "  ";
    return stream << nnn.data << "\n";
}

template <class Whatever>
/**
 * Insert Tree's insert delegates to TNode's insert if the tree is not empty
 * @param element data to insert into the tree
 */
unsigned long Tree<Whatever> ::Insert(const Whatever &element)
{
    if (occupancy == 0) // Creates a root if the tree is empty
    {
        if (debug)
        {
            cerr << TREE << tree_count << INSERT <<
                 (const char *) (element) << "]\n";
        }
        root = new TNode<Whatever> (element, *this);
        return true;
    }
    else // Delegates to TNode's insert if the tree is not empty
        return root->Insert(element, root);

}

template <class Whatever>
/**
 * Lookup Searches for a node in the tree and returns true/false based on
 * whether or not the node was found
 * @param element to lookup
 */
unsigned long Tree<Whatever> ::Lookup(Whatever &element) const
{

    TNode<Whatever> *current = root; // Starts at the root
    // If we reach a null node then the element was not found
    while (current)
    {
        if (Tree<Whatever>::debug)
        {
            cerr << TREE << tree_count << COMPARE << (const char *) (element)
                 << AND << (const char *) (current->data) << "]\n";
        }

        if ((current->data) == (element))
        {
            // The element was found
            {
                element = current->data;
                return TRUE; // Return the data
            }
        }
        // Go to the left
        else if ((current->data) > (element))
        {
            current = current->left;
        }
        // Go to the right
        else if (!((current->data) > (element)))
        {
            current = current->right;
        }
    }
    return FALSE; // not there
}

template <class Whatever>
/**
 * Remove Tree's remove delegates to TNode's remove if the tree is not empty
 * @param element to remove
 */
unsigned long Tree<Whatever> ::Remove(Whatever &element)
{
    bool status; // Tells us whether the remove was successful
    TNode<Whatever> current(element, *this); // Temp node

    if (occupancy == 0) // The tree is emtpy
        return FALSE;

    else // Delegate to TNode's remove
    {
        status = root->Remove(current, root);
        element = current.data; // Store that data that was removed
        return status;
    }

}

template <class Whatever>
/**
 * Tree destructor Delegates to delete_AllTNodes to delete the tree
 */
Tree<Whatever> ::~Tree(void)
/***************************************************************************
% Routine Name : Tree<Whatever> :: ~Tree  (public)
% File :         Tree.c
%
% Description :  deallocates memory associated with the Tree.  It
%                will also delete all the memory of the elements within
%                the table.
 ***************************************************************************/

{
    if (Tree<Whatever>::debug)
    {
        cerr << TREE << tree_count << DEALLOCATE;
    }

    if (root) // Deletes all nodes in the tree
        root->delete_AllTNodes();
}

template <class Whatever>
ostream &Tree<Whatever> ::Write(ostream &stream) const
/***************************************************************************
% Routine Name : Tree<Whatever> :: Write (public)
% File :         Tree.c
%
% Description : This function will output the contents of the Tree table
%               to the stream specificed by the caller.  The stream could be
%               cerr, cout, or any other valid stream.
%
% Parameters descriptions :
%
% name               description
% ------------------ ------------------------------------------------------
% stream             A reference to the output stream.
% <return>           A reference to the output stream.
 ***************************************************************************/

{
    stream << "Tree " << tree_count << ":\n"
           << "occupancy is " << occupancy << " elements.\n";

    return (root) ? root->Write_AllTNodes(stream) : stream;
}
