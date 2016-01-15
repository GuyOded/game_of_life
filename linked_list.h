/*
 * linked_list.h
 *
 *  Created on: Dec 23, 2015
 *      Author: guy
 */

#ifndef LINKED_LIST_H_
#define LINKED_LIST_H_

#define DEBUG

typedef struct point{
	int x;
	int y;
} Point;

typedef struct list_node {
	struct list_node *next;
	struct list_node *prev;
	Point p;
} Node;

typedef struct list{
	Node *first;
	Node *last;
	int length;
}List;

List *init_list();

void destroy(List *l);

Node *delete(List *l, Node *pos);

Node *add_node(List *l, Point value);

void set_val(Point value, Node *n, List *l);

Point get_val(Node *n);

void print_list(List *l);

Node *get_prev(Node *n);

Node *get_next(Node *n);

Node *get_first(List *l);

Node *get_last(List *l);

int get_length(List *l);

int exist(List *l, Point val);

void copy_lists(List *source, List *dest);

#endif /* LINKED_LIST_H_ */
