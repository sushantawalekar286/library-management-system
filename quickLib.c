#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define MAX_BOOKS 100
#define MAX_USERS 100

struct Book {
    char title[50];
    char author[50];
    int isBorrowed;
    int copyCount;
};
typedef struct Book Book;

struct User {
    char username[50];
    char password[50];
};
typedef struct User User;

Book books[MAX_BOOKS];
User staff;
User students[MAX_USERS];
int bookCount = 0;
int studentCount = 0;

void staffMenu();
void studentMenu();
void addBook();
void deleteBook();
void searchBook();
void borrowBook();
void returnBook();
void registerStudent();
void registerStaff();
void displayMainMenu();
void existingUserMenu();
void loadBooks();
void saveBooks();
void loadStudents();
void saveStudents();
void loadStaff();
void saveStaff();
void logActivity(const char *activity);
void viewLogForPeriod(int filterType);

int main() {
    loadBooks();
    loadStudents();
    loadStaff(); // Load staff credentials
    displayMainMenu();
    return 0;
}

void displayMainMenu() {
    int choice;

    do {
        printf("\n\n\n--- Library Management System ---\n");
        printf("1. Login\n");
        printf("2. Exit\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1:
                existingUserMenu();
                break;
            case 2:
                printf("Exiting...\n");
                saveBooks();
                saveStudents();
                saveStaff(); // Save staff credentials before exiting
                break;
            default:
                printf("Invalid choice! Please try again.\n");
        }
    } while (choice != 2);
}

void existingUserMenu() {
    int choice;

    do {
        printf("\n--- Choose User Type ---\n");
        printf("1. Register New Student\n");
        printf("2. Register New Staff\n");
        printf("3. Login as Student\n");
        printf("4. Login as Staff\n");
        printf("5. Back to Main Menu\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1:
                registerStudent();
                break;
            case 2:
                registerStaff();
                break;
            case 3:
                studentMenu();
                break;
            case 4:
                staffMenu();
                break;
            case 5:
                printf("Returning to main menu...\n");
                break;
            default:
                printf("Invalid choice! Please try again.\n");
        }
    } while (choice != 5);
}

void staffMenu() {
    char username[50], password[50];

    printf("Enter staff username: ");
    scanf(" %[^\n]", username);
    printf("Enter staff password: ");
    scanf(" %[^\n]", password);

    if (strcmp(username, staff.username) == 0 && strcmp(password, staff.password) == 0) {
        int choice;
        do {
            printf("\n--- Staff Menu ---\n");
            printf("1. Add Book\n");
            printf("2. Delete Book\n");
            printf("3. View Daily Log\n");
            printf("4. View Monthly Log\n");
            printf("5. Logout\n");
            printf("Enter your choice: ");
            scanf("%d", &choice);

            switch (choice) {
                case 1:
                    addBook();
                    break;
                case 2:
                    deleteBook();
                    break;
                case 3:
                    printf("\n--- Viewing Daily Log ---\n");
                    viewLogForPeriod(1);
                    break;
                case 4:
                    printf("\n--- Viewing Monthly Log ---\n");
                    viewLogForPeriod(2);
                    break;
                case 5:
                    printf("Logging out...\n");
                    break;
                default:
                    printf("Invalid choice! Please try again.\n");
            }
        } while (choice != 5);
    } else {
        printf("Invalid credentials!\n");
    }
}

void studentMenu() {
    char username[50], password[50];

    printf("Enter student username: ");
    scanf(" %[^\n]", username);
    printf("Enter student password: ");
    scanf(" %[^\n]", password);

    int studentIndex = -1;
    for (int i = 0; i < studentCount; i++) {
        if (strcmp(username, students[i].username) == 0 && strcmp(password, students[i].password) == 0) {
            studentIndex = i;
            break;
        }
    }

    if (studentIndex == -1) {
        printf("Invalid credentials!\n");
        return;
    }

    int choice;
    do {
        printf("\n\n\n--- Student Menu ---\n");
        printf("1. Search Book\n");
        printf("2. Borrow Book\n");
        printf("3. Return Book\n");
        printf("4. Logout\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1:
                searchBook();
                break;
            case 2:
                borrowBook();
                break;
            case 3:
                returnBook();
                break;
            case 4:
                printf("Logging out...\n");
                break;
            default:
                printf("Invalid choice! Please try again.\n");
        }
    } while (choice != 4);
}

void addBook() {
    if (bookCount >= MAX_BOOKS) {
        printf("Library is full!\n");
        return;
    }

    printf("Enter book title: ");
    scanf(" %[^\n]", books[bookCount].title);
    printf("Enter book author: ");
    scanf(" %[^\n]", books[bookCount].author);
    printf("Enter number of copies: ");
    scanf("%d", &books[bookCount].copyCount);
    books[bookCount].isBorrowed = 0;
    bookCount++;

    char logMessage[200];
    snprintf(logMessage, sizeof(logMessage), "Book added: %s by %s (Copies: %d)",
             books[bookCount - 1].title, books[bookCount - 1].author, books[bookCount - 1].copyCount);
    logActivity(logMessage);

    printf("Book added successfully!\n");
    saveBooks();
}

void borrowBook() {
    char title[50];
    printf("Enter book title to borrow: ");
    scanf(" %[^\n]", title);

    for (int i = 0; i < bookCount; i++) {
        if (strcmp(books[i].title, title) == 0) {
            if (books[i].copyCount > 0) {
                books[i].copyCount--;
                books[i].isBorrowed++;
                char logMessage[200];
                snprintf(logMessage, sizeof(logMessage), "Book borrowed: %s", books[i].title);
                logActivity(logMessage);
                printf("Book borrowed successfully!\n");
                saveBooks();
                return;
            } else {
                printf("No copies available for borrowing.\n");
                return;
            }
        }
    }
    printf("Book not found!\n");
}

void returnBook() {
    char title[50];
    printf("Enter book title to return: ");
    scanf(" %[^\n]", title);

    for (int i = 0; i < bookCount; i++) {
        if (strcmp(books[i].title, title) == 0) {
            if (books[i].isBorrowed > 0) {
                books[i].copyCount++;
                books[i].isBorrowed--;
                char logMessage[200];
                snprintf(logMessage, sizeof(logMessage), "Book returned: %s", books[i].title);
                logActivity(logMessage);
                printf("Book returned successfully!\n");
                saveBooks();
                return;
            } else {
                printf("No borrowed copies to return.\n");
                return;
            }
        }
    }
    printf("Book not found!\n");
}

void deleteBook() {
    char title[50];
    printf("Enter book title to delete: ");
    scanf(" %[^\n]", title);

    for (int i = 0; i < bookCount; i++) {
        if (strcmp(books[i].title, title) == 0) {
            char logMessage[200];
            snprintf(logMessage, sizeof(logMessage), "Book deleted: %s by %s",
                     books[i].title, books[i].author);
            logActivity(logMessage);

            for (int j = i; j < bookCount - 1; j++) {
                books[j] = books[j + 1];
            }
            bookCount--;
            printf("Book deleted successfully!\n");
            saveBooks();
            return;
        }
    }
    printf("Book not found!\n");
}

void searchBook() {
    char title[50];
    printf("Enter book title to search: ");
    scanf(" %[^\n]", title);

    for (int i = 0; i < bookCount; i++) {
        if (strcmp(books[i].title, title) == 0) {
            printf("Book found: %s by %s (Copies available: %d)\n",
                   books[i].title, books[i].author, books[i].copyCount);
            return;
        }
    }
    printf("Book not found!\n");
}

void registerStudent() {
    if (studentCount >= MAX_USERS) {
        printf("Cannot register more students!\n");
        return;
    }

    printf("Enter student username: ");
    scanf(" %[^\n]", students[studentCount].username);
    printf("Enter student password: ");
    scanf(" %[^\n]", students[studentCount].password);
    studentCount++;

    char logMessage[200];
    snprintf(logMessage, sizeof(logMessage), "Student registered: %s", students[studentCount - 1].username);
    logActivity(logMessage);

    printf("Student registered successfully!\n");
    saveStudents();
}

void registerStaff() {
    printf("Enter new staff username: ");
    scanf(" %[^\n]", staff.username);
    printf("Enter new staff password: ");
    scanf(" %[^\n]", staff.password);

    char logMessage[200];
    snprintf(logMessage, sizeof(logMessage), "New staff registered: %s", staff.username);
    logActivity(logMessage);

    printf("Staff registered successfully!\n");
    saveStaff();
}

void loadBooks() {
    FILE *file = fopen("books.dat", "rb");
    if (file == NULL) return;

    fread(&bookCount, sizeof(int), 1, file);
    fread(books, sizeof(Book), bookCount, file);
    fclose(file);
}

void saveBooks() {
    FILE *file = fopen("books.dat", "wb");
    if (file == NULL) {
        printf("Error saving books!\n");
        return;
    }

    fwrite(&bookCount, sizeof(int), 1, file);
    fwrite(books, sizeof(Book), bookCount, file);
    fclose(file);
}

void loadStudents() {
    FILE *file = fopen("students.dat", "rb");
    if (file == NULL) return;

    fread(&studentCount, sizeof(int), 1, file);
    fread(students, sizeof(User), studentCount, file);
    fclose(file);
}

void saveStudents() {
    FILE *file = fopen("students.dat", "wb");
    if (file == NULL) {
        printf("Error saving students!\n");
        return;
    }

    fwrite(&studentCount, sizeof(int), 1, file);
    fwrite(students, sizeof(User), studentCount, file);
    fclose(file);
}

void loadStaff() {
    FILE *file = fopen("staff.dat", "rb");
    if (file == NULL) {
        strcpy(staff.username, "admin");
        strcpy(staff.password, "admin");
        return;
    }

    fread(&staff, sizeof(User), 1, file);
    fclose(file);
}

void saveStaff() {
    FILE *file = fopen("staff.dat", "wb");
    if (file == NULL) {
        printf("Error saving staff credentials!\n");
        return;
    }

    fwrite(&staff, sizeof(User), 1, file);
    fclose(file);
}

void logActivity(const char *activity) {
    FILE *file = fopen("activity.log", "a");
    if (file == NULL) {
        printf("Error writing log!\n");
        return;
    }

    time_t now = time(NULL);
    fprintf(file, "[%s] %s\n", ctime(&now), activity);
    fclose(file);
}

void viewLogForPeriod(int filterType) {
    FILE *file = fopen("activity.log", "r");
    if (file == NULL) {
        printf("No log available!\n");
        return;
    }

    char line[256];
    while (fgets(line, sizeof(line), file)) {
        printf("%s", line);
    }

    fclose(file);
}
