# 📚 Library Management System – C Programming

<p align="center">
  <b>A Console-Based Library Management Application Developed Using C</b>
</p>

<p align="center">
  <img src="https://img.shields.io/badge/Language-C-blue.svg" alt="C Language">
  <img src="https://img.shields.io/badge/Platform-Windows-lightgrey.svg" alt="Platform">
  <img src="https://img.shields.io/badge/Storage-File%20Handling-orange.svg" alt="File Handling">
  <img src="https://img.shields.io/badge/Project-Completed-brightgreen.svg" alt="Project Status">
</p>

## 📖 About the Project

The **Library Management System (LMS)** is a menu-driven console application developed using the C programming language. It simplifies library operations by providing features for book management, searching, issuing and returning books, tracking issue records, and calculating overdue fines.

The application uses file handling to store library records persistently, allowing data to be retained even after the program is closed.

## 🎯 Objectives

- Simplify library book management.
- Maintain book and transaction records.
- Automate issue dates, due dates, and return dates.
- Calculate fines for overdue books.
- Provide secure access through administrator authentication.
- Understand practical applications of C programming concepts.

## ✨ Key Features

### 📚 Book Management
- Add new books to the library.
- Update existing book information.
- Delete books from the library.
- Search books by ID, title, or author.
- View all available book records.

### 📕 Issue and Return Management
- Issue books to library users.
- Record book issue details.
- Return previously issued books.
- Display issued-book records.
- Automatically calculate issue dates, due dates, and return dates.
- Calculate overdue fines based on the applicable fine rate.

### 🔐 Administrator Authentication
- Administrator login before accessing protected operations.
- Password-based authentication.
- Limited login attempts for improved security.
- Administrator password change functionality.
- Persistent storage of administrator credentials in a file.

### 💾 File Handling
The application uses binary files to maintain records between program executions.

| File | Purpose |
|---|---|
| `books.dat` | Stores book records |
| `issues.dat` | Stores issue and return records |
| `admin.dat` | Stores administrator credentials |

## ⚙️ Technologies Used

| Technology | Application |
|---|---|
| C Programming | Application development |
| Structures | Organizing book and transaction records |
| Functions | Modular program design |
| Pointers | Accessing and manipulating data |
| File Handling | Persistent data storage |
| `time.h` | Date and time calculations |
| `stdio.h` | Input, output, and file operations |
| `string.h` | String manipulation |
| `stdlib.h` | Standard utility functions |

## 🔄 How the System Works

1. **Admin Login:** The administrator enters the login credentials.
2. **Main Menu:** After successful authentication, the management menu is displayed.
3. **Book Operations:** The administrator can add, update, delete, search, or view book records.
4. **Issue Book:** The system records the issue date and calculates the due date.
5. **Return Book:** The system records the return date and checks whether the book is overdue.
6. **Fine Calculation:** The applicable fine is calculated according to the overdue period and book category.
7. **Data Storage:** Book, transaction, and administrator records are maintained in files.
8. **Exit:** The program terminates while preserving saved records.

## 💰 Fine Calculation

The system calculates overdue fines using the number of days a book is returned after its due date.

**Formula:**

`Fine = Overdue Days × Fine Per Day`

For example, if the applicable fine is ₹5 per day and a book is returned 4 days late:

`Fine = 4 × ₹5 = ₹20`

The fine rate depends on the book category configured in the program. Reference-book fines can be configured separately from normal-book fines.

## 🧠 C Programming Concepts Demonstrated

- Structures and user-defined data types
- Functions and modular programming
- Arrays and strings
- Pointers
- Conditional statements and loops
- File operations using `fopen()`, `fread()`, `fwrite()`, and `fclose()`
- Date calculations using `time.h`
- Input validation and menu-driven programming

## ▶️ How to Run the Project

### Prerequisites

- GCC compiler or another compatible C compiler
- A C development environment, such as Code::Blocks, Dev-C++, or VS Code with GCC

### Compilation

1. Download or clone this repository.
2. Open a terminal in the project directory.
3. Compile the C source file:

```bash
gcc main.c -o library
```

4. Run the application.

**Windows:**

```bash
library.exe
```

**Linux:**

```bash
./library
```

If your source file has a different name, replace `main.c` with the appropriate filename.

## 📂 Project Structure

```text
Library-Management-System/
│
├── main.c
├── README.md
├── books.dat       # Created/used for book records
├── issues.dat      # Created/used for issue and return records
└── admin.dat       # Created/used for administrator credentials
```

The `.dat` files are runtime data files and may be created when the application initializes or saves records. The exact repository structure depends on how the source code is organized.

## 🚀 Future Enhancements

- Add a graphical user interface.
- Introduce multiple user accounts and role-based access.
- Add book availability reports and transaction summaries.
- Improve password protection and credential storage.
- Integrate a database for larger libraries.
- Add automated backups and advanced search filters.

## 🎓 Learning Outcomes

This project provided practical experience in C programming, modular software development, structures, file handling, authentication, date calculations, and transaction management. It demonstrates how fundamental programming concepts can be combined to build a useful real-world application.

## 👨‍💻 Author

**Burri Srihari**

GitHub: [@burrisrihari8](https://github.com/burrisrihari8)

---

<p align="center">
  <b>Library Management System | C Programming Project</b>
</p>
