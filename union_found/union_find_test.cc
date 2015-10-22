#include "union_find.h"

#include <functional>
#include <iostream>
#include <string>
#include <vector>

using std::vector;

namespace {
int NumIslands(const vector<vector<char>>& grid) {
  typedef common::UnionFind<bool> UF;

  if (grid.empty()) return 0;

  UF uf;
  auto f = [](char c) -> bool {return c - '0';};

  vector<UF::Node*> prev_row(grid[0].size(), nullptr);

  for (int i = 0; i < grid.size(); ++i) {
    UF::Node* prev_col = nullptr;
    for (int j = 0; j < grid.at(i).size(); ++j) {
      const bool c = f(grid[i][j]);
      if (c) {
	auto* node = uf.Create(c);
	// up.
	if (prev_row[j]) uf.Union(node, prev_row[j]);
	prev_row[j] = node;
	// left.
	if (prev_col) uf.Union(node, prev_col);
	prev_col = node;
      } else {
	prev_row[j] = nullptr;
	prev_col = nullptr;
      }
    }
  }

  return uf.NumRoots();
}
}  // namespace

int main(int argc, char** argv) {
  //  vector<std::string> grid = {"11110", "11010", "11000", "00000"};
  vector<std::string> grid = {"1011011"};
  auto t = [](const vector<std::string> &grid) -> vector<vector<char>> {
    vector<vector<char>> ret;
    for (auto i : grid) {
      ret.emplace_back();
      for (auto c : i) {
	ret.back().push_back(c);
      }
    }
    return ret;
  };

  std::cout << NumIslands(t(grid)) << std::endl;

  return 0;
}
