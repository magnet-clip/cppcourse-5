#include <algorithm>
#include <fstream>
#include <iostream>
#include <optional>
#include <sstream>
#include <string>
#include <string_view>
#include <vector>

#include "test_runner.h"

using namespace std;

template <typename It>
class Range {
 public:
  Range(It begin, It end) : begin_(begin), end_(end) {}
  It begin() const { return begin_; }
  It end() const { return end_; }

 private:
  It begin_;
  It end_;
};

pair<string_view, optional<string_view>> SplitTwoStrict(string_view s, string_view delimiter = " ") {
  const size_t pos = s.find(delimiter);
  if (pos == s.npos) {
    return {s, nullopt};
  } else {
    return {s.substr(0, pos), s.substr(pos + delimiter.length())};
  }
}

vector<string_view> Split(string_view s, string_view delimiter = " ") {
  vector<string_view> parts;
  if (s.empty()) {
    return parts;
  }
  while (true) {
    const auto [lhs, rhs_opt] = SplitTwoStrict(s, delimiter);
    parts.push_back(lhs);
    if (!rhs_opt) {
      break;
    }
    s = *rhs_opt;
  }
  return parts;
}

class Domain {
 public:
  explicit Domain(string_view text) {
    vector<string_view> parts = Split(text, ".");
    parts_reversed_.assign(rbegin(parts), rend(parts));
  }

  size_t GetPartCount() const {
    return parts_reversed_.size();
  }

  auto GetReversedParts() const {
    return Range(begin(parts_reversed_), end(parts_reversed_));
  }

  string ToStr() const {
    stringstream x;
    for (const auto& z : parts_reversed_) {
      x << z << " ";
    }
    return x.str();
  }

 private:
  vector<string> parts_reversed_;
};

// domain is subdomain of itself
bool IsSubdomain(const Domain& subdomain, const Domain& domain) {
  const auto subdomain_reversed_parts = subdomain.GetReversedParts();
  const auto domain_reversed_parts = domain.GetReversedParts();
  return subdomain.GetPartCount() >= domain.GetPartCount() && equal(begin(domain_reversed_parts), end(domain_reversed_parts),
                                                                    begin(subdomain_reversed_parts));
}

bool IsSubOrSuperDomain(const Domain& lhs, const Domain& rhs) {
  return lhs.GetPartCount() >= rhs.GetPartCount()
             ? IsSubdomain(lhs, rhs)
             : IsSubdomain(rhs, lhs);
}

class DomainChecker {
 public:
  template <typename InputIt>
  DomainChecker(InputIt domains_begin, InputIt domains_end) {
    sorted_domains_.reserve(distance(domains_begin, domains_end));
    for (const Domain& domain : Range(domains_begin, domains_end)) {
      sorted_domains_.push_back(&domain);
    }
    sort(begin(sorted_domains_), end(sorted_domains_), IsDomainLess);
    sorted_domains_ = AbsorbSubdomains(move(sorted_domains_));
  }

  // Check if candidate is subdomain of some domain
  bool IsSubdomain(const Domain& candidate) const {
    const auto it = upper_bound(
        begin(sorted_domains_), end(sorted_domains_),
        &candidate, IsDomainLess);
    if (it == begin(sorted_domains_)) {
      return false;
    }
    return ::IsSubdomain(candidate, **prev(it));
  }

 private:
  vector<const Domain*> sorted_domains_;

  static bool IsDomainLess(const Domain* lhs, const Domain* rhs) {
    const auto lhs_reversed_parts = lhs->GetReversedParts();
    const auto rhs_reversed_parts = rhs->GetReversedParts();
    return lexicographical_compare(
        begin(lhs_reversed_parts), end(lhs_reversed_parts),
        begin(rhs_reversed_parts), end(rhs_reversed_parts));
  }

  static vector<const Domain*> AbsorbSubdomains(vector<const Domain*> domains) {
    domains.erase(
        unique(begin(domains), end(domains),
               [](const Domain* lhs, const Domain* rhs) {
                 return IsSubOrSuperDomain(*lhs, *rhs);
               }),
        end(domains));
    return domains;
  }
};

vector<Domain> ReadDomains(istream& in_stream = cin) {
  vector<Domain> domains;

  size_t count;
  in_stream >> count;
  domains.reserve(count);

  for (size_t i = 0; i < count; ++i) {
    string domain_text;
    in_stream >> domain_text;
    domains.emplace_back(domain_text);
  }
  return domains;
}

vector<bool> CheckDomains(const vector<Domain>& banned_domains, const vector<Domain>& domains_to_check) {
  const DomainChecker checker(begin(banned_domains), end(banned_domains));

  vector<bool> check_results;
  check_results.reserve(domains_to_check.size());
  for (const Domain& domain_to_check : domains_to_check) {
    check_results.push_back(!checker.IsSubdomain(domain_to_check));
  }

  return check_results;
}

void PrintCheckResults(const vector<bool>& check_results, ostream& out_stream = cout) {
  for (const bool check_result : check_results) {
    out_stream << (check_result ? "Good" : "Bad") << "\n";
  }
}

void RunIntegrationTest(string name, istream& input, istream& expected) {
  stringstream os;

  cout << name << endl;
  cout << " - reading" << endl;

  const vector<Domain> banned_domains = ReadDomains(input);
  const vector<Domain> domains_to_check = ReadDomains(input);
  cout << " - checking" << endl;

  stringstream actual;
  auto res = CheckDomains(banned_domains, domains_to_check);

  cout << " - printing" << endl;
  PrintCheckResults(res, actual);

  string a, e;
  while ((expected >> e) && (actual >> a)) {
    cout << a << endl;
    cout << e << endl;

    ASSERT_EQUAL(a, e);
    cout << "Ok" << endl;
  }
  ASSERT_EQUAL(expected.eof(), actual.eof());
}

void TestFiles(const string& name, const string& input_file, const string& output_file) {
  ifstream f_input("./test/" + input_file);
  ifstream f_output("./test/" + output_file);
  if (!f_input.is_open() || !f_output.is_open()) {
    throw domain_error("Failed to open test files");
  }
  RunIntegrationTest(name, f_input, f_output);
}

void Test1() {
  TestFiles("Test 1", "input-1.txt", "output-1.txt");
}

void RunIntegrationTests() {
  TestRunner tr;
  RUN_TEST(tr, Test1);
}

int main() {
#ifdef RUN_TESTS
  RunIntegrationTests();
#else
  ios::sync_with_stdio(false);
  cin.tie(nullptr);
  const vector<Domain> banned_domains = ReadDomains();
  const vector<Domain> domains_to_check = ReadDomains();

  PrintCheckResults(CheckDomains(banned_domains, domains_to_check));
  return 0;
#endif
}
