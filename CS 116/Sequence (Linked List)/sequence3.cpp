// INVARIANT for the sequence ADT:
//   1. The items in the sequence are stored on a linked list, with the start of the sequence at the head of the list;
//   2. The pointer to the head of the list is stored in the member variable head_ptr;
//   3. The pointer to the tail of the list is stored in the member variable tail_ptr;
//   4. If a current item exists, cursor points to the node containing that item; otherise it is NULL. 
//   5. If an item before the current item exists, precursor points to the node containing that item; otherwise it is NULL.
//   6. The total number of items in the list is stored in the member variable many_nodes.

#include<iostream>
#include <cstdlib>
#include <cassert>
#include "sequence3.h"
#include "node1.h"

namespace main_savitch_5 {
	void list_piece(const node* start_ptr, const node* end_ptr, node*& head_ptr, node*& tail_ptr) { // O(n)
		head_ptr = tail_ptr = NULL;
		if (start_ptr == end_ptr) {
			return;
		}
		list_head_insert(head_ptr, start_ptr->data());
		tail_ptr = head_ptr;
		start_ptr = start_ptr->link();
		while (start_ptr != end_ptr) {
			list_insert(tail_ptr, start_ptr->data());
			tail_ptr = tail_ptr->link();
			start_ptr = start_ptr->link();
		}
	}
	
	sequence::sequence() { //O(1)
		head_ptr = NULL;
		tail_ptr = NULL;
		cursor = NULL;
		precursor = NULL;
		many_nodes = 0;
	}

	sequence::sequence(const sequence& source) { //O(n)
		if (source.cursor == NULL) {
			list_copy(source.head_ptr, head_ptr, tail_ptr);
			cursor = precursor = NULL;
		}
		else if (source.precursor == NULL) {
			list_copy(source.head_ptr, head_ptr, tail_ptr);
			cursor = head_ptr;
			precursor = NULL;
		}
		else {
			list_piece(source.head_ptr, source.cursor, head_ptr, precursor);
			list_piece(source.cursor, NULL, cursor, tail_ptr);
			precursor->set_link(cursor);
		}

		many_nodes = source.many_nodes;
	}
	
	sequence::~sequence() { //0(n)
		list_clear(head_ptr);
		many_nodes = 0;
	}

	void sequence::start() { //O(1)
		cursor = head_ptr;
		precursor = NULL;
	}

	void sequence::advance() { //O(1)
		assert(is_item());
		precursor = cursor;
		cursor = cursor->link();
		if (cursor == NULL) {
			precursor = NULL;
		}
	}
	void sequence::insert(const value_type& entry) { //O(1)
		if (precursor == NULL) {
			list_head_insert(head_ptr, entry);
			cursor = head_ptr;
			if (tail_ptr == NULL) {
				tail_ptr = head_ptr;
			}
		}
		else {
			list_insert(precursor, entry);
			cursor = precursor->link();
		}
		many_nodes++;
	}

	void sequence::attach(const value_type& entry) {//O(1)
		if (tail_ptr == NULL) {
			insert(entry);
		}
		else if (cursor == NULL) {
			list_insert(tail_ptr, entry);
			precursor = tail_ptr;
			cursor = tail_ptr = tail_ptr->link();
			many_nodes++;
		}
		else {
			list_insert(cursor, entry);
			precursor = cursor;
			cursor = cursor->link();
			if (tail_ptr == precursor) {
				tail_ptr = cursor;
			}
			many_nodes++;
		}
	}

	void sequence::remove_current() { //O(1)
		assert(is_item());
		if (cursor == head_ptr) {
			list_head_remove(head_ptr);
			cursor = head_ptr;
			if (cursor == NULL) {
				tail_ptr = NULL;
			}
		}
		else {
			list_remove(precursor);
			cursor = precursor->link();
			if (cursor == NULL) {
				tail_ptr = precursor;
				precursor = NULL;
			}
		}
		many_nodes--;
	}

	void sequence::operator =(const sequence& source) { //O(n)
		if (this == &source) {
			return;
		}
		list_clear(head_ptr);
		if (source.cursor == NULL) {
			list_copy(source.head_ptr, head_ptr, tail_ptr);
			cursor = precursor = NULL;
		}
		else if (source.precursor == NULL) {
			list_copy(source.head_ptr, head_ptr, tail_ptr);
			cursor = head_ptr;
			precursor = NULL;
		}
		else {
			list_piece(source.head_ptr, source.cursor, head_ptr, precursor);
			list_piece(source.cursor, NULL, cursor, tail_ptr);
			precursor->set_link(cursor);
		}

		many_nodes = source.many_nodes;
	}

	sequence::value_type sequence::current() const { //O(1)
		assert(is_item());
		return cursor->data();
	}


}