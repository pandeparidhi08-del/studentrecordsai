# Student Record and Result Management System

A menu-driven console project written in C. It stores up to 100 student records in memory for the current run and supports adding, searching, displaying, deleting, CGPA calculation, year-wise averages, sorting, and CGPA-based academic advice.

## Build and run

```sh
gcc -std=c11 -Wall -Wextra -pedantic SAP_ID.c -o student_records
./student_records
```

On Windows, run `student_records.exe` after compiling. Rename `SAP_ID.c` to your assigned numeric SAP ID filename before submission (for example, `40001546.c`). Replace `SAP_ID` in the repository filename if your course requires an ID-specific name.

## Record and calculation notes

- Each record contains SAP ID, roll number, name, phone, email, year (1–4), current semester (1–8), and eight SGPA slots.
- Enter `0` for an SGPA that is not available. Available non-zero SGPA values are averaged equally to calculate CGPA.
- Year averages include students in that year who have at least one available SGPA.
- Records live in memory and are cleared when the program exits; this keeps the project focused on the required C structures, functions, and menu.
- SAP IDs must be unique. Name sorting is alphabetical and case-sensitive according to the C runtime's `strcmp` ordering.
- The academic-advisor thresholds are simple study guidance, not an official institutional assessment.

## Menu

1. Add student
2. Search student by SAP ID
3. Display all students
4. Delete a student by SAP ID
5. Show average CGPA for each year
6. Display records in increasing SAP ID order
7. Display records in increasing name order
8. Get academic advice from CGPA
0. Exit

See `tests/sample_input.txt` and `tests/sample_output.txt` for an illustrative run with three advice categories.
