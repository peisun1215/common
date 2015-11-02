#include "trie.h"

#include <iostream>
#include <string>

using std::string;

void print(const string& s, bool has) {
  std::cout << (has ? "Has " + s : "No " + s) << std::endl;
}

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
}
