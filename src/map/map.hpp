/**
 * implement a container like std::map
 */
#ifndef SJTU_MAP_HPP
#define SJTU_MAP_HPP

// only for std::less<T>
#include <functional>
#include <cstddef>
#include "utility.hpp"
#include "exceptions.hpp"

namespace sjtu {

template<
	class Key,
	class T,
	class Compare = std::less<Key>
> class map {
public:
	/**
	 * the internal type of data.
	 * it should have a default constructor, a copy constructor.
	 * You can use sjtu::map as value_type by typedef.
	 */
	typedef pair<const Key, T> value_type;

public:
  class node {
  public:
    value_type data;
    node *le;
    node *ri;
    node *fa;
    size_t height;

  public:
    node() {
      le = nullptr;
      ri = nullptr;
      fa = nullptr;
      height = 0;
    }
    node(value_type da) : data(da) {
      le = nullptr;
      ri = nullptr;
      fa = nullptr;
      height = 1;
    }
    node(node *other, node* father) :data(other->data) {
      le = nullptr;
      ri = nullptr; // not now
      fa = father;
      height = other->height;
    }
    ~node() = default;
  };

private:
  size_t GetHeight(node *n) {
    if (n == nullptr) return 0;
    return n->height;
  }

  int GetBalanceFactor(node *n) {
    if (n == nullptr) return 0;
    return GetHeight(n->le) - GetHeight(n->ri);
  }

  static bool IsLeftSon(const node *n) {
    if (n->fa == nullptr) return false;
    if (n->fa->le == n) return true; // left
    if (n->fa->ri == n) return false; // right
    return false;
  }

  void L(node *&root) {
    node *father = root->fa;
    bool which = IsLeftSon(root);
    node *left = root->le;

    root->le = root->le->ri;
    if (root->le) root->le->fa = root;

    left->ri = root;
    root->fa = left;
    left->fa = father;

    if (father) {
      if (which) father->le = left;
      else father->ri = left;
    } // root -> left

    root = left;
    left = root->ri;
    left->height = std::max(GetHeight(left->le), GetHeight(left->ri)) + 1;
    root->height = std::max(GetHeight(root->le), GetHeight(root->ri)) + 1;
  }

  void R(node *&root) {
    node *father = root->fa;
    bool which = IsLeftSon(root);
    node *right = root->ri;

    root->ri = root->ri->le;
    if (root->ri) root->ri->fa = root;

    right->le = root;
    root->fa = right;
    right->fa =father;

    if (father) {
      if (which) father->le = right;
      else father->ri = right;
    } // root -> right

    root = right;
    right = root->le;
    right->height = std::max(GetHeight(right->le), GetHeight(right->ri)) + 1;
    root->height = std::max(GetHeight(root->le), GetHeight(root->ri)) + 1;
  }

  void LR(node *&root) {
    R(root->le);
    L(root);
  }

  void RL(node *&root) {
    L(root->ri);
    R(root);
  }

  void insert(value_type x, node *&root, node *father) {
    if (root == nullptr) {
      root = new node(x);
      root->fa = father;
    } else if (Compare()(x.first, root->data.first)) {
      insert(x, root->le, root);
      if (GetBalanceFactor(root) == 2) {
        if (Compare()(x.first, root->le->data.first)) L(root);
        else LR(root);
      }
    } else if (Compare()(root->data.first, x.first)) {
      insert(x, root->ri, root);
      if (GetBalanceFactor(root) == -2) {
        if (Compare()(root->ri->data.first, x.first)) R(root);
        else RL(root);
      }
    }
    root->height = std::max(GetHeight(root->le), GetHeight(root->ri)) + 1;
  }

  bool adjust(node *&root, bool Is_Left_Tree) {
    if (Is_Left_Tree) { // remove at the left subtree
      int bf = GetBalanceFactor(root);
      if (bf == -1) return true;
      if (bf == 0) {
//        --root->height;
        root->height = std::max(GetHeight(root->le), GetHeight(root->ri)) + 1;
        return false;
      }
      if (GetHeight(root->ri->le) > GetHeight(root->ri->ri)) {
        RL(root);
        return false;
      }
//      else if (root->le == nullptr && root->ri->le) {
//        RL(root);
//        return true;
//      }
      R(root);
      if (GetHeight(root->le) == GetHeight(root->ri)) return false;
      else return true;
    }
    else {              // remove at the right subtree
      int bf = GetBalanceFactor(root);
      if (bf == 1) return true;
      if (bf == 0) {
//        --root->height;
        root->height = std::max(GetHeight(root->le), GetHeight(root->ri)) + 1;
        return false;
      }
      if (GetHeight(root->le->ri) > GetHeight(root->le->le)) {
        LR(root);
        return false;
      }
//      else if (root->le->ri && root->ri == nullptr) {
//        LR(root);
//        return true;
//      }
      L(root);
      if (GetHeight(root->le) == GetHeight(root->ri)) return false;
      else return true;
    }
  }

  bool remove(Key x, node *&root) {
    if (root == nullptr) return true;
//    std::cout << root->data.first << " " << std::endl;
    if (!(Compare()(x, root->data.first) || Compare()(root->data.first, x))) { // i.e. x == root->data->first
      if (root->le == nullptr || root->ri == nullptr) {
        node *old = root;
        node *father = root->fa;

        bool left_is_not_null = (root->le != nullptr);
        if (father) {
          if (IsLeftSon(root)) father->le = (left_is_not_null) ? root->le : root->ri;
          else father->ri = (left_is_not_null) ? root->le : root->ri; // father -> son;
          if (root) root->fa = father;
        } // root changed
        else {
          if (left_is_not_null) { // when remove the root of the entire tree, & can keep the root(in map) useful
            root = old->le;
          }
          else {
            root = old->ri;
          }
          if (root) root->fa = father;
        }
        delete old;
        return false;
      }
      else { // two son
        node *ori = root;
        node *tmp = ori->ri;
        bool left_get = false;
        while (tmp->le) {
          tmp = tmp->le;
          left_get = true;
        }
        size_t temp = tmp->height;
        tmp->height = ori->height;
        ori->height = temp;
//        root->data = tmp->data; // pair has no copy, so this can't work
        if (left_get) {
          bool Is_Left_Son = IsLeftSon(ori);
          node *tmp_fa = tmp->fa;
          node *tmp_le = tmp->le;
          node *tmp_ri = tmp->ri;

          tmp->fa = ori->fa;
          if (ori->fa) {
            if (Is_Left_Son) ori->fa->le = tmp;
            else ori->fa->ri = tmp;
          } // & root -> tmp
          else root = tmp;
          tmp->le = ori->le;
          ori->le->fa = tmp;
          tmp->ri = ori->ri;
          ori->ri->fa = tmp;

          ori->le = tmp_le; // nullptr, tmp is the most left
          ori->ri = tmp_ri;
          if (tmp_ri) tmp_ri->fa = ori;
          ori->fa = tmp_fa;
          tmp_fa->le = ori; // tmp is left son, no if is needed
        }
        else { // tmp is root's right son
          bool Is_Left_Son = IsLeftSon(ori);
          node *tmp_fa = tmp->fa;
          node *tmp_le = tmp->le;
          node *tmp_ri = tmp->ri;

          tmp->fa = ori->fa;
          if (ori->fa) {
            if (Is_Left_Son) ori->fa->le = tmp;
            else ori->fa->ri = tmp;
          }
          tmp->le = ori->le;
          ori->le->fa = tmp; // has two sons, so no need to if
          tmp->ri = ori;
          ori->fa = tmp;

          ori->le = tmp_le; // nullptr
          ori->ri = tmp_ri;
          if (tmp_ri) tmp_ri->fa = ori;
        }

        if (remove(x, tmp->ri)) {
          root = tmp;
          return true;
        }
        else {
          bool re = adjust(tmp, false);
          root = tmp;
          return re;
        }
      }
    } else if (Compare()(x, root->data.first)) {
      if (remove(x, root->le)) return true;
      return adjust(root, true);
    } else if (Compare()(root->data.first, x)) {
      if (remove(x, root->ri)) return true;
      return adjust(root, false);
    }
  }

private:
  void copy(node *&p, node *c, node* fa) {
    if (c == nullptr) {
      p = nullptr;
      return;
    }
    p = new node(c, fa);
    copy(p->le, c->le, p);
    copy(p->ri, c->ri, p);
  }
  void destroy(node *t) {
    if (t == nullptr) return;
    destroy(t->le);
    destroy(t->ri);
    delete t;
  }
  node* find(node *root, Key x) const {
    if (root == nullptr) return nullptr;
    if (Compare()(x, root->data.first)) return find(root->le, x);
    if (Compare()(root->data.first, x)) return find(root->ri, x);
    return root;
  }
  static node* get_next(node *root, bool From_Left_Son) {
    if (root == nullptr) return root;
    if (From_Left_Son && root->ri) {
      node *tmp = root->ri;
      while (tmp->le) tmp = tmp->le;
      return tmp;
    }
    if (IsLeftSon(root)) {
      return root->fa;
    }
    else return get_next(root->fa, 0);
  }
  static node* get_last(node *root, bool From_Left_Son) {
    if (root == nullptr) return root;
    if (!From_Left_Son && root->le) {
      node *tmp = root->le;
      while (tmp->ri) tmp = tmp->ri;
      return tmp;
    }
    if (!IsLeftSon(root)) {
      return root->fa;
    }
    else return get_last(root->fa, 1);
  }
  static const node* get_next(const node *root, bool From_Left_Son) {
    if (root == nullptr) return root;
    if (From_Left_Son && root->ri) {
      node *tmp = root->ri;
      while (tmp->le) tmp = tmp->le;
      return tmp;
    }
    if (IsLeftSon(root)) {
      return root->fa;
    }
    else return get_next(root->fa, 0);
  }
  static const node* get_last(const node *root, bool From_Left_Son) {
    if (root == nullptr) return root;
    if (!From_Left_Son && root->le) {
      node *tmp = root->le;
      while (tmp->ri) tmp = tmp->ri;
      return tmp;
    }
    if (!IsLeftSon(root)) {
      return root->fa;
    }
    else return get_last(root->fa, 1);
  }

public:

  node* ROOT; // the root of entire tree
  size_t SIZE; // the number of nodes in the tree
  node* MINN;
  node* MAXN;

	/**
	 * see BidirectionalIterator at CppReference for help.
	 *
	 * if there is anything wrong throw invalid_iterator.
	 *     like it = map.begin(); --it;
	 *       or it = map.end(); ++end();
	 */
	class const_iterator;
	class iterator {
	public:
		/**
		 *  add data members
		 *   just add whatever you want.
		 */
    node* p;
    const map* R;

	public:
		iterator() {
      p = nullptr;
      R = nullptr;
		}
		iterator(const iterator &other) {
			p = other.p;
      R = other.R;
		}
    iterator(node* P) {
      p = P;
      R = nullptr;
    }
    iterator(node* P, const map* r) {
      p = P;
      R = r; // map
    }
		/**
		 * iter++
		 */
		iterator operator++(int) {
      iterator tmp = *this;
      ++(*this);
      return tmp;
    }
		/**
		 * ++iter
		 */
		iterator & operator++() {
      if (p == nullptr) throw invalid_iterator();
      p = get_next(p, 1);
      return *this;
    }
		/**
		 * iter--
		 */
		iterator operator--(int) {
      iterator tmp = *this;
      --(*this);
      return tmp;
    }
		/**
		 * --iter
		 */
		iterator & operator--() {
      if (p == nullptr) {
        if (R == nullptr) throw invalid_iterator();
        p = R->ROOT;
        if (p) {
          while (p->fa) p = p->fa;
          while (p->ri) p = p->ri;
        }
        if (p) return *this;
        else throw invalid_iterator();
      }
      p = get_last(p, 0);
      if (p == nullptr) throw invalid_iterator();
      return *this;
    }
		/**
		 * a operator to check whether two iterators are same (pointing to the same memory).
		 */
		value_type & operator*() const {
      if (p == nullptr) throw invalid_iterator();
      return p->data;
    }
		bool operator==(const iterator &rhs) const {
      if (p) return p == rhs.p;
      else {
        if (p == rhs.p) return R == rhs.R;
        else return false;
      }
    }
		bool operator==(const const_iterator &rhs) const {
      if (p) return p == rhs.p;
      else {
        if (p == rhs.p) return R == rhs.R;
        else return false;
      }
    }
		/**
		 * some other operator for iterator.
		 */
		bool operator!=(const iterator &rhs) const {
      return !(*this == rhs);
    }
		bool operator!=(const const_iterator &rhs) const {
      return !(*this == rhs);
    }

		/**
		 * for the support of it->first. 
		 * See <http://kelvinh.github.io/blog/2013/11/20/overloading-of-member-access-operator-dash-greater-than-symbol-in-cpp/> for help.
		 */
		value_type* operator->() const noexcept {
      if (p == nullptr) throw invalid_iterator();
      value_type *tmp = &(p->data);
      return tmp;
    }
	};
	class const_iterator {
		// it should have similar member method as iterator.
		//  and it should be able to construct from an iterator.
  public:
			// data members.
    const node *p;
    const map *R;

  public:
    const_iterator() {
      p = nullptr;
      R = nullptr;
    }
    const_iterator(const const_iterator &other) {
      p = other.p;
      R = other.R;
    }
    const_iterator(const iterator &other) {
      p = other.p;
      R = other.R;
    }
    const_iterator(node* P, const map* r) {
      p = P;
      R = r; // map
    }

    const_iterator operator++(int) {
      const_iterator tmp = *this;
      ++(*this);
      return tmp;
    }
    const_iterator & operator++() {
      if (p == nullptr) throw invalid_iterator();
      p = get_next(p, 1);
      return *this;
    }
    const_iterator operator--(int) {
      const_iterator tmp = *this;
      --(*this);
      return tmp;
    }
    const_iterator & operator--() {
      if (p == nullptr) {
        if (R == nullptr) throw invalid_iterator();
        p = R->ROOT;
        if (p) {
          while (p->fa) p = p->fa;
          while (p->ri) p = p->ri;
        }
        if (p) return *this;
        else throw invalid_iterator();
      }
      p = get_last(p, 0);
      if (p == nullptr) throw invalid_iterator();
      return *this;
    }
    const value_type & operator*() const {
      if (p == nullptr) throw invalid_iterator();
      return p->data;
    }
    bool operator==(const iterator &rhs) const {
      if (p) return p == rhs.p;
      else {
        if (p == rhs.p) return R == rhs.R;
        else return false;
      }
    }
    bool operator==(const const_iterator &rhs) const {
      if (p) return p == rhs.p;
      else {
        if (p == rhs.p) return R == rhs.R;
        else return false;
      }
    }
    bool operator!=(const iterator &rhs) const {
      return !(*this == rhs);
    }
    bool operator!=(const const_iterator &rhs) const {
      return !(*this == rhs);
    }

    const value_type* operator->() const noexcept {
      if (p == nullptr) throw invalid_iterator();
      const value_type *tmp = &(p->data);
      return tmp;
    }

	};
	/**
	 * two constructors
	 */
public:
	map() {
    ROOT = nullptr;
    SIZE = 0;
    MINN = nullptr;
    MAXN = nullptr;
  }
	map(const map &other) {
    copy(ROOT, other.ROOT, nullptr);
    SIZE = other.SIZE;
    MINN = ROOT;
    MAXN = ROOT;
    if (ROOT) {
      while (MINN->le) MINN = MINN->le;
      while (MAXN->ri) MAXN = MAXN->ri;
    }
  }
	/**
	 * assignment operator
	 */
	map & operator=(const map &other) {
    if (ROOT == other.ROOT) return *this;
    destroy(ROOT);
    copy(ROOT, other.ROOT, nullptr);
    SIZE = other.SIZE;
    MINN = ROOT;
    MAXN = ROOT;
    if (ROOT) {
      while (MINN->le) MINN = MINN->le;
      while (MAXN->ri) MAXN = MAXN->ri;
    }
    return *this;
  }
	/**
	 * Destructors
	 */
	~map() {
    destroy(ROOT);
  }
	/**
	 * TODO
	 * access specified element with bounds checking
	 * Returns a reference to the mapped value of the element with key equivalent to key.
	 * If no such element exists, an exception of type `index_out_of_bound'
	 */
	T & at(const Key &key) {
    node *tmp = find(ROOT, key);
    if (tmp) return tmp->data.second;
    else throw index_out_of_bound();
  }
	const T & at(const Key &key) const {
    node *tmp = find(ROOT, key);
    if (tmp) return tmp->data.second;
    else throw index_out_of_bound();
  }
	/**
	 *
	 * access specified element 
	 * Returns a reference to the value that is mapped to a key equivalent to key,
	 *   performing an insertion if such key does not already exist.
	 */
	T & operator[](const Key &key) {
    node *tmp = find(ROOT, key);
    if (tmp) return tmp->data.second;
    value_type n = value_type(key, T());
    insert(n, ROOT, nullptr);
    ++SIZE;
    Renew();
    tmp = find(ROOT, key);
    return tmp->data.second;
  }
	/**
	 * behave like at() throw index_out_of_bound if such key does not exist.
	 */
	const T & operator[](const Key &key) const {
    node *tmp = find(ROOT, key);
    if (tmp) return tmp->data.second;
    throw index_out_of_bound();
  }
	/**
	 * return a iterator to the beginning
	 */
	iterator begin() {
    return iterator(MINN, this);
  }
	const_iterator cbegin() const {
    return const_iterator(MINN, this);
  }
	/**
	 * return a iterator to the end
	 * in fact, it returns past-the-end.
	 */
	iterator end() {
    return iterator(nullptr, this);
  }
	const_iterator cend() const {
    return const_iterator(nullptr, this);
  }
	/**
	 * checks whether the container is empty
	 * return true if empty, otherwise false.
	 */
	bool empty() const {
    return SIZE == 0;
  }
	/**
	 * returns the number of elements.
	 */
	size_t size() const {
    return SIZE;
  }
	/**
	 * clears the contents
	 */
	void clear() {
    destroy(ROOT);
    ROOT = nullptr;
    MINN = nullptr;
    MAXN = nullptr;
    SIZE = 0;
  }

  void RenewMin() {
    MINN = ROOT;
    if (ROOT) {
      while (MINN->le) MINN = MINN->le;
    }
  }

  void RenewMax() {
    MAXN = ROOT;
    if (ROOT) {
      while (MAXN->ri) MAXN = MAXN->ri;
    }
  }

  void Renew() {
    RenewMin();
    RenewMax();
  }
	/**
	 * insert an element.
	 * return a pair, the first of the pair is
	 *   the iterator to the new element (or the element that prevented the insertion), 
	 *   the second one is true if insert successfully, or false.
	 */
	pair<iterator, bool> insert(const value_type &value) {
    node *tmp = find(ROOT, value.first);
    if (tmp) return sjtu::pair(iterator(tmp, this), 0); // or the element that prevented the insertion
    insert(value, ROOT, nullptr);
    tmp = find(ROOT, value.first);
    ++SIZE;
    Renew();
    return sjtu::pair(iterator(tmp, this), 1);
  }
	/**
	 * erase the element at pos.
	 *
	 * throw if pos pointed to a bad element (pos == this->end() || pos points an element out of this)
	 */
	void erase(iterator pos) {
    if (pos == end()) throw invalid_iterator();
    if (pos.R && pos.R != this) throw invalid_iterator();
    if (find(ROOT, pos->first)) {
      remove(pos->first, ROOT);
      --SIZE;
    }
    Renew();
  }
  void erase(Key x) {
    if (find(ROOT, x)) {
      remove(x, ROOT);
      --SIZE;
    }
    Renew();
  }
	/**
	 * Returns the number of elements with key 
	 *   that compares equivalent to the specified argument,
	 *   which is either 1 or 0 
	 *     since this container does not allow duplicates.
	 * The default method of check the equivalence is !(a < b || b > a)
	 */
	size_t count(const Key &key) const {
    if (find(ROOT, key) != nullptr) return 1;
    else return 0;
  }
	/**
	 * Finds an element with key equivalent to key.
	 * key value of the element to search for.
	 * Iterator to an element with key equivalent to key.
	 *   If no such element is found, past-the-end (see end()) iterator is returned.
	 */
	iterator find(const Key &key) {
    return iterator(find(ROOT, key), this);
  }
	const_iterator find(const Key &key) const {
    return const_iterator(find(ROOT, key), this);
  }
};

}

#endif
