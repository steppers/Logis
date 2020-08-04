#ifndef _STEALING_STACK_
#define _STEALING_STACK_

#include <atomic>

template <typename T>
class StealingStack {
public:
	StealingStack();

	void push(T val);
	bool pop(T& val);

private:
	struct Node {
		T val;
		Node* next;
	};

	std::atomic<Node*> m_top;
};

template <typename T>
StealingStack<T>::StealingStack()
:	m_top(nullptr)
{}

template <typename T>
void StealingStack<T>::push(T val) {
	Node* n = new Node();
	n->val = val;
	n->next = m_top.load(std::memory_order_consume);

	while (!m_top.compare_exchange_weak(	n->next,
											n,
											std::memory_order_release,
											std::memory_order_consume))
	{}
}

template <typename T>
bool StealingStack<T>::pop(T& val) {
	Node* f = m_top.load(std::memory_order_consume);

	if (f == nullptr) {
		return false;
	}

	while (!m_top.compare_exchange_weak(	f,
											f->next,
											std::memory_order_release,
											std::memory_order_consume))
	{
		if(f == nullptr) {
			return false;
		}
	}

	val = f->val;
	
	return true;
}

#endif // _STEALING_STACK_