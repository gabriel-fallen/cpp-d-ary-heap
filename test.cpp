#include <cassert>
#include <iostream>

#include "heap.h"


template<int d>
void basic_test(heap<int, d> heap) {
  heap.insert(10);
  assert(!heap.empty());
  assert(heap.minimal() == 10);

  heap.insert(5);
  assert(!heap.empty());
  assert(heap.minimal() == 5);

  heap.insert(20);
  assert(!heap.empty());
  assert(heap.minimal() == 5);

  heap.insert(25);
  assert(!heap.empty());
  assert(heap.minimal() == 5);

  heap.insert(15);
  assert(!heap.empty());
  assert(heap.minimal() == 5);

  heap.insert(7);
  assert(!heap.empty());
  assert(heap.minimal() == 5);

  heap.insert(-5);
  assert(!heap.empty());
  assert(heap.minimal() == -5);
}

int main(int argc, char *argv[]) {
  heap<int> binary_heap([](int v) { return v; });
  assert(binary_heap.empty());

  basic_test(binary_heap);

  heap<int, 4> quadro_heap([](int v) { return v; });
  assert(quadro_heap.empty());

  basic_test(quadro_heap);

  std::cout << "All tests are passed." << std::endl;
}
