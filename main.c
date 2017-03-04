//Access to the C Standard Library
#include <stdio.h>
#include <stdlib.h>

//Preprocessor constant definitions
#define EMPLOYEE_FILE "../employee.dat"
#define JOB_FILE "../job.dat"

//Struct definitions
typedef struct {
    int number;             //The employee number
    char* empName;          //The employee name
}Employee;

typedef struct {
    int empNumber;          //The assigned employee number
    int jobNumber;          //The job number
    char* customerName;     //The customer's name
    time_t dueDate;         //The job due date
    time_t completionDate;  //The job's actual completion date
}Job;

//Function Prototypes
int mainMenu();
void clrscr();

int getUserInput();

int main() {


    int userSelection = 0;

    while (userSelection != 10){
        clrscr();

        userSelection = mainMenu();

        if(userSelection < 1 || userSelection > 10){
            puts("Invalid Selection - Please try again.");
            printf("%s", "Press [ENTER] to continue...");
            getchar();

        }

    }
    //User selected Option 10
    //Save data to files and exit....

    //saveData();

    //Assuming successful completion of program...
    return 0;
}

int mainMenu() {



    puts("#################");
    puts("### Main Menu ###");
    puts("#################\n");

    puts("1. Add an Employee");
    puts("2. Add a Job");
    puts("3. List all Employees");
    puts("4. Sort Employees by Name");
    puts("5. List all Jobs by due date");
    puts("6. List all Jobs by Customer");
    puts("7. Sort Jobs by due date and time and list??");
    puts("8. Set Job as completed");
    puts("9. List Jobs by employee\n");

    puts("10. Exit");

    printf("%s", "Please make a selection :");
    int userInput;
    scanf("%d", &userInput);
    while (fgetc(stdin) != '\n'); //Clear whatever is left of the stdin buffer

    return userInput;
}

int getUserInput() {

    int myInt;
    int result = scanf("%d", &myInt);

    if (result == EOF) {
        /* ... you're not going to get any input ... */
    }
    if (result == 0) {
        while (fgetc(stdin) != '\n') // Read until a newline is found
            ;
    }


    return myInt;
}

void clrscr(){
    system("@cls||clear");
}