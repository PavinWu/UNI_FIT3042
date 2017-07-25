#ifndef FIT_BST_H
#define FIT_BST_H

/* 
 * structure and functions to represent a binary search tree
 * by convention, an empty tree is represented by a NULL pointer
 * at all times, we maintain the invariant that all keys
 * in the left node  are "smaller" than the 
 * current key, and all keys in the right node are larger.  
 * There should be no duplicate keys. 

 */
 
/* KVP represents a key-value pair in a binary search tree*/ 
typedef struct {
	char *key;
	void *value;
} KVP;

/* BST represents a node of the binary search tree */
typedef struct bst {
	struct bst *left;
	struct bst *right;
	KVP kvp;	///*???*/// WHY NO *???
} BST;


/*
 * insert a key/value pair into the BST
 * only copies the pointers so you should make a copy before
 * insertion if they are subject to change
 * returns: the updated BST.
 * //make a copy of ...? key/value pair
 * //shouldn't be able to access new value until function finished???
 * //=> no change of value from user is likely during function run
 * //=> as a function, no need to 'copy' value unless it's part of algorithm
 * //=> this 'copying' refers to before the function call
 */
BST *bst_insert(BST *bst, char *key, void *value);

/*
 * returns a key/value pair from the BST
 * returns null if the key is not present
 */
KVP *bst_lookup(BST *bst, char *key);


#endif
