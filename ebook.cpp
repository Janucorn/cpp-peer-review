#include <iostream>
#include <iomanip> // for std::setprecision
#include <string>
#include <vector>

using namespace std::literals;

class ReadersManager {
public:
	ReadersManager()
		: pages_to_user_(MAX_USERS + 1, 0)
		, users_to_page_(MAX_PAGES + 1, 0) {
	}
	void AddReader(int user_id, int page);
	void GetCheer(int user_id);
	const std::vector<double>& GetStatus() const;

private:
	static const int MAX_USERS = 100'000;
	static const int MAX_PAGES = 1000;
	// хранится номер страницы, до которой дочитал пользователь pages_to_user_[n]
	std::vector<int> pages_to_user_;
	// хранится количество пользователей, дочитавших до страницы users_to_page[m]
	std::vector<int> users_to_page_;
	std::vector<double> status_;
};

void ReadersManager::AddReader(int user_id, int page) {
	for (int i = pages_to_user_[user_id] + 1; i <= page; ++i) {
		++users_to_page_[i];
	}
	pages_to_user_[user_id] = page;
}

void ReadersManager::GetCheer(int user_id) {
	int page = pages_to_user_[user_id];
	if (page == 0) {
		status_.push_back(0.0);
	}
	// если только один пользователь дочитал до текущей страницы
	else if (users_to_page_[page] == 1) {
		status_.push_back(1.0);
	} else {
		const int total_users = users_to_page_[1];
		// из всех читателей оставляем только тех, которые не дочитали до текущей страницы page
		status_.push_back((total_users - users_to_page_[page]) * 1.0 / (total_users - 1));
	}
}

const std::vector<double>& ReadersManager::GetStatus() const {
	return status_;
}

void ProcessQuery(std::istream& input, ReadersManager& manager) {
	std::string query;
	int user_id;
	input >> query >> user_id;
	if (query == "CHEER"s) {
		manager.GetCheer(user_id);
	}
	if (query == "READ"s) {
		int page;
		input >> page;
		manager.AddReader(user_id, page);
	}
}

void PrintStatus(std::ostream& out, const ReadersManager& manager) {
	for (const auto& status : manager.GetStatus()) {
		out << std::setprecision(6) << status << std::endl;
	}
}

int main() {
	ReadersManager manager;
	int count;
	std::cin >> count;

	for (int i = 0; i < count; ++i) {
		ProcessQuery(std::cin, manager);
	}

	PrintStatus(std::cout, manager);
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