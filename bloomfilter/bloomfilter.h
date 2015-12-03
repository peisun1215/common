#ifndef COMMON_BLOOMFILTER_BLOOMFILTER_H_
#define COMMON_BLOOMFILTER_BLOOMFILTER_H_

#include <functional>
#include <vector>

namespace common {

template <class T>
class BloomFilter {
 public:
  typedef long long int64;

  BloomFilter(const std::vector<std::function<int64(const T&)>> hashes,
              int64 size) : hashes_(hashes), bits_(size, false) {}

  void Add(const T& key) {
    for (auto& f : hashes_) {
      bits_[f(key) % bits_.size()] = true;
    }
  }

  bool Contains(const T& key) const {
    bool contain = true;
    for (auto& f : hashes_) {
      contain &= bits_.at(f(key) % bits_.size());
    }
    return contain;
  }

 private:
  std::vector<std::function<int64(const T&)>> hashes_;
  std::vector<bool> bits_;
};

}  // namespace common

#endif  // COMMON_BLOOMFILTER_BLOOMFILTER_H_
