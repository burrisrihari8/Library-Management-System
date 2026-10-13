#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define MAX_BOOKS 100
#define MAX_ISSUES 100

#define BOOK_FILE "books.dat"
#define ISSUE_FILE "issues.dat"
#define ADMIN_FILE "admin.dat"

#define FINE_PER_DAY 5.0
#define ADMIN_ID "admin"
#define MAX_ATTEMPTS 3

/* ==========================================================
   BLOCK 1 - STRUCTURES
   ========================================================== */

typedef struct
{
    int bookID;
    char title[50];
    char author[50];
    int quantity;
} Book;

typedef struct
{
    int issueID;
    int bookID;
    int userID;
    char userName[50];

    char issueDate[20];
    char dueDate[20];
    char returnDate[20];

    float fineAmount;
    int returned;
} Issue;


/* ==========================================================
   BLOCK 2 - GLOBAL VARIABLES
   ========================================================== */

Book books[MAX_BOOKS];
Issue issues[MAX_ISSUES];

int bookCount = 0;
int issueCount = 0;

/* Default admin password */
char adminPassword[50] = "123";


/* ==========================================================
   BLOCK 3 - ADMIN PASSWORD FUNCTIONS
   ========================================================== */

/* Load the saved admin password */

void loadAdminPassword()
{
    FILE *fp;

    fp = fopen(ADMIN_FILE, "rb");

    if (fp != NULL)
    {
        if (fread(adminPassword,
                  sizeof(char),
                  sizeof(adminPassword),
                  fp) != sizeof(adminPassword))
        {
            strcpy(adminPassword, "123");
        }

        adminPassword[sizeof(adminPassword) - 1] = '\0';

        fclose(fp);
    }
    else
    {
        /* First run: use the default password */
        strcpy(adminPassword, "123");
    }
}


/* Save the changed admin password */

void saveAdminPassword()
{
    FILE *fp;

    fp = fopen(ADMIN_FILE, "wb");

    if (fp == NULL)
    {
        printf("\nError saving admin password!\n");
        return;
    }

    fwrite(adminPassword,
           sizeof(char),
           sizeof(adminPassword),
           fp);

    fclose(fp);

    printf("\nAdmin password saved successfully!\n");
}


/* Admin login with three attempts */

int adminLogin()
{
    char enteredID[50];
    char enteredPassword[50];
    int attempts;

    attempts = 0;

    printf("\n========================================\n");
    printf("          ADMIN LOGIN\n");
    printf("========================================\n");

    while (attempts < MAX_ATTEMPTS)
    {
        printf("\nEnter Admin ID: ");
        scanf("%49s", enteredID);

        printf("Enter Admin Password: ");
        scanf("%49s", enteredPassword);

        if (strcmp(enteredID, ADMIN_ID) == 0 &&
            strcmp(enteredPassword, adminPassword) == 0)
        {
            printf("\nAdmin login successful!\n");
            return 1;
        }

        attempts++;

        printf("\nInvalid Admin ID or Password!\n");
        printf("Attempts remaining: %d\n",
               MAX_ATTEMPTS - attempts);
    }

    printf("\nMaximum login attempts exceeded!\n");
    printf("Program terminated for security.\n");

    return 0;
}


/* Change the admin password */

void changeAdminPassword()
{
    char currentPassword[50];
    char newPassword[50];
    char confirmPassword[50];

    printf("\n========================================\n");
    printf("       CHANGE ADMIN PASSWORD\n");
    printf("========================================\n");

    printf("Enter Current Password: ");
    scanf("%49s", currentPassword);

    if (strcmp(currentPassword, adminPassword) != 0)
    {
        printf("\nIncorrect current password!\n");
        return;
    }

    printf("Enter New Password: ");
    scanf("%49s", newPassword);

    if (strcmp(newPassword, adminPassword) == 0)
    {
        printf("\nNew password must be different!\n");
        return;
    }

    printf("Confirm New Password: ");
    scanf("%49s", confirmPassword);

    if (strcmp(newPassword, confirmPassword) != 0)
    {
        printf("\nPasswords do not match!\n");
        return;
    }

    strcpy(adminPassword, newPassword);

    saveAdminPassword();
}


/* ==========================================================
   BLOCK 4 - DATE FUNCTIONS
   ========================================================== */

void formatDate(time_t t, char date[])
{
    struct tm *info;

    info = localtime(&t);

    sprintf(date, "%02d-%02d-%04d",
            info->tm_mday,
            info->tm_mon + 1,
            info->tm_year + 1900);
}

void getToday(char date[])
{
    time_t now;

    now = time(NULL);

    formatDate(now, date);
}

void getFutureDate(int days, char date[])
{
    time_t now;

    now = time(NULL);

    now = now + (days * 24 * 60 * 60);

    formatDate(now, date);
}

time_t convertToTime(char date[])
{
    struct tm t;

    int day;
    int month;
    int year;

    memset(&t, 0, sizeof(t));

    sscanf(date, "%d-%d-%d",
           &day,
           &month,
           &year);

    t.tm_mday = day;
    t.tm_mon = month - 1;
    t.tm_year = year - 1900;

    t.tm_hour = 0;
    t.tm_min = 0;
    t.tm_sec = 0;

    return mktime(&t);
}

int calculateLateDays(char dueDate[], char returnDate[])
{
    time_t due;
    time_t returned;

    double difference;

    due = convertToTime(dueDate);
    returned = convertToTime(returnDate);

    difference = difftime(returned, due);

    if (difference <= 0)
    {
        return 0;
    }

    return (int)(difference / (24 * 60 * 60));
}


/* ==========================================================
   BLOCK 5 - FIND BOOK
   ========================================================== */

int findBook(int id)
{
    int i;

    for (i = 0; i < bookCount; i++)
    {
        if (books[i].bookID == id)
        {
            return i;
        }
    }

    return -1;
}


/* ==========================================================
   BLOCK 6 - FIND ISSUE
   ========================================================== */

int findIssue(int bookID, int userID)
{
    int i;

    for (i = 0; i < issueCount; i++)
    {
        if (issues[i].bookID == bookID &&
            issues[i].userID == userID &&
            issues[i].returned == 0)
        {
            return i;
        }
    }

    return -1;
}


/* ==========================================================
   BLOCK 7 - ADD BOOK
   ========================================================== */

void addBook()
{
    Book b;

    if (bookCount >= MAX_BOOKS)
    {
        printf("\nBook storage is full!\n");
        return;
    }

    printf("\n================================\n");
    printf("          ADD BOOK\n");
    printf("================================\n");

    printf("Enter Book ID: ");
    scanf("%d", &b.bookID);

    if (findBook(b.bookID) != -1)
    {
        printf("\nBook ID already exists!\n");
        return;
    }

    printf("Enter Book Title: ");
    scanf(" %[^\n]", b.title);

    printf("Enter Author Name: ");
    scanf(" %[^\n]", b.author);

    printf("Enter Quantity: ");
    scanf("%d", &b.quantity);

    if (b.quantity < 0)
    {
        printf("\nInvalid quantity!\n");
        return;
    }

    books[bookCount] = b;
    bookCount++;

    printf("\nBook added successfully!\n");
}


/* ==========================================================
   BLOCK 8 - UPDATE BOOK
   ========================================================== */

void updateBook()
{
    int choice;
    int id;
    int index;
    int i;

    char name[50];

    if (bookCount == 0)
    {
        printf("\nThere are no books!\n");
        return;
    }

    printf("\n================================\n");
    printf("          UPDATE BOOK\n");
    printf("================================\n");

    printf("1. By Book ID\n");
    printf("2. By Book Name\n");
    printf("3. Back\n");

    printf("Enter choice: ");
    scanf("%d", &choice);

    index = -1;

    if (choice == 1)
    {
        printf("Enter Book ID: ");
        scanf("%d", &id);

        index = findBook(id);
    }
    else if (choice == 2)
    {
        printf("Enter Book Name: ");
        scanf(" %[^\n]", name);

        for (i = 0; i < bookCount; i++)
        {
            if (strcmp(books[i].title, name) == 0)
            {
                index = i;
                break;
            }
        }
    }
    else
    {
        return;
    }

    if (index == -1)
    {
        printf("\nBook not found!\n");
        return;
    }

    printf("\nEnter new Book Title: ");
    scanf(" %[^\n]", books[index].title);

    printf("Enter new Author Name: ");
    scanf(" %[^\n]", books[index].author);

    printf("Enter new Quantity: ");
    scanf("%d", &books[index].quantity);

    if (books[index].quantity < 0)
    {
        printf("\nInvalid quantity!\n");
        return;
    }

    printf("\nBook updated successfully!\n");
}


/* ==========================================================
   BLOCK 9 - REMOVE BOOK QUANTITY
   ========================================================== */

void removeBook()
{
    int choice;
    int id;
    int index;
    int i;
    int removeQuantity;

    char name[50];

    if (bookCount == 0)
    {
        printf("\nThere are no books!\n");
        return;
    }

    printf("\n================================\n");
    printf("        REMOVE BOOK QUANTITY\n");
    printf("================================\n");

    printf("1. By Book ID\n");
    printf("2. By Book Name\n");
    printf("3. Back\n");

    printf("Enter choice: ");
    scanf("%d", &choice);

    index = -1;

    if (choice == 1)
    {
        printf("Enter Book ID: ");
        scanf("%d", &id);

        index = findBook(id);
    }
    else if (choice == 2)
    {
        printf("Enter Book Name: ");
        scanf(" %[^\n]", name);

        for (i = 0; i < bookCount; i++)
        {
            if (strcmp(books[i].title, name) == 0)
            {
                index = i;
                break;
            }
        }
    }
    else
    {
        return;
    }

    if (index == -1)
    {
        printf("\nBook not found!\n");
        return;
    }

    printf("\nBook ID       : %d\n", books[index].bookID);
    printf("Book Title    : %s\n", books[index].title);
    printf("Current Quantity: %d\n", books[index].quantity);

    if (books[index].quantity == 0)
    {
        printf("\nNo copies available to remove!\n");
        return;
    }

    printf("\nEnter quantity to remove: ");
    scanf("%d", &removeQuantity);

    if (removeQuantity <= 0)
    {
        printf("\nInvalid quantity!\n");
        return;
    }

    if (removeQuantity > books[index].quantity)
    {
        printf("\nCannot remove more copies than available!\n");
        return;
    }

    books[index].quantity -= removeQuantity;

    printf("\n%d copies removed successfully!\n", removeQuantity);
    printf("Remaining Quantity: %d\n", books[index].quantity);

    if (books[index].quantity == 0)
    {
        for (i = index; i < bookCount - 1; i++)
        {
            books[i] = books[i + 1];
        }

        bookCount--;

        printf("All copies removed. Book record deleted.\n");
    }
}

/* ==========================================================
   BLOCK 10 - SEARCH BOOK
   ========================================================== */

void searchBook()
{
    int choice;
    int id;
    int i;
    int found;

    char name[50];

    if (bookCount == 0)
    {
        printf("\nThere are no books!\n");
        return;
    }

    found = 0;

    printf("\n================================\n");
    printf("         SEARCH BOOK\n");
    printf("================================\n");

    printf("1. By Book ID\n");
    printf("2. By Book Name\n");
    printf("3. By Author Name\n");
    printf("4. Back\n");

    printf("Enter choice: ");
    scanf("%d", &choice);

    if (choice == 1)
    {
        printf("Enter Book ID: ");
        scanf("%d", &id);

        for (i = 0; i < bookCount; i++)
        {
            if (books[i].bookID == id)
            {
                printf("\nBook ID : %d", books[i].bookID);
                printf("\nTitle   : %s", books[i].title);
                printf("\nAuthor  : %s", books[i].author);
                printf("\nQuantity: %d\n", books[i].quantity);

                found = 1;
            }
        }
    }
    else if (choice == 2)
    {
        printf("Enter Book Name: ");
        scanf(" %[^\n]", name);

        for (i = 0; i < bookCount; i++)
        {
            if (strcmp(books[i].title, name) == 0)
            {
                printf("\nBook ID : %d", books[i].bookID);
                printf("\nTitle   : %s", books[i].title);
                printf("\nAuthor  : %s", books[i].author);
                printf("\nQuantity: %d\n", books[i].quantity);

                found = 1;
            }
        }
    }
    else if (choice == 3)
    {
        printf("Enter Author Name: ");
        scanf(" %[^\n]", name);

        for (i = 0; i < bookCount; i++)
        {
            if (strcmp(books[i].author, name) == 0)
            {
                printf("\nBook ID : %d", books[i].bookID);
                printf("\nTitle   : %s", books[i].title);
                printf("\nAuthor  : %s", books[i].author);
                printf("\nQuantity: %d\n", books[i].quantity);

                found = 1;
            }
        }
    }
    else
    {
        return;
    }

    if (!found)
    {
        printf("\nBook not found!\n");
    }
}


/* ==========================================================
   BLOCK 11 - VIEW ALL BOOKS
   ========================================================== */

void viewAllBooks()
{
    int i;

    if (bookCount == 0)
    {
        printf("\nThere are no books!\n");
        return;
    }

    printf("\n=================================================================\n");
    printf("                         ALL BOOKS\n");
    printf("=================================================================\n");

    printf("%-8s %-25s %-20s %-8s\n",
           "ID", "TITLE", "AUTHOR", "QTY");

    printf("-----------------------------------------------------------------\n");

    for (i = 0; i < bookCount; i++)
    {
        printf("%-8d %-25s %-20s %-8d\n",
               books[i].bookID,
               books[i].title,
               books[i].author,
               books[i].quantity);
    }
}


/* ==========================================================
   BLOCK 12 - ISSUE BOOK
   ========================================================== */

void issueBook()
{
    int bookID;
    int userID;
    int index;

    Issue newIssue;

    if (bookCount == 0)
    {
        printf("\nThere are no books!\n");
        return;
    }

    if (issueCount >= MAX_ISSUES)
    {
        printf("\nIssue record storage is full!\n");
        return;
    }

    printf("\n================================\n");
    printf("          ISSUE BOOK\n");
    printf("================================\n");

    printf("Enter Book ID: ");
    scanf("%d", &bookID);

    index = findBook(bookID);

    if (index == -1)
    {
        printf("\nBook not found!\n");
        return;
    }

    if (books[index].quantity <= 0)
    {
        printf("\nBook is not available!\n");
        return;
    }

    printf("Enter User ID: ");
    scanf("%d", &userID);

    printf("Enter User Name: ");
    scanf(" %[^\n]", newIssue.userName);

    newIssue.issueID = issueCount + 1;
    newIssue.bookID = bookID;
    newIssue.userID = userID;

    getToday(newIssue.issueDate);
    getFutureDate(7, newIssue.dueDate);

    strcpy(newIssue.returnDate, "Not Returned");

    newIssue.fineAmount = 0;
    newIssue.returned = 0;

    issues[issueCount] = newIssue;
    issueCount++;

    books[index].quantity--;

    printf("\nBook issued successfully!\n");
    printf("Issue ID  : %d\n", newIssue.issueID);
    printf("Issue Date: %s\n", newIssue.issueDate);
    printf("Due Date  : %s\n", newIssue.dueDate);
}


/* ==========================================================
   BLOCK 13 - RETURN BOOK
   ========================================================== */

void returnBook()
{
    int bookID;
    int userID;

    int issueIndex;
    int bookIndex;

    int lateDays;

    char returnDate[20];

    printf("\n================================\n");
    printf("          RETURN BOOK\n");
    printf("================================\n");

    printf("Enter Book ID: ");
    scanf("%d", &bookID);

    printf("Enter User ID: ");
    scanf("%d", &userID);

    issueIndex = findIssue(bookID, userID);

    if (issueIndex == -1)
    {
        printf("\nIssue record not found!\n");
        return;
    }

    bookIndex = findBook(bookID);

    if (bookIndex == -1)
    {
        printf("\nBook not found!\n");
        return;
    }

    getToday(returnDate);

    strcpy(issues[issueIndex].returnDate, returnDate);

    lateDays = calculateLateDays(
                   issues[issueIndex].dueDate,
                   issues[issueIndex].returnDate);

    issues[issueIndex].fineAmount =
        lateDays * FINE_PER_DAY;

    issues[issueIndex].returned = 1;

    books[bookIndex].quantity++;

    printf("\nBook returned successfully!\n");

    printf("Return Date : %s\n",
           issues[issueIndex].returnDate);

    printf("Late Days   : %d\n", lateDays);

    printf("Fine Amount : Rs. %.2f\n",
           issues[issueIndex].fineAmount);
}


/* ==========================================================
   BLOCK 14 - LIST ISSUED BOOKS
   ========================================================== */

void listIssuedBooks()
{
    int i;
    int found = 0;

    printf("\n================================================================================\n");
    printf("                            ISSUED BOOKS\n");
    printf("================================================================================\n");

    printf("%-5s %-8s %-8s %-15s %-12s %-12s %-15s %-10s\n",
           "IID", "BookID", "UserID", "User Name",
           "Issue Date", "Due Date", "Return Date", "Fine");

    printf("--------------------------------------------------------------------------------\n");

    for (i = 0; i < issueCount; i++)
    {
        printf("%-5d %-8d %-8d %-15s %-12s %-12s %-15s Rs.%-7.2f\n",
               issues[i].issueID,
               issues[i].bookID,
               issues[i].userID,
               issues[i].userName,
               issues[i].issueDate,
               issues[i].dueDate,
               issues[i].returnDate,
               issues[i].fineAmount);

        found = 1;
    }

    if (!found)
    {
        printf("No issued books found.\n");
    }
}


/* ==========================================================
   BLOCK 15 - SAVE DATA
   ========================================================== */

void saveData()
{
    FILE *fp;

    /* Save books */

    fp = fopen(BOOK_FILE, "wb");

    if (fp == NULL)
    {
        printf("\nError opening books file!\n");
        return;
    }

    fwrite(&bookCount, sizeof(int), 1, fp);
    fwrite(books, sizeof(Book), bookCount, fp);

    fclose(fp);

    /* Save issue records */

    fp = fopen(ISSUE_FILE, "wb");

    if (fp == NULL)
    {
        printf("\nError opening issue file!\n");
        return;
    }

    fwrite(&issueCount, sizeof(int), 1, fp);
    fwrite(issues, sizeof(Issue), issueCount, fp);

    fclose(fp);

    printf("\nData saved successfully!\n");
}


/* ==========================================================
   BLOCK 16 - LOAD DATA
   ========================================================== */

void loadData()
{
    FILE *fp;

    /* Load books */

    fp = fopen(BOOK_FILE, "rb");

    if (fp != NULL)
    {
        if (fread(&bookCount, sizeof(int), 1, fp) == 1 &&
            bookCount >= 0 && bookCount <= MAX_BOOKS)
        {
            fread(books, sizeof(Book), bookCount, fp);
        }
        else
        {
            bookCount = 0;
        }

        fclose(fp);
    }

    /* Load issues */

    fp = fopen(ISSUE_FILE, "rb");

    if (fp != NULL)
    {
        if (fread(&issueCount, sizeof(int), 1, fp) == 1 &&
            issueCount >= 0 && issueCount <= MAX_ISSUES)
        {
            fread(issues, sizeof(Issue), issueCount, fp);
        }
        else
        {
            issueCount = 0;
        }

        fclose(fp);
    }
}


/* ==========================================================
   BLOCK 17 - MAIN MENU
   ========================================================== */

void menu()
{
    int choice;

    do
    {
        printf("\n\n");

        printf("+----------------------------------------+\n");
        printf("|       LIBRARY MANAGEMENT SYSTEM        |\n");
        printf("+----------------------------------------+\n");

        printf("| 1. Add New Book                        |\n");
        printf("| 2. Update Book Details                 |\n");
        printf("| 3. Remove Book                         |\n");
        printf("| 4. Search Book                         |\n");
        printf("| 5. View All Books                      |\n");
        printf("| 6. Issue Book                          |\n");
        printf("| 7. Return Book                         |\n");
        printf("| 8. List Issued Books                   |\n");
        printf("| 9. Save                                |\n");
        printf("| 10. Exit                               |\n");
        printf("| 11. Change Admin Password              |\n");

        printf("+----------------------------------------+\n");

        printf("Enter your choice: ");
        scanf("%d", &choice);

        switch (choice)
        {
            case 1:
                addBook();
                break;

            case 2:
                updateBook();
                break;

            case 3:
                removeBook();
                break;

            case 4:
                searchBook();
                break;

            case 5:
                viewAllBooks();
                break;

            case 6:
                issueBook();
                break;

            case 7:
                returnBook();
                break;

            case 8:
                listIssuedBooks();
                break;

            case 9:
                saveData();
                break;

            case 10:
                saveData();

                printf("\nThank you for using Library Management System!\n");
                break;

            case 11:
                changeAdminPassword();
                break;

            default:
                printf("\nInvalid choice!\n");
        }

    } while (choice != 10);
}


/* ==========================================================
   BLOCK 18 - MAIN FUNCTION
   ========================================================== */

int main()
{
    /* Load the saved admin password */

    loadAdminPassword();

    /* Verify admin credentials before opening the menu */

    if (!adminLogin())
    {
        return 0;
    }

    /* Load previously saved library data */

    loadData();

    /* Start the main menu */

    menu();

    return 0;
}
