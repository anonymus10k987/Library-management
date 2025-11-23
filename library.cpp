#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
using namespace std;

struct Book {
    int id;
    string title;
    string author;
    string isbn;
    bool available;
    Book* next;
};

struct User {
    int id;
    string name;
    string contact;
    User* next;
};

struct Transaction {
    int userId;
    int bookId;
    string action; // "Borrow" or "Return"
    Transaction* next;
};

Book* bookHead = NULL;
User* userHead = NULL;
Transaction* transHead = NULL;

// ---- File paths ----
const string BOOKS_FILE = "books.txt";
const string USERS_FILE = "users.txt";
const string TRANS_FILE  = "transactions.txt";

// ---- Helpers to check existence ----
bool isBookIdExists(int id) {
    Book* temp = bookHead;
    while (temp != NULL) {
        if (temp->id == id) return true;
        temp = temp->next;
    }
    return false;
}

bool isUserIdExists(int id) {
    User* temp = userHead;
    while (temp != NULL) {
        if (temp->id == id) return true;
        temp = temp->next;
    }
    return false;
}

// ---- Save / Load functions ----
// Format for books.txt: id|title|author|isbn|available\n
void saveBooksToFile() {
    ofstream fout(BOOKS_FILE);
    if (!fout) return;
    Book* temp = bookHead;
    while (temp) {
        fout << temp->id << "|" << temp->title << "|" << temp->author << "|" << temp->isbn << "|" << (temp->available ? 1 : 0) << "\n";
        temp = temp->next;
    }
    fout.close();
}

void loadBooksFromFile() {
    ifstream fin(BOOKS_FILE);
    if (!fin) return;
    string line;
    while (getline(fin, line)) {
        if (line.empty()) continue;
        stringstream ss(line);
        string token;
        Book* nb = new Book();
        // id
        getline(ss, token, '|'); nb->id = stoi(token);
        // title
        getline(ss, nb->title, '|');
        // author
        getline(ss, nb->author, '|');
        // isbn
        getline(ss, nb->isbn, '|');
        // available
        getline(ss, token, '|'); nb->available = (token == "1");
        nb->next = NULL;
        // append
        if (!bookHead) bookHead = nb;
        else {
            Book* t = bookHead; while (t->next) t = t->next; t->next = nb;
        }
    }
    fin.close();
}

// users.txt format: id|name|contact\n
void saveUsersToFile() {
    ofstream fout(USERS_FILE);
    if (!fout) return;
    User* temp = userHead;
    while (temp) {
        fout << temp->id << "|" << temp->name << "|" << temp->contact << "\n";
        temp = temp->next;
    }
    fout.close();
}

void loadUsersFromFile() {
    ifstream fin(USERS_FILE);
    if (!fin) return;
    string line;
    while (getline(fin, line)) {
        if (line.empty()) continue;
        stringstream ss(line);
        User* nu = new User();
        string token;
        getline(ss, token, '|'); nu->id = stoi(token);
        getline(ss, nu->name, '|');
        getline(ss, nu->contact, '|');
        nu->next = NULL;
        if (!userHead) userHead = nu;
        else {
            User* t = userHead; while (t->next) t = t->next; t->next = nu;
        }
    }
    fin.close();
}

// transactions.txt format: userId|bookId|action\n
void saveTransactionsToFile() {
    ofstream fout(TRANS_FILE);
    if (!fout) return;
    Transaction* temp = transHead;
    while (temp) {
        fout << temp->userId << "|" << temp->bookId << "|" << temp->action << "\n";
        temp = temp->next;
    }
    fout.close();
}

void loadTransactionsFromFile() {
    ifstream fin(TRANS_FILE);
    if (!fin) return;
    string line;
    while (getline(fin, line)) {
        if (line.empty()) continue;
        stringstream ss(line);
        Transaction* nt = new Transaction();
        string token;
        getline(ss, token, '|'); nt->userId = stoi(token);
        getline(ss, token, '|'); nt->bookId = stoi(token);
        getline(ss, nt->action, '|');
        nt->next = NULL;
        if (!transHead) transHead = nt;
        else {
            Transaction* t = transHead; while (t->next) t = t->next; t->next = nt;
        }
    }
    fin.close();
}

// ---- Core operations (mostly unchanged) ----
void recordTransaction(int userId, int bookId, string action) {
    Transaction* newT = new Transaction;
    newT->userId = userId;
    newT->bookId = bookId;
    newT->action = action;
    newT->next = NULL;
    if (transHead == NULL) transHead = newT;
    else {
        Transaction* temp = transHead;
        while (temp->next != NULL) temp = temp->next;
        temp->next = newT;
    }
    saveTransactionsToFile(); // persist immediately
}

void addBook() {
    Book* newBook = new Book;
    cout << "Enter Book ID: ";
    cin >> newBook->id;
    if (isBookIdExists(newBook->id)) {
        cout << "Book ID already exists!\n";
        delete newBook;
        return;
    }
    cin.ignore();
    cout << "Enter Title: ";
    getline(cin, newBook->title);
    cout << "Enter Author: ";
    getline(cin, newBook->author);
    cout << "Enter ISBN: ";
    getline(cin, newBook->isbn);
    newBook->available = true;
    newBook->next = NULL;
    if (bookHead == NULL) bookHead = newBook;
    else {
        Book* temp = bookHead;
        while (temp->next != NULL) temp = temp->next;
        temp->next = newBook;
    }
    saveBooksToFile(); // persist immediately
    cout << "Book added successfully!\n";
}

void displayBooks() {
    if (bookHead == NULL) {
        cout << "No books in the library.\n";
        return;
    }
    Book* temp = bookHead;
    cout << "Book List: \n";
    while (temp != NULL) {
        cout << "Book ID: " << temp->id << ", Title: " << temp->title
            << ", Author: " << temp->author
            << ", ISBN: " << temp->isbn
            << ", Available: " << (temp->available ? "Yes" : "No") << endl;
        temp = temp->next;
    }
}

void addUser() {
    User* newUser = new User;
    cout << "Enter User ID: ";
    cin >> newUser->id;
    if (isUserIdExists(newUser->id)) {
        cout << "User ID already exists!\n";
        delete newUser;
        return;
    }
    cin.ignore();
    cout << "Enter Name: ";
    getline(cin, newUser->name);
    cout << "Enter Contact: ";
    getline(cin, newUser->contact);
    newUser->next = NULL;
    if (userHead == NULL) userHead = newUser;
    else {
        User* temp = userHead;
        while (temp->next != NULL) temp = temp->next;
        temp->next = newUser;
    }
    saveUsersToFile(); // persist
    cout << "User added successfully!\n";
}

void displayUsers() {
    if (userHead == NULL) {
        cout << "No users registered.\n";
        return;
    }
    User* temp = userHead;
    cout << "User List: \n";
    while (temp != NULL) {
        cout << "User ID: " << temp->id << ", Name: " << temp->name
            << ", Contact: " << temp->contact << endl;
        temp = temp->next;
    }
}
bool userExists(int userId){
    User* temp= userHead;
    while(temp!= NULL){
        if(userId==temp->id){
            return true;
        }
        temp=temp->next;
    }
    return false;
}

void borrowBook() {
    int userId, bookId;
    cout << "Enter User ID: ";
    cin >> userId;
    if(userExists(userId)) {    
    cout << "Enter Book ID to borrow: ";
    cin >> bookId;
    Book* btemp = bookHead;
    while (btemp != NULL && btemp->id != bookId) btemp = btemp->next;
    if (btemp == NULL) {
        cout << "Book not found.\n"; return;
    }
    if (!btemp->available) {
        cout << "Book is already borrowed.\n"; return;
    }
    btemp->available = false;
    recordTransaction(userId, bookId, "Borrow");
    saveBooksToFile(); // update availability on disk
    cout << "Book borrowed successfully!\n";
}
else{
    cout << "User does not exists";
}
}

void returnBook() {
    int userId, bookId;
    cout << "Enter User ID: ";
    cin >> userId;
    cout << "Enter Book ID to return: ";
    cin >> bookId;
    Book* btemp = bookHead;
    while (btemp != NULL && btemp->id != bookId) btemp = btemp->next;
    if (btemp == NULL) {
        cout << "Book not found.\n"; return;
    }
    btemp->available = true;
    recordTransaction(userId, bookId, "Return");
    saveBooksToFile();
    cout << "Book returned successfully!\n";
}

void searchBook() {
    cin.ignore();
    string key;
    cout << "Enter Title or Author or ISBN to search: ";
    getline(cin, key);
    Book* temp = bookHead;
    bool found = false;
    while (temp != NULL) {
        if (temp->title == key || temp->author == key || temp->isbn == key) {
            cout << "Found Book ID: " << temp->id << ", Title: " << temp->title
                << ", Author: " << temp->author
                << ", Available: " << (temp->available ? "Yes" : "No") << endl;
            found = true;
        }
        temp = temp->next;
    }
    if (!found) cout << "No book found with the given details.\n";
}

void sortBooksByTitle() {
    if (bookHead == NULL || bookHead->next == NULL) return;
    for (Book* i = bookHead; i->next != NULL; i = i->next) {
        for (Book* j = i->next; j != NULL; j = j->next) {
            if (i->title > j->title) {
                swap(i->id, j->id);
                swap(i->title, j->title);
                swap(i->author, j->author);
                swap(i->isbn, j->isbn);
                swap(i->available, j->available);
            }
        }
    }
    saveBooksToFile(); // persist ordering change
    cout << "Books sorted by title.\n";
}

void displayTransactions() {
    if (transHead == NULL) {
        cout << "No transactions yet.\n"; return;
    }
    Transaction* temp = transHead;
    cout << "Transaction History: \n";
    while (temp != NULL) {
        cout << "User ID: " << temp->userId << ", Book ID: " << temp->bookId
            << ", Action: " << temp->action << endl;
        temp = temp->next;
    }
}

// ---- Load all on startup ----
void loadAll() {
    loadBooksFromFile();
    loadUsersFromFile();
    loadTransactionsFromFile();
}

int main() {
    loadAll(); // load saved data (if exists)
    int choice;
    do {
        cout << "\n\nLIBRARY MANAGEMENT SYSTEM MENU";
        cout << "\n1. Add Book";
        cout << "\n2. Add User";
        cout << "\n3. Display Books";
        cout << "\n4. Display Users";
        cout << "\n5. Borrow Book";
        cout << "\n6. Return Book";
        cout << "\n7. Search Book";
        cout << "\n8. Sort Books by Title";
        cout << "\n9. View Transactions";
        cout << "\n0. Exit";
        cout << "\nEnter your choice: ";
        cin >> choice;
        switch (choice) {
        case 1: addBook(); break;
        case 2: addUser(); break;
        case 3: displayBooks(); break;
        case 4: displayUsers(); break;
        case 5: borrowBook(); break;
        case 6: returnBook(); break;
        case 7: searchBook(); break;
        case 8: sortBooksByTitle(); break;
        case 9: displayTransactions(); break;
        case 0: cout << "Exiting program...\n"; break;
        default: cout << "Invalid choice.\n";
        }
    } while (choice != 0);
    return 0;
}
