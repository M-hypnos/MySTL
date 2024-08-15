#pragma once
#ifndef _RB_TREE_H_
#define _RB_TREE_H_
#include "../allocator/allocator.h"
#include "../allocator/construct.h"
#include "../iterator/reverseIterator.h"
#include<assert.h>

namespace mySTL {
	namespace rb_tree_detail {
		typedef bool rb_tree_color_type;
		const rb_tree_color_type rb_tree_red = true;
		const rb_tree_color_type rb_tree_black = false;

		typedef int rb_tree_node_type;
		const rb_tree_node_type rb_tree_not_child = 0;
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
			node_type get_child_type() {
				if (parent == nullptr || parent->parent == this) return rb_tree_not_child;
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
					while (tmp->get_child_type() == rb_tree_rightChild) {
						tmp = tmp->parent;
					}
					node = tmp->parent;
				}
				else {
					node = rb_tree_node_base::get_min_node(node->right);
				}
			}

			void decrment() {
				if (node->left == nullptr) {
					base_ptr tmp = node;
					while (tmp->get_child_type() == rb_tree_leftChild) {
						tmp = tmp->parent;
					}            
					node = tmp->parent;
				}
				else {
					node = rb_tree_node_base::get_max_node(node->left);
				}
			}
		};

		template<class T, class Ptr, class Ref>
		struct rb_tree_iterator :public rb_tree_iterator_base {
			typedef bidrectional_iterator_tag iterator_category;
			typedef T value_type;
			typedef ptrdiff_t difference_type;
			typedef size_t size_type;
			typedef Ptr pointer;
			typedef Ref reference;
			typedef rb_tree_node<T>* node_pointer;

			rb_tree_iterator() :rb_tree_iterator_base(nullptr) {}
			rb_tree_iterator(node_pointer node) :rb_tree_iterator_base(node) {}
			rb_tree_iterator(const rb_tree_iterator& other) :rb_tree_iterator_base(other.node) {}

			reference operator*() {
				node_pointer tmp = node_pointer(node);
				return node_pointer(node)->value_field;
				//return statc_cast<node_pointer>(node).vl
			}
			pointer operator->() {
				return &(operator*());
			}
			
			rb_tree_iterator& operator++() {
				incrment();
				return *this;
			}
			rb_tree_iterator operator++(int) {
				rb_tree_iterator tmp = *this;
				incrment();
				return tmp;
			}
			rb_tree_iterator& operator--() {
				decrment();
				return *this;
			}
			rb_tree_iterator operator--(int) {
				rb_tree_iterator tmp = *this;
				decrment();
				return tmp;
			}
			bool operator==(const rb_tree_iterator<T, T*, T&>& other) const { return node == other.node; }
			bool operator!=(const rb_tree_iterator<T, T*, T&>& other) const { return node != other.node; }
			bool operator==(const rb_tree_iterator<T, const T*, const T&>& other) const { return node == other.node; }
			bool operator!=(const rb_tree_iterator<T, const T*, const T&>& other) const { return node != other.node; }
		};
	}

	template<class Value>
	inline void rb_tree_rotate_left(rb_tree_detail::rb_tree_node<Value>* node, rb_tree_detail::rb_tree_node<Value>*& root) {
		//printf("rb_tree_rotate_left\n");
		auto node_type = node->get_child_type();

		rb_tree_detail::rb_tree_node<Value>* child = static_cast<rb_tree_detail::rb_tree_node<Value>*>(node->right);
		rb_tree_detail::rb_tree_node<Value>* parent = static_cast<rb_tree_detail::rb_tree_node<Value>*>(node->parent);

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
		else if (node_type == rb_tree_detail::rb_tree_not_child) {
			parent->parent = child;
			root = child;
		}
	}

	template<class Value>
	inline void rb_tree_rotate_right(rb_tree_detail::rb_tree_node<Value>* node, rb_tree_detail::rb_tree_node<Value>*& root) {
		//printf("rb_tree_rotate_right\n");
		auto node_type = node->get_child_type();

		rb_tree_detail::rb_tree_node<Value>* child = static_cast<rb_tree_detail::rb_tree_node<Value>*>(node->left);
		rb_tree_detail::rb_tree_node<Value>* parent = static_cast<rb_tree_detail::rb_tree_node<Value>*>(node->parent);

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
		else if (node_type == rb_tree_detail::rb_tree_not_child) {
			parent->parent = child;
			root = child;
		}

	}

	template<class Value>
	inline void rb_tree_rebalance(rb_tree_detail::rb_tree_node<Value>* insertNode, rb_tree_detail::rb_tree_node<Value>*& root) {
		//printf("rb_tree_rebalance\n");
		const rb_tree_detail::rb_tree_color_type& red = rb_tree_detail::rb_tree_red;
		const rb_tree_detail::rb_tree_color_type& black = rb_tree_detail::rb_tree_black;
		insertNode->color = red;
		while (insertNode != root && insertNode->parent->color == red) {
			rb_tree_detail::rb_tree_node<Value>* parent = static_cast<rb_tree_detail::rb_tree_node<Value>*>(insertNode->parent);
			if (parent->get_child_type() == rb_tree_detail::rb_tree_leftChild) {
				rb_tree_detail::rb_tree_node<Value>* uncle = static_cast<rb_tree_detail::rb_tree_node<Value>*>(parent->parent->right);
				rb_tree_detail::rb_tree_node<Value>* grandParent = static_cast<rb_tree_detail::rb_tree_node<Value>*>(parent->parent);
				//循环往上时，可能出现uncle-color为黑的情况，因为parent的另一子节点可能为子树。
				//初始时，另一子节点为叶子节点，不可能出现uncle-color为黑
				if (uncle == nullptr || uncle->color == black) {
					rb_tree_detail::rb_tree_node<Value>* grandParent = static_cast<rb_tree_detail::rb_tree_node<Value>*>(parent->parent);
					if (insertNode->get_child_type() == rb_tree_detail::rb_tree_rightChild) {
						insertNode = parent;
						rb_tree_rotate_left(parent, root);
					}
					grandParent->color = red;
					//如果出现左旋，parent和insertNode父子关系互换了，不能直接对parent设置black
					//因此必须先判断是否左旋，再设置着色
					grandParent->left->color = black;
				
					rb_tree_rotate_right(grandParent, root);
				}
				else {
					parent->color = black;
					uncle->color = black;
					grandParent->color = red;
					insertNode = grandParent;
				}
			}
			else {
				rb_tree_detail::rb_tree_node<Value>* uncle = static_cast<rb_tree_detail::rb_tree_node<Value>*>(parent->parent->left);
				rb_tree_detail::rb_tree_node<Value>* grandParent = static_cast<rb_tree_detail::rb_tree_node<Value>*>(parent->parent);
				if (uncle == nullptr || uncle->color == black) {
					rb_tree_detail::rb_tree_node<Value>* grandParent = static_cast<rb_tree_detail::rb_tree_node<Value>*>(parent->parent);
					if (insertNode->get_child_type() == rb_tree_detail::rb_tree_leftChild) {
						insertNode = parent;
						rb_tree_rotate_right(parent, root);
					}
					grandParent->color = red;
					grandParent->right->color = black;
					rb_tree_rotate_left(grandParent, root);
				}
				else {
					parent->color = black;
					uncle->color = black;
					grandParent->color = red;
					insertNode = grandParent;
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
			typedef rb_tree_detail::rb_tree_iterator<value_type, pointer, reference> iterator;
			typedef rb_tree_detail::rb_tree_iterator<value_type, const_pointer, const_reference> const_iterator;
			typedef mySTL::reverse_iterator<iterator> reverse_iterator;
		protected:
			node_pointer header;
			size_type node_num;
			Compare key_compare;

			node_pointer& root() const { return reinterpret_cast<node_pointer&>(header->parent); }
			node_pointer& leftmost() const { return reinterpret_cast<node_pointer&>(header->left); }
			node_pointer& rightmost() const { return reinterpret_cast<node_pointer&>(header->right); }

			static node_pointer& left(node_pointer node) { return reinterpret_cast<node_pointer&>(node->left); }
			static node_pointer& right(node_pointer node) { return reinterpret_cast<node_pointer&>(node->right); }
			static node_pointer& parent(node_pointer node) { return reinterpret_cast<node_pointer&>(node->parent); }
			static reference value(node_pointer node) { return node->value_field; }
			static const Key& key(node_pointer node) { return KeyOfValue()(value(node)); }
			static color_type& color(node_pointer node) { return node->color; }

			static node_pointer& left(base_ptr node) { return reinterpret_cast<node_pointer&>(node->left); }
			static node_pointer& right(base_ptr node) { return reinterpret_cast<node_pointer&>(node->right); }
			static node_pointer& parent(base_ptr node) { return reinterpret_cast<node_pointer&>(node->parent); }
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

			void erase_node(node_pointer node);
			void rb_tree_erase_rebalance(node_pointer node);

			bool equal(iterator first, iterator last, iterator pos) const;

			bool isBalance_aux(node_pointer node, int count, int num);
			bool isBST(node_pointer node);
			void print_aux(node_pointer node, int deep);
		public:
			rb_tree() :node_num(0), key_compare() { initialize(); }
			rb_tree(const Compare& comp) :node_num(0), key_compare(comp) { initialize(); }
			rb_tree(const rb_tree& other) {
				initialize();
				if (other.root() != nullptr) {
					key_compare = other.key_compare;
					node_num = other.node_num;
					root() = copy_aux(other.root());
					root()->parent = header;
					leftmost() = get_min_node(root());
					rightmost() = get_max_node(root());
				}
			}
			rb_tree& operator=(const rb_tree& other) {
				if (this != &other) {
					clear();
					if (other.root() != nullptr) {
						key_compare = other.key_compare;
						node_num = other.node_num;
						root() = copy_aux(other.root());
						root()->parent = header;
						leftmost() = get_min_node(root());
						rightmost() = get_max_node(root());
					}
				}
				return *this;
			}
			~rb_tree() { 
				clear();
				put_node(header);
			}

			void clear() {
				if (node_num > 0) {
					erase_aux(root());
					leftmost() = header;
					rightmost() = header;
					root() = nullptr;
					node_num = 0;
				}
			}

			Compare key_comp() const { return key_compare; }
			iterator begin() { return leftmost(); }
			const_iterator begin() const { return const_iterator(leftmost()); }
			iterator end() { return header; }
			const_iterator end() const { return const_iterator(header); }
			reverse_iterator rbegin() { return reverse_iterator(end()); }
			reverse_iterator rend() { return reverse_iterator(begin()); }

			bool empty() const { return node_num == 0; }
			size_type size() const { return node_num; }

			iterator find(const key_type& k);
			const_iterator find(const key_type& k) const;

			size_type count(const key_type& k) const;

			iterator lower_bound(const key_type& k);
			const_iterator lower_bound(const key_type& k) const;

			iterator upper_bound(const key_type& k);
			const_iterator upper_bound(const key_type& k) const;

			void erase(iterator position);
			void erase(iterator first, iterator last);
			size_type erase(const key_type& k) {
				std::pair<iterator, iterator> p = equal_range(k);
				size_type count = 0;
				for (auto it = p.first; it != p.second; it++) {
					count++;
				}
				erase(p.first, p.second);
				return count;
			}

			std::pair<iterator, iterator> equal_range(const key_type& k);
			std::pair<const_iterator, const_iterator> equal_range(const key_type& k) const;

			void swap(rb_tree& other) {
				std::swap(header, other.header);
				std::swap(node_num, other.node_num);
				std::swap(key_compare, other.key_compare);
			}

			iterator insert_unique(iterator position, const value_type& value);
			std::pair<iterator, bool> insert_unique(const value_type& value);
			template<class InputIterator>
			void insert_unique(InputIterator first, InputIterator last);

			iterator insert_equal(const value_type& value);
			iterator insert_equal(iterator position, const value_type& value);
			template<class InputIterator>
			void insert_equal(InputIterator first, InputIterator last);

			bool isBalance();

			void print();

			template<class Key, class Value, class KeyOfValue, class Compare, class Alloc>
			friend bool operator==(const rb_tree<Key, Value, KeyOfValue, Compare, Alloc>& lhs, const rb_tree<Key, Value, KeyOfValue, Compare, Alloc>& rhs);
	};
	template<class Key, class Value, class KeyOfValue, class Compare, class Alloc>
	bool rb_tree<Key, Value, KeyOfValue, Compare, Alloc>::equal(iterator first, iterator last, iterator pos) const {
		while (first != last) {
			if (*first++ != *pos++) return false;
		}
		return true;
	}

	template<class Key, class Value, class KeyOfValue, class Compare, class Alloc>
	bool operator==(const rb_tree<Key, Value, KeyOfValue, Compare, Alloc>& lhs, const rb_tree<Key, Value, KeyOfValue, Compare, Alloc>& rhs) {
		return lhs.size() == rhs.size() && equal(lhs.begin(), lhs.end(), rhs.begin());
	}


	template<class Key, class Value, class KeyOfValue, class Compare, class Alloc>
	void rb_tree<Key, Value, KeyOfValue, Compare, Alloc>::erase_aux(node_pointer node) {
		while (node != nullptr) {
			erase_aux(right(node));
			node_pointer leftNode = left(node);
			destroy_node(node);
			node = leftNode;
		}
	}

	template<class Key, class Value, class KeyOfValue, class Compare, class Alloc>
	typename rb_tree<Key, Value, KeyOfValue, Compare, Alloc>::node_pointer rb_tree<Key, Value, KeyOfValue, Compare, Alloc>::copy_aux(node_pointer node) {
		if (node == nullptr) return nullptr;
		node_pointer p = clone_node(node);
		left(p) = copy_aux(left(node));
		if (p->left != nullptr) parent(p->left) = p;
		right(p) = copy_aux(right(node));
		if (p->right != nullptr) parent(p->right) = p;
		return p;
	}

	template<class Key, class Value, class KeyOfValue, class Compare, class Alloc>
	typename rb_tree<Key, Value, KeyOfValue, Compare, Alloc>::iterator rb_tree<Key, Value, KeyOfValue, Compare, Alloc>::insert_aux(base_ptr x, base_ptr y, const value_type& value) {
		assert(x == nullptr);
		node_pointer y_ = static_cast<node_pointer>(y);

		node_pointer new_node = create_node(value);
		if (y_ == header || key_compare(KeyOfValue()(value), key(y_))) {
			left(y_) = new_node;
			if (y_ == header) {
				root() = new_node;
				rightmost() = new_node;
			}
			else if (y_ == leftmost()) {
				leftmost() = new_node;
			}
		}
		else {
			right(y_) = new_node;
			if (y_ == rightmost()) {
				rightmost() = new_node;
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
	std::pair<typename rb_tree<Key, Value, KeyOfValue, Compare, Alloc>::iterator, bool> rb_tree<Key, Value, KeyOfValue, Compare, Alloc>::insert_unique(const value_type& value){
		node_pointer parent = header;
		node_pointer node = root();

		bool isLessParent = true;

		//查询到最底叶子节点
		while (node != nullptr) {
			parent = node;
			isLessParent = key_compare(KeyOfValue()(value), key(node));
			node = isLessParent ? left(node) : right(node);
		}

		iterator it = iterator(parent);
		if (isLessParent) {
			if (it == begin()) {
				return std::pair<iterator, bool>(insert_aux(node, parent, value), true);
			}
			else
				//判断parent上面有没有与value相等key的节点
				it--;
		}
		if (key_compare(key(it.node), KeyOfValue()(value)))
			return std::pair<iterator, bool>(insert_aux(node, parent, value), true);
		else
			//printf("insert failed\n");
			return std::pair<iterator, bool>(it, false);
	}

	template<class Key, class Value, class KeyOfValue, class Compare, class Alloc>
	typename rb_tree<Key, Value, KeyOfValue, Compare, Alloc>::iterator rb_tree<Key, Value, KeyOfValue, Compare, Alloc>::insert_unique(iterator position, const value_type& value) {
		if (position.node == header->left) {
			if (size() > 0 && key_compare(KeyOfValue()(value), key(position.node)))
				return insert_aux(nullptr, position.node, value);
			else
				return insert_unique(value).first;
		}
		else if (position.node == header) {
			if (key_compare(key(rightmost()), KeyOfValue()(value)))
				return insert_aux(nullptr, rightmost(), value);
			else
				return insert_unique(value).first;
		}
		else {
			iterator it = position;
			it--;
			if (key_compare(key(position.node), KeyOfValue()(value)) && key_compare(KeyOfValue()(value), key(position.node))) {
				if (right(it.node) == nullptr)
					return insert_aux(nullptr, it.node, value);
				else if (left(position.node) == nullptr)
					return insert_aux(nullptr, position.node, value);
			}else
				return insert_unique(value).first;
		}
	}

	template<class Key, class Value, class KeyOfValue, class Compare, class Alloc>
	template<class InputIterator>
	void rb_tree<Key, Value, KeyOfValue, Compare, Alloc>::insert_unique(InputIterator first, InputIterator last) {
		while (first != last) {
			insert_unique(*first++);
		}
	}

	template<class Key, class Value, class KeyOfValue, class Compare, class Alloc>
	typename rb_tree<Key, Value, KeyOfValue, Compare, Alloc>::iterator rb_tree<Key, Value, KeyOfValue, Compare, Alloc>::insert_equal(const value_type& value) {
		node_pointer parent = header;
		node_pointer node = root();

		while (node != nullptr) {
			parent = node;
			node = key_compare(KeyOfValue()(value), key(node)) ? left(node) : right(node);
		}
		return insert_aux(node, parent, value);
	}

	template<class Key, class Value, class KeyOfValue, class Compare, class Alloc>
	typename rb_tree<Key, Value, KeyOfValue, Compare, Alloc>::iterator rb_tree<Key, Value, KeyOfValue, Compare, Alloc>::insert_equal(iterator position, const value_type& value) {
		if (position.node == header->left) {
			if (size() > 0 && key_compare(KeyOfValue()(value), key(position.node)))
				return insert_aux(nullptr, position.node, value);
			else
				return insert_equal(value);
		}
		else if (position.node == header) {
			if (!key_compare(insert_equal, key(rightmost())))
				return insert_aux(nullptr, rightmost(), value);
			else
				return insert_equal(value);
		}
		else {
			iterator it = position;
			it--;
			if (!key_compare(KeyOfValue(value), key(it.node)) && !key_compare(key(position.node), KeyOfValue(value))) {
				if (right(it.node) == nullptr)
					return insert_aux(nullptr, it.node, value);
				else if (left(position.node) == nullptr)
					return insert_aux(nullptr, position.node, value);
			}
		}
		return insert_equal(value);
	}

	template<class Key, class Value, class KeyOfValue, class Compare, class Alloc>
	template<class InputIterator>
	void rb_tree<Key, Value, KeyOfValue, Compare, Alloc>::insert_equal(InputIterator first, InputIterator last) {
		while (first != last) {
			insert_equal(*first++);
		}
	}

	template<class Key, class Value, class KeyOfValue, class Compare, class Alloc>
	typename rb_tree<Key, Value, KeyOfValue, Compare, Alloc>::iterator rb_tree<Key, Value, KeyOfValue, Compare, Alloc>::find(const key_type& k) {
		node_pointer node = root();
		while (node != nullptr) {
			if (key_compare(k, key(node))) {
				node = left(node);
			}
			else if (key_compare(key(node), k)) {
				node = right(node);
			}
			else {
				return iterator(node);
			}
		}
		return end();
	}

	template<class Key, class Value, class KeyOfValue, class Compare, class Alloc>
	typename rb_tree<Key, Value, KeyOfValue, Compare, Alloc>::const_iterator rb_tree<Key, Value, KeyOfValue, Compare, Alloc>::find(const key_type& k) const {
		node_pointer node = root();
		while (node != nullptr) {
			if (key_compare(k, key(node))) {
				node = left(node);
			}
			else if (key_compare(key(node), k)) {
				node = right(node);
			}
			else {
				return const_iterator(node);
			}
		}
		return end();
	}

	template<class Key, class Value, class KeyOfValue, class Compare, class Alloc>
	typename rb_tree<Key, Value, KeyOfValue, Compare, Alloc>::iterator rb_tree<Key, Value, KeyOfValue, Compare, Alloc>::lower_bound(const key_type& k) {
		node_pointer node = root();
		node_pointer ans = header;
		while (node != nullptr) {
			if (key_compare(key(node), k)) {
				node = right(node);
			}
			else {
				ans = node;
				node = left(node);
			}
		}
		return iterator(ans);
	}

	template<class Key, class Value, class KeyOfValue, class Compare, class Alloc>
	typename rb_tree<Key, Value, KeyOfValue, Compare, Alloc>::const_iterator rb_tree<Key, Value, KeyOfValue, Compare, Alloc>::lower_bound(const key_type& k) const {
		node_pointer node = root();
		node_pointer ans = header;
		while (node != nullptr) {
			if (key_compare(key(node), k)) {
				node = right(node);
			}
			else {
				ans = node;
				node = left(node);
			}
		}
		return const_iterator(ans);
	}

	template<class Key, class Value, class KeyOfValue, class Compare, class Alloc>
	typename rb_tree<Key, Value, KeyOfValue, Compare, Alloc>::iterator rb_tree<Key, Value, KeyOfValue, Compare, Alloc>::upper_bound(const key_type& k) {
		node_pointer node = root();
		node_pointer ans = header;
		while (node != nullptr) {
			if (key_compare(k, key(node))) {
				ans = node;
				node = left(node);
			}
			else {
				node = right(node);
			}
		}
		return iterator(ans);
	}

	template<class Key, class Value, class KeyOfValue, class Compare, class Alloc>
	typename rb_tree<Key, Value, KeyOfValue, Compare, Alloc>::const_iterator rb_tree<Key, Value, KeyOfValue, Compare, Alloc>::upper_bound(const key_type& k) const {
		node_pointer node = root();
		node_pointer ans = header;
		while (node != nullptr) {
			if (key_compare(k, key(node))) {
				ans = node;
				node = left(node);
			}
			else {
				node = right(node);
			}
		}
		return const_iterator(ans);
	}

	template<class Key, class Value, class KeyOfValue, class Compare, class Alloc>
	inline std::pair<typename rb_tree<Key, Value, KeyOfValue, Compare, Alloc>::iterator,
		typename rb_tree<Key, Value, KeyOfValue, Compare, Alloc>::iterator>
		rb_tree<Key, Value, KeyOfValue, Compare, Alloc>::equal_range(const key_type& k) {
		return std::pair<iterator, iterator>(lower_bound(k), upper_bound(k));
	}

	template<class Key, class Value, class KeyOfValue, class Compare, class Alloc>
	inline std::pair<typename rb_tree<Key, Value, KeyOfValue, Compare, Alloc>::const_iterator,
		typename rb_tree<Key, Value, KeyOfValue, Compare, Alloc>::const_iterator>
		rb_tree<Key, Value, KeyOfValue, Compare, Alloc>::equal_range(const key_type& k) const {
		return std::pair<const_iterator, const_iterator>(lower_bound(k), upper_bound(k));
	}

	template<class Key, class Value, class KeyOfValue, class Compare, class Alloc>
	typename rb_tree<Key, Value, KeyOfValue, Compare, Alloc>::size_type rb_tree<Key, Value, KeyOfValue, Compare, Alloc>::count(const key_type& k) const {
		size_type num = 0;
		std::pair<const_iterator, const_iterator> p = equal_range(k);
		for (auto it = p.first; it != p.second; it++) {
			num++;
		}
		return num;
	}

	template<class Key, class Value, class KeyOfValue, class Compare, class Alloc>
	inline void rb_tree<Key, Value, KeyOfValue, Compare, Alloc>::erase(iterator position) {
		node_pointer n = static_cast<node_pointer>(position.node);
		erase_node(static_cast<node_pointer>(position.node));
	}

	template<class Key, class Value, class KeyOfValue, class Compare, class Alloc>
	inline void rb_tree<Key, Value, KeyOfValue, Compare, Alloc>::erase(iterator first, iterator last) {
		if (first == begin() && last == end()) {
			clear();
		}
		else {
			while (first != last) {
				erase(first++);
			}
		}
	}

	template<class Key, class Value, class KeyOfValue, class Compare, class Alloc>
	void rb_tree<Key, Value, KeyOfValue, Compare, Alloc>::erase_node(node_pointer node) {
		if (node->left == nullptr && node->right == nullptr) {
			if (node->color == rb_tree_detail::rb_tree_red) {
				if (leftmost() == node) 
					leftmost() = static_cast<node_pointer>(node->parent);
				else if (rightmost() == node)
					rightmost() = static_cast<node_pointer>(node->parent);
			}
			else {
				if (root() == node) {
					root() == nullptr;
					leftmost() = header;
					rightmost() = header;
				}
				else {
					rb_tree_erase_rebalance(node);
					if (leftmost() == node)
						leftmost() = static_cast<node_pointer>(node->parent);
					else if (rightmost() == node)
						rightmost() = static_cast<node_pointer>(node->parent);
				}
			}
			if (node->get_child_type() == rb_tree_detail::rb_tree_leftChild) 
				node->parent->left = nullptr;
			else if(node->get_child_type() == rb_tree_detail::rb_tree_rightChild) 
				node->parent->right = nullptr;
			destroy_node(node);
			node_num--;
		}
		else if (node->left != nullptr && node->right != nullptr) {
			auto nodeType = node->get_child_type();
			auto nodeParent = node->parent;
			node_pointer preNode = get_max_node(left(node));
			if (node->left == preNode) {
				node->left = preNode->left;
				if (preNode->left != nullptr)
					preNode->left->parent = node;
				preNode->left = node;
				node->parent = preNode;

				preNode->right = node->right;
				node->right->parent = preNode;
				node->right = nullptr;
				if (nodeType == rb_tree_detail::rb_tree_leftChild)
					nodeParent->left = preNode;
				else if (nodeType == rb_tree_detail::rb_tree_rightChild)
					nodeParent->right = preNode;
				else
					root() = preNode;
				preNode->parent = nodeParent;
				if (leftmost() == preNode)
					leftmost() = node;
			}
			else {
				auto nodeLeft = node->left;
				auto nodeRight = node->right;

				auto preNodeType = preNode->get_child_type();
				auto preNodeParent = preNode->parent;
				auto preNodeLeft = preNode->left;

				node->parent = preNodeParent;
				if (preNodeType == rb_tree_detail::rb_tree_leftChild)
					preNodeParent->left = node;
				else
					preNodeParent->right = node;
				node->left = preNodeLeft;
				if (preNodeLeft != nullptr)
					preNodeLeft->parent = node;
				node->right = nullptr;

				preNode->parent = nodeParent;
				if (nodeType == rb_tree_detail::rb_tree_leftChild)
					nodeParent->left = preNode;
				else if (nodeType == rb_tree_detail::rb_tree_rightChild)
					nodeParent->right = preNode;
				else
					root() = preNode;
				preNode->left = nodeLeft;
				nodeLeft->parent = preNode;
				preNode->right = nodeRight;
				nodeRight->parent = preNode;
			}

			std::swap(node->color, preNode->color);

			node_pointer r = root();
			int i = 0;

			erase_node(node);
		}
		else {
			node_pointer child = node->left != nullptr ? child = left(node) : child = right(node);
			child->parent = node->parent;
			if (node->get_child_type() == rb_tree_detail::rb_tree_leftChild) node->parent->left = child;
			else node->parent->right = child;
			child->color = rb_tree_detail::rb_tree_black;
			if (root() == node)
				root() = child;
			if (leftmost() == node)
				leftmost() = child;
			else if (rightmost() == node)
				rightmost() = child;
			destroy_node(node);
			node_num--;
		}
	}

	template<class Key, class Value, class KeyOfValue, class Compare, class Alloc>
	void rb_tree<Key, Value, KeyOfValue, Compare, Alloc>::rb_tree_erase_rebalance(node_pointer balacnNode) {
		if (balacnNode == root()) {
			balacnNode->color = rb_tree_detail::rb_tree_black;
			return;
		}

		node_pointer parentNode = parent(balacnNode);
		if (balacnNode->get_child_type() == rb_tree_detail::rb_tree_leftChild) {
			node_pointer brother = right(parentNode);
			if (brother->color == rb_tree_detail::rb_tree_black) {
				if (brother->right != nullptr && brother->right->color == rb_tree_detail::rb_tree_red) {
					brother->color = parentNode->color;
					parentNode->color = rb_tree_detail::rb_tree_black;
					brother->right->color = rb_tree_detail::rb_tree_black;
					rb_tree_rotate_left(parentNode, root());
				}
				else if (brother->left != nullptr && brother->left->color == rb_tree_detail::rb_tree_red) {
					brother->left->color = parentNode->color;
					parentNode->color = rb_tree_detail::rb_tree_black;
					rb_tree_rotate_right(brother, root());
					rb_tree_rotate_left(parentNode, root());
				}
				else {
					if (parentNode->color == rb_tree_detail::rb_tree_red) {
						brother->color = rb_tree_detail::rb_tree_red;
						parentNode->color = rb_tree_detail::rb_tree_black;
					}
					else {
						brother->color = rb_tree_detail::rb_tree_red;
						rb_tree_erase_rebalance(parentNode);
					}
				}
			}
			else {
				assert(brother->left->color == rb_tree_detail::rb_tree_black);
				assert(brother->right->color == rb_tree_detail::rb_tree_black);
				assert(parentNode->color == rb_tree_detail::rb_tree_black);

				parentNode->color = rb_tree_detail::rb_tree_red;
				brother->color = rb_tree_detail::rb_tree_black;
				rb_tree_rotate_left(parentNode, root());
				rb_tree_erase_rebalance(balacnNode);
			}
		}
		else {
			node_pointer brother = left(parentNode);
			if (brother->color == rb_tree_detail::rb_tree_black) {
				if (brother->left != nullptr && brother->left->color == rb_tree_detail::rb_tree_red) {
					brother->color = parentNode->color;
					parentNode->color = rb_tree_detail::rb_tree_black;
					brother->left->color = rb_tree_detail::rb_tree_black;
					rb_tree_rotate_right(parentNode, root());
				}
				else if (brother->right != nullptr && brother->right->color == rb_tree_detail::rb_tree_red) {
					brother->right->color = parentNode->color;
					parentNode->color = rb_tree_detail::rb_tree_black;
					rb_tree_rotate_left(brother, root());
					rb_tree_rotate_right(parentNode, root());
				}
				else {
					if (parentNode->color == rb_tree_detail::rb_tree_red) {
						brother->color = rb_tree_detail::rb_tree_red;
						parentNode->color = rb_tree_detail::rb_tree_black;
					}
					else {
						brother->color = rb_tree_detail::rb_tree_red;
						rb_tree_erase_rebalance(parentNode);
					}
				}
			}
			else {
				assert(brother->left->color == rb_tree_detail::rb_tree_black);
				assert(brother->right->color == rb_tree_detail::rb_tree_black);
				assert(parentNode->color == rb_tree_detail::rb_tree_black);
				
				parentNode->color = rb_tree_detail::rb_tree_red;
				brother->color = rb_tree_detail::rb_tree_black;
				rb_tree_rotate_right(parentNode, root());
				rb_tree_erase_rebalance(balacnNode);
			}
		}
	}

	template<class Key, class Value, class KeyOfValue, class Compare, class Alloc>
	bool rb_tree<Key, Value, KeyOfValue, Compare, Alloc>::isBalance() {
		if (root() == nullptr) return true;
		if (root()->color == rb_tree_detail::rb_tree_red) return false;
		if (!isBST(root())) return false;

		int count = 0;
		
		node_pointer cur = root();
		while (cur != nullptr) {
			if (cur->color == rb_tree_detail::rb_tree_black)
				count++;
			cur = left(cur);
		}
		int num = 0;
		return isBalance_aux(root(), count, num);
	}

	template<class Key, class Value, class KeyOfValue, class Compare, class Alloc>
	bool rb_tree<Key, Value, KeyOfValue, Compare, Alloc>::isBalance_aux(node_pointer node, int count, int num) {
		if (node == nullptr) return true;
		if (node->color == rb_tree_detail::rb_tree_red && node->parent->color == rb_tree_detail::rb_tree_red) 
			return false;
		if (node->color == rb_tree_detail::rb_tree_black) num++;
		if (node->left == nullptr && node->right == nullptr) {
			if (num != count)
				return false;
		}
		return isBalance_aux(left(node), count, num) && isBalance_aux(right(node), count, num);
	}

	template<class Key, class Value, class KeyOfValue, class Compare, class Alloc>
	bool rb_tree<Key, Value, KeyOfValue, Compare, Alloc>::isBST(node_pointer node) {
		if (node == nullptr) return true;
		if (left(node) != nullptr) {
			auto n = get_max_node(left(node));
			if (key_compare(key(node), key(n))) 
				return false;
		}
		if (right(node) != nullptr) {
			auto n = get_min_node(right(node));
			if (key_compare(key(n), key(node))) 
				return false;
		}
		if (!isBST(left(node)) || !isBST(right(node)))
			return false;
		return true;
	}

	template<class Key, class Value, class KeyOfValue, class Compare, class Alloc>
	void rb_tree<Key, Value, KeyOfValue, Compare, Alloc>::print() {
		node_pointer cur = root();
		if (cur == nullptr)
			return;
		std::cout << " deep 0 " << cur->value_field << std::endl;
		print_aux(cur, 1);
	}

	template<class Key, class Value, class KeyOfValue, class Compare, class Alloc>
	void rb_tree<Key, Value, KeyOfValue, Compare, Alloc>::print_aux(node_pointer node, int deep) {
		if (node == nullptr) return;
		if (node->left != nullptr)
			std::cout << " deep " << deep << " " << left(node)->value_field << (left(node)->color == true? " r": " b" )<< std::endl;
		if (node->right != nullptr)
			std::cout<< " deep " << deep << " " << right(node)->value_field << (right(node)->color == true ? " r" : " b") << std::endl;
		print_aux(left(node), deep + 1);
		print_aux(right(node), deep + 1);
		
	}
}

#endif // !_RB_TREE_H_

