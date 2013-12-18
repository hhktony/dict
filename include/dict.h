/**
 *  Filename: dict.h
 *   Created: 2013-12-18 02:46:28
 *      Desc: TODO (some description)
 *    Author: xutao(butbueatiful), butbueatiful@gmail.com
 *   Company: myself
 */

#ifndef DICT_H_RRPXZCTU
#define DICT_H_RRPXZCTU

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#include "debug.h"

#define DICT_FILENAME "../test/dict.txt"
#define WORD_LEN 128
#define EXPLAIN_LEN 1024

typedef struct node {
	char word[64];
	long pos;
	struct node *pnext;
} node_t, *pnode_t;

#endif /* end of include guard: DICT_H_RRPXZCTU */

