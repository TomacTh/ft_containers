#ifndef FT_NORMAL_ITERATOR_HPP
# define FT_NORMAL_ITERATOR_HPP

namespace ft
{
	//NORMAL ITERATOR USE FOR VECTOR
	template<typename Iterator, typename Container>
	class normal_iterator
	{
		protected:
			Iterator	_current;

			typedef	ft::iterator_traits<Iterator>	_traits_type;

		public:
			typedef	Iterator                                  iterator_type;
			typedef	typename _traits_type::iterator_category  iterator_category;
			typedef	typename _traits_type::value_type         value_type;
			typedef	typename _traits_type::difference_type    difference_type;
			typedef	typename _traits_type::reference          reference;
			typedef	typename _traits_type::pointer            pointer;

		normal_iterator() : _current(Iterator()) {}
		normal_iterator(const Iterator &	src) : _current(src) {}

		// Allow iterator to const_iterator conversion
		template<typename It>
		normal_iterator(const normal_iterator<It, typename enable_if<(are_same<It, typename Container::pointer>::value), Container>::type> & src)
			: _current(src.base()) {}

		// Forward iterator requirements
		reference	operator*() const
		{ return *_current; }
		pointer		operator->() const
		{ return _current; }
		normal_iterator & operator++()
		{
			++_current;
			return (*this);
		}
		normal_iterator operator++(int)
		{ return normal_iterator(_current++); }

		// Bidirectional iterator requirements
		normal_iterator & operator--()
		{
			--_current;
			return (*this);
		}
		normal_iterator operator--(int)
		{ return normal_iterator(_current--); }

		// Random access iterator requirements
		reference	operator[](const difference_type &n) const
		{ return _current[n]; }
		normal_iterator&	operator+=(const difference_type &n)
		{ _current += n; return *this; }
		normal_iterator&	operator-=(const difference_type &n)
		{ _current -= n; return *this; }
		normal_iterator	operator+(const difference_type & n) const
		{ return normal_iterator(_current + n); }
		normal_iterator	operator-(const difference_type & n) const
		{ return normal_iterator(_current - n); }
		const Iterator&	base() const
		{ return _current; }
	};

	// Forward iterator requirements
	template<typename It_left, typename It_right, typename Container>
	inline bool operator==(const normal_iterator<It_left, Container> &lhs,
	const normal_iterator<It_right, Container> &rhs)
	{ return lhs.base() == rhs.base(); }

	template<typename It, typename Container>
	inline bool operator==(const normal_iterator<It, Container>&lhs,
	const normal_iterator<It, Container>rhs)
	{ return lhs.base() == rhs.base(); }

	template<typename It_left, typename It_right, typename Container>
	inline bool operator!=(const normal_iterator<It_left, Container> &lhs,
	const normal_iterator<It_right, Container> &rhs)
	{ return lhs.base() != rhs.base(); }

	template<typename It, typename Container>
	inline bool operator!=(const normal_iterator<It, Container> &lhs,
	const normal_iterator<It, Container> &rhs)
	{ return lhs.base() != rhs.base(); }

	// Random access iterator requirements
	template<typename It_left, typename It_right, typename Container>
	inline bool operator<(const normal_iterator<It_left, Container> &lhs,
	const normal_iterator<It_right, Container> &rhs)
	{ return lhs.base() < rhs.base(); }

	template<typename It, typename Container>
	inline bool operator<(const normal_iterator<It, Container> &lhs,
	const normal_iterator<It, Container> &rhs)
	{ return lhs.base() < rhs.base(); }

	template<typename It_left, typename It_right, typename Container>
	inline bool operator>(const normal_iterator<It_left, Container> &lhs,
	const normal_iterator<It_right, Container> &rhs)
	{ return lhs.base() > rhs.base(); }

	template<typename It, typename Container>
	inline bool operator>(const normal_iterator<It, Container> &lhs,
	const normal_iterator<It, Container> &rhs)
	{ return lhs.base() > rhs.base(); }

	template<typename It, typename Container>
	inline bool operator<=(const normal_iterator<It, Container> &lhs,
	const normal_iterator<It, Container> &rhs)
	{ return lhs.base() <= rhs.base(); }

	template<typename It_left, typename It_right, typename Container>
	inline bool operator<=(const normal_iterator<It_left, Container> &lhs,
	const normal_iterator<It_right, Container> &rhs)
	{ return lhs.base() <= rhs.base(); }


	template<typename It_left, typename It_right, typename Container>
	inline bool operator>=(const normal_iterator<It_left, Container> &lhs,
	const normal_iterator<It_right, Container> &rhs)
	{ return lhs.base() >= rhs.base(); }

	template<typename It, typename Container>
	inline bool operator>=(const normal_iterator<It, Container> &lhs,
	const normal_iterator<It, Container> &rhs)
	{ return lhs.base() >= rhs.base(); }


	template<typename It_left, typename It_right, typename Container>
	inline typename normal_iterator<It_left, Container>::difference_type
	operator-(const normal_iterator<It_left, Container> &lhs,
		const normal_iterator<It_right, Container> &rhs)
		{ return lhs.base() - rhs.base(); }

	template<typename It, typename Container>
	inline typename normal_iterator<It, Container>::difference_type
	operator-(const normal_iterator<It, Container> &lhs,
		const normal_iterator<It, Container> &rhs)
		{ return lhs.base() - rhs.base(); }

	template<typename It, typename Container>
	inline normal_iterator<It, Container>	operator+(typename normal_iterator<It, Container>::difference_type n,
	const normal_iterator<It, Container> &src)
	{ return normal_iterator<It, Container>(src.base() + n); }

}

#endif
