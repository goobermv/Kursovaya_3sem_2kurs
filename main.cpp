#include <iostream>
#include <string>
#include <vector>
#include <algorithm>

class LibraryVisitor { //Класс содержащий информацию о посетителе библиотеки 
public:
    std::string lib_num;
    std::string name;
    std::string address;
    int num_of_books;
};

class HashNode { //Класс ячейки в таблице
public:
    LibraryVisitor value;
    int key;
    bool was_deleted = false;

    HashNode(LibraryVisitor val, int k) : value(val), key(k), was_deleted(false) {} //Конструктор для ячейки
};

class HashMap { //Класс хэн таблицы
    HashNode** arr;
    int capacity;
    int size;

    void resize() { //Вспомогательная функция для увеличения таблицы
        int oldCapacity = capacity;
        capacity *= 2;
        HashNode** newArr = new HashNode*[capacity];

        for (int i = 0; i < capacity; i++)
            newArr[i] = nullptr;

        for (int i = 0; i < oldCapacity; i++) {
            if (arr[i] != nullptr && !arr[i]->was_deleted) {
                int newHashIndex = hashFunction(arr[i]->key);
                while (newArr[newHashIndex] != nullptr) {
                    newHashIndex++;
                    newHashIndex %= capacity;
                }
                newArr[newHashIndex] = arr[i];
            }
        }

        delete[] arr;
        arr = newArr;
    }

public:
    HashMap() { //Конструктор хэш таблицы
        capacity = 1;
        size = 0;
        arr = new HashNode*[capacity];

        for (int i = 0; i < capacity; i++)
            arr[i] = nullptr;
    }

    int hashFunction(int k) { //Хэш функция (метод умнодения)
        const double A = 0.61803;
        double fractionalPart = k * A - static_cast<int>(k * A);
        return static_cast<int>(capacity * fractionalPart);
    }

    void insertNode(LibraryVisitor v, int k) { //Метод добавления нового человека в систему
        if (getNode(k)) {
            std::cout << "This library card number already exists in the system.\n";
            return;
        }
        
        if (size >= capacity * 0.7) {
            resize();
        }

        HashNode* temp = new HashNode(v, k);
        int hashIndex = hashFunction(k);

        while (arr[hashIndex] != nullptr && (arr[hashIndex]->key != k || arr[hashIndex]->was_deleted)) {
            if (arr[hashIndex]->was_deleted) {
                delete arr[hashIndex];
                arr[hashIndex] = temp;
                size++;
                std::cout << "New person has been added to the system!\n";
                return;
            }
            hashIndex++;
            hashIndex %= capacity;
        }

        if (arr[hashIndex] == nullptr) {
            size++;
        }
        
        arr[hashIndex] = temp;
        arr[hashIndex]->was_deleted = false;
        std::cout << "New person has been added to the system!\n";
    }

    LibraryVisitor deleteNode(int k) { //Метод удаления человека по ключу
        int hashIndex = hashFunction(k);

        while (arr[hashIndex] != nullptr) {
            if (arr[hashIndex]->key == k && !arr[hashIndex]->was_deleted) {
                arr[hashIndex]->was_deleted = true;
                size--;
                std::cout << "Person has been removed from system\n";
                return arr[hashIndex]->value;
            }
            hashIndex++;
            hashIndex %= capacity;
        }

        std::cout << "This library card number does not exist in the system.\n";
        return LibraryVisitor();
    }

    bool getNode(int k) { //Метод поиска человека по ключу
        int hashIndex = hashFunction(k);
        int counter = 0;

        while (arr[hashIndex] != nullptr) {
            if (counter++ > capacity) {
                std::cout << "This library card number does not exist in the system.\n";
                return false;
            }
            if (arr[hashIndex]->key == k && !arr[hashIndex]->was_deleted) {
                std::cout << "\nLibrary card number: " << arr[hashIndex]->value.lib_num;
                std::cout << "\nName: " << arr[hashIndex]->value.name;
                std::cout << "\nAddress: " << arr[hashIndex]->value.address;
                std::cout << "\nNumber of books taken: " << arr[hashIndex]->value.num_of_books << "\n";
                return true;
            }
            hashIndex++;
            hashIndex %= capacity;
        }
        std::cout << "This library card number does not exist in the system.\n";
        return false;
    }

    void showAllNodes() { //Метод вывода информации обо всех в системе
        bool isEmpty = true;
        for (int i = 0; i < capacity; i++) {
            if (arr[i] != nullptr && !arr[i]->was_deleted) {
                std::cout << "Card Number: " << arr[i]->value.lib_num << ", Name: " << arr[i]->value.name << ", Address: "
                          << arr[i]->value.address << ", Number of books held: " << arr[i]->value.num_of_books << "\n";
                isEmpty = false;
            }
        }
        if (isEmpty) {
            std::cout << "The system is empty.\n";
        }
    }

    void clearAllNodes() { //Метод удаления всех записей 
        for (int i = 0; i < capacity; i++) {
            if (arr[i] != nullptr) {
                delete arr[i];
                arr[i] = nullptr;
            }
        }
        size = 0;
        std::cout << "Everyone has been removed from the system.\n";
    }

    std::vector<int> computeLPSArray(const std::string& pattern) { //Вспомогательный метод для алгоритма КМП 
        int M = pattern.length();
        std::vector<int> lps(M);
        int length = 0;
        int i = 1;

        while (i < M) {
            if (pattern[i] == pattern[length]) {
                length++;
                lps[i] = length;
                i++;
            } else {
                if (length != 0) {
                    length = lps[length - 1];
                } else {
                    lps[i] = 0;
                    i++;
                }
            }
        }
        return lps;
    }

    bool KMPSearch(const std::string& text, const std::string& pattern) { //Метод поиска по алгоритму КМП
        int N = text.length();
        int M = pattern.length();
        std::vector<int> lps = computeLPSArray(pattern);
        int i = 0;
        int j = 0;

        while (i < N) {
            if (pattern[j] == text[i]) {
                i++;
                j++;
            }
            if (j == M) {
                return true;
            } else if (i < N && pattern[j] != text[i]) {
                if (j != 0) {
                    j = lps[j - 1];
                } else {
                    i++;
                }
            }
        }
        return false;
    }

    void searchByAddress(const std::string& address) { //Метод для вызова функции поиска по адресу
        bool found = false;
        for (int i = 0; i < capacity; i++) {
            if (arr[i] != nullptr && !arr[i]->was_deleted && KMPSearch(arr[i]->value.address, address)) {
                std::cout << "Library Card Number: " << arr[i]->value.lib_num << "\n";
                std::cout << "Name: " << arr[i]->value.name << "\n";
                std::cout << "Address: " << arr[i]->value.address << "\n";
                std::cout << "Number of Books: " << arr[i]->value.num_of_books << "\n\n";
                found = true;
            }
        }
        if (!found) {
            std::cout << "No records found for the address: " << address;
        }
    }
};

void menu(HashMap map){ //Функция меню
    int action;
    std::cout << "\nChoose an action:\n";
    std::cout << "1 - Add a new person to the system\n";
    std::cout << "2 - Delete a person from the system\n";
    std::cout << "3 - View all registered individuals in the system\n";
    std::cout << "4 - Delete all registered people from the system\n";
    std::cout << "5 - Search for a person by their library card number\n";
    std::cout << "6 - Search for a person by their address\n";
    std::cout << "0 - Exit\n";
    std::cin >> action;

    switch (action){ 
    case 1: //Кейс ввода нового человека
    {
        LibraryVisitor new_per;

        bool validNumber = false;
        while (!validNumber) { //Проверка на правильность ввода
            std::cout << "Enter new library card number (6 digit number): ";
            std::cin >> new_per.lib_num;
            std::cin.ignore();

            if (new_per.lib_num.length() == 6 && std::all_of(new_per.lib_num.begin(), new_per.lib_num.end(), ::isdigit)) {
                validNumber = true;
            } else {
                std::cout << "Invalid input. Please enter a 6-digit number.\n";
            }
        }
        
        std::cout << "Enter name: ";
        getline(std::cin, new_per.name);

        std::cout << "Enter address: ";
        getline(std::cin, new_per.address);

        std::cout << "Enter number of taken books: ";
        std::cin >> new_per.num_of_books;

        int k  = std::stoi(new_per.lib_num);
        map.insertNode(new_per, k);

        menu(map);
        break;
    }

    case 2: //Кейс удаления человека из системы
    {
        int k;

        bool validNumber = false;
        while (!validNumber) { //Проверка на правильность ввода
            std::cout << "Enter library card number to delete(6 digit number): ";
            std::cin >> k;
            std::cin.ignore();

            std::string str = std::to_string(k);

            if (str.length() == 6) {
                validNumber = true;
            } else {
                std::cout << "Invalid input. Please enter a 6-digit number.\n";
            }
        }

        map.deleteNode(k);

        menu(map);
        break;
    }

    case 3: //Кейс вывода информации обо всех
    {
        map.showAllNodes();
        menu(map);
        break;
    }

    case 4: //Кейс удаления всех из системы 
    {
        map.clearAllNodes();
        menu(map);
        break;
    }

    case 5: //Кейс поиска человека по ключу
    {
        int k;
        bool validNumber = false;
        while (!validNumber) { //Проверка на правильность ввода
            std::cout << "Enter library card number to find(6 digit number): ";
            std::cin >> k;
            std::cin.ignore();

            std::string str = std::to_string(k);

            if (str.length() == 6) {
                validNumber = true;
            } else {
                std::cout << "Invalid input. Please enter a 6-digit number.\n";
            }
        }

        map.getNode(k);

        menu(map);
        break;
    }

    case 6: //Кейс поиска человека по адресу
    {
        std::string address;
        std::cout << "Enter an address: ";
        std::cin.ignore();
        getline(std::cin, address);

        map.searchByAddress(address);

        menu(map);
        break;
    }

    case 0: //Кейс выхода из программы
        break;
    default:
        break;
    }
}

int main(){
    HashMap map;
    std::cout << "Welcome to the library storage system!\n";
    menu(map);
    return 0;
}