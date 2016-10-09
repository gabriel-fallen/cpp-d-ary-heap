#ifndef HEAP_H
#define HEAP_H

#include <cassert>
#include <algorithm>
#include <functional>
#include <utility>

// Template for n-ary min-heap
template<class T, int n=2> // is binary by default
class heap {
  std::function<int(T)> cost_fn;

  class node {
    T value;
    int cost;
    node *parent;
    node *children[n];
    int nfilled;

    static void swap(node *a, node *b) {
      assert(a != nullptr);
      assert(b != nullptr);
      std::swap(a->value, b->value);
      std::swap(a->cost, b->cost);
    };

    node *minimal_child() {
      if (nfilled == 0) return nullptr; // no children, no problem

      return *std::min_element(children, children+nfilled, [](node *a, node *b) { return a->cost < b->cost; });
    }

  public:

    node(const T& value, int cost) : value(value), cost(cost), parent(nullptr), nfilled(0) {};
    node(const T& value, int cost, node *parent) : value(value), cost(cost), parent(parent), nfilled(0) {};
    ~node() {
      for(int i = 0; i < nfilled; ++i) {
        delete children[i];
      }
    };

    T get_value() const { return value; };

    node *add(const T& value, int cost) {
      assert(nfilled < n);
      node *np = new node(value, cost, this);
      children[nfilled++] = np;
      return np;
    };

    node *remove_last(const T& value) {
      assert(nfilled > 0);
      node *np = children[--nfilled];
      children[nfilled] = nullptr; // just to be sure
      return np;
    }

    bool full() const {
      return nfilled == n;
    };

    bool empty() const {
      return nfilled == 0;
    };

    void bubble_up() {
      if (parent && (parent->cost > cost)) {
        swap(this, parent);
        parent->bubble_up();
      }
    };

    void bubble_down() {
      node *m = minimal_child();
      if (m != nullptr && (cost > m->cost)) {
        swap(this, m);
        m->bubble_down();
      }
    };

    // auxiliary functions

    node *next_sibling(const node *np) const {
      assert(nfilled == n); // this function should be called on fully filled nodes only
      if (children[n-1] == np) {
        return nullptr; // np is the last child, there is no next sibling
      }

      node * const *npp = std::find(children, children+n, np);
      return *(npp+1);
    };

    static node *next_with_free(const node *me) {
      node *with_free = nullptr;
      while ((me->parent != nullptr) && (with_free == nullptr)) {
        with_free = me->parent->next_sibling(me);
        me = me->parent;
      }
      return with_free; // might be nullptr if we started from the lowest rightmost node
    };

    node *lowest_leftmost() {
      if (empty()) return this;
      return children[0]->lowest_leftmost();
    };
  };
  node *root;
  node *with_free;

public:
  heap(std::function<int(T)> cost_fn) : cost_fn(cost_fn), root(nullptr) {};
  virtual ~heap() {
    if (root) {
      delete root;
      root = nullptr;
    }
  };

  void insert(const T& value) {
    if (root == nullptr) {
      root = new node(value, cost_fn(value));
      with_free = root;
      return;
    }

    node *np = with_free->add(value, cost_fn(value));
    np->bubble_up();

    if (with_free->full()) {
      node *new_with_free = node::next_with_free(with_free);
      if (new_with_free != nullptr) {
        with_free = new_with_free;
      } else {
        // lowest row just got full, open a new one
        with_free = root->lowest_leftmost();
      }
    }
  };

  bool empty() const {
    return root == nullptr;
  };

  T minimal() const {
    return root->get_value();
  };
};

#endif
