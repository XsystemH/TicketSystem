#ifndef SJTU_VECTOR_HPP
#define SJTU_VECTOR_HPP

#include "exceptions.hpp"

#include <climits>
#include <cstddef>
#include <memory>

namespace sjtu 
{
/**
 * a data container like std::vector
 * store data in a successive memory and support random access.
 */
template<typename T>
class vector 
{
public:
	/**
	 * a type for actions of the elements of a vector, and you should write
	 *   a class named const_iterator with same interfaces.
	 */
	/**
	 * you can see RandomAccessIterator at CppReference for help.
	 */
	class const_iterator;
	class iterator 
	{
	// The following code is written for the C++ type_traits library.
	// Type traits is a C++ feature for describing certain properties of a type.
	// For instance, for an iterator, iterator::value_type is the type that the 
	// iterator points to. 
	// STL algorithms and containers may use these type_traits (e.g. the following 
	// typedef) to work properly. In particular, without the following code, 
	// @code{std::sort(iter, iter1);} would not compile.
	// See these websites for more information:
	// https://en.cppreference.com/w/cpp/header/type_traits
	// About value_type: https://blog.csdn.net/u014299153/article/details/72419713
	// About iterator_category: https://en.cppreference.com/w/cpp/iterator
	public:
		using difference_type = std::ptrdiff_t;
		using value_type = T;
		using pointer = T*;
		using reference = T&;
		using iterator_category = std::output_iterator_tag;

  public:
		 pointer ptr;
	public:
    iterator() = default;
    iterator(T* p)
    {
      ptr = p;
    }
    iterator(const_iterator p)
    {
      ptr = &(*ptr);
    }
		/**
		 * return a new iterator which pointer n-next elements
		 * as well as operator-
		 */
		iterator operator+(const int &n) const
		{
      iterator temp = *this;
      temp.ptr += n;
      return temp;
		}
		iterator operator-(const int &n) const
		{
      iterator temp = *this;
      temp.ptr -= n;
      return temp;
		}
		// return the distance between two iterators,
		// if these two iterators point to different vectors, throw invaild_iterator.
		int operator-(const iterator &rhs) const 
		{
			return ptr - rhs.ptr;
		}
		iterator& operator+=(const int &n) 
		{
			ptr = ptr + n;
      return *this;
		}
		iterator& operator-=(const int &n) 
		{
			ptr = ptr - n;
      return *this;
		}
		iterator operator++(int)
    {
      iterator temp = *this;
      ++(*this);
      return temp;
    }
		iterator& operator++()
    {
      ++ptr;
      return *this;
    }
		iterator operator--(int)
    {
      iterator temp = *this;
      --(*this);
      return temp;
    }
		iterator& operator--()
    {
      --ptr;
      return *this;
    }
		T& operator*() const{ return *ptr; }
		/**
		 * a operator to check whether two iterators are same (pointing to the same memory address).
		 */
		bool operator==(const iterator &rhs) const { return ptr == rhs.ptr; }
		bool operator==(const const_iterator &rhs) const { return ptr == rhs.ptr; }
		/**
		 * some other operator for iterator.
		 */
		bool operator!=(const iterator &rhs) const { return ptr != rhs.ptr; }
		bool operator!=(const const_iterator &rhs) const { return ptr != rhs.ptr; }
	};
	/**
	 * has same function as iterator, just for a const object.
	 */
	class const_iterator 
	{
	public:
		using difference_type = std::ptrdiff_t;
		using value_type = T;
		using pointer = T*;
		using reference = T&;
		using iterator_category = std::output_iterator_tag;
	
	public:
		pointer ptr;
  public:
    const_iterator() = default;
    const_iterator(iterator it)
    {
      ptr = it.ptr;
    }
    /**
     * return a new iterator which pointer n-next elements
     * as well as operator-
     */
    const_iterator operator+(const int &n) const
    {
      ptr = ptr + n;
      return *this;
    }
    const_iterator operator-(const int &n) const
    {
      ptr = ptr - n;
      return *this;
    }
    // return the distance between two iterators,
    // if these two iterators point to different vectors, throw invaild_iterator.
    int operator-(const iterator &rhs) const
    {
      return ptr - rhs.ptr;
    }
    const_iterator& operator+=(const int &n)
    {
      ptr = ptr + n;
      return *this;
    }
    const_iterator& operator-=(const int &n)
    {
      ptr = ptr - n;
      return *this;
    }
    const_iterator operator++(int)
    {
      iterator temp = *this;
      ++(*this);
      return temp;
    }
    const_iterator& operator++()
    {
      ++ptr;
      return *this;
    }
    const_iterator operator--(int)
    {
      const_iterator temp = *this;
      --(*this);
      return temp;
    }
    const_iterator& operator--()
    {
      --ptr;
      return *this;
    }
    T& operator*() const{ return *ptr; }
    /**
     * a operator to check whether two iterators are same (pointing to the same memory address).
     */
    bool operator==(const iterator &rhs) const { return ptr == rhs.ptr; }
    bool operator==(const const_iterator &rhs) const { return ptr == rhs.ptr; }
    /**
     * some other operator for iterator.
     */
    bool operator!=(const iterator &rhs) const { return ptr != rhs.ptr; }
    bool operator!=(const const_iterator &rhs) const { return ptr != rhs.ptr; }

	};

  std::allocator<T> alloc;
  iterator start;
  iterator finish;
  iterator end_of_storage;

  void construct(iterator pos, T x)
  {
    std::construct_at(&(*pos), x);
  }

  void destroy(iterator pos)
  {
    std::destroy_at(&(*pos));
  }

  void copy_backward(iterator a, iterator b, iterator destination)
  {
    iterator copy = b;
    iterator paste = destination;
    if (b - a <= 0) return;
    for (; copy != a; copy--)
    {
      *paste = *copy;
      --paste;
    }
    *paste = *copy;
  }

  void copy_forward(iterator a, iterator b, iterator destination)
  {
    iterator copy = a;
    iterator paste = destination;
    for (; copy != b; copy++)
    {
      *paste = *copy;
      ++paste;
    }
    *paste = *copy;
  }

  iterator insert_aux(iterator pos, const T& x)
  {
    if (finish != end_of_storage)
    {
      construct(finish, T()); // 确保能够正常复制
      ++finish;
      T x_copy = x;
      copy_backward(pos, finish - 2, finish - 1); // 将pos到finish-2向后对齐到finish-1
      *pos = x_copy;
      return pos;
    }
    else
    {
      const size_t olds = size();
      const size_t news = olds != 0 ? 2 * olds : 1;
      iterator new_start = alloc.allocate(news);
      iterator new_finish = new_start;

      for (iterator i = start; i != pos; ++i)
      {
        construct(new_finish, *i);
        ++new_finish;
      }
      construct(new_finish, x);
      iterator temp = new_finish; // for return
      ++new_finish;
      for (iterator i = pos; i != finish; ++i)
      {
        construct(new_finish, *i);
        ++new_finish;
      }
      // copy & insert

      for (int i = 0; i < olds; i++)
      {
        destroy(start + i);
      }
      alloc.deallocate(&(*start), olds + 1);

      start = new_start;
      finish = new_finish;
      end_of_storage = new_start + news;

      return temp;
    }
  }

	/**
	 * Constructs
	 * At least two: default constructor, copy constructor
	 */
	vector() : start(0), finish(0), end_of_storage(0) {}
	vector(const vector &other)
  {
    finish = start = alloc.allocate(other.space());
    for (iterator copy = other.begin(); copy != other.end(); copy++)
    {
      construct(finish, *copy);
      finish++;
    }
    end_of_storage = start + other.space();
  }
	/**
	 * Destructor
	 */
	~vector()
  {
    clear();
    alloc.deallocate(&(*start), end_of_storage - start + 1);
  }
	/**
	 * Assignment operator
	 */
	vector &operator=(const vector &other)
  {
    if (start == other.start) return *this;
    clear();
    alloc.deallocate(&(*start), end_of_storage - start + 1);
    finish = start = alloc.allocate(other.space());
    for (iterator copy = other.begin(); copy != other.end(); ++copy)
    {
      construct(finish, *copy);
      finish++;
    }
    end_of_storage = start + other.space();
    return *this;
  }
	/**
	 * assigns specified element with bounds checking
	 * throw index_out_of_bound if pos is not in [0, size)
	 */
	T & at(const size_t &pos)
  {
    if (pos >= size()) throw index_out_of_bound();
    return *(start + pos);
  }
	const T & at(const size_t &pos) const
  {
    if (pos >= size()) throw index_out_of_bound();
    return *(start + pos);
  }
	/**
	 * assigns specified element with bounds checking
	 * throw index_out_of_bound if pos is not in [0, size)
	 * !!! Pay attentions
	 *   In STL this operator does not check the boundary but I want you to do.
	 */
	T & operator[](const size_t &pos)
  {
    if (pos >= size()) throw index_out_of_bound();
    return *(start + pos);
  }
	const T & operator[](const size_t &pos) const
  {
    if (pos >= size()) throw index_out_of_bound();
    return *(start + pos);
  }
	/**
	 * access the first element.
	 * throw container_is_empty if size == 0
	 */
  /**
  * returns an iterator to the beginning.
  */
  iterator begin() const
  {
    if (size() == 0) throw container_is_empty();
    return iterator(start);
  }
  const_iterator cbegin() const
  {
    if (size() == 0) throw container_is_empty();
    return const_iterator(start);
  }
  /**
   * returns an iterator to the end.
   */
  iterator end() const { return iterator(finish); }
  const_iterator cend() const { return const_iterator(finish); }
	const T & front() const { return *begin(); }
	/**
	 * access the last element.
	 * throw container_is_empty if size == 0
	 */
	const T & back() const
  {
    if (size() == 0) throw container_is_empty();
    return *(end() - 1);
  }
	/**
	 * checks whether the container is empty
	 */
	bool empty() const { return finish == start; }
	/**
	 * returns the number of elements
	 */
	size_t size() const
  {
    return finish - start;
  }
  size_t space() const { return end_of_storage - start; }
	/**
	 * clears the contents
	 */
	void clear()
  {
    while(finish != start)
    {
      finish--;
      destroy(finish);
    }
  }
	/**
	 * inserts ptr before pos
	 * returns an iterator pointing to the inserted ptr.
	 */
	iterator insert(iterator pos, const T &value)
  {
    return insert_aux(pos, value);
  }
	/**
	 * inserts ptr at index ind.
	 * after inserting, this->at(ind) == ptr
	 * returns an iterator pointing to the inserted ptr.
	 * throw index_out_of_bound if ind > size (in this situation ind can be size because after inserting the size will increase 1.)
	 */
	iterator insert(const size_t &ind, const T &value)
  {
    if (ind > size()) throw index_out_of_bound();
    iterator pos = begin();
    pos += ind - 1;
    insert_aux(pos, value);
    return pos;
  }

  void free()
  {
    size_t s = space();
    if (size() >= s / 3) return;
    const size_t olds = size();
    const size_t news = s / 2;
    iterator new_start = alloc.allocate(news);
    iterator new_finish = new_start;

    for (iterator i = start; i != finish; ++i)
    {
      construct(new_finish, *i);
      ++new_finish;
      destroy(i);
    }
    alloc.deallocate(&(*start), olds + 1);

    start = new_start;
    finish = new_finish;
    end_of_storage = new_start + news;
  }
	/**
	 * removes the element at pos.
	 * return an iterator pointing to the following element.
	 * If the iterator pos refers the last element, the end() iterator is returned.
	 */
	iterator erase(iterator pos)
  {
    if (pos == finish - 1)
    {
      pop_back();
      return end();
    }
    size_t ind = pos - start;
    copy_forward(pos + 1, finish - 1, pos);
    --finish;
    destroy(finish);
    free();
    pos = start + ind;
    return pos;
  }
	/**
	 * removes the element with index ind.
	 * return an iterator pointing to the following element.
	 * throw index_out_of_bound if ind >= size
	 */
	iterator erase(const size_t &ind)
  {
    if (ind >= size()) throw index_out_of_bound();
    iterator pos = begin();
    pos += ind - 1;
    copy_forward(pos + 1, finish - 1, pos);
    --finish;
    destroy(finish);
    free();
    pos = start + ind;
    return pos;
  }
	/**
	 * adds an element to the end.
	 */
	void push_back(const T &value)
  {
    insert_aux(finish, value);
  }
	/**
	 * remove the last element from the end.
	 * throw container_is_empty if size() == 0
	 */
	void pop_back()
  {
    if (size() == 0)
      throw container_is_empty();
    --finish;
    destroy(finish);
    free();
  }
};


}

#endif

// 参考资料 《STL源码剖析》侯捷