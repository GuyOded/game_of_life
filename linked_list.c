#include "linked_list.h"
#include <stdlib.h>
#include <stdio.h>
#include <assert.h>

/*
 * List *init_list()
 *
 * initialize a new list with zero elements and returns
 * pointer to it.
 *
 */
List *init_list()
{
	/* allocate block for list */
	List *l = (List *)malloc(sizeof(List));
	if(l == NULL)
	{
		return NULL;
	}
	/* initializing list */
	l->first = NULL;
	l->last = NULL;
	l->length = 0;
	return l;
}

/*
 * void destroy(List *l)
 *
 * removes all elements from the given list l
 * and frees the list
 *
 */
void destroy(List *l)
{
	/* check if list is already empty */
	if(l->length == 0)
	{
		return;
	}
	Node *np = l->first;
	while(np != NULL)
	{
		np = delete(l, np);
	}
	free(l);
}

/*
 * Node *delete(List *l, Node *pos)
 *
 * deletes the node pos from the given list l
 * returns the next node of the deleted one, unless
 * it was the last node in which case the function returns
 * its previous node.
 *
 */
Node *delete(List *l, Node *pos)
{
	Node *next_pos;
	if(pos == NULL)
	{
		return NULL;
	}

	/* check if pos is first and chain it correctly */
	if(l->first == pos)
	{
		l->first = pos->next;
		next_pos = pos->next;
		if(next_pos != NULL)
			next_pos->prev = NULL;
	}
	else if(l->last == pos)
	{
		l->last = pos->prev;
		next_pos = pos->prev;
		next_pos->next = NULL;
	}
	else
	{
		pos->prev->next = pos->next;
		pos->next->prev = pos->prev;
		next_pos = pos->next;
	}

	/* updating all the list properties */
	l->length--;

	/* releasing memory */
	free(pos);
	return next_pos;
}

/*
 * Node *add_node(List *l, int val)
 *
 * Given an integer value, a node is added to the end of
 * the list. Its pointer is returned.
 *
 */
Node *add_node(List *l, Point value)
{
	/* allocate block for the node and assign data */
	Node *np = (Node *)malloc(sizeof(Node));
	np->next = NULL;
	np->prev = NULL;
	if(np == NULL)
	{
		return NULL;
	}
	np->p = value;
	if(l->length == 0)
	{
		l->first = np;
		l->last = np;
	}
	else
	{
		np->prev = l->last;
		l->last->next = np;
		np->next = NULL;
		l->last = np;
	}

	l->length++;
	return np;
}

/*
 * void set_val(int value, Node *n, List *l)
 *
 * given an integer and a node sets the node current val
 * member to the given value.
 * The list pointer is given as a parameter so the members
 * sum and length will be correctly updated.
 *
 */
void set_val(Point value, Node *n, List *l)
{
	if(n == NULL)
	{
		return;
	}
	n->p = value;
}

/*
 *  void print_list(List *l)
 *
 * This function is for debugging purposes only !
 * Given a list pointer, all nodes values will be printed
 *
 *
 */
void print_list(List *l)
{
	Node *np = l->first;
	while(np != NULL)
	{
		printf("(%d, %d) ", np->p.x, np->p.y);
		np = np->next;
	}
	printf("\n");
}

/*
 * Node *get_prev(Node *n)
 *
 * given a node n returns the pointer to
 * the previous node in the list.
 * NULL(0) is returned if either n or the previous
 * Node points to NULL
 *
 */
Node *get_prev(Node *n)
{
	if(n == NULL || n->prev == NULL)
	{
		return NULL;
	}
	return n->prev;

}

/*
 * Node *get_prev(Node *n)
 *
 * given a node n returns the pointer to
 * the following node in the list.
 * NULL(0) is returned if either n or the following
 * Node points to NULL
 *
 */
Node *get_next(Node *n)
{
	if(n == NULL || n->next == NULL)
	{
		return NULL;
	}
	return n->next;
}

/*
 * Node *get_first(List *l)
 *
 * given a list pointer l, returns the first node
 * of the list. NULL is returned if the first node
 * or l is NULL
 *
 */
Node *get_first(List *l)
{
	if(l == NULL || l->first == NULL)
	{
		return NULL;
	}
	return l->first;
}

/*
 * Node *get_first(List *l)
 *
 * given a list pointer l, returns the last node
 * of the list. NULL is returned if the last node
 * or l equals to NULL
 *
 */
Node *get_last(List *l)
{
	if(l == NULL || l->last == NULL)
	{
		return NULL;
	}
	return l->last;
}


/*
 * int get_val(Node *n)
 *
 * given a node n, returns its val member
 *
 */
Point get_val(Node *n)
{
	assert(n == NULL);
	return n->p;
}


/*
 * int get_length(List *l)
 *
 * returns the given list's length
 *
 */
int get_length(List *l)
{
	if(l == NULL)
	{
		return 0;
	}
	return l->length;
}


/**
 *
 * @param l
 * @param val
 * @return 1 if val exists in l false otherwise
 */
int exist(List *l, Point val)
{
	Node *pos = l->first;
	while(pos != NULL)
	{
		if((pos->p.x == val.x) && (pos->p.y == val.y))
		{
			return 1;
		}
		pos = get_next(pos);
	}

	return 0;
}

void copy_lists(List *source, List *dest)
{
	Node *pos = get_first(source);
	while(pos != NULL)
	{
		add_node(dest, pos->p);
		pos = get_next(pos);
	}
}
