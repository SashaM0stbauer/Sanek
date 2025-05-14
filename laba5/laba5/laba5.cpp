Вот тобі код, наступним сообщеніям я скажу що потрібно зробити

#include <iostream>
#include <fstream>
#include <memory>
#include <vector>
#include <string>
#include <stdexcept>
#include <limits>
#include <algorithm>

using namespace std;

// Базовий клас для елементів бібліотеки
class LibraryItem {
protected:
    string title;
    string author;
    string id;

public:
    LibraryItem(const string& t, const string& a, const string& i) 
        : title(t), author(a), id(i) {}
    virtual ~LibraryItem() = default;

    virtual void display() const {
        cout << "Title: " << title << ", Author: " << author << ", ID: " << id;
    }

    virtual string toFileString() const {
        return title + "|" + author + "|" + id;
    }

    virtual void fromFileString(const string& data) {
        size_t pos1 = data.find('|');
        size_t pos2 = data.find('|', pos1 + 1);
        if (pos1 == string::npos || pos2 == string::npos) {
            throw invalid_argument("Invalid data format");
        }
        title = data.substr(0, pos1);
        author = data.substr(pos1 + 1, pos2 - pos1 - 1);
        id = data.substr(pos2 + 1);
    }
};

// Клас для книг
class Book : public LibraryItem {
    string ISBN;
    bool isBorrowed;

public:
    Book(const string& t = "", const string& a = "", const string& i = "", 
         const string& isbn = "", bool borrowed = false)
        : LibraryItem(t, a, i), ISBN(isbn), isBorrowed(borrowed) {}

    void display() const override {
        LibraryItem::display();
        cout << ", ISBN: " << ISBN << ", Status: " << (isBorrowed ? "Borrowed" : "Available") << endl;
    }

    string toFileString() const override {
        return LibraryItem::toFileString() + "|" + ISBN + "|" + (isBorrowed ? "1" : "0");
    }

    void fromFileString(const string& data) override {
        vector<string> parts;
        size_t start = 0;
        size_t end = data.find('|');
        
        while (end != string::npos) {
            parts.push_back(data.substr(start, end - start));
            start = end + 1;
            end = data.find('|', start);
        }
        parts.push_back(data.substr(start));

        if (parts.size() != 5) {
            throw invalid_argument("Invalid book data format");
        }

        title = parts[0];
        author = parts[1];
        id = parts[2];
        ISBN = parts[3];
        isBorrowed = parts[4] == "1";
    }
};

// Клас для журналів
class Magazine : public LibraryItem {
    int issueNumber;

public:
    Magazine(const string& t = "", const string& a = "", const string& i = "", int issue = 0)
        : LibraryItem(t, a, i), issueNumber(issue) {}

    void display() const override {
        LibraryItem::display();
        cout << ", Issue: " << issueNumber << endl;
    }

    string toFileString() const override {
        return LibraryItem::toFileString() + "|" + to_string(issueNumber);
    }

    void fromFileString(const string& data) override {
        size_t pos = data.rfind('|');
        if (pos == string::npos) {
            throw invalid_argument("Invalid magazine data format");
        }
        
        LibraryItem::fromFileString(data.substr(0, pos));
        issueNumber = stoi(data.substr(pos + 1));
    }
};

// Клас для користувача
class User {
    string name;
    vector<shared_ptr<LibraryItem>> borrowedItems;

public:
    User(const string& n = "") : name(n) {}

    void borrowItem(const shared_ptr<LibraryItem>& item) {
        borrowedItems.push_back(item);
        cout << "Item borrowed successfully!\n";
    }

    void displayBorrowed() const {
        if (borrowedItems.empty()) {
            cout << "No items borrowed.\n";
            return;
        }
        cout << "\nBorrowed items by " << name << ":\n";
        for (const auto& item : borrowedItems) {
            item->display();
        }
    }

    void saveToFile(ofstream& file) const {
        file << "USER|" << name << "\n";
        for (const auto& item : borrowedItems) {
            file << "ITEM|" << item->toFileString() << "\n";
        }
    }
};

// Клас для роботи з файлами
class FileManager {
    const string itemsFile = "library_items.dat";
    const string usersFile = "users_history.dat";

public:
    void saveItems(const vector<shared_ptr<LibraryItem>>& items) {
        ofstream file(itemsFile, ios::binary);
        if (!file.is_open()) {
            throw runtime_error("Cannot open file for writing: " + itemsFile);
        }

        for (const auto& item : items) {
            string type;
            if (dynamic_cast<Book*>(item.get())) {
                type = "BOOK|";
            } else if (dynamic_cast<Magazine*>(item.get())) {
                type = "MAGAZINE|";
            }
            file << type << item->toFileString() << "\n";
        }
    }

    vector<shared_ptr<LibraryItem>> loadItems() {
        vector<shared_ptr<LibraryItem>> items;
        ifstream file(itemsFile, ios::binary);
        if (!file.is_open()) {
            return items;
        }

        string line;
        while (getline(file, line)) {
            size_t typePos = line.find('|');
            if (typePos == string::npos) continue;

            string type = line.substr(0, typePos);
            string data = line.substr(typePos + 1);

            shared_ptr<LibraryItem> item;
            if (type == "BOOK") {
                item = make_shared<Book>();
            } else if (type == "MAGAZINE") {
                item = make_shared<Magazine>();
            } else {
                continue;
            }

            try {
                item->fromFileString(data);
                items.push_back(item);
            } catch (const exception& e) {
                cerr << "Error loading item: " << e.what() << endl;
            }
        }

        return items;
    }

    void saveUserHistory(const User& user) {
        ofstream file(usersFile, ios::app | ios::binary);
        if (!file.is_open()) {
            throw runtime_error("Cannot open file for writing: " + usersFile);
        }
        user.saveToFile(file);
    }

    vector<string> loadUserHistory() {
        vector<string> history;
        ifstream file(usersFile, ios::binary);
        if (!file.is_open()) {
            return history;
        }

        string line;
        while (getline(file, line)) {
            history.push_back(line);
        }
        return history;
    }
};

// Головний клас програми
class LibrarySystem {
    vector<shared_ptr<LibraryItem>> items;
    FileManager fileManager;
    User currentUser;
    const string adminPassword = "admin123";

    void clearInput() {
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
    }

    int getIntInput(const string& prompt) {
        int value;
        while (true) {
            cout << prompt;
            if (cin >> value) {
                clearInput();
                return value;
            }
            clearInput();
            cout << "Invalid input. Please enter a number.\n";
        }
    }

public:
    LibrarySystem() {
        try {
            items = fileManager.loadItems();
        } catch (const exception& e) {
            cerr << "Error loading items: " << e.what() << endl;
        }
    }

    ~LibrarySystem() {
        try {
            fileManager.saveItems(items);
        } catch (const exception& e) {
            cerr << "Error saving items: " << e.what() << endl;
        }
    }

    void run() {
        while (true) {
            cout << "\n=== Library System ===\n";
            cout << "1. Login as Admin\n";
            cout << "2. Login as User\n";
            cout << "3. View User History\n";
            cout << "4. Exit\n";
            
            int choice = getIntInput("Choose option: ");

            try {
                switch (choice) {
                    case 1: adminMenu(); break;
                    case 2: userMenu(); break;
                    case 3: viewHistory(); break;
                    case 4: return;
                    default: cout << "Invalid choice!\n";
                }
            } catch (const exception& e) {
                cerr << "Error: " << e.what() << endl;
            }
        }
    }

    void adminMenu() {
        cout << "Enter admin password: ";
        string password;
        getline(cin, password);

        if (password != adminPassword) {
            cout << "Incorrect password!\n";
            return;
        }

        while (true) {
            cout << "\n=== Admin Menu ===\n";
            cout << "1. Add Book\n";
            cout << "2. Add Magazine\n";
            cout << "3. List All Items\n";
            cout << "4. Back to Main Menu\n";
            
            int choice = getIntInput("Choose option: ");

            try {
                switch (choice) {
                    case 1: addBook(); break;
                    case 2: addMagazine(); break;
                    case 3: listItems(); break;
                    case 4: return;
                    default: cout << "Invalid choice!\n";
                }
            } catch (const exception& e) {
                cerr << "Error: " << e.what() << endl;
            }
        }
    }

    void userMenu() {
        cout << "Enter your name: ";
        string name;
        getline(cin, name);
        currentUser = User(name);

        while (true) {
            cout << "\n=== User Menu (" << name << ") ===\n";
            cout << "1. Browse Items\n";
            cout << "2. Borrow Item\n";
            cout << "3. View My Borrowed Items\n";
            cout << "4. Back to Main Menu\n";
            
            int choice = getIntInput("Choose option: ");

            try {
                switch (choice) {
                    case 1: listItems(); break;
                    case 2: borrowItem(); break;
                    case 3: currentUser.displayBorrowed(); break;
                    case 4: {
                        fileManager.saveUserHistory(currentUser);
                        return;
                    }
                    default: cout << "Invalid choice!\n";
                }
            } catch (const exception& e) {
                cerr << "Error: " << e.what() << endl;
            }
        }
    }

    void viewHistory() {
        try {
            auto history = fileManager.loadUserHistory();
            if (history.empty()) {
                cout << "No history found.\n";
                return;
            }
            
            cout << "\n=== User History ===\n";
            for (const auto& entry : history) {
                size_t pos = entry.find('|');
                if (pos != string::npos) {
                    cout << entry.substr(pos + 1) << endl;
                }
            }
        } catch (const exception& e) {
            cerr << "Error loading history: " << e.what() << endl;
        }
    }

    void addBook() {
        cout << "Enter title: ";
        string title;
        getline(cin, title);

        cout << "Enter author: ";
        string author;
        getline(cin, author);

        cout << "Enter ID: ";
        string id;
        getline(cin, id);

        cout << "Enter ISBN: ";
        string isbn;
        getline(cin, isbn);

        items.push_back(make_shared<Book>(title, author, id, isbn));
        cout << "Book added successfully!\n";
    }

    void addMagazine() {
        cout << "Enter title: ";
        string title;
        getline(cin, title);

        cout << "Enter author: ";
        string author;
        getline(cin, author);

        cout << "Enter ID: ";
        string id;
        getline(cin, id);

        int issue = getIntInput("Enter issue number: ");

        items.push_back(make_shared<Magazine>(title, author, id, issue));
        cout << "Magazine added successfully!\n";
    }

    void listItems() const {
        if (items.empty()) {
            cout << "No items available.\n";
            return;
        }
        
        cout << "\n=== Library Items (" << items.size() << ") ===\n";
        for (size_t i = 0; i < items.size(); ++i) {
            cout << i + 1 << ". ";
            items[i]->display();
        }
    }

    void borrowItem() {
        listItems();
        if (items.empty()) return;

        int index = getIntInput("Enter item number to borrow (0 to cancel): ");
        if (index == 0) return;
        
        if (index < 1 || index > static_cast<int>(items.size())) {
            cout << "Invalid item number!\n";
            return;
        }

        currentUser.borrowItem(items[index - 1]);
    }
};

int main() {
    try {
        LibrarySystem system;
        system.run();
    } catch (const exception& e) {
        cerr << "Fatal error: " << e.what() << endl;
        return 1;
    }
    return 0;
}