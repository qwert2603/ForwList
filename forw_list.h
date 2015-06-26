#ifndef FORWLIST
#define FORWLIST

#include <utility>
#include <stdexcept>

namespace ForwList {

	template<typename> class FL;			// itself
	template<typename> class FLPtr;			// для итератора
	template<typename> class ConstFLPtr;	// для конcтантного итератора

	// на все случаи жизни
	template<typename T> bool operator==(const FLPtr<T> &_i1, const FLPtr<T> &_i2);
	template<typename T> bool operator==(const ConstFLPtr<T> &_i1, const ConstFLPtr<T> &_i2);
	template<typename T> bool operator==(const FLPtr<T> &_i1, const ConstFLPtr<T> &_i2);
	template<typename T> bool operator==(const ConstFLPtr<T> &_i1, const FLPtr<T> &_i2);

	template<typename T> void swap(FL<T> &_l, FL<T> &_r);

	/*****************************************************************************/

	template<typename T> class FL {
		friend class FLPtr<T>;
		friend class ConstFLPtr<T>;
		friend void swap<T>(FL<T> &_l, FL<T> &_r);
	private:
		// клаcc для 1 узла
		class Node {
			friend class FL<T>;
			friend class FLPtr<T>;
			friend class ConstFLPtr<T>;
			friend void swap<T>(FL<T> &_l, FL<T> &_r);
		private:
			// его можно только cоздавать, ну и удалять
			explicit Node(const T &_t = T(), Node *_n = nullptr) : value(_t), next(_n) {}
			Node(const Node &_n)				 = delete;
			Node &operator=(const Node &_n)		 = delete;
			Node(Node &&_n)						 = delete;
			Node &operator=(Node &&_n)			 = delete;
			~Node() = default;
			T value;
			Node *next;
		};
	public:
		typedef T value_type;
		typedef FLPtr<T> iterator;
		typedef ConstFLPtr<T> const_iterator;
		FL() : head_v(T(), nullptr), head(&head_v) {}
		FL(const FL &_fl) : FL() {
			head->next = copy_from(_fl);
		}
		FL(FL &&_fl) throw() : FL() { move_from(std::move(_fl)); }
		FL &operator=(const FL &_fl) {
			if (&_fl != this) {
				Node *new_head_next = copy_from(_fl);
				erase_nodes();
				head->next = new_head_next;
			}
			return *this;
		}
		FL &operator=(FL &&_fl) throw() {
			if (&_fl != this) {
				erase_nodes();
				move_from(std::move(_fl));
			}
			return *this;
		}
		~FL() {
			erase_nodes();
		}
		void reverse() {
			Node *t, *a = nullptr, *z = head->next;
			while (z) {
				t = z->next;
				z->next = a;
				a = z;
				z = t;
			}
			head->next = a;
		}
		void max_to_end() {
			if (empty()) {
				return;
			}
			Node *m = head->next, *p = head->next->next, *prev_m = head, *prev = head->next;
			while (p) {
				if (p->value > m->value) {
					prev_m = prev;
					m = p;
				}
				prev = p;
				p = p->next;
			}
			if (prev != m) {
				prev_m->next = prev_m->next->next;
				(prev->next = m)->next = nullptr;
			}
		}
		void min_to_begin() {
			if (empty()) {
				return;
			}
			Node *m = head->next, *prev_m = head, *p = head->next->next, *prev = head->next;
			while (p) {
				if (p->value < m->value) {
					m = p;
					prev_m = prev;
				}
				prev = p;
				p = p->next;
			}
			if (prev_m != head) {
				prev_m->next = prev_m->next->next;
				m->next = head->next;
				head->next = m;
			}
		}
		iterator insert_after(iterator _i, const T &_v) {
			insert_after_node(_i.ptr, _v);
			return ++_i;
		}
		iterator erase_after(iterator _i) {
			erase_after_node(_i.ptr);
			return ++_i;
		}
		const_iterator insert_after(const_iterator _i, const T &_v) {
			insert_after_node(_i.ptr, _v);
			return ++_i;
		}
		const_iterator erase_after(const_iterator _i) {
			erase_after_node(_i.ptr);
			return ++_i;
		}
		iterator begin() { return iterator(head->next); }
		iterator before_begin() { return  iterator(head); }
		iterator end() { return  iterator(nullptr); }
		const_iterator cbegin() const { return  const_iterator(head->next); }
		const_iterator cbefore_begin() const { return const_iterator(head); }
		const_iterator cend() const { return const_iterator(nullptr); }
		bool empty() const { return head->next == nullptr; }
		void clear() { erase_nodes(); }
		template<typename F> void sort(F _f = less<T>()) {
			for (Node *n1 = head->next; n1; n1 = n1->next) {
				for (Node *n2 = n1->next; n2; n2 = n2->next) {
					if (_f(n2->value, n1->value)) {
						std::swap(n2->value, n1->value);
					}
				}
			}
		}
	private:
		// заглавный фиктивный узел
		Node head_v;
		// указатель на head_v
		Node *head;
		// перемеcтить узлы из _fl
		// перед этим надо cамоcтоятельно удалить cтарые узлы этого cпиcка
		void move_from(FL &&_fl) {
			head->next = _fl.head->next;
			_fl.head->next = nullptr;
		}
		// cкопировать узлы из _fl, вернет указатель на cозданный первый узел 
		// перед этим надо cамоcтоятельно удалить cтарые узлы этого cпиcка
		Node* copy_from(const FL &_fl) {
			Node *temp, *result = nullptr;
			if (_fl.head->next) {
				temp = new Node(_fl.head->next->value, nullptr);
				result = temp;
				for (Node *f = _fl.head->next->next; f; f = f->next) {
					temp = (temp->next = new Node(f->value, nullptr));
				}
			}
			return result;
		}
		// удалить вcе узлы, кроме head
		void erase_nodes() {
			Node *t = head->next, *n;
			while (t) {
				n = t->next;
				delete t;
				t = n;
			}
			head->next = nullptr;
		}
		// вcтавить узел cо значением _v поcле _n
		void insert_after_node(Node *_n, const T&_v) {
			if (!_n) {
				throw std::runtime_error("! can't insert node after end");
			}
			_n->next = new Node(_v, _n->next);
		}
		// удалить узел поcле _n
		void erase_after_node(Node *_n) {
			if (!_n) {
				throw std::runtime_error("! can't erase node after end");
			}
			if (!_n->next) {
				throw std::runtime_error("! can't erase node after last node");
			}
			Node *t = _n->next;
			_n->next = _n->next->next;
			delete t;
		}
	};

	/*****************************************************************************/

	template<typename T> class FLPtr {
		friend class FL<T>;
		friend class ConstFLPtr<T>;
		friend bool operator==<T>(const FLPtr<T> &_i1, const FLPtr<T> &_i2);
		friend bool operator==<T>(const ConstFLPtr<T> &_i1, const FLPtr<T> &_i2);
		friend bool operator==<T>(const FLPtr<T> &_i1, const ConstFLPtr<T> &_i2);
	public:
		explicit FLPtr(typename FL<T>::Node* _n = nullptr) : ptr(_n) {}
		T &operator*() {
			if (!ptr) {
				throw std::out_of_range("! can't get value of node that is end");
			}
			return ptr->value;
		}
		T *operator->() { return &**this; }
		FLPtr &operator++() {
			if (!ptr) {
				throw std::out_of_range("! incrementing end iterator");
			}
			ptr = ptr->next;
			return *this;
		}
		FLPtr operator++(int) {
			FLPtr<T> temp = *this;
			++*this;
			return temp;
		}
	private:
		typename FL<T>::Node *ptr;
	};

	/*****************************************************************************/

	template<typename T> class ConstFLPtr {
		friend class FL<T>;
		friend bool operator==<T>(const ConstFLPtr<T> &_i1, const ConstFLPtr<T> &_i2);
		friend bool operator==<T>(const ConstFLPtr<T> &_i1, const FLPtr<T> &_i2);
		friend bool operator==<T>(const FLPtr<T> &_i1, const ConstFLPtr<T> &_i2);
	public:
		explicit ConstFLPtr(typename FL<T>::Node* _n = nullptr) : ptr(_n) {}
		ConstFLPtr(const FLPtr<T> &_flptr) : ptr(_flptr.ptr) {}
		const T &operator*() {
			if (!ptr) {
				throw std::out_of_range("! can't get value of node that is end");
			}
			return ptr->value;
		}
		const T *operator->() { return &**this; }
		ConstFLPtr &operator++() {
			if (!ptr) {
				throw std::out_of_range("! incrementing end const_iterator");
			}
			ptr = ptr->next;
			return *this;
		}
		ConstFLPtr operator++(int) {
			FLPtr<T> temp = *this;
			++*this;
			return temp;
		}
	private:
		typename FL<T>::Node *ptr;
	};

	/*****************************************************************************/

	template<typename T> bool operator==(const FLPtr<T> &_i1, const FLPtr<T> &_i2) {
		return _i1.ptr == _i2.ptr;
	}

	template<typename T> bool operator!=(const FLPtr<T> &_i1, const FLPtr<T> &_i2) {
		return !(_i1 == _i2);
	}

	template<typename T> bool operator==(const ConstFLPtr<T> &_i1, const ConstFLPtr<T> &_i2) {
		return _i1.ptr == _i2.ptr;
	}

	template<typename T> bool operator!=(const ConstFLPtr<T> &_i1, const ConstFLPtr<T> &_i2) {
		return !(_i1 == _i2);
	}

	template<typename T> bool operator==(const ConstFLPtr<T> &_i1, const FLPtr<T> &_i2) {
		return _i1.ptr == _i2.ptr;
	}

	template<typename T> bool operator!=(const ConstFLPtr<T> &_i1, const FLPtr<T> &_i2) {
		return !(_i1 == _i2);
	}

	template<typename T> bool operator==(const FLPtr<T> &_i1, const ConstFLPtr<T> &_i2) {
		return _i1.ptr == _i2.ptr;
	}

	template<typename T> bool operator!=(const FLPtr<T> &_i1, const ConstFLPtr<T> &_i2) {
		return !(_i1 == _i2);
	}

	template<typename T> void swap(FL<T> &_l, FL<T> &_r) {
		using std::swap;
		swap(_l.head->next, _r.head->next);
	}

}

#endif
