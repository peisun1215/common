#ifndef COMMON_TRIE_H_
#define COMMON_TRIE_H_

#include <cassert>
#include <memory>
#include <string>

namespace common {

// The words should only contain lower case [a-z] chars. Any invalid words can
// crash this library.
class Trie {
public:
  struct Node {
    bool has_value = false;
    Node* children[26] = {nullptr};

    ~Node() {
      for (auto* c : children) {
        if (c) delete c;
      }
    }
  };

  Trie() : root_(new Node()) {}

  // Insert one word to the trie.
  void Insert(const std::string& word) {
    if (word.empty()) root_->has_value = true;

    auto* c = root_.get();
    for (int i = 0; i < word.size(); ++i) {
      assert(IsValid(word.at(i)));
      if (!c->children[word.at(i) - 'a']) {
        c->children[word.at(i) - 'a'] = new Node();
      }
      c = c->children[word.at(i) - 'a'];
    }
    c->has_value = true;
  }

  bool HasWord(const std::string& word) {
    auto* c = root_.get();
    for (int i = 0; i < word.size(); ++i) {
      assert(IsValid(word.at(i)));
      if (!c->children[word.at(i) - 'a']) return false;
      c = c->children[word.at(i) - 'a'];
    }
    return c->has_value;
  }

  bool HasPrefix(const std::string& prefix) {
    auto* c = root_.get();
    for (int i = 0; i < prefix.size(); ++i) {
      assert(IsValid(prefix.at(i)));
      if (!c->children[prefix.at(i) - 'a']) return false;
      c = c->children[prefix.at(i) - 'a'];
    }
    return true;
  }

  bool IsValid(char c) {
    return c >= 'a' && c <= 'z';
  }

 private:
  std::unique_ptr<Node> root_;
};

}  // namespace common

#endif  // COMMON_TRIE_H_
