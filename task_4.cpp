#include <iostream> 
#include <string>

using namespace std::string_literals;

#define ERROR_HINT(hint) std::cout << "������. "s << hint << std::endl;
const size_t STACK_SIZE = 10;

struct Stack {
	size_t size_; // ����������� �������
	size_t current_size_; // ������� ���������� ��������� � �������
	int stack_[STACK_SIZE]; // ������������� ����� 
	enum class MenuStatus {
		EXIT,
		CHECK_EMPTY_STACK, 
		APPEND,
		DELETE,
		CURRENT_STACK_STATUS,
		WAIT_CHOISE
	};
	MenuStatus status_ = MenuStatus::WAIT_CHOISE;

	Stack() {
		size_ = STACK_SIZE;
		current_size_ = 0;
		for (auto& element : stack_) {
			element = NULL;
		}
	}

	//~Stack() {
	//	//delete[] stack_;
	//}

	bool isCorrectMenuInput(const std::string& string) {
		if (string.empty() or string.length() > 1 or
			(string != "0"s and string != "1"s and
				string != "2"s and string != "3"s and string != "4"s)) {
			return false;
		}
		else { status_ = static_cast<MenuStatus>(std::stoi(string)); return true; }
	}
	
	bool isOnlyNumbers(const std::string& string) {
		for (const char& c : string) {
			if (c < '0' || c > '9') {
				return false;
			}
		}
		return true;
	}

	bool StackIsEmpty() {
		if (current_size_ == 0) return true;
		else return false;
	}

	bool StackIsFull() {
		if (current_size_ == size_) return true;
		else return false;
	}

	void Append(const int element) {
		if (StackIsFull()) {
			ERROR_HINT("���������� �������� �������, ���� ����������."s);
		}
		else {
			stack_[current_size_] = element;
			++current_size_;
			std::cout << "������� �������� � ����."s << std::endl;
		}
	}
	
	void Delete() {
		if (StackIsEmpty()) {
			ERROR_HINT("���������� ������� �������, ���� ����."s);
		}
		else {
			stack_[current_size_] = 0;
			--current_size_;
			std::cout << "������� ����� �� �����."s << std::endl;
		}
	}

	void PrintCurrentStack() {
		if (StackIsEmpty()) {
			std::cout << "���� ����." << std::endl;
			return;
		}
		else {
			std::cout << "��������� �����:"s << std::endl;
			for (size_t i = 0; i < size_; ++i){
				std::cout << "["s << i << "]: "s << stack_[i] << std::endl;
			}
		}
	}

	void Menu() {
		do {
			std::string user_input; 
			std::cout << "������� ����� �������"s << std::endl;
			std::cout << "1 >> �������� ������� �����"s << std::endl;
			std::cout << "2 >> ���������� �������� � ������� �����"s << std::endl;
			std::cout << "3 >> �������� �������� �� ������� �����"s << std::endl;
			std::cout << "4 >> ����� �������� ��������� ����� �� �����"s << std::endl;
			std::cout << "0 >> ���������� ������ ���������"s << std::endl;
			std::getline(std::cin, user_input);
			if (isCorrectMenuInput(user_input)) {
				switch (status_) {
				case Stack::MenuStatus::EXIT:
					break;

				case Stack::MenuStatus::CHECK_EMPTY_STACK:
					if (StackIsEmpty()) {
						std::cout << "���� ����."s << std::endl;
					}
					else std::cout << "���� �� ����."s << std::endl;
					break;

				case Stack::MenuStatus::APPEND: {
					bool added = false;
					do
					{
						std::cout << "������� ����������� ������� (����������� �����): "s;
						std::string n_string = ""s;
						getline(std::cin, n_string);
						if (isOnlyNumbers(n_string)) {
							Append(stoi(n_string));
							added = true;
						}
						else ERROR_HINT("��������� ����������� ����� [1, inf).");
					} while (added != true);
				}
					break;

				case Stack::MenuStatus::DELETE: {
					bool deleted = false;
					do
					{
						Delete();
						deleted = true;
					} while (deleted != true);
				}
					break;

				case Stack::MenuStatus::CURRENT_STACK_STATUS:
					PrintCurrentStack();
					break;

				default:
					break;
				}
			}
			else ERROR_HINT("����� ������� ����� ���� � ��������� [0,4].");
		} while (status_ != MenuStatus::EXIT);
	}
};

int main() {
	setlocale(LC_ALL, "RUS");
	Stack stack_object_;
	stack_object_.Menu();
	return 0;
}