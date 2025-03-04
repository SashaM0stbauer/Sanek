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
    Book(std::string t, std::string a, std::string isbn) : title(t), author(a), ISBN(isbn), isBorrowed(false) {}
    ~Book() {}

    void borrowBook() { isBorrowed = true; }
    void returnBook() { isBorrowed = false; }
    std::string getTitle() const { return title; }
    bool getBorrowedStatus() const { return isBorrowed; }
};

class Reader {
private:
    std::string name;
    int readerId;
    std::vector<Book*> borrowedBooks;

public:
    Reader(std::string n, int id) : name(n), readerId(id) {}
    ~Reader() {}

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
};

class Librarian {
private:
    std::string name;
    int employeeId;

public:
    Librarian(std::string n, int id) : name(n), employeeId(id) {}
    ~Librarian() {}

    void addBook(Book* book, std::vector<Book*>& library) {
        library.push_back(book);
    }
};

int main() {
    std::vector<Book*> library;
    Librarian librarian("John Doe", 1);
    Book book1("1984", "George Orwell", "123456789");
    librarian.addBook(&book1, library);

    Reader reader("Alice", 101);
    reader.borrowBook(&book1);

    std::cout << "Book borrowed status: " << book1.getBorrowedStatus() << std::endl;

    return 0;
}