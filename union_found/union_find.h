#include <cstddef>
#include <memory>
#include <vector>

// This class is not threadsafe.
template <class T>
class UnionFound {
 public:
  struct Node {
    T data;
    Node* parent = nullptr;
    // The height of the tree rooted at this.
    int rank = 1;
    Node(const T& _d) : data(_d) : parent(this) {}
  };

  Node* Create(const T& data) {
    roots_.emplace_back(new Node(data));
    return roots_.back().get();
  }

  void Union(Node* a, Node* b) {
    a = Find(a);
    b = Find(b);
    if (a->rank == b->rank) {
      b->parent = a;
      ++a->rank;
    } else if (a->rank > b->rank) {
      b->parent = a;
    } else {
      a->parent = b;
    }
  }

  Node* Find(Node* node) {
    std::vector<Node*> nodes_in_path;
    while (node->parent != node) {
      nodes_in_path.push_back(node);
      node = node->parent;
    }
    for (auto* n : nodes_in_path) {
      n->parent = node;
    }
    return node;
  }

private:
  std::vector<std::unique_ptr<Node>> roots_;
};
