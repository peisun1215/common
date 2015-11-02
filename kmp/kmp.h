#include <string>
#include <vector>

namespace common {
namespace kmp {

class KMP {
 public:
	KMP(const std::string& pattern);

	int Match(const std::string& s);

	// The length of the prefix of pattern is that also a suffix of p_[0, pos].
	int Prefix(int pos) const;

 private:
	void ComputePrefixFunction();

	// pi_[i]: p_[0:pi_[i]) = p_[i - pi_[i] + 1:i]. Where pi_[i] < i.
	// More formally:
	// pi_[i] = max{k | k < i, p_k is a suffix of p_{i+1}}. p_i is p_[0, i).
	// In other words, the prefix of p_ with length pi_[i] is the same as the
	// suffix of p_[0:i] with length pi_[i].
	// Example: abcab:
	// pi_ = {0, 0, 0, 1, 2}.
	std::vector<int> pi_;
	const std::string p_;
};

}  // namespace kmp
}  // namespace common
