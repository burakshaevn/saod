#include <iostream> 
#include <string>

using namespace std::string_literals;

#define ERROR_HINT(hint) std::cout << "������. "s << hint << std::endl;
const size_t QUERY_SIZE = 10;

struct Query {
	size_t size_, current_size_;
	int query_[QUERY_SIZE];
	int front, back;

	enum class MenuStatus {
		EXIT_,
		IS_EMPTY_,
		IS_FULLNESS_,
		APPEND_,
		DELETE_,
		CURRENT_STATUS_,
		WAIT_CHOISE
	};
	MenuStatus status_ = MenuStatus::WAIT_CHOISE;

	Query() {
		size_ = QUERY_SIZE;
		current_size_ = 0;
		front = 0;
		back = 0;
		query_[QUERY_SIZE] = {};
		/*for (auto& element : query_) {
			element = NULL;
		}*/
	}

	bool isOnlyNumbers(const std::string& string, const char limit_begin, const char limit_end) const {
		if (string.empty()) return false;
		else {
			for (const char& c : string) {
				if (c < limit_begin || c > limit_end) {
					return false;
				}
			}
			return true;
		}
	}

	bool isCorrectMenuInput(const std::string& string) {
		if (!isOnlyNumbers(string, '0', '5')) return false;
		else {
			status_ = static_cast<MenuStatus>(std::stoi(string));
			return true;
		}
	}

	bool QueryIsEmpty() const {
		return front == back;
	}

	bool QueryIsFull() const {
		return current_size_ == size_;
	}

	void Append(const int element) {
		if (QueryIsFull()) {
			ERROR_HINT("���������� �������� �������, ������� �����������."s);
		}
		else {
			query_[current_size_] = element;
			++current_size_;
			back = current_size_;
			std::cout << "������� �������� � �������."s << std::endl;
		}
	}

	void Delete() {
		if (QueryIsEmpty()) {
			ERROR_HINT("���������� ������� �������, ������� �����."s);
		}
		else {
			query_[current_size_] = 0;
			--current_size_;
			back = current_size_;
			std::cout << "������� ����� �� �������."s << std::endl;
		}
	}

	void PrintCurrentQuery() const {
		if (QueryIsEmpty()) {
			std::cout << "������� �����." << std::endl;
			return;
		}
		else {
			std::cout << "��������� �������:"s << std::endl; \
				for (int i = front; i < back; ++i) {
					std::cout << "["s << i << "]: "s << query_[i] << std::endl;
				}
		}
	}

	void Menu() {
		std::cout << "������� ����� �������"s << std::endl;
		do {
			std::cout << "1 >> �������� ������� �������"s << std::endl;
			std::cout << "2 >> �������� ������������� �������"s << std::endl;
			std::cout << "3 >> ���������� �������� � ������� �������"s << std::endl;
			std::cout << "4 >> �������� �������� �� ������� �������"s << std::endl;
			std::cout << "5 >> ����� �������� ��������� ������� �� �����"s << std::endl;
			std::cout << "0 >> ���������� ������ ���������"s << std::endl;
			std::string user_input;
			std::getline(std::cin, user_input);

			if (isCorrectMenuInput(user_input)) {
				switch (status_) {
				case Query::MenuStatus::EXIT_:
					break;

				case Query::MenuStatus::IS_EMPTY_:
					if (QueryIsEmpty()) {
						std::cout << "������� �����."s << std::endl;
					}
					else std::cout << "������� �� �����."s << std::endl;
					std::cout << std::endl;
					break;

				case Query::MenuStatus::IS_FULLNESS_:
					if (QueryIsFull()) {
						std::cout << "������� ���������."s << std::endl;
					}
					else std::cout << "������� �� ���������."s << std::endl;
					std::cout << std::endl;
					break;

				case Query::MenuStatus::APPEND_: {
					bool added = false;
					do
					{
						std::cout << "������� ����������� ������� (����������� ����� ��� 0): "s;
						std::string n_string = ""s;
						getline(std::cin, n_string);
						if (isOnlyNumbers(n_string, '0', '9')) {
							Append(stoi(n_string));
							added = true;
						}
						else { ERROR_HINT("��������� ����� ����� [1, inf)."); std::cout << std::endl; }
					} while (added != true);
				} std::cout << std::endl; break;

				case Query::MenuStatus::DELETE_: {
					bool deleted = false;
					do
					{
						Delete();
						deleted = true;
					} while (deleted != true);
				}std::cout << std::endl; break;

				case Query::MenuStatus::CURRENT_STATUS_:
					PrintCurrentQuery();
					std::cout << std::endl;
					break;

				default:
					break;
				}
			}
			else { ERROR_HINT("��������� ����������� ����� � ��������� [0,5]."); std::cout << std::endl; }
		} while (status_ != MenuStatus::EXIT_);
		std::cout << std::endl;
	}
};

int main() {
	setlocale(LC_ALL, "RUS");
	Query query;
	query.Menu();
	return 0;
}