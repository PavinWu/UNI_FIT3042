#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "bst.h"

BST *bst_insert(BST *bst, char *key, void *value){
	if (bst == NULL) {		// if no head node
		KVP *kvpin = (KVP *)malloc(sizeof(KVP));
		kvpin -> key = *key;
		kvpin -> value = *value;
		
		bst = (BST *) malloc(sizeof(BST));
		bst -> kvp = kvpin;
		bst -> left = NULL;
		bst -> right = NULL;
		
		//free(kvpin);		///????
		return bst;
	}	
	else if (*key < (bst -> kvp -> key)) {	
		return bst_insert(bst -> left, key, value);
	}
	else {
		return bst_insert(bst -> right, key, value);
	}
		
		//////////////////Don't have to do this! Inserting step has left/right as top!//////////////////////////////////////
		/*
		if (key < ((bst -> kvp) -> key)){	//*key?
			BST *subbst = (BST *) malloc(sizeof(bst -> left));	//sizeof(BST)?
			*subbst = bst -> left;		// otherway around?
		}
		else {
			BST *subbst = (BST *) malloc(sizeof(bst -> left));
			*subbst = bst -> right;
		}
		
		if (*subbst != NULL){
			subbst = BST *bst_insert(BST *subbst, char *key, void *value);		// this should be at top?
		}
		else {		// should do this before assigning to bst -> left ?
			subbst = (BST *) malloc(sizeof(BST));
			subbst -> kvp -> key = *key;
			subbst -> kvp -> value = *value;
			subbst -> left = NULL;
			subbst -> right = NULL;	
		}
		return subbst;
		*/
	}
	
	// likely to have memory leak
	// haven't even tested if these codes work
	// nope
}

KVP *bst_lookup(BST *bst, char *key){
	if (bst == NULL) {
		return NULL;	// NULL tree, definitely won't find key
	}
	else if (*key < bst -> kvp -> key) {
		return bst_lookup(bst -> left, *key);
	}
	else if (*key > bst -> kvp -> key) {
		return bst_lookup(bst -> right, *key);
	}
	else {
		return bst -> kvp -> key;
	}
	
	/*
	else{
		if (key < (bst -> key)){
			BST *subbst = (BST *) malloc(sizeof(bst -> left));
			*subbst = bst -> left;
		}
		else if (key > (bst -> key)){
			BST *subbst = (BST *) malloc(sizeof(bst -> left));
			*subbst = bst -> right;
		}
		else {
			return subdst -> kvp;
		}
		
		if (*subbst != NULL){
			subbst = BST *bst_insert(BST *subbst, char *key, void *value);
		}
		else {
			return NULL;	// not find key
		}
		return subbst;
	}
	*/
}
