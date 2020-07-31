#ifndef _STEALING_QUEUE_H_
#define _STEALING_QUEUE_H_

#include <mutex>
#include <vector>
#include <stdint.h>

template <typename T>
class StealingQueue {
public:
	StealingQueue();

	void	push(T val);
	bool	pop(T& val);

	bool	empty();

private:
	struct Node {
		T val;
		Node* next;
	};

	Node* m_front;
	std::vector<Node*> m_free_nodes;
	std::mutex m_mut;
	uint64_t m_size;

	Node* allocate_node();
	void free_node(Node* n);
};

template <typename T>
StealingQueue<T>::StealingQueue() 
:	m_front(nullptr)
,	m_size(0)
{}

template <typename T>
void StealingQueue<T>::push(T val) {
	std::lock_guard<std::mutex> guard(m_mut);

	Node* n = allocate_node();
	n->val = val;
	n->next = m_front;

	m_front = n;
	++m_size;
}

template <typename T>
bool StealingQueue<T>::pop(T& val) {
	std::lock_guard<std::mutex> guard(m_mut);

	if (m_front == nullptr) {
		return false;
	}

	Node* n = m_front;
	val = m_front->val;
	m_front = m_front->next;
	free_node(n);

	--m_size;

	return true;
}

template <typename T>
typename StealingQueue<T>::Node* StealingQueue<T>::allocate_node() {
	if (m_free_nodes.empty()) {
		return new Node();
	} else {
		Node* r = m_free_nodes.back();
		m_free_nodes.pop_back();
		return r;
	}
}

template <typename T>
void StealingQueue<T>::free_node(StealingQueue<T>::Node* n) {
	m_free_nodes.push_back(n);
}

template <typename T>
bool StealingQueue<T>::empty() {
	return m_size == 0;
}

#endif // _STEALING_QUEUE_H_