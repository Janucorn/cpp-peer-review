#include <iostream>
#include <iomanip> // for std::setprecision
#include <string>
#include <vector>

using namespace std::literals;

namespace ebook {

void AddReader(std::vector<int>& users, std::vector<int>& pages, int user_id, int page);
double Cheer(const std::vector<int>& users, const std::vector<int>& pages, int user);
}

double ebook::Cheer(const std::vector<int>& users, const std::vector<int>& pages, int user) {
	int readed_page = users[user];
	if (users[user] == 0) {
		return 0;
	}
	// если только один пользователь дочитал до текущей страницы
	if (pages[readed_page] == 1) {
		return 1;
	}
	// pages[1] - всего читателей
	// из всех читателей оставляем только тех, которые не дочитали до текущей страницы readed_page
	return (pages[1] - pages[readed_page]) * 1.0 / (pages[1] - 1);
}

void ebook::AddReader(std::vector<int>& users, std::vector<int>& pages, int user_id, int page) {
	for (int i = users[user_id] + 1; i <= page; ++i) {
			++pages[i];
	}
	users[user_id] = page;
}

int main() {
	int count;
	std::cin >> count;
	static const int MAX_USERS = 100'000;
	static const int MAX_PAGES = 1000;
	// хранится номер страницы, до которой дочитал пользователь users[n]
	std::vector<int> users(MAX_USERS + 1);
	// хранится количество пользователей, дочитавших до страницы pages[m]
	std::vector<int> pages(MAX_PAGES + 1);
	std::vector<double> result;

	for (int i = 0; i < count; ++i) {
		std::string query;
		int user_id;
		std::cin >> query >> user_id;
		if (query == "CHEER"s) {
			result.push_back(ebook::Cheer(users, pages, user_id));
		}
		if (query == "READ"s) {
			int page;
			std::cin >> page;
			ebook::AddReader(users, pages, user_id, page);
		}
	}

	for (const auto& res : result) {
		std::cout << std::setprecision(6) << res << std::endl;
	}

}
/*
INPUT:				OUTPUT
12
CHEER 5				0
READ 1 10			(от 1 до 10 стр - только 1)
CHEER 1				1 (ни один из других пользоваелей не прочел меньше)
READ 2 5			(от 1 до 5 стр - уже 2)
READ 3 7			(от 1 до 5 стр - уже 3; от 6 до 7 - 2; от 8 до 10 - 1)
CHEER 2				0 (ни один из других пользователей не прочел меньше)
CHEER 3				0.5 (только пользователь 2 из 2 других прочел меньше)
READ 3 10			(от 6 до 10 - уже 2)
CHEER 3				0.5 (только пользователь 2 из 2 других прочел меньше)
READ 3 11			(от 10 до 11 - только 1)
CHEER 3				1 (пользователи 1 и 2 из 2 других прочли меньше)
CHEER 1				0.5 (только пользователь 2 из 2 других прочел меньше)
*/

/*
OUTPUT
0
1
0
0.5
0.5
1
0.5
*/