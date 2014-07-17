/* $Id: tess_hash.c,v 1.7 1999/11/11 03:21:43 kendallb Exp $ */

/*
 * Mesa 3-D graphics library
 * Version:  3.1
 *
 * Copyright (C) 1999  Brian Paul   All Rights Reserved.
 *
 * Permission is hereby granted, free of charge, to any person obtaining a
 * copy of this software and associated documentation files (the "Software"),
 * to deal in the Software without restriction, including without limitation
 * the rights to use, copy, modify, merge, publish, distribute, sublicense,
 * and/or sell copies of the Software, and to permit persons to whom the
 * Software is furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included
 * in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS
 * OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.  IN NO EVENT SHALL
 * BRIAN PAUL BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN
 * AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
 * CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 */

/*****************************************************************************
 *
 * GLU 1.3 Polygon Tessellation - Implementation of hash table
 *
 * Gareth Hughes <garethh@bell-labs.com>, August 1999
 *
 *****************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <GL/glu.h>

#include "gluP.h"

#include "tess.h"
#include "tess_hash.h"

/*****************************************************************************
 * Internal function prototypes:
 *****************************************************************************/



/*****************************************************************************
 *
 *				HASHTABLE FUNCTIONS
 *
 *****************************************************************************/


/*****************************************************************************
 * hashtable_init
 *
 * Allocate and initialize a new heap.
 *****************************************************************************/
hashtable_t *hashtable_init( int size )
{
    hashtable_t	*table;
    GLuint	i;

    table = ( hashtable_t * )
	malloc( size * sizeof(hashtable_t) );
    if ( table == NULL ) {
	return NULL;
    }

    table->elements = ( hashtable_elt_t ** )
	malloc( size * sizeof(hashtable_elt_t *) );
    if ( table->elements == NULL ) {
	free( table );
	return NULL;
    }

    for ( i = 0; i < size; i++ ) {
	table->elements[ i ] = NULL;
    }

    table->size = size;
    table->nelements = 0;

    return table;
}


/*****************************************************************************
 * hashtable_insert
 *****************************************************************************/
void hashtable_insert( hashtable_t *table, int key, void *ptr )
{
    hashtable_elt_t	*elt = (hashtable_elt_t *)
	malloc( sizeof(hashtable_elt_t) );
    GLuint		index = key % table->size;

    elt->key = key;
    elt->ptr = ptr;

    if ( ! table->elements[ index ] )
    {
	table->elements[ index ] = elt;

	elt->next = elt->previous = NULL;
    }
    else
    {
	elt->next = table->elements[ index ];
	elt->next->previous = elt;

	elt->previous = NULL;

	table->elements[ index ] = elt;
    }

    table->nelements++;
}


/*****************************************************************************
 * hashtable_search
 *****************************************************************************/
GLboolean hashtable_search( hashtable_t *table, int key )
{
    hashtable_elt_t	*elt;
    GLuint		index = key % table->size;

    if ( table->elements[ index ] )
    {
	elt = table->elements[ index ];

	while ( elt )
	{
	    if ( elt->key == key ) {
		return GL_TRUE;
	    }

	    elt = elt->next;
	}
    }

    return GL_FALSE;
}


/*****************************************************************************
 * hashtable_element
 *****************************************************************************/
void *hashtable_element( hashtable_t *table, int key )
{
    hashtable_elt_t	*elt;
    GLuint		index = key % table->size;

    if ( table->elements[ index ] )
    {
	elt = table->elements[ index ];

	while ( elt )
	{
	    if ( elt->key == key ) {
		return elt->ptr;
	    }

	    elt = elt->next;
	}
    }

    return NULL;
}


/*****************************************************************************
 * hashtable_delete
 *****************************************************************************/
void *hashtable_delete( hashtable_t *table, int key )
{
    hashtable_elt_t	*elt;
    void		*ret = NULL;
    GLuint		index = key % table->size;

    if ( table->elements[ index ] )
    {
	elt = table->elements[ index ];

	while ( elt )
	{
	    if ( elt->key == key )
	    {
		if ( elt == table->elements[ index ] )
		{
		    /* Remove the entry from the head of the chain. */
		    if ( elt->next )
		    {
			table->elements[ index ] = elt->next;
			elt->next->previous = NULL;
		    }
		    else
		    {
			table->elements[ index ] = NULL;
		    }
		}
		else
		{
		    if ( elt->next )
		    {
			/* Remove the entry from inside the chain. */
			elt->next->previous = elt->previous;
			elt->previous->next = elt->next;
		    }
		    else
		    {
			/* Remove the entry from the end of the chain. */
			elt->previous->next = NULL;
		    }
		}

		ret = elt->ptr;
		free( elt );

		break;
	    }

	    elt = elt->next;
	}
    }

    if ( ret ) {
	table->nelements--;
    }
    return ret;
}


/*****************************************************************************
 * hashtable_cleanup
 *****************************************************************************/
void hashtable_cleanup( hashtable_t **table )
{
	/* KB: Why does this just return?? */
    return;
#if 0
    if ( *table )
    {
	if ( (*table)->elements )
	{
	    free( (*table)->elements );
	}

	free( *table );
	*table = NULL;
    }

    if ( *table )
    {
	GLuint		i;

	for ( i = 0; i < (*table)->size; i++ )
	{
	    hashtable_elt_t	*elt = (*table)->elements[ i ];

	    while ( elt )
	    {
		hashtable_elt_t	*next;

		next = elt->next;
		free( elt );
		elt = next;
	    }
	}

	free( (*table)->elements );
	free( *table );
	*table = NULL;
	}
#endif
}
