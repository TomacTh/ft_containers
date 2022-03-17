#ifndef STACK_HPP
# define STACK_HPP

# include "vector.hpp"

namespace ft
{
	template<typename T, typename Container = ft::vector<T> >
	class stack
	{
		template<typename T1, typename C1>
		friend bool	operator==(const stack<T1, C1>&, const stack<T1, C1>&);

		template<typename T1, typename C1>
		friend bool	operator<(const stack<T1, C1>&, const stack<T1, C1>&);

		public:
			typedef typename Container::value_type			value_type;
			typedef typename Container::reference			reference;
			typedef typename Container::const_reference		const_reference;
			typedef typename Container::size_type			size_type;
			typedef Container								container_type;
		protected:
			container_type	c;
		public:
			//Constructor
			explicit stack(const Container &_c = Container()) : c(_c) {}

			bool empty() const
			{ return c.empty(); }

			size_type	size() const
			{ return c.size(); }

			reference	top()
			{ return c.back(); }

			const_reference	top() const
			{ return c.back(); }

			void	push(const value_type &val)
			{ c.push_back(val); }

			void	pop()
			{ c.pop_back(); }
	};

	template<typename T, typename C>
	inline bool	operator<(const stack<T,C> &x, const stack<T,C> &y)
	{ return x.c < y.c; }

	template<typename T, typename C>
	inline bool	operator==(const stack<T,C> &x, const stack<T,C> &y)
	{ return x.c == y.c; }

	template<typename T, typename C>
	inline bool	operator!=(const stack<T,C> &x, const stack<T,C> &y)
	{ return !(x == y); }

	template<typename T, typename C>
	inline bool	operator>(const stack<T,C> &x, const stack<T,C> &y)
	{ return y < x;}

	template<typename T, typename C>
	inline bool	operator<=(const stack<T,C> &x, const stack<T,C> &y)
	{ return !(y < x);}

	template<typename T, typename C>
	inline bool	operator>=(const stack<T,C> &x, const stack<T,C> &y)
	{ return !(x < y);}

}

#endif
