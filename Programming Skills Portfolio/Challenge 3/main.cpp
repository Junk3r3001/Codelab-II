#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <iomanip>

class Book {
private:
    std::string title;
    std::string author;
    int pages;
    std::string id;
    bool borrowed;

public:
    Book(std::string t, std::string a, int p, std::string i, bool b)
        : title(t), author(a), pages(p), id(i), borrowed(b) {}

    std::string getId() const { return id; }
    std::string getTitle() const { return title; }
    bool isBorrowed() const { return borrowed; }

    void setBorrowed(bool status) { borrowed = status; }

    void displayRow() const {
        std::cout << std::left << std::setw(6) << id
                  << std::setw(30) << title
                  << std::setw(22) << author
                  << std::setw(8) << pages
                  << (borrowed ? "Borrowed" : "Available") << "\n";
    }

    void displayDetails() const {
        std::cout << "\nID:       " << id << "\n"
                  << "Title:    " << title << "\n"
                  << "Author:   " << author << "\n"
                  << "Pages:    " << pages << "\n"
                  << "Status:   " << (borrowed ? "Borrowed" : "Available") << "\n";
    }
};

std::string cleanString(std::string s) {
    while (!s.empty() && (s.back() == '\r' || s.back() == '\n' || s.back() == ' ')) {
        s.pop_back();
    }
    return s;
}

std::vector<Book> loadBooks(const std::string& filename) {
    std::vector<Book> books;
    std::ifstream file(filename);
    std::string line;

    while (std::getline(file, line)) {
        if (line.empty()) continue;
        std::stringstream ss(line);
        std::string title, author, pagesStr, id, statusStr;

        if (std::getline(ss, title, ',') &&
            std::getline(ss, author, ',') &&
            std::getline(ss, pagesStr, ',') &&
            std::getline(ss, id, ',') &&
            std::getline(ss, statusStr)) {
            
            title = cleanString(title);
            author = cleanString(author);
            pagesStr = cleanString(pagesStr);
            id = cleanString(id);
            statusStr = cleanString(statusStr);

            int pages = std::stoi(pagesStr);
            bool borrowed = (statusStr == "true" || statusStr == "1");
            books.emplace_back(title, author, pages, id, borrowed);
        }
    }
    return books;
}

void viewAllBooks(const std::vector<Book>& books) {
    std::cout << "\n" << std::left << std::setw(6) << "ID"
              << std::setw(30) << "Title"
              << std::setw(22) << "Author"
              << std::setw(8) << "Pages"
              << "Status\n";
    std::cout << std::string(75, '-') << "\n";
    for (const auto& book : books) {
        book.displayRow();
    }
}

Book* findBook(std::vector<Book>& books, const std::string& query) {
    for (auto& book : books) {
        if (book.getId() == query || book.getTitle() == query) {
            return &book;
        }
    }
    return nullptr;
}

int main() {
    std::vector<Book> books = loadBooks("bookData.txt");
    if (books.empty()) {
        std::cerr << "Error: Could not read bookData.txt\n";
        return 1;
    }

    int choice = 0;
    while (choice != 5) {
        std::cout << "\n=== Library Management System ===\n"
                  << "1. View All Books\n"
                  << "2. View Single Book\n"
                  << "3. Borrow a Book\n"
                  << "4. Return a Book\n"
                  << "5. Exit\n"
                  << "Enter choice (1-5): ";

        if (!(std::cin >> choice)) {
            std::cin.clear();
            std::cin.ignore(10000, '\n');
            std::cout << "Invalid input. Enter a number 1-5.\n";
            continue;
        }

        std::cin.ignore(10000, '\n');

        if (choice == 1) {
            viewAllBooks(books);
        } else if (choice == 2) {
            std::cout << "Enter Book ID or Title: ";
            std::string query;
            std::getline(std::cin, query);
            query = cleanString(query);
            Book* book = findBook(books, query);
            if (book) {
                book->displayDetails();
            } else {
                std::cout << "Book not found.\n";
            }
        } else if (choice == 3) {
            std::cout << "Enter Book ID or Title to borrow: ";
            std::string query;
            std::getline(std::cin, query);
            query = cleanString(query);
            Book* book = findBook(books, query);
            if (!book) {
                std::cout << "Book not found.\n";
            } else if (book->isBorrowed()) {
                std::cout << "Error: This book is already borrowed.\n";
            } else {
                book->setBorrowed(true);
                std::cout << "Book successfully borrowed.\n";
            }
        } else if (choice == 4) {
            std::cout << "Enter Book ID or Title to return: ";
            std::string query;
            std::getline(std::cin, query);
            query = cleanString(query);
            Book* book = findBook(books, query);
            if (!book) {
                std::cout << "Book not found.\n";
            } else if (!book->isBorrowed()) {
                std::cout << "Error: This book is not currently borrowed.\n";
            } else {
                book->setBorrowed(false);
                std::cout << "Book successfully returned.\n";
            }
        } else if (choice == 5) {
            std::cout << "Exiting program.\n";
        } else {
            std::cout << "Invalid selection. Enter a number between 1 and 5.\n";
        }
    }

    return 0;
}