#include <iostream>
#include <string>
#include <vector>

class Book {
private:
    std::string title;
    std::string author;
    std::string ISBN;
    bool isBorrowed;

public:
    // �������� �����������
    Book(std::string t, std::string a, std::string isbn)
        : title(t), author(a), ISBN(isbn), isBorrowed(false) {
    }

    // ����������� ���������
    Book(const Book& other)
        : title(other.title), author(other.author), ISBN(other.ISBN), isBorrowed(other.isBorrowed) {
        std::cout << "Copy constructor called for " << title << std::endl;
    }

    // ����������� ����������
    Book(Book&& other) noexcept
        : title(std::move(other.title)), author(std::move(other.author)),
        ISBN(std::move(other.ISBN)), isBorrowed(other.isBorrowed) {
        other.isBorrowed = false;
        std::cout << "Move constructor called for " << title << std::endl;
    }

    ~Book() {}

    void borrowBook() { isBorrowed = true; }
    void returnBook() { isBorrowed = false; }
    std::string getTitle() const { return title; }
    bool getBorrowedStatus() const { return isBorrowed; }

    // �������������� ��������� ���������
    Book& operator=(const Book& other) {
        if (this != &other) {
            title = other.title;
            author = other.author;
            ISBN = other.ISBN;
            isBorrowed = other.isBorrowed;
        }
        return *this;
    }

    // �������������� �������� ��������� !
    bool operator!() const {
        return !isBorrowed;
    }

    // �������������� �������� ��������� +
    Book operator+(const Book& other) const {
        return Book(title + " & " + other.title, author + " & " + other.author, ISBN + " & " + other.ISBN);
    }

    // ������ �������� stream insertion
    friend std::ostream& operator<<(std::ostream& os, const Book& book);
};

// ��������� ��������� ��������� stream insertion
std::ostream& operator<<(std::ostream& os, const Book& book) {
    os << "Title: " << book.title << ", Author: " << book.author << ", ISBN: " << book.ISBN << ", Borrowed: " << (book.isBorrowed ? "Yes" : "No");
    return os;
}

class Reader {
private:
    std::string name;
    int readerId;
    std::vector<Book*> borrowedBooks;
    static int readerCount; // �������� ���� ��� ��������� �������

public:
    Reader(std::string n, int id) : name(n), readerId(id) {
        readerCount++;
    }

    ~Reader() {
        readerCount--;
    }

    void borrowBook(Book* book) {
        if (!book->getBorrowedStatus()) {
            book->borrowBook();
            borrowedBooks.push_back(book);
        }
    }

    void returnBook(Book* book) {
        book->returnBook();
        borrowedBooks.erase(std::remove(borrowedBooks.begin(), borrowedBooks.end(), book), borrowedBooks.end());
    }

    // ��������� ����� ��� ��������� ������� �������
    static int getReaderCount() {
        return readerCount;
    }

    // ������������ ��������� this
    void display() const {
        std::cout << "Reader: " << this->name << ", ID: " << this->readerId << std::endl;
    }
};

int Reader::readerCount = 0; // ����������� ���������� ����

int main() {
    Book book1("1984", "George Orwell", "123456789");
    Book book2("Animal Farm", "George Orwell", "987654321");

    // ������������ ������������ ���������
    Book book3 = book1;
    std::cout << book3 << std::endl;

    // ������������ ������������ ����������
    Book book4 = std::move(book2);
    std::cout << book4 << std::endl;

    // ������������ ��������������� �������� ��������� !
    if (!book1) {
        std::cout << book1.getTitle() << " is available." << std::endl;
    }

    // ������������ ��������������� �������� ��������� +
    Book book5 = book1 + book4;
    std::cout << book5 << std::endl;

    // ������������ ���������� ���� �� ������
    Reader reader1("Alice", 101);
    Reader reader2("Bob", 102);
    std::cout << "Total readers: " << Reader::getReaderCount() << std::endl;

    // ������������ ��������� this
    reader1.display();

    // ������ � const ��'�����
    const Book book6("Brave New World", "Aldous Huxley", "1122334455");
    std::cout << book6.getTitle() << std::endl; // ������ ������������ ������

    return 0;
}