#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

// Cross-platform sleep and clear screen sathi  ☠️
#ifdef _WIN32
    #include <windows.h>
    #define CLEAR_SCREEN "cls"
    #define SLEEP(ms) Sleep(ms)
#else
    #include <unistd.h>
    #define CLEAR_SCREEN "clear"
    #define SLEEP(ms) usleep((ms) * 1000)
#endif

// Rangit UI sathi ANSI Color Codes 🙂
#define RESET   "\033[0m"
#define RED     "\033[1;31m"
#define GREEN   "\033[1;32m"
#define YELLOW  "\033[1;33m"
#define BLUE    "\033[1;34m"
#define MAGENTA "\033[1;35m"
#define CYAN    "\033[1;36m"
#define WHITE   "\033[1;37m"

// File chi nav
#define BOOK_FILE "books_data.dat"
#define MEMBER_FILE "members_data.dat"
#define ISSUE_FILE "issues_data.dat"

// book imfosathi 
typedef struct {
    int id;
    char title[100];
    char author[100];
    char category[50];
    int quantity;
    int available;
} Book;

// Member cha data save local storege madhi
typedef struct {
    int id;
    char name[100];
    char phone[15];
    int books_issued;
} Member;

// Pustak issue kelyachi mahiti ka structure
typedef struct {
    int issue_id;
    int book_id;
    int member_id;
    char issue_date[20];
    int is_returned;
} IssueRecord;

// Functions declare karayala
void clearScreen();
void printHeader();
void loadingAnimation();
void flushInput();
char* getCurrentDate();

// Book Functions 😕
void manageBooks();
void addBook();
void viewAllBooks();
void searchBook();
void deleteBook();

// Member Functions sathi 
void manageMembers();
void addMember();
void viewAllMembers();

// Issue/Return Functions la 
void manageTransactions();
void issueBook();
void returnBook();
void viewIssuedBooks();

// Main function starting point
int main() {
    int choice;
    loadingAnimation(); // loading bar la

    while (1) {
        clearScreen();
        printHeader();
        
        // Main Menu ul 
        printf(CYAN "\n\t\t[ MAIN MENU ]\n" RESET);
        printf(WHITE "\t\t1. " YELLOW "Manage Books\n" RESET);
        printf(WHITE "\t\t2. " YELLOW "Manage Members\n" RESET);
        printf(WHITE "\t\t3. " YELLOW "Issue / Return Books\n" RESET);
        printf(WHITE "\t\t4. " RED "Exit System\n" RESET);
        printf(CYAN "\n\t\t==================================================\n" RESET);
        printf(GREEN "\t\tEnter your choice: " RESET);
        
        if (scanf("%d", &choice) != 1) {
            flushInput();
            continue;
        }

        // number choosing method 
        switch (choice) {
            case 1: manageBooks(); break;
            case 2: manageMembers(); break;
            case 3: manageTransactions(); break;
            case 4: 
                printf(MAGENTA "\n\t\tThank you for using  Library System!\n" RESET);
                exit(0);
            default: 
                printf(RED "\n\t\tInvalid Choice! Please try again.\n" RESET);
                SLEEP(1500);
        }
    }
    return 0;
}

// Screen clear karnya sathi 
void clearScreen() {
    system(CLEAR_SCREEN);
}

// Input buffer clear karnya sathi 
void flushInput() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

// date sathi 
char* getCurrentDate() {
    static char dateStr[20];
    time_t t = time(NULL);
    struct tm tm = *localtime(&t);
    sprintf(dateStr, "%02d-%02d-%d", tm.tm_mday, tm.tm_mon + 1, tm.tm_year + 1900);
    return dateStr;
}

// first top ul 
void printHeader() {
    printf(MAGENTA);
    printf("\n\t\t==================================================\n");
    printf("\t\t████  █   █  █████      █     █████  █████\n");
    printf("\t\t█   █ █   █  █          █       █    █   █\n");
    printf("\t\t█   █ █████  ████       █       █    █████\n");
    printf("\t\t█   █ █   █  █          █       █    █   █\n");
    printf("\t\t████  █   █  █████      █████ █████  █   █\n");
    printf("\t\t==================================================\n");
    printf(CYAN "\t\t      LIBRARY MANAGEMENT      \n");
    printf("\t\t==================================================\n" RESET);
}

// loading animation
void loadingAnimation() {
    clearScreen();
    printf(CYAN "\n\n\n\n\n\t\t\tInitializing System...\n\t\t\t" RESET);
    for (int i = 0; i < 20; i++) {
        printf(GREEN "█" RESET);
        fflush(stdout);
        SLEEP(100); // Thoda vel thambnya sathi
    }
}

// BOOK MANAGEMENT code 
void manageBooks() {
    int choice;
    while (1) {
        clearScreen();
        printHeader();
        printf(CYAN "\n\t\t[ BOOK MANAGEMENT ]\n" RESET);
        printf(WHITE "\t\t1. Add New Book\n");
        printf("\t\t2. View All Books\n");
        printf("\t\t3. Search Book\n");
        printf("\t\t4. Delete Book\n");
        printf("\t\t5. " YELLOW "Back to Main Menu\n" RESET);
        printf(GREEN "\n\t\tEnter choice: " RESET);
        
        scanf("%d", &choice);
        flushInput();

        switch (choice) {
            case 1: addBook(); break;
            case 2: viewAllBooks(); break;
            case 3: searchBook(); break;
            case 4: deleteBook(); break;
            case 5: return; // Main menu madhe parat janya sathi
            default: printf(RED "\t\tInvalid choice!\n" RESET); SLEEP(1000);
        }
    }
}

// Navin pustak add karnya sathi
void addBook() {
    FILE *fp = fopen(BOOK_FILE, "ab"); // Append mode madhe file open keli
    if (!fp) {
        printf(RED "\t\tError opening file!\n" RESET);
        return;
    }

    Book b;
    printf(CYAN "\n\t\t--- Add New Book ---\n" RESET);
    printf("\t\tEnter Book ID: ");
    scanf("%d", &b.id);
    flushInput();

    printf("\t\tEnter Book Title: ");
    fgets(b.title, sizeof(b.title), stdin);
    b.title[strcspn(b.title, "\n")] = 0; // Newline character kadhun taknya sathi

    printf("\t\tEnter Author Name: ");
    fgets(b.author, sizeof(b.author), stdin);
    b.author[strcspn(b.author, "\n")] = 0;

    printf("\t\tEnter Category: ");
    fgets(b.category, sizeof(b.category), stdin);
    b.category[strcspn(b.category, "\n")] = 0;

    printf("\t\tEnter Quantity: ");
    scanf("%d", &b.quantity);
    b.available = b.quantity; // Suruvatila sagli pustake available ahet

    fwrite(&b, sizeof(Book), 1, fp); // File madhe data write kela
    fclose(fp);

    printf(GREEN "\n\t\tBook Added Successfully!\n" RESET);
    printf("\t\tPress Enter to continue...");
    flushInput();
    getchar();
}

// Sagli pustake dakhvnya sathi
void viewAllBooks() {
    FILE *fp = fopen(BOOK_FILE, "rb"); // Read mode madhe file open keli
    if (!fp) {
        printf(RED "\n\t\tNo books found in the database!\n" RESET);
        SLEEP(2000);
        return;
    }

    Book b;
    clearScreen();
    printf(MAGENTA "\n================================================================================================\n" RESET);
    printf(CYAN "%-5s %-30s %-25s %-15s %-10s %-10s\n" RESET, "ID", "TITLE", "AUTHOR", "CATEGORY", "TOTAL", "AVAIL");
    printf(MAGENTA "================================================================================================\n" RESET);

    int count = 0;
    // File madhun ek ek record read karat ahot
    while (fread(&b, sizeof(Book), 1, fp)) {
        printf("%-5d %-30.30s %-25.25s %-15.15s %-10d %-10d\n", 
               b.id, b.title, b.author, b.category, b.quantity, b.available);
        count++;
    }
    fclose(fp);

    if (count == 0) {
        printf(RED "\n\t\tNo records available.\n" RESET);
    }

    printf(MAGENTA "================================================================================================\n" RESET);
    printf("\n\t\tPress Enter to return...");
    getchar();
}

// Pustak search karnya sathi
void searchBook() {
    FILE *fp = fopen(BOOK_FILE, "rb");
    if (!fp) return;

    int searchId, found = 0;
    Book b;
    printf(CYAN "\n\t\tEnter Book ID to search: " RESET);
    scanf("%d", &searchId);
    flushInput();

    // Loop lavun pustak shodhat ahot
    while (fread(&b, sizeof(Book), 1, fp)) {
        if (b.id == searchId) {
            printf(GREEN "\n\t\tBook Found!\n" RESET);
            printf("\t\tTitle: %s\n", b.title);
            printf("\t\tAuthor: %s\n", b.author);
            printf("\t\tCategory: %s\n", b.category);
            printf("\t\tAvailable: %d / %d\n", b.available, b.quantity);
            found = 1;
            break;
        }
    }
    fclose(fp);

    if (!found) {
        printf(RED "\n\t\tBook with ID %d not found!\n" RESET, searchId);
    }
    printf("\n\t\tPress Enter to continue...");
    getchar();
}

// Pustak delete karnya sathi (Temporary file cha wapar karun)
void deleteBook() {
    FILE *fp = fopen(BOOK_FILE, "rb");
    FILE *temp = fopen("temp.dat", "wb");
    if (!fp || !temp) return;

    int delId, found = 0;
    Book b;
    printf(CYAN "\n\t\tEnter Book ID to delete: " RESET);
    scanf("%d", &delId);
    flushInput();

    // Je pustak delete karayche ahe te sodun baki sagli pustake temp file madhe taka
    while (fread(&b, sizeof(Book), 1, fp)) {
        if (b.id == delId) {
            found = 1;
        } else {
            fwrite(&b, sizeof(Book), 1, temp);
        }
    }
    
    fclose(fp);
    fclose(temp);

    remove(BOOK_FILE); // Juni file delete keli
    rename("temp.dat", BOOK_FILE); // Temp file la navin nav dile

    if (found) printf(GREEN "\n\t\tBook deleted successfully!\n" RESET);
    else printf(RED "\n\t\tBook not found!\n" RESET);
    
    printf("\n\t\tPress Enter to continue...");
    getchar();
}

// MEMBER MANAGEMENT la
void manageMembers() {
    int choice;
    while (1) {
        clearScreen();
        printHeader();
        printf(CYAN "\n\t\t[ MEMBER MANAGEMENT ]\n" RESET);
        printf(WHITE "\t\t1. Add New Member\n");
        printf("\t\t2. View All Members\n");
        printf("\t\t3. " YELLOW "Back to Main Menu\n" RESET);
        printf(GREEN "\n\t\tEnter choice: " RESET);
        
        scanf("%d", &choice);
        flushInput();

        switch (choice) {
            case 1: addMember(); break;
            case 2: viewAllMembers(); break;
            case 3: return;
            default: printf(RED "\t\tInvalid choice!\n" RESET); SLEEP(1000);
        }
    }
}

void addMember() {
    FILE *fp = fopen(MEMBER_FILE, "ab");
    if (!fp) return;

    Member m;
    printf(CYAN "\n\t\t--- Add New Member ---\n" RESET);
    printf("\t\tEnter Member ID: ");
    scanf("%d", &m.id);
    flushInput();

    printf("\t\tEnter Name: ");
    fgets(m.name, sizeof(m.name), stdin);
    m.name[strcspn(m.name, "\n")] = 0;

    printf("\t\tEnter Phone Number: ");
    fgets(m.phone, sizeof(m.phone), stdin);
    m.phone[strcspn(m.phone, "\n")] = 0;

    m.books_issued = 0; 

    fwrite(&m, sizeof(Member), 1, fp);
    fclose(fp);

    printf(GREEN "\n\t\tMember Added Successfully!\n" RESET);
    printf("\t\tPress Enter to continue...");
    getchar();
}

void viewAllMembers() {
    FILE *fp = fopen(MEMBER_FILE, "rb");
    if (!fp) {
        printf(RED "\n\t\tNo members found!\n" RESET);
        SLEEP(2000);
        return;
    }

    Member m;
    clearScreen();
    printf(MAGENTA "\n======================================================================\n" RESET);
    printf(CYAN "%-10s %-30s %-15s %-10s\n" RESET, "ID", "NAME", "PHONE", "ISSUED");
    printf(MAGENTA "======================================================================\n" RESET);

    while (fread(&m, sizeof(Member), 1, fp)) {
        printf("%-10d %-30.30s %-15s %-10d\n", m.id, m.name, m.phone, m.books_issued);
    }
    fclose(fp);

    printf(MAGENTA "======================================================================\n" RESET);
    printf("\n\t\tPress Enter to return...");
    getchar();
}
// TRANSACTION Mode
void manageTransactions() {
    int choice;
    while (1) {
        clearScreen();
        printHeader();
        printf(CYAN "\n\t\t[ ISSUE / RETURN BOOKS ]\n" RESET);
        printf(WHITE "\t\t1. Issue a Book\n");
        printf("\t\t2. Return a Book\n");
        printf("\t\t3. View Issued Records\n");
        printf("\t\t4. " YELLOW "Back to Main Menu\n" RESET);
        printf(GREEN "\n\t\tEnter choice: " RESET);
        
        scanf("%d", &choice);
        flushInput();

        switch (choice) {
            case 1: issueBook(); break;
            case 2: returnBook(); break;
            case 3: viewIssuedBooks(); break;
            case 4: return;
            default: printf(RED "\t\tInvalid choice!\n" RESET); SLEEP(1000);
        }
    }
}

// Pustak issue karnya sathi
void issueBook() {
    int bId, mId, foundBook = 0, foundMember = 0;
    Book b;
    Member m;
    
    printf(CYAN "\n\t\t--- Issue Book ---\n" RESET);
    printf("\t\tEnter Book ID: ");
    scanf("%d", &bId);
    printf("\t\tEnter Member ID: ");
    scanf("%d", &mId);
    flushInput();

    // Pustak available ahe ka te check kara
    FILE *fb = fopen(BOOK_FILE, "rb+");
    while (fread(&b, sizeof(Book), 1, fb)) {
        if (b.id == bId) {
            foundBook = 1;
            if (b.available > 0) {
                b.available--; // Available count kami kela
                fseek(fb, -sizeof(Book), SEEK_CUR);
                fwrite(&b, sizeof(Book), 1, fb);
            } else {
                printf(RED "\n\t\tBook is currently out of stock!\n" RESET);
                fclose(fb);
                SLEEP(2000);
                return;
            }
            break;
        }
    }
    fclose(fb);

    if (!foundBook) {
        printf(RED "\n\t\tBook ID not found!\n" RESET);
        SLEEP(2000);
        return;
    }

    // Member ahe ka te check kara
    FILE *fm = fopen(MEMBER_FILE, "rb+");
    while (fread(&m, sizeof(Member), 1, fm)) {
        if (m.id == mId) {
            foundMember = 1;
            m.books_issued++; // Member chya account var pustak add kele
            fseek(fm, -sizeof(Member), SEEK_CUR);
            fwrite(&m, sizeof(Member), 1, fm);
            break;
        }
    }
    fclose(fm);

    if (!foundMember) {
        printf(RED "\n\t\tMember ID not found!\n" RESET);
        // Member naslyas pustak parat available kara (Rollback)
        fb = fopen(BOOK_FILE, "rb+");
        while (fread(&b, sizeof(Book), 1, fb)) {
            if (b.id == bId) {
                b.available++;
                fseek(fb, -sizeof(Book), SEEK_CUR);
                fwrite(&b, sizeof(Book), 1, fb);
                break;
            }
        }
        fclose(fb);
        SLEEP(2000);
        return;
    }

    // Issue record save kara
    FILE *fi = fopen(ISSUE_FILE, "ab");
    IssueRecord ir;
    ir.issue_id = time(NULL) % 10000; // Random ID generate kela
    ir.book_id = bId;
    ir.member_id = mId;
    strcpy(ir.issue_date, getCurrentDate());
    ir.is_returned = 0; // 0 mhanje ajun return kele nahi

    fwrite(&ir, sizeof(IssueRecord), 1, fi);
    fclose(fi);

    printf(GREEN "\n\t\tBook Issued Successfully! (Issue ID: %d)\n" RESET, ir.issue_id);
    printf("\t\tPress Enter to continue...");
    getchar();
}

// Pustak parat ghenya sathi (Return)
void returnBook() {
    int bId, mId, found = 0;
    IssueRecord ir;
    
    printf(CYAN "\n\t\t--- Return Book ---\n" RESET);
    printf("\t\tEnter Book ID: ");
    scanf("%d", &bId);
    printf("\t\tEnter Member ID: ");
    scanf("%d", &mId);
    flushInput();

    FILE *fi = fopen(ISSUE_FILE, "rb+");
    if (!fi) return;

    // Issue record shodha
    while (fread(&ir, sizeof(IssueRecord), 1, fi)) {
        if (ir.book_id == bId && ir.member_id == mId && ir.is_returned == 0) {
            ir.is_returned = 1; // Status update kela (Returned)
            fseek(fi, -sizeof(IssueRecord), SEEK_CUR);
            fwrite(&ir, sizeof(IssueRecord), 1, fi);
            found = 1;
            break;
        }
    }
    fclose(fi);

    if (found) {
        // Pustakachi availability vadhva
        Book b;
        FILE *fb = fopen(BOOK_FILE, "rb+");
        while (fread(&b, sizeof(Book), 1, fb)) {
            if (b.id == bId) {
                b.available++;
                fseek(fb, -sizeof(Book), SEEK_CUR);
                fwrite(&b, sizeof(Book), 1, fb);
                break;
            }
        }
        fclose(fb);

        // Member chya account madhun pustak kami kara
        Member m;
        FILE *fm = fopen(MEMBER_FILE, "rb+");
        while (fread(&m, sizeof(Member), 1, fm)) {
            if (m.id == mId) {
                m.books_issued--;
                fseek(fm, -sizeof(Member), SEEK_CUR);
                fwrite(&m, sizeof(Member), 1, fm);
                break;
            }
        }
        fclose(fm);

        printf(GREEN "\n\t\tBook Returned Successfully!\n" RESET);
    } else {
        printf(RED "\n\t\tNo active issue record found for this Book and Member!\n" RESET);
    }

    printf("\t\tPress Enter to continue...");
    getchar();
}

// Issue keleli sagli pustake dakhvnya sathi
void viewIssuedBooks() {
    FILE *fp = fopen(ISSUE_FILE, "rb");
    if (!fp) {
        printf(RED "\n\t\tNo issue records found!\n" RESET);
        SLEEP(2000);
        return;
    }

    IssueRecord ir;
    clearScreen();
    printf(MAGENTA "\n======================================================================\n" RESET);
    printf(CYAN "%-10s %-10s %-10s %-15s %-15s\n" RESET, "ISSUE ID", "BOOK ID", "MEMBER ID", "DATE", "STATUS");
    printf(MAGENTA "======================================================================\n" RESET);

    while (fread(&ir, sizeof(IssueRecord), 1, fp)) {
        printf("%-10d %-10d %-10d %-15s ", ir.issue_id, ir.book_id, ir.member_id, ir.issue_date);
        if (ir.is_returned) {
            printf(GREEN "Returned\n" RESET);
        } else {
            printf(RED "Issued\n" RESET);
        }
    }
    fclose(fp);

    printf(MAGENTA "======================================================================\n" RESET);
    printf("\n\t\tPress Enter to return...");
    getchar();
}
