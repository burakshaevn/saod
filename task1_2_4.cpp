#include <iostream> 
#include <string>
#include <vector> 

using namespace std::string_literals;

#define ERROR_HINT(hint) std::cout << "Ошибка. "s << hint << std::endl;

struct StackElement {
	uint64_t data_;
	StackElement* next_;

	StackElement() : data_(0), next_(nullptr)
	{}
	StackElement(uint64_t data, StackElement* next) : data_(data), next_(next)
	{}

} *stack_ = nullptr, * delete_stack_ = nullptr;

struct Stack {
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
	size_t size_ = 0;
	size_t deleted_size_ = 0;

	~Stack() {
		while (stack_) {
			StackElement* current = stack_;
			stack_ = stack_->next_;
			delete current;
		}
		while (delete_stack_) {
			StackElement* current_deleted = delete_stack_;
			delete_stack_ = delete_stack_->next_;
			delete current_deleted;
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

	bool StackIsEmpty(const size_t& size) const {
		return size == 0;
	}

	uint64_t Randomize() {
		return rand() % 18446744073709551615 + 1;
	}

	[[nodiscard]] bool Append(const std::vector<uint64_t>& data) {
		for (const auto& element : data) {
			StackElement* new_element = new StackElement(element, stack_);
			stack_ = new_element;
			++size_;
		}
		return true;
	}

	[[nodiscard]] bool Moving(StackElement** from, StackElement** to, size_t& from_size, size_t& to_size) {
		if (StackIsEmpty(from_size)) {
			ERROR_HINT("Невозможно произвести перемещение: источник пуст.");
			return false;
		}
		else {
			StackElement* new_element = new StackElement((*from)->data_, *to);
			*to = new_element;
			++to_size;
			(void)Delete(from, from_size);
			return true;
		}
	}


	[[nodiscard]] bool Delete(StackElement** from, size_t& from_size) {
		if (StackIsEmpty(from_size)) {
			ERROR_HINT("Невозможно произвести удаление: стек пуст."s);
			return false;
		}
		else {
			StackElement* temp = *from;
			*from = (*from)->next_;
			delete temp;
			--from_size;
			return true;
		}
	}

	void PrintStack(StackElement* stack, const size_t& size) const {
		std::cout << "СОСТОЯНИЕ СТЕКА:" << std::endl;
		if (StackIsEmpty(size)) {
			std::cout << "Стек пуст." << std::endl;
			return;
		}
		else {
			StackElement* current = stack;
			bool isFirst = true;
			while (current != nullptr) {
				if (isFirst) {
					std::cout << current->data_;
					current = current->next_;
					isFirst = false;
				}
				else {
					std::cout << ", "s << current->data_;
					current = current->next_;
				}
			}
			std::cout << ". "s << std::endl;
			delete current;
		}
	}

	void Menu() {
		std::cout << "ВВЕДИТЕ НОМЕР КОМАНДЫ"s << std::endl;
		do {
			std::cout << "1 >> Проверка пустоты стека"s << std::endl;
			std::cout << "2 >> Добавление элемента в вершину стека"s << std::endl;
			std::cout << "3 >> Удаление элемента из вершины стека"s << std::endl;
			std::cout << "4 >> Вывод текущего состояния стека на экран"s << std::endl;
			std::cout << "5 >> Вывод текущего состояния стека удалённых элементов на экран"s << std::endl;
			std::cout << "0 >> Завершение работы программы"s << std::endl;
			std::string user_input;
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
					do {
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
								do {
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
									else ERROR_HINT("Ожидается натуральное число или 0.");

								} while (nums.size() != stoi(count));
								if (Append(nums)) {
									std::cout << "Добавление выполнено успешно."s << std::endl;
									added = true;
								}
							}
							else ERROR_HINT("Ожидается натуральное число или 0."s);
						}

						else if (isOnlyNumbers(menu, '1', '2') and menu == "2"s) {
							std::cout << "Укажите количество восстанавливаемых элементов: "s;
							std::string count;
							getline(std::cin, count);
							if (isOnlyNumbers(count, '0', '9') and stoi(count) <= static_cast<int>(deleted_size_) and stoi(count) != '0') {
								for (size_t i = 0; i < static_cast<size_t>(stoi(count)); ++i) {
									Moving(&delete_stack_, &stack_, deleted_size_, size_);
									added = true;
								}
							}
							else { ERROR_HINT("Ожидается натуральное число, не превышающее число содержимого корзины ("s << static_cast<int>(deleted_size_) << ")."s); std::cout << std::endl; }
						}

					} while (added != true);
					std::cout << std::endl;
				}break;

				case Stack::MenuStatus::DELETE_: {
					bool deleted = false;
					do {
						std::cout << "1 >> Удалить с освобождением памяти"s << std::endl;
						std::cout << "2 >> Удалить и внести в стек удалённых элементов"s << std::endl;
						std::string menu;
						getline(std::cin, menu);

						if (isOnlyNumbers(menu, '1', '2') and menu == "1"s) {
							if (Delete(&stack_, size_)) {
								std::cout << "Удаление выполнено успешно."s << std::endl;
							}
						}
						else if (isOnlyNumbers(menu, '1', '2') and menu == "2"s) {
							if (Moving(&stack_, &delete_stack_, size_, deleted_size_)) {
								std::cout << "Удаление и перенос выполнены успешно."s << std::endl;
								deleted = true;
							}
						}
						else ERROR_HINT("Ожидается число в интервале [1,2].");
					} while (deleted != true);
					std::cout << std::endl;
				}break;

				case Stack::MenuStatus::PRINT_STACK_:
					PrintStack(stack_, size_);
					std::cout << std::endl;
					break;

				case Stack::MenuStatus::PRINT_STACK_DELETED_ELEMENTS_:
					PrintStack(delete_stack_, deleted_size_);
					std::cout << std::endl;
					break;

				default:
					break;
				}
			}
			else ERROR_HINT("Ожидается число в интервале [0,5].");
		} while (status_ != MenuStatus::EXIT_);
	}
};

int main() {
	setlocale(LC_ALL, "RUS");
	Stack stack_object_;
	stack_object_.Menu();
	return 0;
}