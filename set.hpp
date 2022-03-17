#ifndef SET_HPP
# define SET_HPP

# include <functional>
# include <memory>
# include "ft_iterator.hpp"
# include "ft_rbtree.hpp"
# include "ft_utilities.hpp"

namespace	ft
{
	template<typename T>
	struct Identity
	{
		T&	operator()(T &x) const
		{ return (x); }

		const T&	operator()(const T &x) const
		{ return (x); }
	};

	template
	<
		typename Key,
		typename Compare = std::less<Key>,
		typename Alloc = std::allocator<Key>
	>
	class set
	{
		public:
			typedef Key		key_type;
			typedef Key		value_type;
			typedef Compare	key_compare;
			typedef	Compare	value_compare;
			typedef Alloc	allocator_type;

			typedef typename allocator_type::reference			reference;
			typedef typename allocator_type::const_reference	const_reference;
			typedef typename allocator_type::pointer			pointer;
			typedef typename allocator_type::const_pointer		const_pointer;

		private:
			typedef Rb_tree<key_type, value_type, ft::Identity<value_type>, key_compare, allocator_type>	_Rb_tree;
			_Rb_tree _rb_tree;
		public:
			typedef typename _Rb_tree::const_iterator						iterator;
			typedef typename _Rb_tree::const_iterator						const_iterator;
			typedef typename _Rb_tree::const_reverse_iterator				reverse_iterator;
			typedef typename _Rb_tree::const_reverse_iterator				const_reverse_iterator;
			typedef typename ft::iterator_traits<iterator>::difference_type	difference_type;
			typedef typename _Rb_tree::size_type							size_type;
		//Constructor and operator=
		explicit	set(const Compare &comp = key_compare(),
			const allocator_type &alloc = allocator_type())
			: _rb_tree(comp, alloc) {}

		template <typename It>
		set(It first, It last, const Compare &comp = key_compare(),
			const allocator_type &alloc = allocator_type())
			: _rb_tree(comp, alloc)
		{ _rb_tree.insert_unique(first, last); }

		set(const set &x) : _rb_tree(x._rb_tree) {}

		set&	operator=(const set &src)
		{
			_rb_tree = src._rb_tree;
			return *this;
		}
		//Accessors
		key_compare	key_comp() const
		{ return _rb_tree.key_comp(); }

		value_compare	value_comp() const
		{ return _rb_tree.key_comp(); }

		//Iterators
		iterator	begin()
		{ return _rb_tree.begin(); }

		const_iterator	begin() const
		{ return _rb_tree.begin(); }

		iterator	end()
		{ return _rb_tree.end(); }

		const_iterator	end() const
		{ return _rb_tree.end(); }

		reverse_iterator	rbegin()
		{ return _rb_tree.rbegin(); }

		const_reverse_iterator	rbegin() const
		{ return _rb_tree.rbegin(); }

		reverse_iterator	rend()
		{ return _rb_tree.rend(); }

		const_reverse_iterator	rend() const
		{ return _rb_tree.rend(); }
		//Capacity
		bool	empty() const
		{ return _rb_tree.empty(); }

		size_type	size() const
		{ return _rb_tree.size(); }

		size_type	max_size() const
		{ return _rb_tree.max_size(); }
		//Modifiers
		void	swap(set &x)
		{ _rb_tree.swap(x._rb_tree); }

		ft::pair<iterator, bool>	insert(const value_type &val)
		{ return _rb_tree.insert_unique(val); }

		iterator	insert(iterator position, const value_type &val)
		{ return _rb_tree.insert_unique(position, val); }

		template<typename It>
		void	insert(It first, It last)
		{ _rb_tree.insert_unique(first, last); }

		void	erase(iterator position)
		{ _rb_tree.erase(position); }

		size_type	erase(const value_type &val)
		{ return _rb_tree.erase(val); }

		void	erase(iterator first, iterator last)
		{ return _rb_tree.erase(first, last); }

		void	clear()
		{ _rb_tree.clear(); }
		//Operations
		iterator	find(const value_type &val)
		{ return _rb_tree.find(val); }

		const_iterator	find(const value_type &val) const
		{ return _rb_tree.find(val); }

		size_type	count(const value_type &val) const
		{ return (_rb_tree.find(val) == _rb_tree.end() ? 0 : 1); }

		iterator	lower_bound(const value_type &val)
		{ return _rb_tree.lower_bound(val); }

		const_iterator	lower_bound(const value_type &val) const
		{ return _rb_tree.lower_bound(val); }

		iterator	upper_bound(const value_type &val)
		{ return _rb_tree.upper_bound(val); }

		const_iterator	upper_bound(const value_type &val) const
		{ return _rb_tree.upper_bound(val); }

		ft::pair<iterator, iterator>	equal_range(const value_type &val)
		{ return _rb_tree.equal_range(val); }

		ft::pair<const_iterator, const_iterator>	equal_range(const value_type &val) const
		{ return _rb_tree.equal_range(val); }

		//Allocator
		allocator_type	get_allocator() const
		{ return _rb_tree.get_allocator(); }
		//Friend prototype to have access to private _rb_tree outside
		template<typename K, typename C, typename A>
		friend bool	operator==(const set<K, C, A>&, const set<K, C, A>&);

		template<typename K, typename C, typename A>
		friend bool	operator<(const set<K, C, A>&, const set<K, C, A>&);
	};

	template<typename Key, typename Compare, typename Alloc>
	inline bool operator==(const set<Key, Compare, Alloc> &x,
		const set<Key, Compare, Alloc> &y)
	{ return (x._rb_tree == y._rb_tree); }

	template<typename Key, typename Compare, typename Alloc>
	inline bool operator<(const set<Key, Compare, Alloc> &x,
		const set<Key, Compare, Alloc> &y)
	{ return (x._rb_tree < y._rb_tree); }

	template<typename Key, typename Compare, typename Alloc>
	inline bool operator!=(const set<Key, Compare, Alloc> &x,
		const set<Key, Compare, Alloc> &y)
	{ return !(x == y); }

	template<typename Key, typename Compare, typename Alloc>
	inline bool operator>(const set<Key, Compare, Alloc> &x,
		const set<Key, Compare, Alloc> &y)
	{ return (y < x); }

	template<typename Key, typename Compare, typename Alloc>
	inline bool operator<=(const set<Key, Compare, Alloc> &x,
		const set<Key, Compare, Alloc> &y)
	{ return !(y < x); }

	template<typename Key, typename Compare, typename Alloc>
	inline bool operator>=(const set<Key, Compare, Alloc> &x,
		const set<Key, Compare, Alloc> &y)
	{ return !(x < y); }


	template<typename Key, typename Compare, typename Alloc>
	inline bool swap(const set<Key, Compare, Alloc> &x,
		const set<Key, Compare, Alloc> &y)
	{ x.swap(y); }
}
#endif
