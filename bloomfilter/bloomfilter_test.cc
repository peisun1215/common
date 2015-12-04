#include "bloomfilter.h"

#include <iostream>
#include <functional>
#include <string>
#include <vector>
#include <ctime>
#include <cstdlib>
#include <unordered_set>
#include <cassert>

typedef long long int64;
typedef common::BloomFilter<int64> BF;

int main(int argc, char** argv) {
  std::vector<std::function<int64(const int64&)>> hash;

  const int test_size = std::stoi(argv[1]);

  std::srand(std::time(nullptr));

  // (a*key+b) % (2^31-1) 
  const int64 size = std::stol(argv[2]);
  for (int i = 1; i < size / test_size * 0.693; ++i) {
    int a = std::rand();
    int b = std::rand();
    hash.push_back([=](const int64& key)->int64 {
        return (a * key + b) % ((1ll << 31) - 1);
      });
  }
  BF hf(hash, size);


  std::unordered_set<int64> added;
  for (int i = 0; i < test_size; ++i) {
    int64 r = std::rand();
    added.insert(r);
    hf.Add(r);
  }

  int64 total_positive = 0;
  int64 false_positive = 0;
  const int tmp = std::stoi(argv[3]);
  for (int i = 0; i < tmp; ++i) {
    int r = std::rand();

    if (!hf.Contains(r)) {
      assert(!added.count(r));
    } else {
      ++total_positive;
      if (!added.count(r)) ++false_positive;
    }
  }

  std::cout << total_positive << " " << false_positive << std::endl;

  for (auto i : added) {
    assert(hf.Contains(i));
  }

  return 0;
}
