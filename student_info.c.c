#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#ifdef _WIN32
    #define CLEAR "cls"
    #define PAUSE "pause"
#else
    #define CLEAR "clear"
    #define PAUSE "read -p 'Press enter to continue...'"
#endif

// Student structure definition
struct Stud {
    char name[100];
    char dept[50];
    char sem[50];
    int ID;
    float cgpa;
};

// Function declarations
void add(FILE *fp);
FILE *del(FILE *fp);
void edit(FILE *fp);
void displayList(FILE *fp);
void searchRecord(FILE *fp);
void printChar(char ch, int n);
void printHead();

int main() {
    FILE *fp;
    int option;

    // Open file or create if it doesn't exist
    if ((fp = fopen("studentInfo.txt", "rb+")) == NULL) {
        if ((fp = fopen("studentInfo.txt", "wb+")) == NULL) {
            printf("Can't open file.\n");
            return 1;
        }
    }

    while (1) {
        printHead();
        printf("\n\n\t\t\t1. INSERT Student");
        printf("\n\n\t\t\t2. DELETE Student");
        printf("\n\n\t\t\t3. EDIT Student LIST");
        printf("\n\n\t\t\t4. DISPLAY Student LIST");
        printf("\n\n\t\t\t5. SEARCH Record");
        printf("\n\n\t\t\t0. EXIT");

        printf("\n\n\t\tEnter Your Option : ");
        scanf("%d", &option);

        switch (option) {
            case 0:
                fclose(fp);
                return 0;
            case 1:
                add(fp);
                break;
            case 2:
                fp = del(fp);
                break;
            case 3:
                edit(fp);
                break;
            case 4:
                displayList(fp);
                break;
            case 5:
                searchRecord(fp);
                break;
            default:
                printf("\n\t\tInvalid Option!\n");
                system(PAUSE);
        }
    }
}

// Prints a character multiple times
void printChar(char ch, int n) {
    for (int i = 0; i < n; i++) {
        putchar(ch);
    }
}

// Clears the screen and prints header
void printHead() {
    system(CLEAR);
    printf("\n\n\t");
    printChar('=', 16);
    printf(" [STUDENT  INFORMATION  SYSTEM] ");
    printChar('=', 16);
    printf("\n");
}

// Adds a new student record
void add(FILE *fp) {
    struct Stud s;
    char another = 'y';
    fseek(fp, 0, SEEK_END);

    while (another == 'y' || another == 'Y') {
        printHead();

        printf("\n\n\t\tEnter Full Name of Student:\t");
        fflush(stdin);
        fgets(s.name, sizeof(s.name), stdin);
        s.name[strcspn(s.name, "\n")] = 0;

        printf("\n\n\t\tEnter Department:\t");
        fgets(s.dept, sizeof(s.dept), stdin);
        s.dept[strcspn(s.dept, "\n")] = 0;

        printf("\n\n\t\tEnter Semester: \t");
        fgets(s.sem, sizeof(s.sem), stdin);
        s.sem[strcspn(s.sem, "\n")] = 0;

        printf("\n\n\t\tEnter ID: \t");
        scanf("%d", &s.ID);

        printf("\n\n\t\tEnter CGPA: \t");
        scanf("%f", &s.cgpa);

        fwrite(&s, sizeof(s), 1, fp);

        printf("\n\n\t\tAdd another student? (Y/N): ");
        fflush(stdin);
        another = getchar();
    }
}

// Deletes a student record by ID
FILE *del(FILE *fp) {
    struct Stud s;
    FILE *ft;
    int tempID, flag = 0;

    printHead();
    printf("\n\n\tEnter ID of Student to Delete the Record: ");
    scanf("%d", &tempID);

    if ((ft = fopen("temp.txt", "wb+")) == NULL) {
        printf("\n\t\t!!! ERROR !!!\n");
        system(PAUSE);
        return fp;
    }

    rewind(fp);
    while (fread(&s, sizeof(s), 1, fp) == 1) {
        if (s.ID == tempID) {
            flag = 1;
            printf("\n\tRecord Deleted for:\n\t\t%s\n\t\t%s\n\t\t%s\n\t\t%d\n\t\t%.2f",
                   s.name, s.dept, s.sem, s.ID, s.cgpa);
        } else {
            fwrite(&s, sizeof(s), 1, ft);
        }
    }

    fclose(fp);
    fclose(ft);
    remove("studentInfo.txt");
    rename("temp.txt", "studentInfo.txt");

    if ((fp = fopen("studentInfo.txt", "rb+")) == NULL) {
        printf("ERROR opening updated file.\n");
        return NULL;
    }

    if (!flag)
        printf("\n\n\t\t**** RECORD NOT FOUND ****");

    printChar('-', 65);
    printf("\n\t");
    system(PAUSE);
    return fp;
}

// Edits a student record
void edit(FILE *fp) {
    struct Stud s;
    int tempID, flag = 0;

    printHead();
    printf("\n\n\tEnter ID of Student to EDIT the Record: ");
    scanf("%d", &tempID);

    rewind(fp);
    while (fread(&s, sizeof(s), 1, fp) == 1) {
        if (s.ID == tempID) {
            flag = 1;
            fseek(fp, -sizeof(s), SEEK_CUR);
            break;
        }
    }

    if (flag) {
        printf("\n\n\t\tRECORD FOUND. Enter New Data:\n");

        printf("\n\n\t\tEnter Full Name: ");
        fflush(stdin);
        fgets(s.name, sizeof(s.name), stdin);
        s.name[strcspn(s.name, "\n")] = 0;

        printf("\n\n\t\tEnter Department: ");
        fgets(s.dept, sizeof(s.dept), stdin);
        s.dept[strcspn(s.dept, "\n")] = 0;

        printf("\n\n\t\tEnter Semester: ");
        fgets(s.sem, sizeof(s.sem), stdin);
        s.sem[strcspn(s.sem, "\n")] = 0;

        printf("\n\n\t\tEnter ID: ");
        scanf("%d", &s.ID);

        printf("\n\n\t\tEnter CGPA: ");
        scanf("%f", &s.cgpa);

        fwrite(&s, sizeof(s), 1, fp);
    } else {
        printf("\n\n\t!!!! ERROR !!!! RECORD NOT FOUND");
    }

    printf("\n\n\t");
    system(PAUSE);
}

// Displays all student records
void displayList(FILE *fp) {
    struct Stud s;

    printHead();
    rewind(fp);

    while (fread(&s, sizeof(s), 1, fp) == 1) {
        printf("\n\t\tNAME : %s", s.name);
        printf("\n\t\tDEPARTMENT : %s", s.dept);
        printf("\n\t\tSEMESTER : %s", s.sem);
        printf("\n\t\tID : %d", s.ID);
        printf("\n\t\tCGPA: %.2f", s.cgpa);
        printf("\n\t");
        printChar('-', 50);
    }

    printf("\n\n\t");
    system(PAUSE);
}

// Searches for a student record by ID
void searchRecord(FILE *fp) {
    struct Stud s;
    int tempID, flag, siz;
    char another = 'y';

    siz = sizeof(s);

    while (another == 'y' || another == 'Y') {
        flag = 0;
        printHead();

        printf("\n\n\tEnter ID Number of Student to search: ");
        scanf("%d", &tempID);

        rewind(fp);
        while (fread(&s, siz, 1, fp) == 1) {
            if (s.ID == tempID) {
                flag = 1;
                break;
            }
        }

        if (flag) {
            printf("\n\t\tNAME : %s", s.name);
            printf("\n\t\tDEPARTMENT : %s", s.dept);
            printf("\n\t\tSEMESTER : %s", s.sem);
            printf("\n\t\tID : %d", s.ID);
            printf("\n\t\tCGPA: %.2f", s.cgpa);
        } else {
            printf("\n\n\t\t!!!! ERROR RECORD NOT FOUND !!!!");
        }

        printf("\n\t");
        printChar('-', 50);
        printf("\n\n\t\tSearch again? (Y/N): ");
        fflush(stdin);
        another = getchar();
    }
}
