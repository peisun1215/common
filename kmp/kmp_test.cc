#include <iostream>
#include "kmp.h"

int main(int argc, char** argv) {
	if (argc != 3) {
		std:: cout << "Invalid args.\n";
		return 1;
	}

	std::string s(argv[1]);
	std::string p(argv[2]);

	common::kmp::KMP kmp(p);

	for (int i = 0; i < p.size(); ++i) {
		std::cout << kmp.Prefix(i) << " ";
	}
	std::cout << std::endl;

	if (kmp.Match(s)) {
		std::cout << s << " matches " << p << std::endl;
	} else {
		std::cout << "doesn't match!\n";
	}

	return 0;
}
