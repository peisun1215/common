#ifndef COMMON_CSV_PARSER_H_
#define COMMON_CSV_PARSER_H_

#include <iostream>
#include <fstream>
#include <string>
#include <vector>

using std::string;
using std::vector;

namespace common {
class CSV {
 public:
  static bool Parse(const string& fname, string* result) {
    vector<string> rows;
    std::ifstream i(fname); // std::ifstream::in);

    string s;
    while (std::getline(i, s)) {
      rows.emplace_back(std::move(s));
    }
    i.close();

    vector<vector<string>> results;
    if (!Parse(rows, results)) return false;

    if (results.size() <= 1) return true;  // no actual content.

    int num_elements = results[0].size();
    for (int i = 1; i < results.size(); ++i) {
      if (results[i].size() != num_elements) {
	return false;
      }

      *result += "{\n";
      for (int j = 0; j < num_elements; ++j) {
	*result += " " + results[0][j] + ":" + results[i][j] + "\n";
      }
      *result += "}\n";
    }
    return true;
  }

  static bool Parse(const vector<string>& rows,
		    vector<vector<string>>& result) {
    for (auto& r : rows) {
      vector<string> res;
      if (!Parse(r, res)) return false;
      result.emplace_back(std::move(res));
    }

    return true;
  }

  static bool Parse(const string& row, vector<string>& result) {
    // Where to start.
    int start = 0;
    // Number of ".
    int num = 0;
    for (int i = 0; i < row.size(); ++i) {
      if (row[i] == '\\') {
	++i; continue;
      }
      if (row[i] == '\"') {
	num++;
	continue;
      }

      // Find one.
      if (row[i] == ',' && num % 2 == 0) {
	result.push_back(row.substr(start, i - start));
	start = i + 1;
	num = 0;
      }
    }

    // The last one.
    if (start != row.size()) {
      if (num % 2 == 1) return false;
      result.push_back(row.substr(start));
    }

    for (auto& s : result) {
      strip(s);
    }

    return true;
  }

  static void strip(string& s) {
    if (s.empty()) return;
    int i = 0;
    while (i < s.size() && std::isspace(s[i])) {
      ++i;
    }
    if (i < s.size() && s[i] == '\"') {
      ++i;
    }
    s.erase(0, i);

    if (s.empty()) return;

    i = s.size() - 1;
    while (i >= 0 && std::isspace(s[i])) {
      s.pop_back();
      --i;
    }
    if (i >= 0 && s[i] == '\"') {
      s.pop_back();
    }
  }
};

}  // namespace common

#endif  // COMMON_CSV_PARSER_H_
