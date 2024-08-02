#ifndef _RB_TREE_H_
#define _RB_TREE_H_
#pragma once
#include "../iterator/iterator.h"
#include "../allocator/allocator.h"
#include "../allocator/construct.h"
#include<assert.h>

namespace mySTL {
	namespace rb_tree_detail {
		typedef bool rb_tree_color_type;
		const rb_tree_color_type rb_tree_red = true;
		const rb_tree_color_type rb_tree_black = false;

		typedef int rb_tree_node_type;
		const rb_tree_node_type rb_tree_header = -1;
		const rb_tree_node_type rb_tree_root = 0;
		const rb_tree_node_type rb_tree_leftChild = 1;
		const rb_tree_node_type rb_tree_rightChild = 2;
		struct rb_tree_node_base {
			typedef rb_tree_color_type color_type;
			typedef rb_tree_node_type node_type;
			typedef rb_tree_node_base* base_ptr;
			color_type color;
			base_ptr parent;
			base_ptr left;
			base_ptr right;
			static base_ptr get_min_node(base_ptr node) {
				while (node->left != nullptr) node = node->left;
				return node;
			}
			static base_ptr get_max_node(base_ptr node) {
				while (node->right != nullptr) node = node->right;
				return node;
			}
			node_type get_node_type() {
				if (parent == nullptr) return rb_tree_header;
				else if (parent->parent = this) return color == rb_tree_red ? rb_tree_header : rb_tree_root;
				else if (parent->left == this) return rb_tree_leftChild;
				else return rb_tree_rightChild;
			}
		};
		template<class T>
		struct rb_tree_node :public rb_tree_node_base {
			typedef rb_tree_node<T>* node_pointer;
			T value_field;
		};

		struct rb_tree_iterator_base
		{
			typedef rb_tree_node_base::base_ptr base_ptr;
			typedef bidrectional_iterator_tag iterator_category;
			typedef ptrdiff_t difference_type;
			typedef rb_tree_node_type node_type;
			base_ptr node;

			rb_tree_iterator_base(base_ptr x) :node(x) {  }

			void incrment() {
				if (node->right == nullptr) {
					base_ptr tmp = node;
					while (tmp->get_node_type() == rb_tree_rightChild) {
						tmp = tmp->parent;
					}
					if (tmp->get_node_type() == rb_tree_leftChild) {
						node = tmp->parent;
					}
				}
				else {
					node = rb_tree_node_base::get_min_node(node->right);
				}
			}

			void decrment() {
				if (node->left == nullptr) {
					base_ptr tmp = node;
					while (tmp->get_node_type() == rb_tree_leftChild) {
						tmp = tmp->parent;
					}
					if (tmp->get_node_type() == rb_tree_rightChild) {
						node = tmp->parent;
					}
				}
				else {
					node = rb_tree_node_base::get_max_node(node->left);
				}
			}
		};

		template<class T>
		struct rb_tree_iterator :public rb_tree_iterator_base {
			typedef bidrectional_iterator_tag iterator_category;
			typedef T value_type;
			typedef ptrdiff_t difference_type;
			typedef size_t size_type;
			typedef T* pointer;
			typedef T& reference;
			typedef rb_tree_node<T>* node_pointer;

			rb_tree_iterator() :rb_tree_iterator_base(nullptr) {}
			rb_tree_iterator(node_pointer node) :rb_tree_iterator_base(node) {}
			rb_tree_iterator(const rb_tree_iterator& other) :rb_tree_iterator_base(other.node) {}

			reference operator*() {
				return node_pointer(node)->value_field;
				//return statc_cast<node_pointer>(node).vl
			}
			pointer operator->() {
				return &(operator*());
			}
			
			rb_tree_iterator& operator++() {
				incrment();
				return this;
			}
			rb_tree_iterator operator++(int) {
				rb_tree_iterator tmp = this;
				incrment();
				return tmp;
			}
			rb_tree_iterator& operator--() {
				decrment();
				return this;
			}
			rb_tree_iterator operator--(int) {
				rb_tree_iterator tmp = this;
				decrment();
				return tmp;
			}
			bool operator==(const rb_tree_iterator& other) const { return node == other.node; }
			bool operator!=(const rb_tree_iterator& other) const { return node != other.node; }
		};
	}

	template<class Value>
	inline void rb_tree_rotate_left(rb_tree_detail::rb_tree_node<Value>* node, rb_tree_detail::rb_tree_node<Value>*& root) {
		auto node_type = node->get_node_type();
		if (node_type == rb_tree_detail::rb_tree_header) {
			assert(false);
			return;
		}

		rb_tree_detail::rb_tree_node<Value>* child = node->right;
		rb_tree_detail::rb_tree_node<Value>* parent = node->parent;

		node->right = child->left;
		if (child->left != nullptr)
			child->left->parent = node;
		node->parent = child;
		child->left = node;
		child->parent = parent;
		if (node_type == rb_tree_detail::rb_tree_rightChild) {
			parent->right = child;
		}
		else if (node_type == rb_tree_detail::rb_tree_leftChild) {
			parent->left = child;
		}
		else if (node_type == rb_tree_detail::rb_tree_root) {
			parent->parent = child;
			root = child;
		}
	}

	template<class Value>
	inline void rb_tree_rotate_right(rb_tree_detail::rb_tree_node<Value>* node, rb_tree_detail::rb_tree_node<Value>*& root) {
		auto node_type = node->get_node_type();
		if (node_type == rb_tree_detail::rb_tree_header) {
			assert(false);
			return;
		}
		rb_tree_detail::rb_tree_node<Value>* child = node->left;
		rb_tree_detail::rb_tree_node<Value>* parent = node->parent;

		node->parent = child;
		node->left = child->right;
		if (child->right != nullptr) 
			child->right->parent = node;
		

		child->parent = parent;
		child->right = node;

		if (node_type == rb_tree_detail::rb_tree_rightChild) {
			parent->right = child;
		}
		else if (node_type == rb_tree_detail::rb_tree_leftChild) {
			parent->left = child;
		}
		else if (node_type == rb_tree_detail::rb_tree_root) {
			parent->parent = child;
			root = child;
		}

	}

	template<class Value>
	inline void rb_tree_rebalance(rb_tree_detail::rb_tree_node<Value>* insertNode, rb_tree_detail::rb_tree_node<Value>*& root) {
		insertNode->color = rb_tree_detail::rb_tree_red;
		typedef typename rb_tree_detail::rb_tree_red red;
		typedef typename rb_tree_detail::rb_tree_black black;
		while (insertNode != root && insertNode->parent->color == red) {
			rb_tree_detail::rb_tree_node<Value>* parent = insertNode->parent;
			if (parent == parent->parent->left) {
				if (parent->color == red) {
					rb_tree_detail::rb_tree_node<Value>* uncle = parent->parent->right;
					if (uncle->color == red) {
						parent->color = black;
						uncle->color = black;
						grandParent->color = red;
						insertNode = grandParent;
					}
					//循环往上时，可能出现uncle-color为黑的情况，因为parent的另一子节点可能为子树。
					//初始时，另一子节点为叶子节点，不可能出现uncle-color为黑
					else if (uncle == nullptr || uncle->color == black) {
						rb_tree_detail::rb_tree_node<Value>* grandParent = parent->parent;
						if (insertNode->get_node_type() == rb_tree_detail::rb_tree_rightChild) {
							rb_tree_rotate_left(parent, root);
						}
						grandParent->color = red;
						//如果出现左旋，parent和insertNode父子关系互换了，不能直接对parent设置black
						//因此必须先判断是否左旋，再设置着色
						grandParent->left->color = black;
						rb_tree_rotate_right(grandParent, root);
					}
				}
			}
			else {
				if (parent->color == red) {
					rb_tree_detail::rb_tree_node<Value>* uncle = parent->parent->left;
					if (uncle->color == red) {
						parent->color = black;
						uncle->color = black;
						grandParent->color = red;
						insertNode = grandParent;
					}
					else if (uncle == nullptr || uncle->color == black) {
						rb_tree_detail::rb_tree_node<Value>* grandParent = parent->parent;
						if (insertNode->get_node_type() == rb_tree_detail::rb_tree_leftChild) {
							rb_tree_rotate_right(parent, root);
						}
						grandParent->color = red;
						grandParent->right->color = black;
						rb_tree_rotate_left(grandParent, root);
					}
				}
			}
		}
		root->color = black;
	}

	template<class Key, class Value,class KeyOfValue, class Compare, class Alloc = Allocator<rb_tree_detail::rb_tree_node<Value>>>
	class rb_tree {
		protected:
			typedef rb_tree_detail::rb_tree_color_type color_type;
			typedef rb_tree_detail::rb_tree_node_type node_type;
			typedef rb_tree_detail::rb_tree_node<Value> rb_tree_node;
			typedef rb_tree_detail::rb_tree_node_base* base_ptr;
			typedef Alloc node_allocator;
		public:
			typedef Value value_type;

			typedef Key key_type;
			typedef value_type* pointer;
			typedef const value_type* const_pointer;
			typedef value_type& reference;
			typedef const value_type& const_reference;
			typedef rb_tree_node* node_pointer;
			
			typedef size_t size_type;
			typedef ptrdiff_t difference_type;
			typedef rb_tree_detail::rb_tree_iterator<value_type> iterator;
			typedef const rb_tree_detail::rb_tree_iterator<value_type> const_iterator;
			typedef mySTL::reverse_iterator<iterator> reverse_iterator;
		protected:
			node_pointer header;
			size_type node_num;
			Compare key_compare;

			node_pointer& root() const { return static_cast<node_pointer>(header->parent); }
			node_pointer& leftmost() const { return static_cast<node_pointer>(header->left); }
			node_pointer& rightmost() const { return static_cast<node_pointer>(header->right); }

			static node_pointer& left(node_pointer node) { return static_cast<node_pointer>(node->left); }
			static node_pointer& right(node_pointer node) { return static_cast<node_pointer>(node->right); }
			static node_pointer& parent(node_pointer node) { return static_cast<node_pointer>(node->parent); }
			static reference value(node_pointer node) { return node->value_field; }
			static const Key& key(node_pointer node) { return KeyOfValue()(value(node)); }
			static color_type& color(node_pointer node) { return node->color; }

			static node_pointer& left(base_ptr node) { return static_cast<node_pointer>(node->left); }
			static node_pointer& right(base_ptr node) { return static_cast<node_pointer>(node->right); }
			static node_pointer& parent(base_ptr node) { return static_cast<node_pointer>(node->parent); }
			static reference value(base_ptr node) { return (static_cast<node_pointer>(node))->value_field; }
			static const Key& key(base_ptr node) { return KeyOfValue()(value(node)); }
			static color_type& color(base_ptr node) { return node->color; }

			static node_pointer get_min_node(node_pointer node) { return static_cast<node_pointer>(rb_tree_detail::rb_tree_node_base::get_min_node(node)); }
			static node_pointer get_max_node(node_pointer node) { return static_cast<node_pointer>(rb_tree_detail::rb_tree_node_base::get_max_node(node)); }

			node_pointer get_node() { return node_allocator::allocate(1); }
			void put_node(node_pointer node) { node_allocator::deallocate(node); }

			node_pointer create_node(const value_type& value) {
				node_pointer nodePtr = get_node();
				mySTL::construct(&nodePtr->value_field, value);
				return nodePtr;
			}

			node_pointer clone_node(node_pointer node) {
				node_pointer nodePtr = create_node(node->value_field);
				nodePtr->color = node->color;
				nodePtr->left = nullptr;
				nodePtr->right = nullptr;
				return nodePtr;
			}

			void destroy_node(node_pointer node) {
				mySTL::destroy(&node->value_field);
				put_node(node);
			}
		private:
			//x为新值插入点，y为插入点的父节点，v为待插入值。返回指向新插入节点的迭代器
			iterator insert_aux(base_ptr x, base_ptr y, const value_type& value);
			//拷贝整棵树
			node_pointer copy_aux(node_pointer node);
			//删除整棵树
			void erase_aux(node_pointer node);
			void initialize() {
				header = get_node();
				header->color = rb_tree_detail::rb_tree_red;
				header->left = header;
				header->right = header;
				header->parent = nullptr;
			}
		public:
			rb_tree() :node_num(0), key_compare() { initialize(); }
			rb_tree(const Compare& comp) :node_num(0), key_compare(comp) { initialize(); }
			rb_tree(const rb_tree& other) {
				initialize();
				if (other.root() != nullptr) {
					key_compare = other.key_compare;
					node_num = other.node_num;
					root() = copy(other.root());
					root()->parent = header;
					leftmost() = get_min_node(root());
					rightmost = get_max_node(root());
				}
			}
			rb_tree& operator=(const rb_tree& other) {
				if (this != &other) {
					clear();
					initialize();
					if (other.root() != nullptr) {
						key_compare = other.key_compare;
						node_num = other.node_num;
						root() = copy(other.root());
						root()->parent = header;
						leftmost() = get_min_node(root());
						rightmost = get_max_node(root());
					}
				}
				return *this;
			}
			~rb_tree() { clear(); }

			void clear() {
				if (node_num > 0) {
					erase_aux(root());
					leftmost() = header;
					rightmost() = header;
					root() == nullptr;
					node_num = 0;
				}
			}

			Compare key_comp() const { return key_compare; }
			iterator begin() { return leftmost(); }
			const_iterator begin() const { return leftmost(); }
			iterator end() { return rightmost(); }
			const_iterator end() const { return rightmost(); }
			reverse_iterator rbegin() { return reverse_iterator(end()); }
			reverse_iterator rend() { return reverse_iterator(begin()); }

			bool empty() const { return node_num == 0; }
			size_type size() const { return node_num; }

			void swap(rb_tree& other) {
				std::swap(header, other.header);
				std::swap(node_num, other.node_num);
				std::swap(key_compare, other.key_compare);
			}

			std::pair<iterator, bool> insert_unique(const value_type& v);
			template<class InputIterator>
			void insert_unique(InputIterator first, InputIterator last);
	};

	template<class Key, class Value, class KeyOfValue, class Compare, class Alloc>
	void rb_tree<Key, Value, KeyOfValue, Compare, Alloc>::erase_aux(node_pointer node) {
		while (node != nullptr) {
			erase_aux(right(node));
			node_pointer left = left(node);
			destroy_node(node);
			node = left;
		}
	}

	template<class Key, class Value, class KeyOfValue, class Compare, class Alloc>
	typename rb_tree<Key, Value, KeyOfValue, Compare, Alloc>::iterator rb_tree<Key, Value, KeyOfValue, Compare, Alloc>::insert_aux(base_ptr x, base_ptr y, const value_type& value) {
		assert(x == nullptr);
		node_pointer x_ = static_cast<node_pointer>(x);
		node_pointer y_ = static_cast<node_pointer>(y);

		node_pointer new_node = create_node(value);
		if (y_ == header || key_compare(KeyOfValue(value), Key(y_))) {
			left(y_) = new_node;
			if (y_ == header) {
				root() == new_node;
				rightmost() = new_node;
			}
			else if (y_ == leftmost()) {
				leftmost() = new_node;
			}
		}
		else {
			right(y_) = new_node;
			if (y_ == rightmost()) {
				rightmost() == new_node;
			}
		}
		parent(new_node) = y_;
		left(new_node) = nullptr;
		right(new_node) = nullptr;

		rb_tree_rebalance(new_node, root());

		node_num++;
		return iterator(new_node);
	}

	template<class Key, class Value, class KeyOfValue, class Compare, class Alloc>
	std::pair<typename rb_tree<Key, Value, KeyOfValue, Compare, Alloc>::iterator, bool> rb_tree<Key, Value, KeyOfValue, Compare, Alloc>::insert_unique(const value_type& v){
		node_pointer parent = header;
		node_pointer node = root();

		bool isLessParent = true;

		//查询到最底叶子节点
		while (node != nullptr) {
			parent = node;
			isLessParent = key_compare(KeyOfValue(value), Key(node));
			node = isLessParent ? left(node) : right(node);
		}

		iterator it = iterator(parent);
		if (isLessParent) {
			if (it == begin()) return std::pair<iterator, bool>(insert_aux(node, parent, value), true);
			else
				//判断parent上面有没有与value相等key的节点
				it--;
		}
		if (key_compare(Key(it.node), KeyOfValue(value)))
			return std::pair<iterator, bool>(insert_aux(node, parent, value), true);
		else
			return std::pair<iterator, bool>(it, false);
	}

	template<class Key, class Value, class KeyOfValue, class Compare, class Alloc>
	template<class InputIterator>
	void rb_tree<Key, Value, KeyOfValue, Compare, Alloc>::insert_unique(InputIterator first, InputIterator last) {
		while (first != last) {
			insert_unique(*first++);
		}
	}
}

#endif // !_RB_TREE_H_

