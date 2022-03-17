#ifndef MAP_HPP
# define MAP_HPP

# include <functional>
# include <memory>
# include "ft_rbtree.hpp"

namespace ft
{
	template<typename Pair>
	struct Select1st
	{
		typename Pair::first_type &operator()(Pair &x) const
		{ return x.first; }

		const typename Pair::first_type &operator()(Pair const &x) const
		{ return x.first; }
	};

	template
	<
		typename Key,
		typename Value,
		typename Compare = std::less<Key>,
		typename Alloc = std::allocator<ft::pair <const Key, Value> >
	>
	class map
	{
		public:
			typedef Key								key_type;
			typedef	Value							mapped_type;
			typedef ft::pair<const Key, Value>		value_type;
			typedef Compare							key_compare;
			typedef Alloc							allocator_type;

			typedef typename allocator_type::reference			reference;
			typedef typename allocator_type::const_reference	const_reference;
			typedef typename allocator_type::pointer			pointer;
			typedef typename allocator_type::const_pointer		const_pointer;

		private:
			typedef Rb_tree<key_type, value_type, ft::Select1st<value_type>, key_compare, allocator_type>	_Rb_tree;
			_Rb_tree _rb_tree;

		public:
			class value_compare : public std::binary_function<value_type, value_type, bool>
			{
				friend class map<Key, Value, Compare, Alloc>;

				protected:
					Compare	comp;
					value_compare(Compare c) : comp(c) {}
				public:
					bool	operator()(const value_type &x, const value_type &y) const
					{ return comp(x.first, y.first); }
			};

			typedef typename _Rb_tree::iterator								iterator;
			typedef typename _Rb_tree::const_iterator						const_iterator;
			typedef typename _Rb_tree::reverse_iterator						reverse_iterator;
			typedef typename _Rb_tree::const_reverse_iterator				const_reverse_iterator;
			typedef typename ft::iterator_traits<iterator>::difference_type	difference_type;
			typedef typename _Rb_tree::size_type							size_type;

			//CONSTRUCTORS, OPERATOR=

			explicit map(const key_compare &comp = key_compare(),
				const allocator_type &alloc = allocator_type())
				: _rb_tree(comp, alloc) {}

			map(const map &src) : _rb_tree(src._rb_tree) {}

			template<typename Iterator>
			map(Iterator first, Iterator last,
				const key_compare &comp = key_compare(),
				const allocator_type &alloc = allocator_type())
				: _rb_tree(comp, alloc)
			{ _rb_tree.insert_unique(first, last); }

			map&	operator=(const map &src)
			{
				_rb_tree = src._rb_tree;
				return (*this);
			}
			//ITERATORS
			iterator begin()
			{ return _rb_tree.begin(); }

			const_iterator begin() const
			{ return _rb_tree.begin(); }

			iterator end()
			{ return _rb_tree.end(); }

			const_iterator end() const
			{ return _rb_tree.end(); }

			reverse_iterator rbegin()
			{ return _rb_tree.rbegin(); }

			const_reverse_iterator rbegin() const
			{ return _rb_tree.rbegin(); }

			reverse_iterator rend()
			{ return _rb_tree.rend(); }

			const_reverse_iterator rend()  const
			{ return _rb_tree.rend(); }

			//CAPACITY
			bool empty() const
			{ return _rb_tree.empty(); }

			size_type	max_size() const
			{ return _rb_tree.max_size(); }

			size_type	size() const
			{ return _rb_tree.size(); }

			//ELEMENT ACCESS
			mapped_type&	operator[](const key_type &k)
			{
				iterator it = lower_bound(k);
				if (it == end() || key_comp()(k, (*it).first))
					it = insert(it, value_type(k, mapped_type()));
				return (*it).second;
			}
			//MODIFIERS
			ft::pair<iterator,bool>	insert(const value_type &val)
			{ return _rb_tree.insert_unique(val); }

			iterator	insert(iterator position, const value_type &val)
			{ return _rb_tree.insert_unique(position, val); }

			template<typename Iterator>
			void	insert(Iterator first, Iterator last)
			{ _rb_tree.insert_unique(first, last); }

			void	erase(iterator position)
			{ _rb_tree.erase(position); }

			size_type	erase(const key_type &k)
			{ return _rb_tree.erase(k); }

			void	erase(iterator first, iterator last)
			{ _rb_tree.erase(first, last); }

			void	swap(map &s)
			{ _rb_tree.swap(s._rb_tree); }

			void clear()
			{ _rb_tree.clear(); }

			//OBSERVERS
			key_compare	key_comp() const
			{ return _rb_tree.key_comp(); }

			value_compare	value_comp() const
			{ return value_compare(_rb_tree.key_comp()); }

			//OPERATIONS
			iterator find (const key_type &k)
			{ return _rb_tree.find(k); }

			const_iterator find (const key_type &k) const
			{ return _rb_tree.find(k); }

			size_type	count(const key_type &k) const
			{ return (_rb_tree.find(k) == _rb_tree.end() ? 0 : 1); }

			iterator lower_bound(const key_type &k)
			{ return _rb_tree.lower_bound(k); }

			const_iterator lower_bound(const key_type &k) const
			{ return _rb_tree.lower_bound(k); }

			iterator upper_bound(const key_type &k)
			{ return _rb_tree.upper_bound(k); }

			const_iterator upper_bound(const key_type &k) const
			{ return _rb_tree.upper_bound(k); }

			ft::pair<iterator, iterator>	equal_range(const key_type &k)
			{ return _rb_tree.equal_range(k); }

			ft::pair<const_iterator, const_iterator>	equal_range(const key_type &k) const
			{ return _rb_tree.equal_range(k); }

			//ALLOCATOR
			allocator_type	get_allocator() const
			{ return _rb_tree.get_allocator(); }

			//Friend prototype to have access to private _rb_tree outside
			template<typename K, typename T, typename C, typename A>
			friend bool	operator==(const map<K, T, C, A>&, const map<K, T, C, A>&);

			template<typename K, typename T, typename C, typename A>
			friend bool	operator<(const map<K, T, C, A>&, const map<K, T, C, A>&);
	};

	template<typename Key, typename Tp, typename Compare, typename Alloc>
	inline bool operator==(const map<Key, Tp, Compare, Alloc> &x,
		const map<Key, Tp, Compare, Alloc> &y)
	{ return (x._rb_tree == y._rb_tree); }

	template<typename Key, typename Tp, typename Compare, typename Alloc>
	inline bool operator<(const map<Key, Tp, Compare, Alloc> &x,
		const map<Key, Tp, Compare, Alloc> &y)
	{ return (x._rb_tree < y._rb_tree); }

	template<typename Key, typename Tp, typename Compare, typename Alloc>
	inline bool operator!=(const map<Key, Tp, Compare, Alloc> &x,
		const map<Key, Tp, Compare, Alloc> &y)
	{ return !(x == y); }

	template<typename Key, typename Tp, typename Compare, typename Alloc>
	inline bool operator>(const map<Key, Tp, Compare, Alloc> &x,
		const map<Key, Tp, Compare, Alloc> &y)
	{ return (y < x); }

	template<typename Key, typename Tp, typename Compare, typename Alloc>
	inline bool operator<=(const map<Key, Tp, Compare, Alloc> &x,
		const map<Key, Tp, Compare, Alloc> &y)
	{ return !(y < x); }

	template<typename Key, typename Tp, typename Compare, typename Alloc>
	inline bool operator>=(const map<Key, Tp, Compare, Alloc> &x,
		const map<Key, Tp, Compare, Alloc> &y)
	{ return !(x < y); }

	template<typename Key, typename Tp, typename Compare, typename Alloc>
	inline void swap(map<Key, Tp, Compare, Alloc> &x, map<Key, Tp, Compare, Alloc> &y)
	{ x.swap(y); }
}

#endif
