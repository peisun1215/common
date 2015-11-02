#include "trie.h"

#include <iostream>
#include <string>
#include <vector>

using std::string;
using std::vector;

void print(const string& s, bool has) {
  std::cout << (has ? "Has " + s : "No " + s) << std::endl;
}

typedef common::Trie Trie;

class Solution {
public:
  vector<string> findWords(vector<vector<char>>& board,
                           vector<string>& words) {
    if (board.empty()) return {};
    if (board[0].empty()) return {};
    Trie trie;
    for (const auto &s : words) trie.Insert(s);

    string prefix;
    vector<string> result;

    for (int i = 0; i < board.size(); ++i) {
      for (int j = 0; j < board[0].size(); ++j) {
        string prefix(1, board[i][j]);
        board[i][j] = '.';
        findWords(board, i, j, trie, result, prefix);
        board[i][j] = prefix[0];
      }
    }

    return result;
  }

  void findWords(vector<vector<char>>& board, int i, int j,
                 const Trie& trie, vector<string>& result, string& prefix) {
    if (trie.HasWord(prefix)) {
      result.push_back(prefix);
      //      trie.Delete(prefix); (NOT implemented).
    }
    if (!trie.HasPrefix(prefix)) return;

    auto f = [this, &board, &trie, &result, &prefix](int i, int j) {
      prefix.push_back(board[i][j]);
      board[i][j] = '.';
      findWords(board, i, j, trie, result, prefix);
      board[i][j] = prefix.back();
      prefix.pop_back();
    };

    // top.
    if (i > 0 && board[i - 1][j] != '.') f(i - 1, j);
    // bot.
    if (i < board.size() - 1 && board[i + 1][j] != '.') f(i + 1, j);
    // left.
    if (j > 0 && board[i][j - 1] != '.') f(i, j - 1);
    // right.
    if (j < board[0].size() - 1 && board[i][j + 1] != '.') f(i, j + 1);
  }
};

int main(int argc, char** argv) {
  common::Trie t;

  t.Insert("a");
  t.Insert("abbb");
  t.Insert("acbb");
  t.Insert("def");

  print("bb", t.HasWord("bb"));
  print("ab", t.HasWord("ab"));
  print("ad", t.HasWord("ad"));

  print("a", t.HasWord("a"));
  print("def", t.HasWord("def"));

  std::cout << "prefix\n";
  print("ab", t.HasPrefix("ab"));
  print("abcd", t.HasPrefix("abcd"));


  Solution s;
  vector<string> board_s = {"aaaa","aaaa","aaaa"};
  vector<vector<char>> board;
  for (auto& s : board_s) {
    board.emplace_back();
    for (int i = 0; i < s.size(); ++i) {
      board.back().push_back(s[i]);
    }
  }

  vector<string> dict = {"aaaaaaaaaaaa","aaaaaaaaaaaaa","aaaaaaaaaaab"};
  auto result = s.findWords(board, dict);
  std::cout << std::endl;
  for (auto& r : result) {
    std::cout << r << std::endl;
  }
}
