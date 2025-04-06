#include <iostream>
#include <string>
#include <vector>
#include <memory>

// 1. ������� ���� LibraryItem � ����������� ���������
class LibraryItem {
protected:
    std::string title;
    std::string author;
    std::string id;

public:
    LibraryItem(std::string t, std::string a, std::string i)
        : title(t), author(a), id(i) {
    }

    // 4. ³��������� ���������� (������ �4)
    virtual ~LibraryItem() {
        std::cout << "LibraryItem destructor: " << title << std::endl;
    }

    // 2. ³������� ������� (������ �2)
    virtual void display() const {
        std::cout << "Title: " << title << ", Author: " << author << ", ID: " << id;
    }

    virtual void checkOut() {
        std::cout << "Checking out item: " << title << std::endl;
    }

    // 7. ����� ��������� ������� (������ �7)
    virtual void returnItem() = 0;

    // 5. Գ������� ����� (������ �5)
    virtual std::string getLocation() const final {
        return "Library Shelf A";
    }
};

// 8. ��������� ��� ���������� �������� (������ �8)
class IPrintable {
public:
    virtual ~IPrintable() = default;
    virtual void printInfo() const = 0;
};

// 3. ���� Book � ��������������� ����������� ��������� (������ �3)
class Book : public LibraryItem, public IPrintable {
private:
    std::string ISBN;
    bool isBorrowed;

public:
    Book(std::string t, std::string a, std::string i, std::string isbn)
        : LibraryItem(t, a, i), ISBN(isbn), isBorrowed(false) {
    }

    // �������������� ���������� �������
    void display() const override {
        LibraryItem::display();
        std::cout << ", ISBN: " << ISBN << ", Status: " << (isBorrowed ? "Borrowed" : "Available") << std::endl;
    }

    void checkOut() override {
        isBorrowed = true;
        std::cout << "Book checked out: " << title << std::endl;
    }

    void returnItem() override {
        isBorrowed = false;
        std::cout << "Book returned: " << title << std::endl;
    }

    // ��������� ����������
    void printInfo() const override {
        std::cout << "Printing book info: " << title << " by " << author << std::endl;
    }
};

// 5. Գ������� ���� (������ �5)
final class Magazine : public LibraryItem {
private:
    int issueNumber;

public:
    Magazine(std::string t, std::string a, std::string i, int issue)
        : LibraryItem(t, a, i), issueNumber(issue) {
    }

    // �������������� ���������� �������
    void display() const override {
        LibraryItem::display();
        std::cout << ", Issue: " << issueNumber << std::endl;
    }

    void returnItem() override {
        std::cout << "Magazine returned: " << title << std::endl;
    }
};

// 1. ������������ �������� ����'���� (������ �1)
class StaticBindingDemo {
public:
    void showInfo() { // �� ���������� �����
        std::cout << "Static binding demo - base class" << std::endl;
    }
};

class DerivedStatic : public StaticBindingDemo {
public:
    void showInfo() {
        std::cout << "Static binding demo - derived class" << std::endl;
    }
};

// ������� ��� ������������ �����������
void demonstratePolymorphism() {
    std::cout << "\n=== Demonstrating Polymorphism ===\n";

    // ��������� ��'����
    Book book("1984", "George Orwell", "B001", "123456789");
    Magazine magazine("National Geographic", "Various", "M001", 256);

    // 3. ��������� ���������� ����� �������� �� ������� ���� (������ �3)
    LibraryItem* item1 = &book;
    LibraryItem* item2 = &magazine;

    std::cout << "\nUsing base class pointers:\n";
    item1->display();
    item1->checkOut();
    item1->returnItem();

    item2->display();
    item2->returnItem();

    // 6. ��������� ���������� ����� ��������� (������ �6)
    LibraryItem& ref1 = book;
    LibraryItem& ref2 = magazine;

    std::cout << "\nUsing base class references:\n";
    ref1.display();
    ref2.display();

    // 8. ������������ ���������� (������ �8)
    IPrintable* printable = &book;
    std::cout << "\nUsing interface:\n";
    printable->printInfo();

    // 1. ������������ �������� ����'���� (������ �1)
    StaticBindingDemo staticObj;
    DerivedStatic derivedStaticObj;
    StaticBindingDemo* staticPtr = &derivedStaticObj;

    std::cout << "\nStatic binding demo:\n";
    staticObj.showInfo();    // ������ ������ �������� �����
    derivedStaticObj.showInfo(); // ������ ������ ��������� �����
    staticPtr->showInfo();   // �������� ����'���� - ������ ������ �������� �����
}

int main() {
    demonstratePolymorphism();

    // ��������� ������������ � smart pointers
    std::cout << "\n=== Smart Pointers Demo ===\n";
    std::unique_ptr<LibraryItem> item = std::make_unique<Book>("The Hobbit", "J.R.R. Tolkien", "B002", "555555555");
    item->display();
    item->returnItem();

    return 0;
}