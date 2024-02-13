#include <iostream> 
#include <string>
#include <vector> 

using namespace std::string_literals;

#define ERROR_HINT(hint) std::cout << "Ошибка. "s << hint << std::endl;

struct StackElement {
	uint64_t data_ = 0;
	StackElement* next_ = nullptr;
	StackElement(uint64_t data) : data_(data), next_(nullptr)
	{}
};

struct Stack {
	size_t size_;
	size_t size_second_;
	StackElement* last_;
	StackElement* last_second_;
	enum class MenuStatus {
		EXIT_,
		CHECK_EMPTY_,
		APPEND_,
		DELETE_,
		PRINT_STACK_,
		PRINT_STACK_DELETED_ELEMENTS_,
		WAIT_CHOISE_
	};
	MenuStatus status_ = MenuStatus::WAIT_CHOISE_;

	Stack() : size_(0), size_second_(0), last_(nullptr), last_second_(nullptr)
	{}

	~Stack() {
		while (last_) {
			StackElement* temp = last_;
			last_ = last_->next_;
			delete temp;
		}
		while (last_second_) {
			StackElement* temp = last_second_;
			last_second_ = last_second_->next_;
			delete temp;
		}
	}

	bool isOnlyNumbers(const std::string& string, const char limit_begin, const char limit_end) {
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

	template<typename T_size>
	bool StackIsEmpty(const T_size& size) const {
		return size == 0;
	}

	uint64_t Randomize() {
		return rand() % 18446744073709551615 + 1;
	}

	template<typename T_last, typename T_size>
	void Append(T_last& last, T_size& size, const std::vector<uint64_t>& data) {
		for (const auto& num : data) {
			StackElement* new_element = new StackElement(num);
			new_element->next_ = last;
			last = new_element;
			++size;
		}
	}

	template<typename T_last, typename T_size>
	void Delete(T_last& last, T_size& size, const bool needappend = false) {
		if (StackIsEmpty(size)) {
			ERROR_HINT("Невозможно удалить элемент: стек пуст."s);
			return;
		}
		else {
			StackElement* temp = last;
			last = last_->next_;
			if (!needappend) std::cout << "Элемент удалён из стека."s << std::endl;
			else {
				Append(last_second_, size_second_, { temp->data_ });
				std::cout << "Элемент удалён из стека и добавлен в стек удалённых элементов."s << std::endl;
			}
			delete temp;
			--size;
		}
	}

	template<typename T_size, typename T_last>
	void PrintStack(const T_size& size, const T_last& last) const {
		if (StackIsEmpty(size)) {
			std::cout << "Стек пуст." << std::endl;
			return;
		}
		else {
			std::cout << "СОСТОЯНИЕ СТЕКА:" << std::endl;
			StackElement* current = last;
			size_t index = 0;
			while (current) {
				std::cout << "[" << index << "]: " << current->data_ << std::endl;
				current = current->next_;
				++index;
			}
			delete current;
		}
	}

	void Menu() {
		do {
			std::string user_input;
			std::cout << "ВВЕДИТЕ НОМЕР КОМАНДЫ"s << std::endl;
			std::cout << "1 >> Проверка пустоты стека"s << std::endl;
			std::cout << "2 >> Добавление элемента в вершину стека"s << std::endl;
			std::cout << "3 >> Удаление элемента из вершины стека"s << std::endl;
			std::cout << "4 >> Вывод текущего состояния стека на экран"s << std::endl;
			std::cout << "5 >> Вывод текущего состояния стека удалённых элементов на экран"s << std::endl;
			std::cout << "0 >> Завершение работы программы"s << std::endl;
			std::getline(std::cin, user_input);
			std::cout << std::endl;
			if (isCorrectMenuInput(user_input)) {
				switch (status_) {
				case Stack::MenuStatus::EXIT_:
					break;

				case Stack::MenuStatus::CHECK_EMPTY_:
					if (StackIsEmpty(size_)) {
						std::cout << "Стек пуст."s << std::endl;
					}
					else std::cout << "Стек не пуст."s << std::endl;
					std::cout << std::endl;
					break;

				case Stack::MenuStatus::APPEND_: {
					bool added = false;
					do
					{
						std::cout << "1 >> Создать новый элемент"s << std::endl;
						std::cout << "2 >> Восстановить элемент из стека удалённых элементов"s << std::endl;
						std::string menu;
						getline(std::cin, menu);

						if (isOnlyNumbers(menu, '1', '2') and menu == "1"s) {
							std::cout << "Укажите количество вводимых элементов: "s;
							std::string count;
							getline(std::cin, count);
							if (isOnlyNumbers(count, '0', '9') and stoi(count) != '0') {
								std::vector<uint64_t> nums;
								std::cout << "Чтобы сгенерировать случайное число, введите пустую строку."s << std::endl;
								do
								{
									std::string element;
									std::cout << "Введите добавляемый элемент (натуральное число или 0): "s;
									getline(std::cin, element);
									if (element.empty()) {
										uint64_t num = Randomize();
										nums.push_back(num);
									}
									else if (isOnlyNumbers(element, '0', '9')) {
										nums.push_back(stoi(element));
									}
									else ERROR_HINT("Ожидается натуральное число [1, inf) или 0.");
								} while (nums.size() != stoi(count));
								Append(last_, size_, nums);
								added = true;
							}
							else ERROR_HINT("Ожидается натуральное число [1, inf)."s);
						}

						else if (isOnlyNumbers(menu, '1', '2') and menu == "2"s) {
							std::cout << "Укажите количество восстанавливаемых элементов: "s;
							std::string count;
							getline(std::cin, count);
							if (isOnlyNumbers(count, '0', '9') and stoi(count) <= static_cast<int>(size_second_) and stoi(count) != '0') {
								for (size_t i = 0; i < static_cast<size_t>(stoi(count)); ++i) {
									StackElement* temp = last_second_;
									Append(last_, size_, { temp->data_ });
									last_second_ = last_second_->next_;
									delete temp;
									added = true;
								}
							}
							else { ERROR_HINT("Ожидается натуральное число [1, inf) и не больше содержимого корзины ("s << static_cast<int>(size_second_) << ")."s); std::cout << std::endl; }
						}

					} while (added != true);
					std::cout << std::endl;
				}break;

				case Stack::MenuStatus::DELETE_: {
					bool deleted = false;
					do
					{
						std::cout << "1 >> Удалить с освобождением памяти"s << std::endl;
						std::cout << "2 >> Удалить и внести в стек удалённых элементов"s << std::endl;
						std::string menu;
						getline(std::cin, menu);

						if (isOnlyNumbers(menu, '1', '2') and menu == "1"s) {
							Delete(last_, size_);
							deleted = true;
						}
						else if (isOnlyNumbers(menu, '1', '2') and menu == "2"s) {
							Delete(last_, size_, true);
							deleted = true;
						}
						else ERROR_HINT("Ожидается натуральное число [1,2].");
					} while (deleted != true);
					std::cout << std::endl;
				}break;

				case Stack::MenuStatus::PRINT_STACK_:
					PrintStack(size_, last_);
					std::cout << std::endl;
					break;

				case Stack::MenuStatus::PRINT_STACK_DELETED_ELEMENTS_:
					PrintStack(size_second_, last_second_);
					std::cout << std::endl;
					break;

				default:
					break;
				}
			}
			else ERROR_HINT("Номер команды может быть в интервале [0,5].");
		} while (status_ != MenuStatus::EXIT_);
	}
};

int main() {
	setlocale(LC_ALL, "RUS");
	Stack stack_object_;
	stack_object_.Menu();
	return 0;
}