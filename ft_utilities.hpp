#ifndef FT_UTILITIES_HPP
# define FT_UTILITIES_HPP

# include <cstddef>

namespace ft
{
  //////////////////IS_INTEGRAL//////////////////
  struct true_type{ static const bool value = true; };
  struct false_type{ static const bool value = false; };

  template<bool B>
  struct integral_constant : public false_type
  {
    typedef bool  value_type;
    false_type    type;
  };

  template<>
  struct integral_constant<true> : public true_type
  {
    typedef bool  value_type;
    true_type     type;
  };

  template<typename T>
  struct is_integral : public integral_constant<false> {};

  template<>
  struct is_integral<bool> : public integral_constant<true> {};
  template<>
  struct is_integral<char> : public integral_constant<true> {};
  template<>
  struct is_integral<wchar_t> : public integral_constant<true> {};
  template<>
  struct is_integral<signed char> : public integral_constant<true> {};
  template<>
  struct is_integral<short int> : public integral_constant<true> {};
  template<>
  struct is_integral<int> : public integral_constant<true> {};
  template<>
  struct is_integral<long int> : public integral_constant<true> {};
  template<>
  struct is_integral<long long int> : public integral_constant<true> {};
  template<>
  struct is_integral<unsigned char> : public integral_constant<true> {};
  template<>
  struct is_integral<unsigned short int> : public integral_constant<true> {};
  template<>
  struct is_integral<unsigned int> : public integral_constant<true> {};
  template<>
  struct is_integral<unsigned long int> : public integral_constant<true> {};
  template<>
  struct is_integral<unsigned long long int> : public integral_constant<true> {};

  ////////////ARE SAME///////////////
  template<typename, typename>
  struct are_same
  {
    enum { value = false };
  };

  template<typename Type>
  struct are_same<Type, Type>
  {
    enum { value = true };
  };

  //////////////////ENABLE_IF////////////
  template<bool B, class T = void>
  struct enable_if {};

  template<class T>
  struct enable_if<true, T> { typedef T type; };

  /////////////////////LEXICOGRAPHICAL_COMPARE/////////////////////////////
  template <class InputIterator1, class InputIterator2>
  bool lexicographical_compare (InputIterator1 first1, InputIterator1 last1,
  InputIterator2 first2, InputIterator2 last2)
  {
    while (first1 != last1)
    {
      if (first2 == last2 || *first2 < *first1)
        return false;
      else if (*first1 < *first2)
        return true;
      ++first1; ++first2;
    }
    return (first2 != last2);
  }

  template <class InputIterator1, class InputIterator2>
  bool equal ( InputIterator1 first1, InputIterator1 last1, InputIterator2 first2 )
  {
    while (first1 != last1) {
      if (!(*first1 == *first2))
        return false;
      ++first1; ++first2;
    }
    return true;
  }
}
#endif
