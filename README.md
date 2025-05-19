# C Programming Exam Overview

## How to run 
Download repo and navigate to each folder:

```sh
cd oppgave2
```

```sh
mkdir obj
```

```sh
make
```

```sh
./oppgave2
```

## Exam Details
- **Duration:** 14 days  
- **Grading Scale:** A – F (National Grading Scale)  
- **Submission Window:** April 26 – May 10, 2024  
- **Programming Language:** ANSI C (C89 standard) — *Only code written in this version will be considered valid for grading.*  
- **Total Tasks:** 6 

---

## Task 1: (Theory) Fundamentals of C and Compilation (5%)

### Description
This task focuses on theoretical understanding of the C programming language and its ecosystem.

### Requirements
- Describe the typical use cases of the C programming language in modern software development.
- Provide a brief biography of Linus Torvalds, highlighting his influence in computing.
- Outline the process of compiling a C program — from writing the source code to executing the final binary.

---

## Task 2: File Processing and Data Serialization (15%)

### Description
Build a utility that processes text files and stores structured data in binary format.

### Requirements
- Read individual words from a given input text file.
- Define a `struct` to hold metadata such as word length, position, or frequency.
- Store the structured data and original word in a binary output file.
- Organize the project using header files and a Makefile to automate builds.
- Ensure the program handles file errors gracefully.

---

## Task 3: List handling – Reservation System (25%)

### Description
Design and implement a reservation system using linked data structures.

### Structure
- Use a **doubly linked list** to maintain table reservations.
- Use a **singly linked list** to manage food and drink orders per table.

### Functional Requirements
- Add new reservations, sorted in alphabetical order by customer name.
- Retrieve and diplay a details about a table reservation.
- Search and display reservations based on name.
- Remove a reservation from the list.
- Add food and drinks orders to a table.
- Display all orders for a table and calculate the bill.
- Display all orders and individual bill for every person at a table.

### Additional Notes
- Include a text-based interactive menu in `main()` for user actions.
- Properly free allocated memory on exit.
- Include a Makefile and organized headers for compilation.
- Save a screenshot of the program in action as `task3_screenshot.png`.

---

## Task 4: Multithreading and Synchronization (15%)

### Description
Enhance an existing threaded application to improve its structure and thread safety.

### Provided
- Base C application using threads.
- Test file (Hamlet).

### Requirements
- Refactor global shared variables into local variables sent in as arguments.
- Accept the file name as a command-line argument.
- Replace basic condition checks with proper semaphores for synchronization.
- Use mutexes and semaphores initialized explicitly.
- Compute and display the SHA1 checksum of the processed file using the provided `sha1.c` and `sha1.h`.

### Additional Notes
- Document the project with a screenshot (`task4_screenshot.png`).
- Include a Makefile and appropriate headers for modular compilation.

---

## Task 5: Client-Server Communication (Networking) (25%)

### Description
Develop a TCP-based client-server system to exchange simple messages.

### Requirements
- Implement a server that listens on a specified port number provided via command line.
- Build a client that connects to the server and sends messages typed by the user.
- The server should receive and print all messages sent by the client.
- Ensure the connection is gracefully closed by either party.
- Implement a minimal communication protocol (e.g., messages end with newline or a special terminator).

### Additional Notes
- Include a Makefile and structured headers.
- Save a screenshot showing message exchange as `task5_screenshot.png`.
- Follow good coding style and include comments for maintainability.

---

## Task 6: File Handling and Text Parsing – Code Beautifier (20%)

### Description
Develop a C program that automatically reformats another C source file according to specific code style rules. The application acts as a basic "code beautifier".

### Requirements
- Accept a `.c` source file name as a command-line argument.
- Read the file and create a new version with `_beautified` inserted before the `.c` extension.
- Apply the following transformations:

- Replace all for loops with while loops.
- Change all char* variables to use hungarian notation.
- Change all instances of three spaces with a tab.

### Additional Notes
- Include a Makefile and structured headers.
- Include original file: **oppgave6_test.c**
- Include beautified file: **oppgave6_test_beautified.c**




  
