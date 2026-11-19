#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define MAX_STUDENTS 100
#define NAME_LEN 80
#define TEXT_LEN 120
#define SEMESTERS 8

typedef struct {
    int sapId;
    int rollNo;
    char name[NAME_LEN];
    char phone[TEXT_LEN];
    char email[TEXT_LEN];
    int year;
    int semester;
    float sgpa[SEMESTERS]; /* -1 means that semester has no result yet. */
} Student;

static Student students[MAX_STUDENTS];
static int studentCount = 0;

static void readLine(const char *prompt, char *buffer, size_t size) {
    printf("%s", prompt);
    if (fgets(buffer, (int)size, stdin) == NULL) { buffer[0] = '\0'; return; }
    buffer[strcspn(buffer, "\r\n")] = '\0';
}

static void readRequired(const char *prompt, char *buffer, size_t size) {
    do {
        readLine(prompt, buffer, size);
        if (buffer[0] == '\0') puts("This field cannot be empty.");
    } while (buffer[0] == '\0');
}

static int readInt(const char *prompt, int min, int max) {
    char line[80], *end;
    long value;
    for (;;) {
        readLine(prompt, line, sizeof line);
        value = strtol(line, &end, 10);
        if (end != line && *end == '\0' && value >= min && value <= max) return (int)value;
        printf("Enter a whole number from %d to %d.\n", min, max);
    }
}

static float readGrade(const char *prompt) {
    char line[80], *end;
    float value;
    for (;;) {
        readLine(prompt, line, sizeof line);
        value = strtof(line, &end);
        if (end != line && *end == '\0' && value >= 0.0f && value <= 10.0f) return value;
        puts("Enter an SGPA from 0.00 to 10.00.");
    }
}

static int findIndex(int sapId) {
    int i;
    for (i = 0; i < studentCount; ++i) if (students[i].sapId == sapId) return i;
    return -1;
}

static float cgpaOf(const Student *s) {
    float sum = 0.0f;
    int count = 0, i;
    for (i = 0; i < SEMESTERS; ++i) if (s->sgpa[i] >= 0.0f) { sum += s->sgpa[i]; ++count; }
    return count ? sum / count : -1.0f;
}

static void displayStudent(const Student *s) {
    int i, any = 0;
    float cgpa = cgpaOf(s);
    printf("\nSAP ID: %d | Roll No.: %d | Name: %s\nPhone: %s | Email: %s\nYear: %d | Current semester: %d\nSGPA:", s->sapId, s->rollNo, s->name, s->phone, s->email, s->year, s->semester);
    for (i = 0; i < SEMESTERS; ++i) {
        if (s->sgpa[i] >= 0.0f) { printf(" S%d=%.2f", i + 1, s->sgpa[i]); any = 1; }
    }
    if (!any) printf(" no results entered");
    if (cgpa >= 0.0f) printf("\nCGPA (average of available SGPAs): %.2f\n", cgpa);
    else printf("\nCGPA: not available\n");
}

static void addStudent(void) {
    Student s;
    int i, existing;
    if (studentCount == MAX_STUDENTS) { puts("Student list is full."); return; }
    s.sapId = readInt("SAP ID: ", 1, 2147483647);
    if (findIndex(s.sapId) >= 0) { puts("That SAP ID already exists."); return; }
    s.rollNo = readInt("Roll No.: ", 1, 2147483647);
    readRequired("Name: ", s.name, sizeof s.name);
    readRequired("Phone No.: ", s.phone, sizeof s.phone);
    readRequired("Email ID: ", s.email, sizeof s.email);
    s.year = readInt("Year (1-4): ", 1, 4);
    s.semester = readInt("Current semester (1-8): ", 1, 8);
    for (i = 0; i < SEMESTERS; ++i) s.sgpa[i] = -1.0f;
    puts("Enter available SGPA values; enter 0 to mark a semester as not available.");
    for (i = 0; i < SEMESTERS; ++i) {
        char prompt[50]; float grade;
        snprintf(prompt, sizeof prompt, "Semester %d SGPA (0-10): ", i + 1);
        grade = readGrade(prompt);
        s.sgpa[i] = grade == 0.0f ? -1.0f : grade;
    }
    existing = studentCount;
    students[existing] = s;
    ++studentCount;
    puts("Student record added.");
}

static void searchStudent(void) {
    int id = readInt("Enter SAP ID to search: ", 1, 2147483647);
    int index = findIndex(id);
    if (index < 0) puts("Student not found."); else displayStudent(&students[index]);
}

static void displayAll(void) {
    int i;
    if (!studentCount) { puts("No student records available."); return; }
    for (i = 0; i < studentCount; ++i) displayStudent(&students[i]);
}

static void deleteStudent(void) {
    int id = readInt("Enter SAP ID to delete: ", 1, 2147483647);
    int index = findIndex(id), i;
    if (index < 0) { puts("Student not found."); return; }
    for (i = index; i < studentCount - 1; ++i) students[i] = students[i + 1];
    --studentCount;
    puts("Student record deleted.");
}

static int compareSap(const void *a, const void *b) {
    const Student *x = (const Student *)a, *y = (const Student *)b;
    return (x->sapId > y->sapId) - (x->sapId < y->sapId);
}

static int compareName(const void *a, const void *b) {
    const Student *x = (const Student *)a, *y = (const Student *)b;
    int result = strcmp(x->name, y->name);
    return result ? result : compareSap(a, b);
}

static void displaySorted(int byName) {
    Student copy[MAX_STUDENTS];
    int i;
    if (!studentCount) { puts("No student records available."); return; }
    memcpy(copy, students, (size_t)studentCount * sizeof(Student));
    qsort(copy, (size_t)studentCount, sizeof(Student), byName ? compareName : compareSap);
    for (i = 0; i < studentCount; ++i) displayStudent(&copy[i]);
}

static void averageByYear(void) {
    int year, i, count;
    float sum;
    for (year = 1; year <= 4; ++year) {
        sum = 0.0f; count = 0;
        for (i = 0; i < studentCount; ++i) {
            float cgpa = cgpaOf(&students[i]);
            if (students[i].year == year && cgpa >= 0.0f) { sum += cgpa; ++count; }
        }
        if (count) printf("Year %d average CGPA: %.2f (%d student%s)\n", year, sum / count, count, count == 1 ? "" : "s");
        else printf("Year %d average CGPA: not available\n", year);
    }
}

static void academicAdvisor(void) {
    int id = readInt("Enter SAP ID for advice: ", 1, 2147483647);
    int index = findIndex(id);
    float cgpa;
    if (index < 0) { puts("Student not found."); return; }
    cgpa = cgpaOf(&students[index]);
    if (cgpa < 0.0f) { puts("No SGPA results are available for this student yet."); return; }
    printf("CGPA: %.2f — ", cgpa);
    if (cgpa >= 9.0f) puts("Excellent performance — maintain your current study pattern.");
    else if (cgpa >= 7.5f) puts("Good performance — keep a steady routine and review challenging topics.");
    else if (cgpa >= 6.0f) puts("Needs improvement — focus more on subjects with lower SGPA.");
    else puts("Requires immediate attention — meet an academic adviser and make a focused study plan.");
}

int main(void) {
    int choice;
    do {
        puts("\n=== Student Record and Result Management System ===");
        puts("1. Add student\n2. Search student\n3. Display all students\n4. Delete student");
        puts("5. Average CGPA by year\n6. Sort by SAP ID\n7. Sort by name\n8. Academic advisor\n0. Exit");
        choice = readInt("Choice: ", 0, 8);
        switch (choice) {
            case 1: addStudent(); break;
            case 2: searchStudent(); break;
            case 3: displayAll(); break;
            case 4: deleteStudent(); break;
            case 5: averageByYear(); break;
            case 6: displaySorted(0); break;
            case 7: displaySorted(1); break;
            case 8: academicAdvisor(); break;
            case 0: puts("Goodbye."); break;
        }
    } while (choice != 0);
    return 0;
}


