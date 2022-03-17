#ifndef VECTOR_HPP
# define VECTOR_HPP

# include <stdexcept>
# include <algorithm>
# include <memory>
# include <iterator>
# include "ft_iterator.hpp"
# include "ft_normal_iterator.hpp"
# include "ft_utilities.hpp"

namespace ft
{
  //BASE CLASS FOR VECTOR
  template<typename Tp, typename Alloc>
  struct Ft_vector_base
  {

    typedef typename Alloc::template rebind<Tp>::other  Tp_alloc_type;
    typedef typename Tp_alloc_type::pointer             pointer;

    //STRUCT CONTAINS POINTERS AND ALLOCATOR
    struct Ft_vector_impl : public Tp_alloc_type
    {
      pointer start;
      pointer finish;
      pointer end_of_storage;

      Ft_vector_impl(Tp_alloc_type const &a)
        : Tp_alloc_type(a), start(0), finish(0), end_of_storage(0) { };
    };
    typedef Alloc allocator_type;

    //MEMBER
    Ft_vector_impl  Ft_impl;

    //CONSTRUCTORS && DESTRUCTOR
    Ft_vector_base(const allocator_type &a) : Ft_impl(a) {}

    Ft_vector_base(size_t n, const allocator_type &a)
    : Ft_impl(a)
    {
      this->Ft_impl.start = this->Ft_allocate(n);
      this->Ft_impl.finish = this->Ft_impl.start;
      this->Ft_impl.end_of_storage = this->Ft_impl.start + n;
    }

    ~Ft_vector_base()
    { Ft_deallocate(this->Ft_impl.start, this->Ft_impl.end_of_storage - this->Ft_impl.start); }

    //FUNCTIONS
    Tp_alloc_type&  Ft_get_Tp_allocator()
    { return *static_cast<Tp_alloc_type*>(&this->Ft_impl); }

    const Tp_alloc_type&  Ft_get_Tp_allocator() const
    { return *static_cast<const Tp_alloc_type*>(&this->Ft_impl); }

    allocator_type get_allocator() const
    { return allocator_type(Ft_get_Tp_allocator()); }

    pointer  Ft_allocate(size_t n)
    { return (n != 0 ? Ft_impl.allocate(n) : 0); }

    void  Ft_deallocate(pointer p, size_t n)
    {
      if (p)
        Ft_impl.deallocate(p, n);
    }
  };

  template<typename Tp, typename Alloc = std::allocator<Tp> >
  class vector : protected Ft_vector_base<Tp, Alloc>
  {
    typedef Ft_vector_base<Tp, Alloc>       Base;
    typedef typename Base::Tp_alloc_type    Tp_alloc_type;

    public:
      typedef Tp                                                      value_type;
      typedef typename Tp_alloc_type::pointer                         pointer;
      typedef typename Tp_alloc_type::const_pointer                   const_pointer;
      typedef typename Tp_alloc_type::reference                       reference;
      typedef typename Tp_alloc_type::const_reference                 const_reference;
      typedef ft::normal_iterator<pointer, vector>                    iterator;
      typedef ft::normal_iterator<const_pointer, vector>              const_iterator;
      typedef ft::reverse_iterator<const_iterator>                    const_reverse_iterator;
      typedef ft::reverse_iterator<iterator>                          reverse_iterator;
      typedef typename Tp_alloc_type::size_type                       size_type;
      typedef typename ft::iterator_traits<iterator>::difference_type difference_type;
      typedef Alloc                                                   allocator_type;
      protected:
        using Base::Ft_allocate;
        using Base::Ft_deallocate;
        using Base::Ft_impl;
        using Base::Ft_get_Tp_allocator;
        using Base::get_allocator;

    private:
      template <typename Iterator>
      Iterator _copy_n(Iterator first, size_type n, iterator result)
      {
        while (n--)
          *result++ = *first++;
        return (first);
      }

      template <typename Iterator>
      void  _construct_n(Iterator first, size_type n, pointer result)
      {
        while (n--)
          this->Ft_impl.construct(result++, *first++);
      }

      void _destroy(pointer first, pointer last)
      {
        while (first != last)
        {
          this->Ft_impl.destroy(first);
          ++first;
        }
      }

      size_type _new_size(size_type n)
      {
        if (max_size() - size() < n)
          throw std::length_error("length error\n");
        const size_type len = size() + std::max(size(), n);
        return ((len < size() || len > max_size()) ? max_size() : len);
      }


      size_type _free_n()
      { return size_type(this->Ft_impl.end_of_storage - this->Ft_impl.finish); }

      template<typename Integral>
      void  _dispatch_initialize(Integral n, Integral value, true_type)
      {
        size_type len = static_cast<size_type>(n);
        const value_type  &val = value;

        this->Ft_impl.start = this->Ft_allocate(len);
        this->Ft_impl.end_of_storage = this->Ft_impl.start + len;

        std::uninitialized_fill_n(this->Ft_impl.start, n, val);
        this->Ft_impl.finish = this->Ft_impl.end_of_storage;
      }

      template<typename Iterator>
      void  _dispatch_initialize(Iterator first, Iterator last, false_type)
      { _range_initialize(first, last); }

      template<typename Integral>
      void  _dispatch_insert(iterator position, Integral n, Integral value, true_type)
      { _fill_insert(position, size_type(n), static_cast<value_type>(value)); }

      template<typename Iterator>
      void  _dispatch_insert(iterator position, Iterator first, Iterator last, false_type)
      {
        difference_type diff = std::distance(first, last);
        if (diff > 0)
          _range_insert(position, first, last, size_type(diff));
      }

      template<typename Integral>
      void  _dispatch_assign(Integral n, Integral val, true_type)
      { _fill_assign(n, static_cast<value_type>(val)); }

      template<typename Iterator>
      void  _dispatch_assign(Iterator first, Iterator last, false_type)
      { _range_assign(first, last); }

      template<typename ForwardIterator>
      void  _range_assign(ForwardIterator first, ForwardIterator last)
      {
        const size_type len = std::distance(first, last);

        if (len > capacity())
        {
          pointer new_start = this->Ft_allocate(len);

          std::uninitialized_copy(first, last, new_start);
          _destroy(this->Ft_impl.start, this->Ft_impl.finish);
          this->Ft_deallocate(this->Ft_impl.start, this->Ft_impl.end_of_storage - this->Ft_impl.start);
          this->Ft_impl.start = new_start;
          this->Ft_impl.finish = new_start + len;
          this->Ft_impl.end_of_storage = this->Ft_impl.finish;
        }
        else if (size() >= len)
          erase(iterator(std::copy(first, last, this->Ft_impl.start)), end());
        else
        {
          ForwardIterator it = first;

          std::advance(it, size());
          std::copy(first, it, this->Ft_impl.start);
          this->Ft_impl.finish = std::uninitialized_copy(it, last, this->Ft_impl.finish);
        }
      }

      template<typename Iterator>
      void  _range_insert(iterator position, Iterator first, Iterator last, size_type n)
      {
        if (n)
        {
          if (_free_n() >= n)
          {
            if (position == end())
              std::uninitialized_copy(first, last, this->Ft_impl.finish);
            else
            {
              const size_type els_after = end() - position;
              if (els_after > n)
              {
                iterator tmp = position + els_after - n;

                std::uninitialized_copy(tmp, end(), this->Ft_impl.finish);
                std::copy_backward(position, tmp, end());
                std::copy(first, last, position);
              }
              else
              {
                std::uninitialized_copy(position, end(), end() + (n - els_after));
                Iterator tmp = _copy_n(first, els_after, position);
                _construct_n(tmp, n - els_after, this->Ft_impl.finish);
              }
            }
            this->Ft_impl.finish += n;
          }
          else
          {
            size_type len = _new_size(n);
            pointer new_start = this->Ft_allocate(len);
            pointer pos = std::uninitialized_copy(begin(), position, new_start);
            pointer new_finish = 0;

            std::uninitialized_copy(first, last, pos);
             if (position < end())
              new_finish = std::uninitialized_copy(position, end(), pos + n);
            else
              new_finish = pos + n;
            _destroy(this->Ft_impl.start, this->Ft_impl.finish);
            this->Ft_deallocate(this->Ft_impl.start,  this->Ft_impl.end_of_storage -  this->Ft_impl.start);
            this->Ft_impl.start = new_start;
            this->Ft_impl.finish = new_finish;
            this->Ft_impl.end_of_storage = new_start + len;
          }
        }
      }

      template<typename ForwardIterator>
      void  _range_initialize(ForwardIterator first, ForwardIterator last)
      {
        size_type n = std::distance(first, last);

        this->Ft_impl.start = Ft_allocate(n);
        this->Ft_impl.finish = std::uninitialized_copy(first, last, this->Ft_impl.start);
        this->Ft_impl.end_of_storage = this->Ft_impl.finish;
      }

      void _fill_assign(size_type n, const value_type& val)
      {
        if (n > capacity())
        {
          vector  tmp(n, val);
          tmp.swap(*this);
   /*        _destroy(this->Ft_impl.start, this->Ft_impl.end_of_storage);
          this->Ft_deallocate(this->Ft_impl.start, this->Ft_impl.end_of_storage - this->Ft_impl.start);
          this->Ft_impl.start = this->Ft_allocate(n);
          this->Ft_impl.finish = this->Ft_impl.start + n;
          this->Ft_impl.end_of_storage = this->Ft_impl.finish;
          std::uninitialized_fill(this->Ft_impl.start, this->Ft_impl.finish, val); */
        }
        else
        {
          size_type len = size();

          if (len > n)
          {
            erase(begin() + n, end());
            std::fill(begin(), end(), val);
          }
          else
          {
            std::fill(begin(), end(), val);
            std::uninitialized_fill_n(end(), n - len, val);
            this->Ft_impl.finish += (n - len);
          }
        }
      }

      void  _fill_insert(iterator position, size_type n, const value_type& val)
      {
        if (n)
        {
          if (_free_n() >= n)
          {
            if(position == end())
              std::uninitialized_fill_n(this->Ft_impl.finish, n, val);
            else
            {
              const size_type els_after = ( end()) - position;
              if (els_after > n)
              {
                iterator tmp = position + els_after - n;
                std::uninitialized_copy(tmp, end(), this->Ft_impl.finish);
                std::copy_backward(position, tmp, end());
                std::fill(position, position + n, val);
              }
              else
              {
                std::uninitialized_copy(position, end(), end() + (n - els_after));
                std::fill(position, position + els_after, val);
                std::uninitialized_fill_n(this->Ft_impl.finish, (n - els_after), val);
              }
            }
            this->Ft_impl.finish += n;
          }
          else
          {
            size_type len = _new_size(n);
            pointer new_start = this->Ft_allocate(len);
            pointer pos = std::uninitialized_copy(begin(), position, new_start);
            pointer new_finish = 0;

            std::uninitialized_fill(pos, pos + n, val);
            if (position < end())
              new_finish = std::uninitialized_copy(position, end(), pos + n);
            else
              new_finish = pos + n;
            _destroy(this->Ft_impl.start, this->Ft_impl.finish);
            this->Ft_deallocate(this->Ft_impl.start,  this->Ft_impl.end_of_storage -  this->Ft_impl.start);
            this->Ft_impl.start = new_start;
            this->Ft_impl.finish = new_finish;
            this->Ft_impl.end_of_storage = new_start + len;
          }
        }
      }
    public:
      //////////////////////CONSTRUCTORS//////////////////
      //Default constructor
      explicit vector(const allocator_type &a = allocator_type()) : Base(a) { }

      //Fill constructor
      explicit vector(size_type n, const value_type &value = value_type(),
        const allocator_type &a = allocator_type()) : Base(n, a)
      {
        std::uninitialized_fill_n(this->Ft_impl.start, n, value);
        this-> Ft_impl.finish = this->Ft_impl.end_of_storage;
      }

      //Copy constructor
      vector(const vector &src) : Base(src.size(), src.Ft_get_Tp_allocator())
      { this->Ft_impl.finish = std::uninitialized_copy(src.begin(), src.end(), this->Ft_impl.start); }

      //Range constructor
      template<typename Iterator>
      vector(Iterator first, Iterator last, const allocator_type& a = allocator_type()) : Base(a)
      {
        ft::is_integral<Iterator> Integral;
        _dispatch_initialize(first, last, Integral);
      }

      vector<Tp, Alloc> & operator=(const vector<Tp, Alloc> & src)
      {
        if (&src != this)
        {
          const size_type src_len = src.size();
          if (src_len > capacity())
          {
            pointer tmp = this->Ft_allocate(src_len);
            std::uninitialized_copy(src.begin(), src.end(), tmp);
            _destroy(this->Ft_impl.start, this->Ft_impl.finish);
            this->Ft_deallocate(this->Ft_impl.start,  this->Ft_impl.end_of_storage -  this->Ft_impl.start);
            this->Ft_impl.start = tmp;
            this->Ft_impl.end_of_storage = tmp + src_len;
          }
          else if (size() >= src_len)
            _destroy(std::copy(src.begin(), src.end(), this->Ft_impl.start), this->Ft_impl.finish);
          else
          {
            std::copy(src.begin(), src.begin() + size(), this->Ft_impl.start);
            std::uninitialized_copy(src.begin() + size(), src.end(), this->Ft_impl.finish);
          }
          this->Ft_impl.finish = this->Ft_impl.start + src_len;
        }
        return (*this);
      }

      ~vector()
      {
        _destroy(this->Ft_impl.start, this->Ft_impl.finish);
      }
      //////////////////////ITERATORS////////////////////////
      iterator  begin()
      { return iterator(this->Ft_impl.start); }

      iterator  end()
      { return iterator(this->Ft_impl.finish); }

      const_iterator  begin() const
      { return const_iterator(this->Ft_impl.start); }

      const_iterator  end() const
      { return const_iterator(this->Ft_impl.finish); }

      reverse_iterator  rbegin()
      { return reverse_iterator(end()); }

      reverse_iterator  rend()
      { return reverse_iterator(begin()); }

      const_reverse_iterator  rbegin() const
      { return const_reverse_iterator(end()); }

      const_reverse_iterator  rend() const
      { return const_reverse_iterator(begin()); }
      //////////////////////CAPACITY////////////////////////
      size_type size() const
      { return size_type(this->Ft_impl.finish - this->Ft_impl.start); }

      size_type max_size() const
      { return Ft_get_Tp_allocator().max_size(); }

      void resize (size_type n, value_type val = value_type())
      {
        size_type len = size();

        if (n > len)
          insert(end(), n - len, val);
        else if (n < len)
          erase(begin() + n, end());
      }

      size_type capacity() const
      { return size_type(this->Ft_impl.end_of_storage - this->Ft_impl.start); }

      bool  empty() const
      { return this->Ft_impl.start == this->Ft_impl.finish; }

      void reserve (size_type n)
      {
        if (n > max_size())
          throw std::length_error("vector::reserve");
        if (capacity() < n)
        {
          pointer new_start = this->Ft_impl.allocate(n);
          pointer new_finish = new_start + size();

          std::uninitialized_copy(this->Ft_impl.start, this->Ft_impl.finish, new_start);
          _destroy(this->Ft_impl.start, this->Ft_impl.finish);
          this->Ft_impl.deallocate(this->Ft_impl.start, this->Ft_impl.end_of_storage - this->Ft_impl.start);
          this->Ft_impl.start = new_start;
          this->Ft_impl.finish = new_finish;
          this->Ft_impl.end_of_storage = new_start + n;
        }
      }
      //////////////////////ELEMENT_ACCESS////////////////////////
      reference at(size_type n)
      {
        if (n >= size())
          throw std::out_of_range("vector::_M_range_check");
        return (*this)[n];
      }

      const_reference at(size_type n) const
      {
        if (n >= size())
          throw std::out_of_range("vector::_M_range_check");
        return (*this)[n];
      }

      reference front()
      { return *(this->Ft_impl.start); }

      const_reference front() const
      { return *(this->Ft_impl.start); }

      reference back()
      { return *(this->Ft_impl.finish - 1); }

      const_reference back() const
      { return *(this->Ft_impl.finish - 1); }

      const_reference operator[](size_type n) const
      { return *(this->Ft_impl.start + n); }

      reference operator[](size_type n)
      { return *(this->Ft_impl.start + n); }

      //////////////////////MODIFIERS////////////////////////
      void assign (size_type n, const value_type& val)
      { _fill_assign(n, val); }

      template <class InputIterator>
      void assign (InputIterator first, InputIterator last)
      {
        ft::is_integral<InputIterator> Integral;

        _dispatch_assign(first, last, Integral);
      }

      void push_back (const value_type& val)
      { insert(end(), val); }

      void  pop_back()
      {
        if (this->Ft_impl.start != this->Ft_impl.finish)
          this->Ft_impl.destroy(--this->Ft_impl.finish);
      }

      iterator insert (iterator position, const value_type& val)
      {
        size_type n = position - begin();

        if (this->Ft_impl.finish != this->Ft_impl.end_of_storage)
        {
          if (position == end())
          {
            this->Ft_impl.construct(this->Ft_impl.finish, val);
            ++this->Ft_impl.finish;
          }
          else
          {
            this->Ft_impl.construct(this->Ft_impl.finish, *(this->Ft_impl.finish - 1));
            std::copy_backward(position, end() - 1, end());
            *position = val;
            ++this->Ft_impl.finish;
          }
        }
        else
        {
          size_type ns = _new_size(1);
          pointer new_start = this->Ft_impl.allocate(ns);
          pointer tmp = std::uninitialized_copy(begin(), position, new_start);
          pointer new_finish = std::uninitialized_copy(position, end(), tmp + 1);

          this->Ft_impl.construct(tmp, val);
          _destroy(this->Ft_impl.start, this->Ft_impl.finish);
          this->Ft_deallocate(this->Ft_impl.start,  this->Ft_impl.end_of_storage -  this->Ft_impl.start);
          this->Ft_impl.start = new_start;
          this->Ft_impl.finish = new_finish;
          this->Ft_impl.end_of_storage = this->Ft_impl.start + ns;
        }
        return begin() + n;
      }

      void insert (iterator position, size_type n, const value_type& val)
      { _fill_insert(position, n, val); }

      template <class InputIterator>
      void insert (iterator position, InputIterator first, InputIterator last)
      {
        ft::is_integral<InputIterator> Integral;

        _dispatch_insert(position, first, last, Integral);
      }

      iterator erase (iterator position)
      {
        if(position + 1 != end())
          std::copy(position + 1, end(), position);
        --this->Ft_impl.finish;
        this->Ft_impl.destroy(this->Ft_impl.finish);
        return position;
      }

      iterator erase (iterator first, iterator last)
      {
        size_type els_after;
        size_type range_len;
        pointer   tmp;

        if (first != end() && first != last)
        {
          range_len = size_type(last - first);

          if (last == end())
            _destroy(first.base(), this->Ft_impl.finish);
          else
          {
            tmp = last.base();
            els_after = (end() - last);

            std::copy(last.base(), this->Ft_impl.finish, first);
            if (range_len > els_after)
              tmp -= els_after;
            else if (range_len < els_after)
              tmp += (els_after - range_len);
            _destroy(tmp, this->Ft_impl.finish);
          }
          this->Ft_impl.finish -= range_len;
        }
        return (first);
      }

      void swap (vector& x)
      {
        std::swap(this->Ft_impl.start, x.Ft_impl.start);
        std::swap(this->Ft_impl.finish, x.Ft_impl.finish);
        std::swap(this->Ft_impl.end_of_storage, x.Ft_impl.end_of_storage);
        if (this->Ft_get_Tp_allocator() != x.Ft_get_Tp_allocator())
          std::swap(this->Ft_get_Tp_allocator(), x.Ft_get_Tp_allocator());
      }

      void  clear()
      {
        _destroy(this->Ft_impl.start, this->Ft_impl.finish);
        this->Ft_impl.finish = this->Ft_impl.start;
      }
  };

  template<typename T, typename Allo>
  inline bool operator==(const vector<T, Allo> &x, const vector<T, Allo> &y)
  { return (x.size() == y.size() && ft::equal(x.begin(), x.end(), y.begin())); }

  template<typename T, typename Allo>
  inline bool operator!=(const vector<T, Allo> &x, const vector<T, Allo> &y)
  { return !(x == y); }

  template<typename T, typename Allo>
  inline bool operator<(const vector<T, Allo> &x, const vector<T, Allo> &y)
  { return ft::lexicographical_compare(x.begin(), x.end(), y.begin(), y.end()); }

  template<typename T, typename Allo>
  inline bool operator>(const vector<T, Allo> &x, const vector<T, Allo> &y)
  { return (y < x); }

  template<typename T, typename Allo>
  inline bool operator<=(const vector<T, Allo> &x, const vector<T, Allo> &y)
  { return !(y < x); }

  template<typename T, typename Allo>
  inline bool operator>=(const vector<T, Allo> &x, const vector<T, Allo> &y)
  { return !(x < y); }

  template<typename T, typename Allo>
  inline void swap(vector<T, Allo> &x, vector<T, Allo> &y)
  { x.swap(y); }

}
#endif
