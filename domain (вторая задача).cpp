#include <algorithm>
#include <iostream>
#include <sstream>
#include <string>
#include <string_view>
#include <vector>

#include <cassert>
#include <iterator>

using namespace std;

// .com - домен
// .by.com - поддомен

class Domain {
public:
	// конструктор должен позволять конструировать из string, с сигнатурой определитесь сами
	Domain(string domain)
		: domain_(domain) {
		// записываем домен задом наперед и добавляем точку в конце, чтобы домен являлся префиксом
		// например "ur.cba." - префикс "ur.cba.zyx." 
		reverse(domain_.begin(), domain_.end());
		domain_.push_back('.');
	}

	// разработайте operator==
	bool operator==(const Domain& other) const {
		return domain_ == other.domain_;
	}

	// разработайте метод InSubdomain, принимающий другой домен и возвращающий true, если this его поддомен
	bool IsSubdomain(const Domain& other) const {
		// размер поддомена должен быть больше самого домена
		if (domain_.size() < other.domain_.size()) {
			return false;
		}
		if (domain_ == other.domain_) {
			return true;
		}

		// возвращает пару итераторов указывающих на первые расхождения между other.domain_ и domain_
		// в случае, если не находится ниодного несовпадения, то возвращаются итераторы до первого конца .end()
		auto it = mismatch(other.domain_.begin(), other.domain_.end(), domain_.begin());
		return it.first == other.domain_.end();
	}

	const string& GetDomain() const {
		return domain_;
	}

private:
	string domain_;
};

class DomainChecker {
public:
	// конструктор должен принимать список запрещенных доменов через пару итераторов
	template<typename It>
	DomainChecker(It first, It last)
		: forbidden_domains_(first, last) {

		// сортируем домены в в лексикографическом порядке
		std::sort(forbidden_domains_.begin(), forbidden_domains_.end(),
			[](const Domain& lhs, const Domain& rhs) {
				const auto& lhs_str = lhs.GetDomain();
				const auto& rhs_str = rhs.GetDomain();
				return lexicographical_compare(lhs_str.begin(), lhs_str.end(),
					rhs_str.begin(), rhs_str.end());
			}
		);
		// удаляем поддомены запрещенных доменов, так как они избыточны
		last = std::unique(forbidden_domains_.begin(), forbidden_domains_.end(), [](const Domain& lhs, const Domain& rhs) {
			// проверяем является ли домен поддоменом другого
			return rhs.IsSubdomain(lhs);
			});
		// удаляем ненужные элементы (поддомены)
		forbidden_domains_.erase(last, forbidden_domains_.end());
	}

	// разработайте метод IsForbidden, возвращающий true, если домен запрещен
	bool IsForbidden(const Domain& domain) const {
		auto it = upper_bound(forbidden_domains_.begin(), forbidden_domains_.end(), domain, [](const Domain& lhs, const Domain& rhs) {
			const auto& lhs_str = lhs.GetDomain();
			const auto& rhs_str = rhs.GetDomain();
			return lexicographical_compare(lhs_str.begin(), lhs_str.end(),
				rhs_str.begin(), rhs_str.end());
			});

		// в случае, когда результат поиска указывает на начало - значит проверяемый домен не в списке запрещенных
		if (it == forbidden_domains_.begin()) {
			return false;
		} else {
			// проверяем является ли домен поддоменом предыдущего элемента 
			return domain.IsSubdomain(*prev(it));
		}
	}

	const vector<Domain>& GetDomains() const {
		return forbidden_domains_;
	}

private:
	vector<Domain> forbidden_domains_;
};

// разработайте функцию ReadDomains, читающую заданное количестов доменов из стандартного входа
template <typename Number>
vector<Domain> ReadDomain(istream& input, Number num) {
	vector<Domain> domains;
	domains.reserve(num);
	string domain;
	for (size_t i = 0; i < static_cast<size_t>(num); ++i) {
		getline(input, domain);
		domains.push_back(domain);
	}
	return domains;
}

template <typename Number>
Number ReadNumberOnline(istream& input) {
	string line;
	getline(input, line);

	Number num;
	std::istringstream(line) >> num;

	return num;
}

void TestDomain() {
	string text = "com"s;
	Domain domain(text);
	assert(domain == Domain{"com"s});

	// проверка поддомена
	Domain domain3("bx.com"s);
	Domain domain4("xyz.bx.com"s);
	Domain domain5("com.bx"s);
	Domain domain6("bx.coms"s);
	
	assert(domain3.GetDomain() == "moc.xb."s);
	assert(domain4.GetDomain() == "moc.xb.zyx."s);
	assert(domain5.GetDomain() == "xb.moc."s);
	assert(domain6.GetDomain() == "smoc.xb."s);

	assert(domain.IsSubdomain(Domain{"com"s}) == true);
	// размер поддомена не может быть меньше домена
	assert(domain.IsSubdomain(Domain{"a.com"s}) == false);
	// bx.com - поддомен "com"
	assert(domain3.IsSubdomain(Domain{"com"s}) == true);
	// xyz.bx.com - поддомен "bx.com"
	assert(domain4.IsSubdomain(Domain{"bx.com"s}) == true);
	assert(domain5.IsSubdomain(Domain{"com"s}) == false);
	assert(domain6.IsSubdomain(Domain{"com"s}) == false);
	cout << "TestDomain OK"sv << endl;
}

void TestReadDomain() {
	stringstream stream;
	// вводим домены: com, maps.me, ngz.ru
	string str{"com\nmaps.me\ngdz.ru"s};
	stream << str;
	vector<Domain> domains = ReadDomain(stream, 3);
	assert(domains.size() == 3);
	assert(domains[0] == Domain{ "com"s });
	assert(domains[1] == Domain{ "maps.me"s });
	assert(domains[2] == Domain{ "gdz.ru"s });

	// maps.me - является поддоменом "me" и не являеется для "mee"
	assert(domains[1].IsSubdomain(Domain{ "me"s }) == true);
	assert(domains[1].IsSubdomain(Domain{ "mee"s }) == false);
	cout << "TestReadDomains OK"sv << endl;
}

void TestDomainChecker() {
	vector<Domain> forbidden = { 
		Domain{"gdz.ru"s},
		Domain{"maps.me"s},
		Domain{"m.gdz.ru"s},
		Domain{"com"s}
	};
	DomainChecker checker(forbidden.begin(), forbidden.end());
	// Название домена преобразуется в обратном порядке: maps.me -> em.spam
	const vector<Domain> expected_domains = {Domain{"maps.me"s}, Domain{"com"s}, Domain{"gdz.ru"s}};
	
	// поддомены (m.gbz.ru) должны быть исключены в итоговом списке
	assert(checker.GetDomains() == expected_domains);
	assert(checker.GetDomains()[0] == Domain{ "maps.me"s });

	// Проверяем запрещены ли домены
	assert(checker.IsForbidden(Domain{ "me"s }) == false);
	assert(checker.IsForbidden(Domain{ "mm.gdz.ru"s }) == true);
	
	cout << "TestDomainChecker OK"sv << endl;
}

void Tests() {
	TestDomain();
	TestReadDomain();
	TestDomainChecker();
}

int main() {
	Tests();
	const std::vector<Domain> forbidden_domains = ReadDomain(cin, ReadNumberOnline<size_t>(cin));
	DomainChecker checker(forbidden_domains.begin(), forbidden_domains.end());

	const std::vector<Domain> test_domains = ReadDomain(cin, ReadNumberOnline<size_t>(cin));
	for (const Domain& domain : test_domains) {
		cout << (checker.IsForbidden(domain) ? "Bad"sv : "Good"sv) << endl;
	}
}

/*INPUT:
4
gdz.ru
maps.me
m.gdz.ru
com
7
gdz.ru
gdz.com
m.maps.me
alg.m.gdz.ru
maps.com
maps.ru
gdz.ua
*/

/*OUTPUT:
Bad
Bad
Bad
Bad
Bad
Good
Good
*/