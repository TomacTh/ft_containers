#ifndef FT_RB_TREE_H
# define FT_RB_TREE_H

# include <algorithm>
# include <iterator>
# include <memory>
# include "ft_iterator.hpp"
# include "ft_pair.hpp"
# include "ft_utilities.hpp"

namespace ft
{
	enum Rb_tree_color{ red = false, black = true};

	template<typename Value>
	struct Rb_tree_node
	{
		typedef Rb_tree_node*		node_ptr;
		typedef const Rb_tree_node*	const_node_ptr;

		node_ptr		parent;
		node_ptr		left;
		node_ptr		right;
		Value			value;
		Rb_tree_color	color;

		static node_ptr	minimum(node_ptr x)
		{
			while (x->left != 0)
				x = x->left;
			return (x);
		}

		static node_ptr	maximum(node_ptr x)
		{
			while (x->right != 0)
				x = x->right;
			return (x);
		}

		static const_node_ptr	minimum(const_node_ptr x)
		{
			while (x->left != 0)
				x = x->left;
			return (x);
		}

		static const_node_ptr	maximum(const_node_ptr x)
		{
			while (x->right != 0)
				x = x->right;
			return (x);
		}
	};

	template<typename T>
	Rb_tree_node<T>	*node_increment(Rb_tree_node<T> *x)
	{
		if (x->right != 0)
		{
			x = x->right;
			while (x->left != 0)
				x = x->left;
		}
		else
		{
			Rb_tree_node<T>	*y = x->parent;

			while (x == y->right)
			{
				x = y;
				y = y->parent;
			}
			if (x->right != y)
				x = y;
		}
		return x;
	}

	template<typename T>
	Rb_tree_node<T>	*node_decrement(Rb_tree_node<T> *x)
	{
		if (x->color == ft::red && x->parent->parent == x)
			x = x->right;
		else if (x->left != 0)
		{
			x = x->left;
			while (x->right != 0)
				x = x->right;
		}
		else
		{
			Rb_tree_node<T>	*y;

			y = x->parent;
			while (x == y->left)
			{
				x = y;
				y = y->parent;
			}
			x = y;
		}
		return x;
	}

	template<typename T>
	const Rb_tree_node<T>	*node_decrement(const Rb_tree_node<T> *x)
	{ return (node_decrement(const_cast<Rb_tree_node<T>* >(x))); }

	template<typename T>
	const Rb_tree_node<T>	*node_increment(const Rb_tree_node<T> *x)
	{ return (node_increment(const_cast<Rb_tree_node<T>* >(x))); }

	template<typename T>
	struct	Rb_tree_iterator
	{
		typedef	T	value_type;
		typedef T&	reference;
		typedef T*	pointer;

		typedef std::bidirectional_iterator_tag	iterator_category;
		typedef ptrdiff_t						difference_type;

		typedef Rb_tree_iterator<T>					self;
		typedef typename Rb_tree_node<T>::node_ptr	node_ptr;

		node_ptr	node;

		Rb_tree_iterator() : node(){}

		explicit Rb_tree_iterator(node_ptr x) : node(x){}

		reference	operator*() const
		{ return (node->value); }

		pointer	operator->() const
		{ return &(node->value); }

		self	&operator++()
		{
			node = node_increment(node);
			return (*this);
		}

		self	operator++(int)
		{
			self tmp = *this;
			node = node_increment(node);
			return (tmp);
		}

		self	&operator--()
		{
			node = node_decrement(node);
			return (*this);
		}

		self	operator--(int)
		{
			self tmp = *this;
			node = node_decrement(node);
			return (tmp);
		}

		bool	operator==(const self	&x) const
		{ return (node == x.node); }

		bool	operator!=(const self	&x) const
		{ return (node != x.node); }
	};

	template<typename T>
	struct	Const_rb_tree_iterator
	{
		typedef	T			value_type;
		typedef const T*	pointer;
		typedef const T&	reference;

		typedef	Rb_tree_iterator<T> iterator;

		typedef std::bidirectional_iterator_tag	iterator_category;
		typedef ptrdiff_t						difference_type;

		typedef Const_rb_tree_iterator<T>					self;
		typedef typename Rb_tree_node<T>::const_node_ptr	node_ptr;

		node_ptr	node;

		Const_rb_tree_iterator() : node(){}

		explicit Const_rb_tree_iterator(node_ptr x) : node(x){}

		Const_rb_tree_iterator(const iterator &it) : node(it.node){}

		iterator	it_const_cast() const
		{ return iterator(const_cast<typename iterator::node_ptr>(node)); }

		reference	operator*() const
		{ return (node->value); }

		pointer	operator->() const
		{ return &(node->value); }

		self	&operator++()
		{
			node = node_increment(node);
			return (*this);
		}

		self	operator++(int)
		{
			self tmp = *this;
			node = node_increment(node);
			return (tmp);
		}

		self	&operator--()
		{
			node = node_decrement(node);
			return (*this);
		}

		self	operator--(int)
		{
			self tmp = *this;
			node = node_decrement(node);
			return (tmp);
		}

		bool	operator==(const self	&x) const
		{ return (node == x.node); }

		bool	operator!=(const self	&x) const
		{ return (node != x.node); }
	};

	template<typename Key, typename Value, typename KeyOfValue, typename Compare, typename Alloc = std::allocator<Value> >
	class Rb_tree
	{
		public:
			typedef Rb_tree_node<Value>						node_struct;
			typedef typename node_struct::node_ptr			node_ptr;
			typedef typename node_struct::const_node_ptr	const_node_ptr;

			typedef Key				key_type;
			typedef	Value			value_type;
			typedef Compare			key_compare;
			typedef	Value*			pointer;
			typedef const Value*	const_pointer;
			typedef Value&			reference;
			typedef const Value&	const_reference;
			typedef size_t			size_type;
			typedef ptrdiff_t		difference_type;

			typedef Alloc															allocator_type;
			typedef typename Alloc::template rebind<Rb_tree_node<Value> >::other	node_allocator;

			typedef	Rb_tree_iterator<value_type>		iterator;
			typedef Const_rb_tree_iterator<value_type>	const_iterator;

			typedef ft::reverse_iterator<iterator>			reverse_iterator;
			typedef ft::reverse_iterator<const_iterator>	const_reverse_iterator;

		private:
			node_allocator	_node_alloc;
			key_compare		_comp;
			node_struct		_header;
			size_type		_node_count;

			void	_initialize_header()
			{
				_header.color = ft::red;
				_header.parent = 0;
				_header.left = &_header;
				_header.right = &_header;
			}

			node_ptr	_allocate_node()
			{ return (_node_alloc.allocate(1)); }

			node_ptr	_create_node(const value_type &val)
			{
				node_ptr tmp = _allocate_node();

				get_allocator().construct(&(tmp->value), val);
				return (tmp);
			}

			void	_deallocate_node(node_ptr node)
			{
				_node_alloc.deallocate(node, 1);
			}

			void	_destroy_node(node_ptr node)
			{
				get_allocator().destroy(&(node->value));
				_deallocate_node(node);
			}

			node_ptr	_clone_node(const_node_ptr	node)
			{
				node_ptr tmp = _create_node(node->value);

				tmp->color = node->color;
				tmp->left = 0;
				tmp->right = 0;
				return (tmp);
			}

			node_ptr	&_root()
			{ return (_header.parent); }

			const_node_ptr	_root() const
			{ return (_header.parent); }

			node_ptr	&_rightmost()
			{ return (_header.right); }

			const_node_ptr	_rightmost() const
			{ return (_header.right); }

			node_ptr	&_leftmost()
			{ return (_header.left); }

			const_node_ptr	_leftmost() const
			{ return (_header.left); }

			node_ptr	_end()
			{ return (&_header); }

			const_node_ptr	_end() const
			{ return (&_header); }

			static const Key	&_key(node_ptr x)
			{ return KeyOfValue()(x->value); }

			static const Key	&_key(const_node_ptr x)
			{ return KeyOfValue()(x->value); }

			node_ptr	_copy(const_node_ptr node_src, node_ptr parent)
			{
				node_ptr top = _clone_node(node_src);

				top->parent = parent;
				if (node_src->right)
					top->right = _copy(node_src->right, top);
				parent = top;
				node_src = node_src->left;
				while (node_src != 0)
				{
					node_ptr y = _clone_node(node_src);

					parent->left = y;
					y->parent = parent;
					if (node_src->right)
						y->right = _copy(node_src->right, y);
					parent = y;
					node_src = node_src->left;
				}
				return top;
			}

			iterator	_insert(const_node_ptr x, const_node_ptr p, const value_type &val)
			{
				bool insert_left = (x != 0 || p == _end() || _comp(KeyOfValue()(val), _key(p)));
				node_ptr node = _create_node(val);

				_insert_and_rebalance(insert_left, node, const_cast<node_ptr>(p));
				++_node_count;
				return iterator(node);
			}

			void	_rotate_left(node_ptr const x, node_ptr &root)
			{
				node_ptr const y = x->right;

				x->right = y->left;
				if(y->left != 0)
					y->left->parent = x;
				y->parent = x->parent;
				if (x == root)
					root = y;
				else if (x == x->parent->left)
					x->parent->left = y;
				else
					x->parent->right = y;
				y->left = x;
				x->parent = y;
			}

			void	_rotate_right(node_ptr const x, node_ptr &root)
			{
				node_ptr const y = x->left;

				x->left = y->right;
				if (y->right != 0)
					y->right->parent = x;
				y->parent = x->parent;
				if (x == root)
					root = y;
				else if (x->parent->left == x)
					x->parent->left = y;
				else
					x->parent->right = y;
				y->right = x;
				x->parent = y;
			}

			void	_colorflip(node_ptr &x, node_ptr const x_uncle, node_ptr const x_gp)
			{
				x->parent->color = ft::black;
				x_uncle->color = ft::black;
				x_gp->color = ft::red;
				x = x_gp;
			}

			void	_insert_and_rebalance(const bool insert_left, node_ptr x, node_ptr p)
			{
				node_ptr	&root = _root();

				//Init node
				x->parent = p;
				x->left = 0;
				x->right = 0;
				x->color = ft::red;
				//Insert
				if (insert_left)
				{
					p->left = x;
					if (p == &_header)
					{
						_header.parent = x;
						_header.right = x;
					}
					else if (p == _header.left)
						_header.left = x;
				}
				else
				{
					p->right = x;
					if (p == _header.right)
						_header.right = x;
				}
				//Rebalance
				while (x != root && x->parent->color == ft::red)
				{
					node_ptr const x_gp =  x->parent->parent;
					if (x->parent == x_gp->left) //if xparent is leftchild
					{
						node_ptr const x_uncle = x_gp->right;

						if (x_uncle && x_uncle->color == ft::red)
							_colorflip(x, x_uncle, x_gp);
						else
						{
							if (x == x->parent->right)
							{
								x = x->parent;
								_rotate_left(x, root);
							}
							x->parent->color = ft::black; //uncle is already black
							x_gp->color = ft::red;
							_rotate_right(x_gp, root);
						}
					}
					else // xparent is rightchild
					{
						node_ptr const x_uncle = x_gp->left;

						if (x_uncle && x_uncle->color == ft::red)
							_colorflip(x, x_uncle, x_gp);
						else
						{
							if (x == x->parent->left)
							{
								x = x->parent;
								_rotate_right(x, root);
							}
							x->parent->color = ft::black; //uncle is already black
							x_gp->color = ft::red;
							_rotate_left(x_gp, root);
						}
					}
				}
				root->color = ft::black;
			}

			bool		_is_black(node_ptr x)
			{ return (x == 0 || x->color == ft::black); }

			node_ptr	_rebalance_for_erase(node_ptr const z)
			{
				node_ptr &root = _root();
				node_ptr &leftmost = _header.left;
				node_ptr &rightmost = _header.right;
				node_ptr y = z; //el to navigate;
				node_ptr x = 0; //to find child
				node_ptr x_parent = 0;

				if (y->left == 0)
					x = y->right;
				else
				{
					if (y->right == 0)
						x = y->left;
					else
					{
						y = y->right;
						while (y->left) //find successor;
							y = y->left;
						x = y->right; //child successor
					}
				}
				if (y != z) //relink y(successor) to z;
				{
					z->left->parent = y; //link succesor with z left child
					y->left = z->left;  // same
					if (y != z->right) //if successor is not z right
					{
						x_parent = y->parent;
						if (x)
							x->parent = y->parent;
						y->parent->left = x;
						y->right = z->right;
						z->right->parent = y;
					}
					else
						x_parent = y;
					if (root == z)
						root = y;
					else if (z->parent->left == z)
						z->parent->left = y;
					else
						z->parent->right = y;
					y->parent = z->parent;
					std::swap(y->color, z->color);
					y = z;
				}
				else
				{
					x_parent = y->parent;
					if (x)
						x->parent = y->parent;
					if (root == z)
						root = x;
					else
					{	//make z parent point to x;
						if (z->parent->left == z)
							z->parent->left = x;
						else
							z->parent->right = x;
					}
					if (leftmost == z)
					{
						if (z->right == 0)
							leftmost = z->parent;
						else
							leftmost = node_struct::minimum(x);
					}
					if (rightmost == z)
					{
						if (z->left == 0)
							rightmost = z->parent;
						else
							rightmost = node_struct::maximum(x);
					}
				}
				if (y->color != ft::red)
				{
					while (x != root && _is_black(x))
					{
						if (x == x_parent->left)
						{
							node_ptr w = x_parent->right;
							if (w->color == ft::red)
							{
								w->color = ft::black;
								x_parent->color = ft::red;
								_rotate_left(x_parent, root);
								w = x_parent->right;
							}
							if (_is_black(w->left) && _is_black(w->right))
							{
								w->color = ft::red;
								x = x_parent;
								x_parent = x_parent->parent;
							}
							else
							{
								if (_is_black(w->right))
								{
									w->left->color = ft::black;
									w->color = ft::red;
									_rotate_right(w, root);
									w = x_parent->right;
								}
								w->color = x_parent->color;
								x_parent->color = ft::black;
								if (w->right)
									w->right->color = ft::black;
								_rotate_left(x_parent, root);
								break;
							}
						}
						else
						{
							node_ptr w = x_parent->left;
							if (w->color == ft::red)
							{
								w->color = ft::black;
								x_parent->color = ft::red;
								_rotate_right(x_parent, root);
								w = x_parent->left;
							}
							if (_is_black(w->right) && _is_black(w->left))
							{
								w->color = ft::red;
								x = x_parent;
								x_parent = x_parent->parent;
							}
							else
							{
								if (_is_black(w->left))
								{
									w->right->color = ft::black;
									w->color = ft::red;
									_rotate_left(w, root);
									w = x_parent->left;
								}
								w->color = x_parent->color;
								x_parent->color = ft::black;
								if (w->left)
									w->left->color = ft::black;
								_rotate_right(x_parent, root);
								break;
							}
						}
					}
					if (x)
						x->color = ft::black;
				}
				return (y);
			}

			void	_erase(const_iterator position)
			{
				node_ptr el = _rebalance_for_erase(const_cast<node_ptr>(position.node));
				_destroy_node(el);
				--_node_count;
			}

			void	_erase(const_iterator first, const_iterator last)
			{
				if (first == begin() && last == end())
					clear();
				else
				{
					while (first != last)
						_erase(first++);
				}
			}

			void	_delete(node_ptr x)
			{
				while (x != 0)
				{
					_delete(x->right);
					node_ptr y = x->left;
					_destroy_node(x);
					x = y;
				}
			}

		public:
			//CONSTRUCTOR && DESTRUCTOR && operator=
			Rb_tree(const key_compare& c = key_compare(), const allocator_type &a = allocator_type())
			: _node_alloc(a), _comp(c),_node_count(0)
			{ _initialize_header(); }

			Rb_tree(const Rb_tree &x)
			: _node_alloc(x._node_alloc), _comp(x._comp), _node_count(x._node_count)
			{
				_initialize_header();
				if (x._root() != 0)
				{
					_root() = _copy(x._root(), _end());
					_leftmost() = node_struct::minimum(_root());
					_rightmost() = node_struct::maximum(_root());
				}
			}

			~Rb_tree()
			{ clear(); }

			Rb_tree<Key, Value, KeyOfValue, Compare, Alloc>&
			operator=(const Rb_tree<Key, Value, KeyOfValue, Compare, Alloc> &src)
			{
				if (this != &src)
				{
					clear();
					_comp = src._comp;
					if (src._root())
					{
						_root() = _copy(src._root(), _end());
						_leftmost() = node_struct::minimum(_root());
						_rightmost() = node_struct::maximum(_root());
						_node_count = src._node_count;
					}
				}
				return (*this);
			}

			//Allocator, Iterator && Utils
			allocator_type get_allocator()
			{ return allocator_type();}

			node_allocator	&get_node_allocator()
			{ return node_allocator();}

			const node_allocator	&get_node_allocator() const
			{ return node_allocator();}
			//Iterators
			iterator begin()
			{ return iterator(_header.left); }

			const_iterator begin() const
			{ return const_iterator(_header.left); }

			iterator end()
			{ return iterator(&_header); }

			const_iterator end() const
			{ return const_iterator(&_header); }

			reverse_iterator	rbegin()
			{ return reverse_iterator(end()); }

			const_reverse_iterator rbegin() const
			{ return const_reverse_iterator(end()); }

			reverse_iterator	rend()
			{ return reverse_iterator(begin()); }

			const_reverse_iterator rend() const
			{ return const_reverse_iterator(begin()); }

			//Utils
			Compare	key_comp() const
			{ return (_comp); }

			size_type	size() const
			{ return (_node_count); }

			bool	empty() const
			{ return (_node_count == 0); }

			size_type	max_size() const
			{ return (_node_alloc.max_size()); }

			//Operations
			iterator	lower_bound(const key_type &k)
			{
				node_ptr x = _root();
				node_ptr y = _end();

				while (x != 0)
				{
					if (!_comp(_key(x), k)) //x is not lesser than k
					{
						y = x;
						x = x->left;
					}
					else
						x = x->right;
				}
				return iterator(y);
			}

			const_iterator	lower_bound(const key_type &k) const
			{
				const_node_ptr x = _root();
				const_node_ptr y = _end();

				while (x != 0)
				{
					if (!_comp(_key(x), k)) //x is not lesser than k
					{
						y = x;
						x = x->left;
					}
					else
						x = x->right;
				}
				return const_iterator(y);
			}

			iterator	upper_bound(const key_type &k)
			{
				node_ptr x = _root();
				node_ptr y = _end();

				while (x != 0)
				{
					if (_comp(k, _key(x))) //k is smaller than x
					{
						y = x;
						x = x->left;
					}
					else
						x = x->right;
				}
				return iterator(y);
			}


			const_iterator	upper_bound(const key_type &k) const
			{
				const_node_ptr x = _root();
				const_node_ptr y = _end();

				while (x != 0)
				{
					if (_comp(k, _key(x)))
					{
						y = x;
						x = x->left;
					}
					else
						x = x->right;
				}
				return const_iterator(y);
			}

			ft::pair<iterator, iterator>	equal_range(const key_type &k)
			{ return (ft::pair<iterator, iterator>(lower_bound(k), upper_bound(k))); }

			ft::pair<const_iterator, const_iterator>	equal_range(const key_type &k) const
			{ return (ft::pair<const_iterator, const_iterator>(lower_bound(k), upper_bound(k))); }

			iterator	find(const key_type &k)
			{
				iterator find = lower_bound(k);

				if (find == end() || _comp(k, _key(find.node)))
					return end();
				else
					return find;
			}

			const_iterator	find(const key_type &k) const
			{
				const_iterator find = lower_bound(k);

				if (find == end() || _comp(k, _key(find.node)))
					return end();
				else
					return find;
			}

	 		size_type	count(const Key &k) const
			{
				pair<const_iterator, const_iterator> pair = equal_range(k);
				return (size_type(std::distance(pair.first, pair.second)));
			}

			//insert
			ft::pair<iterator, bool>	insert_unique(const value_type &val)
			{
				node_ptr x = _root();
				node_ptr y = _end();
				bool comp = true;

				while (x != 0)
				{
					y = x;
					comp = _comp(KeyOfValue()(val), _key(x));
					x = comp ? x->left : x->right;
				}
				iterator j = iterator(y);
				if (comp)
				{
					if (j == begin())
						return ft::pair<iterator, bool>(_insert(x, y, val), true);
					else
						--j;
				}
				if (_comp(_key(j.node), KeyOfValue()(val)))
					return ft::pair<iterator, bool>(_insert(x, y, val), true);
				return ft::pair<iterator, bool>(j, false);
			}


			//With hint
			iterator	insert_unique(const_iterator position, const value_type &val)
			{
				if (position.node == _end())
				{
					if (_node_count > 0 && _comp(_key(_rightmost()), KeyOfValue()(val))) //Greater than maxvalue
						return (_insert(0, _rightmost(), val));
					else
						return (insert_unique(val).first);

				}
				else if (_comp(KeyOfValue()(val), _key(position.node))) //val before position (so al smaller)
				{
					const_iterator before = position;

					if (position.node == _leftmost())
						return (_insert(_leftmost(), _leftmost(), val));
					else if (_comp(_key((--before).node), KeyOfValue()(val))) // val greater than before
					{
						if (before.node->right == 0)
							return (_insert(0, before.node, val)); //right insert
						else
							return (_insert(position.node, position.node, val)); //left insert
					}
					else
						return (insert_unique(val).first);
				}
				else if (_comp(KeyOfValue()(position.node->value), KeyOfValue()(val))) // val after position (so val greater)
				{
					const_iterator after = position;

					if (position.node == _rightmost())
						return (_insert(0, _rightmost(), val)); //right;
					else if(_comp(KeyOfValue()(val), _key((++after).node))) //val smaller than after
					{
						if (!position.node->right)
							return (_insert(0, position.node, val));
						else
							return (_insert(after.node, after.node, val));
					}
					else
						return (insert_unique(val).first);
				}
				else //equivalent
					return position.it_const_cast();
			}

			//Range
			template<typename Iterator>
			void	insert_unique(Iterator first, Iterator last)
			{
				for (; first != last; ++first)
					insert_unique(end(), *first);
			}

			void	erase(iterator position)
			{ _erase(position); }

			void	erase(iterator first, iterator last)
			{ _erase(first, last); }

			void	erase(const_iterator position)
			{ _erase(position); }

			void	erase(const_iterator first, const_iterator last)
			{ _erase(first, last); }

			size_type	erase(const Key& k)
			{
				ft::pair<iterator, iterator> p  = equal_range(k);
				const size_type old_size = size();
				erase(p.first, p.second);
				return (old_size - size());
			}

			void	clear()
			{
				_delete(_root());
				_leftmost() = &_header;
				_rightmost() = &_header;
				_header.parent = 0;
				_node_count = 0;
			}

			void	swap(Rb_tree<Key, Value, KeyOfValue, Compare, Alloc> &tree)
			{
				if (_root() == 0)
				{
					if (tree._root() != 0)
					{
						_root() = tree._root();
						_leftmost() = tree._leftmost();
						_rightmost() = tree._rightmost();
						_root()->parent = _end();
						tree._root() = 0;
						tree._leftmost() = tree._end();
						tree._rightmost() =  tree._end();
					}
				}
				else if (tree._root() == 0)
				{
					tree._root() = _root();
					tree._leftmost() = _leftmost();
					tree._rightmost() = _rightmost();
					tree._root()->parent = tree._end();
					_root() = 0;
					_leftmost() = _end();
					_rightmost() = _end();
				}
				else
				{
					std::swap(_root(), tree._root());
					std::swap(_leftmost(), tree._leftmost());
					std::swap(_rightmost(), tree._rightmost());
					_root()->parent = _end();
					tree._root()->parent = tree._end();
				}
				std::swap(_node_count, tree._node_count);
				std::swap(_comp, tree._comp);
				if (_node_alloc != tree._node_alloc)
					std::swap(_node_alloc, tree._node_alloc);
			}
	};
	//Operators
	template<typename Key, typename Val, typename KeyOfValue, typename Compare, typename Alloc>
	inline	bool	operator==(const Rb_tree<Key, Val, KeyOfValue, Compare, Alloc> &x,
	const Rb_tree<Key, Val, KeyOfValue, Compare, Alloc> &y)
	{ return (x.size() == y.size() && ft::equal(x.begin(), x.end(), y.begin())); }

	template<typename Key, typename Val, typename KeyOfValue, typename Compare, typename Alloc>
	inline	bool	operator<(const Rb_tree<Key, Val, KeyOfValue, Compare, Alloc> &x,
	const Rb_tree<Key, Val, KeyOfValue, Compare, Alloc> &y)
	{ return (ft::lexicographical_compare(x.begin(), x.end(), y.begin(), y.end())); }

	template<typename Key, typename Val, typename KeyOfValue, typename Compare, typename Alloc>
	inline	bool	operator!=(const Rb_tree<Key, Val, KeyOfValue, Compare, Alloc> &x,
	const Rb_tree<Key, Val, KeyOfValue, Compare, Alloc> &y)
	{ return !(x == y); }

	template<typename Key, typename Val, typename KeyOfValue, typename Compare, typename Alloc>
	inline	bool	operator>(const Rb_tree<Key, Val, KeyOfValue, Compare, Alloc> &x,
	const Rb_tree<Key, Val, KeyOfValue, Compare, Alloc> &y)
	{ return (y < x); }

	template<typename Key, typename Val, typename KeyOfValue, typename Compare, typename Alloc>
	inline	bool	operator<=(const Rb_tree<Key, Val, KeyOfValue, Compare, Alloc> &x,
	const Rb_tree<Key, Val, KeyOfValue, Compare, Alloc> &y)
	{ return !(y < x); }

	template<typename Key, typename Val, typename KeyOfValue, typename Compare, typename Alloc>
	inline	bool	operator>=(const Rb_tree<Key, Val, KeyOfValue, Compare, Alloc> &x,
	const Rb_tree<Key, Val, KeyOfValue, Compare, Alloc> &y)
	{ return !(x < y); }

	template<typename Key, typename Val, typename KeyOfValue, typename Compare, typename Alloc>
	inline	void	swap(const Rb_tree<Key, Val, KeyOfValue, Compare, Alloc> &x,
	const Rb_tree<Key, Val, KeyOfValue, Compare, Alloc> &y)
	{ return (x.swap(y)); }
}
#endif
