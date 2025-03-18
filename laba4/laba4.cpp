#include <iostream>
#include <string>
#include <vector>

// Базовий клас для всіх елементів бібліотеки
class LibraryItem {
protected:
    std::string title;
    std::string author;
    std::string id;

public:
    LibraryItem(std::string t, std::string a, std::string i) : title(t), author(a), id(i) {}
    virtual ~LibraryItem() {}

    virtual void display() const {
        std::cout << "Title: " << title << ", Author: " << author << ", ID: " << id << std::endl;
    }
};

// Клас для книг, який наслідує LibraryItem
class Book : public LibraryItem {
private:
    std::string ISBN;
    bool isBorrowed;

public:
    Book(std::string t, std::string a, std::string i, std::string isbn)
        : LibraryItem(t, a, i), ISBN(isbn), isBorrowed(false) {
    }

    void borrowBook() { isBorrowed = true; }
    void returnBook() { isBorrowed = false; }
    bool getBorrowedStatus() const { return isBorrowed; }

    void display() const override {
        LibraryItem::display();
        std::cout << "ISBN: " << ISBN << ", Borrowed: " << (isBorrowed ? "Yes" : "No") << std::endl;
    }
};

// Клас для журналів, який наслідує LibraryItem
class Magazine : public LibraryItem {
private:
    int issueNumber;

public:
    Magazine(std::string t, std::string a, std::string i, int issue)
        : LibraryItem(t, a, i), issueNumber(issue) {
    }

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
        borrowedItems.erase(std::remove(borrowedItems.begin(), borrowedItems.end(), item), borrowedItems.end());
    }

    void display() const {
        std::cout << "Reader: " << name << ", ID: " << readerId << std::endl;
        std::cout << "Borrowed Items: " << std::endl;
        for (const auto& item : borrowedItems) {
            item->display();
        }
    }
};

int main() {
    Book book1("1984", "George Orwell", "B001", "123456789");
    Magazine magazine1("National Geographic", "Various", "M001", 123);

    Reader reader("Alice", 101);
    reader.borrowItem(&book1);
    reader.borrowItem(&magazine1);

    reader.display();

    return 0;
}