#include <iostream>
#include <string>
#include <vector>
#include <algorithm>

// Базовий клас для всіх елементів бібліотеки
class LibraryItem {
protected:
    std::string title;
    std::string author;
    std::string id;

public:
    LibraryItem(std::string t, std::string a, std::string i) 
        : title(t), author(a), id(i) {}

    // Віртуальний деструктор для коректного видалення дочірніх класів
    virtual ~LibraryItem() {}

    // Віртуальний метод для виведення інформації
    virtual void display() const {
        std::cout << "Title: " << title 
                  << ", Author: " << author 
                  << ", ID: " << id << std::endl;
    }

    // Оператор присвоєння для базового класу
    LibraryItem& operator=(const LibraryItem& other) {
        if (this != &other) {
            title = other.title;
            author = other.author;
            id = other.id;
        }
        return *this;
    }
};

// Клас для книг
class Book : public LibraryItem {
private:
    std::string ISBN;
    bool isBorrowed;

public:
    // Основний конструктор
    Book(std::string t, std::string a, std::string i, std::string isbn) 
        : LibraryItem(t, a, i), ISBN(isbn), isBorrowed(false) {}

    // Конструктор копіювання
    Book(const Book& other) 
        : LibraryItem(other), ISBN(other.ISBN), isBorrowed(other.isBorrowed) {
        std::cout << "Book Copy Constructor called for " << title << std::endl;
    }

    // Конструктор переміщення
    Book(Book&& other) noexcept 
        : LibraryItem(std::move(other)), 
          ISBN(std::move(other.ISBN)), 
          isBorrowed(other.isBorrowed) {
        other.isBorrowed = false;
        std::cout << "Book Move Constructor called for " << title << std::endl;
    }

    // Оператор присвоєння
    Book& operator=(const Book& other) {
        if (this != &other) {
            LibraryItem::operator=(other);
            ISBN = other.ISBN;
            isBorrowed = other.isBorrowed;
        }
        return *this;
    }

    // Методи для роботи з книгою
    void borrowBook() { isBorrowed = true; }
    void returnBook() { isBorrowed = false; }
    bool getBorrowedStatus() const { return isBorrowed; }

    // Перевизначений метод display
    void display() const override {
        LibraryItem::display();
        std::cout << "ISBN: " << ISBN 
                  << ", Borrowed: " << (isBorrowed ? "Yes" : "No") << std::endl;
    }
};

// Клас для журналів
class Magazine : public LibraryItem {
private:
    int issueNumber;

public:
    Magazine(std::string t, std::string a, std::string i, int issue) 
        : LibraryItem(t, a, i), issueNumber(issue) {}

    void display() const override {
        LibraryItem::display();
        std::cout << "Issue Number: " << issueNumber << std::endl;
    }
};

// Клас для читачів
class Reader {
private:
    std::string name;
    int readerId;
    std::vector<LibraryItem*> borrowedItems;

public:
    Reader(std::string n, int id) : name(n), readerId(id) {}

    void borrowItem(LibraryItem* item) {
        borrowedItems.push_back(item);
    }

    void returnItem(LibraryItem* item) {
        borrowedItems.erase(
            std::remove(borrowedItems.begin(), borrowedItems.end(), item), 
            borrowedItems.end()
        );
    }

    void display() const {
        std::cout << "Reader: " << name << ", ID: " << readerId << std::endl;
        std::cout << "Borrowed Items:" << std::endl;
        for (const auto& item : borrowedItems) {
            item->display();
        }
    }
};

// Демонстрація роботи Copy/Move конструкторів та operator=
void demonstrateBookOperations() {
    std::cout << "\n=== Demonstrating Book Operations ===\n";
    
    Book book1("1984", "George Orwell", "B001", "123456789");
    std::cout << "\nOriginal book1:\n";
    book1.display();

    // Копіювання
    Book book2 = book1;
    std::cout << "\nCopied book2:\n";
    book2.display();

    // Переміщення
    Book book3 = std::move(book1);
    std::cout << "\nMoved book3 (from book1):\n";
    book3.display();
    std::cout << "Original book1 after move:\n";
    book1.display();  // book1 тепер у невизначеному стані

    // Присвоєння
    Book book4("Animal Farm", "George Orwell", "B002", "987654321");
    std::cout << "\nBefore assignment book4:\n";
    book4.display();
    book4 = book2;
    std::cout << "After assignment book4:\n";
    book4.display();
}

int main() {
    // Створення елементів бібліотеки
    Book book("The Hobbit", "J.R.R. Tolkien", "B003", "555555555");
    Magazine magazine("National Geographic", "Various", "M001", 256);

    // Створення читача
    Reader reader("Alice", 101);
    reader.borrowItem(&book);
    reader.borrowItem(&magazine);

    // Виведення інформації
    std::cout << "=== Library System ===\n";
    reader.display();

    // Демонстрація Copy/Move/Assignment
    demonstrateBookOperations();

    return 0;
}