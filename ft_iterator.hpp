#ifndef FT_ITERATOR_HPP
# define FT_ITERATOR_HPP

# include <iterator>
# include "ft_utilities.hpp"

namespace ft
{
	////////////////ITERATOR_TRAITS////////////////////
	template <typename Iterator>
	struct iterator_traits
	{
		typedef typename Iterator::iterator_category iterator_category;
		typedef typename Iterator::value_type        value_type;
		typedef typename Iterator::difference_type   difference_type;
		typedef typename Iterator::pointer           pointer;
		typedef typename Iterator::reference         reference;
	};
	/// Partial specialization for pointer types.
	template<typename Tp>
	struct iterator_traits<Tp*>
	{
		typedef std::random_access_iterator_tag iterator_category;
		typedef Tp                              value_type;
		typedef ptrdiff_t                       difference_type;
		typedef Tp*                             pointer;
		typedef Tp&                             reference;
	};
	/// Partial specialization for const pointer types.
	template<typename Tp>
	struct iterator_traits<const Tp*>
	{
		typedef std::random_access_iterator_tag iterator_category;
		typedef Tp                              value_type;
		typedef ptrdiff_t                       difference_type;
		typedef const Tp*                       pointer;
		typedef const Tp&                       reference;
	};

	///////REVERSE ITERATOR///////////////////////////
	template<typename Iterator>
	class reverse_iterator
	{
		protected:
			Iterator  _curr;

			typedef   ft::iterator_traits<Iterator> _traits_type;

		public:
			typedef	Iterator                                  iterator_type;
			typedef	typename _traits_type::iterator_category  iterator_category;
			typedef	typename _traits_type::value_type         value_type;
			typedef	typename _traits_type::difference_type    difference_type;
			typedef	typename _traits_type::reference          reference;
			typedef	typename _traits_type::pointer            pointer;


			reverse_iterator() : _curr() {}
			explicit reverse_iterator(iterator_type x) : _curr(x) {}

			reverse_iterator(const reverse_iterator & src) : _curr(src._curr) {}

			template<typename Iter>
			reverse_iterator(const reverse_iterator<Iter> & src) : _curr(src.base()) {}

			iterator_type base() const
			{ return _curr; }

			reference operator*() const
			{
			Iterator rit = _curr;
			return  *--rit;
			}

			pointer operator->() const
			{ return &(operator*()); }

			reverse_iterator  operator++()
			{
			--_curr;
			return *this;
			}

			reverse_iterator  operator++(int)
			{ return reverse_iterator(_curr--); }

			reverse_iterator  operator--()
			{
			++_curr;
			return *this;
			}

			reverse_iterator  operator--(int)
			{ return reverse_iterator(_curr++); }

			reverse_iterator  operator+(difference_type n) const
			{ return reverse_iterator(_curr - n); }

			reverse_iterator  &operator+=(difference_type n)
			{
			_curr -= n;
			return *this;
			}

			reverse_iterator  operator-(difference_type n) const
			{ return reverse_iterator(_curr + n); }

			reverse_iterator  &operator-=(difference_type n)
			{
			_curr += n;
			return *this;
			}

			reference operator[](difference_type n) const
			{ return *(*this + n); }
	};

	template<typename Iterator>
	inline bool operator==(const reverse_iterator<Iterator> & x, const reverse_iterator<Iterator> & y)
	{ return x.base() == y.base(); }

	template<typename IteratorL, typename IteratorR>
	inline bool operator==(const reverse_iterator<IteratorL> & x, const reverse_iterator<IteratorR> & y)
	{ return x.base() == y.base(); }

	template<typename Iterator>
	inline bool operator<(const reverse_iterator<Iterator> & x, const reverse_iterator<Iterator> & y)
	{ return y.base() < x.base(); }

	template<typename IteratorL, typename IteratorR>
	inline bool operator<(const reverse_iterator<IteratorL> & x, const reverse_iterator<IteratorR> & y)
	{ return y.base() < x.base(); }

	template<typename Iterator>
	inline bool operator!=(const reverse_iterator<Iterator> & x, const reverse_iterator<Iterator> & y)
	{ return !(x == y); }

	template<typename IteratorL, typename IteratorR>
	inline bool operator!=(const reverse_iterator<IteratorL> & x, const reverse_iterator<IteratorR> & y)
	{ return !(x == y); }

	template<typename Iterator>
	inline bool operator>(const reverse_iterator<Iterator> & x, const reverse_iterator<Iterator> & y)
	{ return y < x; }

	template<typename IteratorL, typename IteratorR>
	inline bool operator>(const reverse_iterator<IteratorL> & x, const reverse_iterator<IteratorR> & y)
	{ return y < x; }

	template<typename Iterator>
	inline bool operator<=(const reverse_iterator<Iterator> & x, const reverse_iterator<Iterator> & y)
	{ return !(y < x); }

	template<typename IteratorL, typename IteratorR>
	inline bool operator<=(const reverse_iterator<IteratorL> & x, const reverse_iterator<IteratorR> & y)
	{ return !(y < x); }

	template<typename Iterator>
	inline bool operator>=(const reverse_iterator<Iterator> & x, const reverse_iterator<Iterator> & y)
	{ return !(x < y); }

	template<typename IteratorL, typename IteratorR>
	inline bool operator>=(const reverse_iterator<IteratorL> & x, const reverse_iterator<IteratorR> & y)
	{ return !(x < y  ); }

	template<typename Iterator>
	inline  reverse_iterator<Iterator>
	operator+(typename reverse_iterator<Iterator>::difference_type n, const reverse_iterator<Iterator> & x)
	{ return reverse_iterator<Iterator>(x.base() - n); }

	template<typename Iterator>
	inline typename reverse_iterator<Iterator>::difference_type
	operator-(typename reverse_iterator<Iterator>::difference_type n, const reverse_iterator<Iterator> & x)
	{ return x.base() - n; }

	template<typename IteratorL, typename IteratorR>
	inline typename reverse_iterator<IteratorL>::difference_type
	operator-(const reverse_iterator<IteratorL> & x, const reverse_iterator<IteratorR> & y)
	{ return y.base() - x.base(); }
}

#endif
