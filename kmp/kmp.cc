#include "kmp.h"

#include <iostream>
#include <string>
#include <vector>

namespace common {
namespace kmp {

KMP::KMP(const std::string& pattern) : p_(pattern) {
	ComputePrefixFunction();
}

int KMP::Match(const std::string& s) {
	if (p_.empty()) return 0;
	if (s.empty()) return -1;

	// pos of p_.
	std::size_t pos = 0;
	// index of s.
	std::size_t i = 0;
	while (pos < p_.size() && i < s.size()) {
		//		std::cout << pos << " " << i << std::endl;
		if (p_.at(pos) == s.at(i)) {
			pos++; i++;
		} else if (pos == 0) {
			i++;
		} else {
			pos = pi_[pos - 1];
		}
	}

	if (pos == p_.size()) return i - p_.size();
	return -1;
}

int KMP::Prefix(int pos) const {
	return pi_.at(pos);
}

// private.
void KMP::ComputePrefixFunction() {
	pi_.resize(p_.size(), 0);
	int pz = p_.size();
	for (int i = 1; i < pz; ++i) {
		int prefix = pi_[i - 1];
		while (prefix >= 0) {
			if (prefix == 0) {
				if (p_.front() == p_.at(i)) {
					pi_[i] = 1;
				}
				break;
			}

			if (p_.at(prefix) == p_.at(i)) {
				pi_[i] = prefix + 1;
				break;
			}
			prefix = pi_[prefix - 1];
		}
	}
}

}  // namespace kmp
}  // namespace common
