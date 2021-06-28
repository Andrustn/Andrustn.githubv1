#ifndef LINKED_LIST_H
#define LINKED_LIST_H

#include <string>
#include <iostream>
#include "LinkedListInterface.h"
using namespace std;

template<typename T>
class LinkedList :
	public LinkedListInterface<T> {

private:
	struct Node {
		T data;
		struct Node* next = NULL;
	};
	typedef struct Node* nodePtr;
	nodePtr headNode = NULL;
public:
	LinkedList() {}
	~LinkedList() {}
	virtual void push_front(const T& value);
	virtual void pop_front(void);
	virtual T& front(void);
	virtual bool empty(void) const;
	virtual void remove(const T& value);
	virtual void clear(void);
	virtual void reverse(void);
	virtual size_t size(void) const;
	virtual string toString(void) const;
	friend ostream& operator<<(ostream& os, LinkedList<T>& userList) {
		os << userList.toString();
		return os;
	}
	class Iterator {
	private:
		Node* iteratorNode;
	public:
		Iterator(Node* headNode) : iteratorNode(headNode) { }
		Iterator() : iteratorNode(NULL) {}
		~Iterator() {}
		bool operator!=(const Iterator& compareNode) const {
			bool notEqual;
			if (this->iteratorNode != compareNode.iteratorNode) {
				notEqual = true;
			}
			else {
				notEqual = false;
			}
			return notEqual;
		}
		Iterator& operator++() {
			if (iteratorNode->next == NULL) {
				iteratorNode = NULL;
			}
			else {
				iteratorNode = iteratorNode->next;
			}
			return *this;
		}
		T& operator*() const {
			return this->iteratorNode->data;
		}
		Node* operator&() const { //overload operator to return the node pointer of the iterator
			return iteratorNode;
		}
		void setIteratorNode(Node* newIteratorNode) {
			iteratorNode = newIteratorNode;
		}
		friend ostream& operator<<(ostream& os, Iterator& userIterator) {
			os << userIterator.toString();
			return os;
		}
		string toString(void) const {
			string outputString = "";
			string data = this->iteratorNode->data;
			outputString = outputString + "[" + data + "]";
			return outputString;
		}
	};
	string iterate(Iterator beginValue) {
		if (headNode == NULL) {
			throw 20;
		}
		else {
			return beginValue.toString();
		}
	}
	Iterator begin(void) {
		return LinkedList<T>::Iterator(headNode);
	}
	Iterator end(void) {
		return LinkedList<T>::Iterator(NULL);
	}
	Iterator find(Iterator first, Iterator last, const T& value) { // TA's said to leave these functions in the class
		LinkedList<T>::Iterator temporaryIterator = first;         // even though they're longer than 10 lines
		if (headNode == NULL) {
			throw 20;
		}
		else {
			while (temporaryIterator != last) {
				if (*temporaryIterator == value) {
					return temporaryIterator;
				}
				else {
					++temporaryIterator;
				}
			}
			throw 2.6; //throw a double to be caught by the "not found" handler
		}
	}
	Iterator find_before(Iterator first, Iterator last, const T& value) { // second find function to be used in "insert before"
		LinkedList<T>::Iterator temporaryIterator = first;			// returns iterator pointing to value before the one to be replaced
		++temporaryIterator;
		LinkedList<T>::Iterator beforeTemporaryIterator = first;
		if (headNode == NULL) {
			throw 20;
		}
		if (*first == value) {
			return beforeTemporaryIterator;
		}
		else {
			while (temporaryIterator != last) {
				if (*temporaryIterator == value) {
					return beforeTemporaryIterator;
				}

				else {
					++temporaryIterator;
					++beforeTemporaryIterator;
				}
			}
			throw 2.6;
		}
	}
	Iterator insert_after(Iterator position, const T& value) {
		if (headNode == NULL) {
			throw 20;
		}
		else {
			LinkedList<T>::Iterator temporaryIterator = position;
			++temporaryIterator;
			nodePtr newNode = new Node;
			newNode->data = value;
			newNode->next = &temporaryIterator;
			Node* insertPosition = &position;
			insertPosition->next = newNode;

			return LinkedList<T>::Iterator(newNode);
		}
	}
	Iterator insert_before(Iterator position, const T& value) {
		if (headNode == NULL) {
			throw 20;
		}
		else {
			if (&position == headNode) {
				push_front(value);
			}
			else {
				insert_after(position, value);
			}
		}
	}
	Iterator erase(Iterator position) {
		nodePtr temporaryNode = headNode;
		nodePtr secondTemporaryNode = headNode;
		if (headNode == NULL) {
			throw 20;
		}
		else {
			if (headNode == &position) {
				temporaryNode = headNode->next; // checking if headNode is Value
				delete headNode;
				headNode = temporaryNode;
				position.setIteratorNode(temporaryNode);
				return position;
			}
			temporaryNode = headNode;
			secondTemporaryNode = temporaryNode->next;
			while (temporaryNode->next != NULL) {
				if (secondTemporaryNode == &position) {
					temporaryNode->next = secondTemporaryNode->next;
					delete secondTemporaryNode;
					secondTemporaryNode = temporaryNode->next;
					position.setIteratorNode(secondTemporaryNode);
					return position;
					temporaryNode = secondTemporaryNode;
					secondTemporaryNode = temporaryNode->next;
				}
				else {
					temporaryNode = secondTemporaryNode;
					secondTemporaryNode = temporaryNode->next;
				}
			}
		}
	}
 	void replace(Iterator first, Iterator last, const T& old_value, const T& new_value) {
		find(first, last, old_value);
		if (headNode == NULL) {
			throw 20;
		}
		else {
			nodePtr temporaryNode = &first;
			nodePtr secondTemporaryNode = &first;
			if (*first == old_value) {
				nodePtr temporaryNode = &first;
				LinkedList<T>::Iterator newPosition = insert_after(first, new_value);
				temporaryNode = &newPosition;
				delete headNode;
				headNode = temporaryNode;
				first.setIteratorNode(temporaryNode);
			}
			while (first != last) {
				temporaryNode = &first;
				if (temporaryNode->next == NULL) {
					break;
				}
				secondTemporaryNode = temporaryNode->next;
				if (secondTemporaryNode->data == old_value) {
					++first;
					LinkedList<T>::Iterator newPosition = insert_after(first, new_value);
					temporaryNode->next = &newPosition;
					delete &first;
					first.setIteratorNode(temporaryNode);
					if (*newPosition == old_value) {
						++first;
					}
				}
				else {
					++first;
				}
			}
		}
		return;
	}
};
template<typename T>  // ************************the rest of this is from the previous lab********************************
void LinkedList<T>::push_front(const T& value) {
	if (headNode == NULL) {
		nodePtr temporaryNode = NULL;
		nodePtr newNode = new Node;
		newNode->data = value;
		temporaryNode = newNode;
		headNode = newNode;
		newNode->next = NULL;
	}
	else {
		nodePtr temporaryNode = NULL;
		nodePtr newNode = new Node;
		newNode->data = value;
		temporaryNode = newNode;
		temporaryNode->next = headNode;
		headNode = newNode;
	}
}
template<typename T>
void LinkedList<T>::pop_front(void) {
	if (headNode == NULL) {
		throw 20;
	}
	else {
		nodePtr temporaryNode = NULL;
		temporaryNode = headNode;
		headNode = temporaryNode->next;
		delete temporaryNode;
	}
}
template<typename T>
T& LinkedList<T>::front(void) {
	if (headNode == NULL) {
		throw 20;
	}
	else {
		return headNode->data;
	}
}
template<typename T>
bool LinkedList<T>::empty(void) const {
	bool empty;
	if (headNode == NULL) {
		empty = true;
	}
	else {
		empty = false;
	}
	return empty;
}
template<typename T>
void LinkedList<T>::remove(const T& value) {
	nodePtr temporaryNode = headNode;
	nodePtr secondTemporaryNode = headNode;
	T newValue = value;

	if (headNode->data == newValue) {
		temporaryNode = headNode->next; // checking if headNode is Value
		delete headNode;
		headNode = temporaryNode;
	}
	secondTemporaryNode = temporaryNode;
	while (secondTemporaryNode->next != NULL) {
		secondTemporaryNode = temporaryNode->next;
		if (secondTemporaryNode->data == newValue) {     // iterating through rest of linked list
			temporaryNode->next = secondTemporaryNode->next;
			delete secondTemporaryNode;
			secondTemporaryNode = temporaryNode;
		}
		else {
			temporaryNode = secondTemporaryNode;
		}
	}
	if (headNode->next == NULL && headNode->data == newValue) {
		delete headNode;
		headNode = NULL;       // checking again if headNode is Value (in the event that it's the only node left)
	}
}
template<typename T>
void LinkedList<T>::clear(void) {
	if (headNode == NULL) {
		throw 20;
	}

	else {
		nodePtr temporaryNode = headNode->next;
		while (temporaryNode != NULL) {
			delete headNode;
			headNode = temporaryNode;
			temporaryNode = temporaryNode->next;
		}
		delete headNode;
		headNode = NULL;
	}
}
template<typename T>
void LinkedList<T>::reverse(void) {
	if (headNode == NULL) {
		throw 20;
	}
	else {
		nodePtr currentNode = headNode;
		nodePtr previousNode = NULL;
		nodePtr nextNode = NULL;
		while (currentNode != NULL) {
			nextNode = currentNode->next;
			currentNode->next = previousNode;
			previousNode = currentNode;
			currentNode = nextNode;
		}
		headNode = previousNode;
	}
}
template<typename T>
size_t LinkedList<T>::size(void) const {
	nodePtr temporaryNode = headNode;
	int counter = 0;
	while (temporaryNode != NULL) {
		counter++;
		temporaryNode = temporaryNode->next;
	}
	return counter;
}
template<typename T>
string LinkedList<T>::toString(void) const {
	nodePtr temporaryNode = headNode;
	string listContents = "";
	if (headNode == NULL) {
		throw 20;
	}
	else {
		while (temporaryNode != NULL) {
			listContents = listContents + " " + temporaryNode->data;
			temporaryNode = temporaryNode->next;
		}
	}
	return listContents;
}
#endif





