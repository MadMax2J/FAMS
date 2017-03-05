//Access to the C Standard Library
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <ctype.h>

//Preprocessor constant definitions
#define EMPLOYEE_FILE "../employee.dat"
#define JOB_FILE "../job.dat"

//Struct definitions
typedef struct {
    int number;             //The employee number
    char* empLastName;          //The employee Last name
    char* empFirstName;          //The employee First name
}Employee;

typedef struct {
    int empNumber;          //The assigned employee number
    int jobNumber;          //The job number
    char* customerName;     //The customer's name
    time_t dueDate;         //The job due date
    time_t completionDate;  //The job's actual completion date
}Job;

//Function Prototypes
int display_menu(void** dataStructurePtrs, size_t numOfElements[2]);
void clrscr();
int getUserInput();
void** load_data(void** dataStructurePtrs, size_t numOfElements[2]);


void save_data(void **dataStructurePtrs, size_t numOfElements[2]);

int main() {

    void** dataStructurePtrs = {NULL};  //A pointer to an array of pointers, storing the locations of my data structures
                                        //dataStructurePtrs[0] will store the address of the employeeArray
                                        //dataStructurePtrs[1] will store the address of the jobArray
    size_t numOfElements[2] = {0};      //An array to store the number of Employees[0] and Jobs[1]


    //Load data from files into my data structures and put the
    // addresses of my data structures into the dataStructuresPtrs array...
    dataStructurePtrs = load_data(dataStructurePtrs, numOfElements);


    //Menu System
    int userSelection = 0;
    while (userSelection != 10){

        userSelection = display_menu(dataStructurePtrs, numOfElements);

        if(userSelection < 1 || userSelection > 10){
            puts("Invalid Selection - Please try again.");
            printf("%s", "Press [ENTER] to continue...");
            getchar();

        }
        clrscr();

    }

    //User selected Option 10
    //Save data to files and exit....
    save_data(dataStructurePtrs, numOfElements);


    //Assuming successful completion of program...
    return 0;
}

void save_data(void **dataStructurePtrs, size_t numOfElements[2]) {
    Employee **employeeArray = dataStructurePtrs[0];
    Job **jobArray = dataStructurePtrs[1];

    FILE *outputFilePtr = NULL;//A pointer to my output file.
    //Check that I can open the file...
    int retry = 1;
    while (retry) {
        if ((outputFilePtr = fopen(EMPLOYEE_FILE, "w")) == NULL) { //If opening fails...
            int userInput;

            fprintf(stderr, "Error opening output file '%s'.\n", EMPLOYEE_FILE);
            fprintf(stderr, "%s\n", "Please ensure the file is not open in another program.\n");
            fprintf(stderr, "%s\n", "This process needs to succeed, or changes made in this session will be lost.");
            fprintf(stderr, "%s", "Retry [y]es or [n]o :");

            userInput = toupper(getchar());
            while (fgetc(stdin) != '\n');   //Clear whatever is left of the stdin buffer
            if(userInput == 78) exit(-1);   //78 is ASCII 'N', So, just exit without saving data.

        }else {
            retry = 0;
        }//End of if / else
    }

    //Continuing if fopen was successful...
    fprintf(outputFilePtr, "%d", (int)numOfElements[0]);  //Top line of the file is the Number of Employees
    for (size_t x = 0; x < numOfElements[0]; x++){             //Iterate through the rest of the dataArray...
        //Send a 'newline' and the next employee record to file
        fprintf(outputFilePtr, "\n%d %s %s",
                employeeArray[x]->number, employeeArray[x]->empLastName, employeeArray[x]->empFirstName);
        // I do it this way so that I'm not left with an empty line at the end of my file.
    }//End of for

    fclose(outputFilePtr); //Close the file

    /////////////////////////////
    //Do the same for my Job data
    /////////////////////////////

    //Check that I can open the file...
    retry = 1;
    while (retry) {
        if ((outputFilePtr = fopen(JOB_FILE, "w")) == NULL) { //If opening fails...
            int userInput;

            fprintf(stderr, "Error opening output file '%s'.\n", JOB_FILE);
            fprintf(stderr, "%s\n", "Please ensure the file is not open in another program.\n");
            fprintf(stderr, "%s\n", "This process needs to succeed, or changes made in this session will be lost.");
            fprintf(stderr, "%s", "Retry [y]es or [n]o :");

            userInput = toupper(getchar());
            while (fgetc(stdin) != '\n');   //Clear whatever is left of the stdin buffer
            if(userInput == 78) exit(-1);   //78 is ASCII 'N', So, just exit without saving data.

        }else {
            retry = 0;
        }//End of if / else
    }

    //Continuing if fopen was successful...
    fprintf(outputFilePtr, "%d", (int)numOfElements[1]);  //Top line of the file is the Number of Jobs
    for (size_t x = 0; x < numOfElements[1]; x++){             //Iterate through the rest of the dataArray...
        //Send a 'newline' and the next employee record to file
        fprintf(outputFilePtr, "\n%d %d %s %llu %llu",
                jobArray[x]->empNumber, jobArray[x]->jobNumber, jobArray[x]->customerName,
                jobArray[x]->dueDate, jobArray[x]->completionDate);
        // I do it this way so that I'm not left with an empty line at the end of my file.
    }//End of for

    fclose(outputFilePtr); //Close the file


}

void** load_data(void **dataStructurePtrs, size_t numOfElements[2]) {

    FILE *inputFilePtr;
    Employee **employeeArray = NULL;
    Job **jobArray = NULL;



    dataStructurePtrs = calloc(2, sizeof(void*));
    if (!dataStructurePtrs){
        puts("FATAL ERROR - Insufficient memory for Data. Exiting...");
        exit(-1);
    }
    //// Read in data from Employees Data file...
    if ((inputFilePtr = fopen(EMPLOYEE_FILE, "r")) == NULL) {
        puts("No existing Employee Data found. Creating new Employee Data file...");
        if ((inputFilePtr = fopen(EMPLOYEE_FILE, "w")) == NULL) {
            puts("FATAL ERROR - Unable to create Employee Data file. Exiting...");
            exit(-1);
        }
        //Created a new File
        puts("Done!");

    } else { //Only run this section if there was an existing file that was successfully opened.
        //Existing file opened
        char temp[BUFSIZ] = ""; //A temporary holder for each record. ## BUFSIZ is declared in stdio.h as 512 ##

        //1st line of file stores the number of records
        fscanf(inputFilePtr, "%s", temp);
        numOfElements[0] = strtoul(temp, NULL, 10);

        ////TESTING
        printf("Number of Employees is :%u\n\n", (unsigned int) numOfElements[0]);
        ///////////

        //Allocate space for an appropriate number of Employee pointers...
        employeeArray = calloc(numOfElements[0], sizeof(Employee*));
        if (!employeeArray){
            puts("FATAL ERROR - Insufficient memory for Employee Data. Exiting...");
            exit(-1);
        }

        for (size_t i = 0; i < numOfElements[0]; i++) {   //Iterate for each Employee record...

            //Allocate memory of an Employee struct
            employeeArray[i] = calloc(1, sizeof(Employee));
            if (!employeeArray[i]){
                puts("FATAL ERROR - Insufficient memory for Employee Data. Exiting...");
                exit(-1);
            }
            //File data is in the format '1 Byrne John'
            //Get the Employee number and store it in the data structure
            fscanf(inputFilePtr, "%s", temp); //Employee Number
            employeeArray[i]->number = (int) strtoul(temp, NULL, 10);

            //Get the Employee Last name, allocate the required memory and store in the data structure
            fscanf(inputFilePtr, "%s", temp); //Employee Last Name
            employeeArray[i]->empLastName = calloc(strlen(temp) + 1, sizeof(char));
            if (!employeeArray[i]->empLastName){
                puts("FATAL ERROR - Insufficient memory for Employee Data. Exiting...");
                exit(-1);
            }
            strcpy(employeeArray[i]->empLastName, temp);

            //Get the Employee First name, allocate the required memory and store in the data structure
            fscanf(inputFilePtr, "%s", temp); //Employee First Name
            employeeArray[i]->empFirstName = calloc(strlen(temp) + 1, sizeof(char));
            if (!employeeArray[i]->empFirstName){
                puts("FATAL ERROR - Insufficient memory for Employee Data. Exiting...");
                exit(-1);
            }
            strcpy(employeeArray[i]->empFirstName, temp);

        }
        ////TESTING////
/*        for (size_t i = 0; i < numOfElements[0]; i++) {   //Iterate for each Employee record...
            printf("%u\t%s, %s\n", employeeArray[i]->number, employeeArray[i]->empLastName,
                   employeeArray[i]->empFirstName);
        }
*/        ///////////////

        fclose(inputFilePtr);
        dataStructurePtrs[0] = employeeArray;
    }

    //// Read in data from Jobs Data file...
    if ((inputFilePtr = fopen(JOB_FILE, "r")) == NULL) {
        puts("No existing Job Data found. Creating new Job Data file...");
        if ((inputFilePtr = fopen(JOB_FILE, "w")) == NULL) {
            puts("FATAL ERROR - Unable to create Job Data file. Exiting...");
            exit(-1);
        }
        //Created a new File
        puts("Done!");

    } else { //Only run this section if there was an existing file that was successfully opened.
        //Existing file opened
        char temp[BUFSIZ] = ""; //A temporary holder for each record. ## BUFSIZ is declared in stdio.h as 512 ##

        //1st line of file stores the number of records
        fscanf(inputFilePtr, "%s", temp);
        numOfElements[1] = strtoul(temp, NULL, 10);

        ////TESTING
        printf("Number of Jobs is :%u\n\n", (unsigned int) numOfElements[1]);
        ///////////

        //Allocate space for an appropriate number of Employee pointers...
        jobArray = calloc(numOfElements[1], sizeof(Job*));
        if (!jobArray){
            puts("FATAL ERROR - Insufficient memory for Job Data. Exiting...");
            exit(-1);
        }
        for (size_t i = 0; i < numOfElements[1]; i++) {   //Iterate for each Employee record...

            //Allocate memory of an Employee struct
            jobArray[i] = calloc(1, sizeof(Job));   //int empNumber;
                                                    //int jobNumber;
                                                    //char* customerName;
                                                    //time_t dueDate;
                                                    //time_t completionDate;
            if (!jobArray[i]){
                puts("FATAL ERROR - Insufficient memory for Job Data. Exiting...");
                exit(-1);
            }
            //File data is in the format '1 1 McDonnalds 23412341234 NULL'
            //Get the Employee number and store it in the data structure
            fscanf(inputFilePtr, "%s", temp); //Employee Number
            jobArray[i]->empNumber = (int) strtoul(temp, NULL, 10);

            //Get the Job number and store it in the data structure
            fscanf(inputFilePtr, "%s", temp); //Job Number
            jobArray[i]->jobNumber = (int) strtoul(temp, NULL, 10);


            //Get the Customer name, allocate the required memory and store in the data structure
            fscanf(inputFilePtr, "%s", temp); //Customer Name
            jobArray[i]->customerName = calloc(strlen(temp) + 1, sizeof(char));
            if (!jobArray[i]->customerName){
                puts("FATAL ERROR - Insufficient memory for Job Data. Exiting...");
                exit(-1);
            }
            strcpy(jobArray[i]->customerName, temp);

            //Get the Due Date and store in the data structure
            fscanf(inputFilePtr, "%s", temp); //Due Date
            jobArray[i]->dueDate = (time_t)strtoull(temp, NULL, 10);

            //Get the Completion Date and store in the data structure
            fscanf(inputFilePtr, "%s", temp); //Completion Date
            jobArray[i]->completionDate = (time_t)strtoull(temp, NULL, 10);

        }

        ////TESTING////
        //int empNumber;
        //int jobNumber;
        //char* customerName;
        //time_t dueDate;
        //time_t completionDate;
    //    for (size_t i = 0; i < numOfElements[1]; i++) {   //Iterate for each Job record...
      //      printf("%u\t%u\t%s\t%lu\t%lu\n", jobArray[i]->empNumber, jobArray[i]->jobNumber,
        //           jobArray[i]->customerName, (unsigned long)jobArray[i]->dueDate, (unsigned long)jobArray[i]->completionDate);
        //}
        ///////////////

        fclose(inputFilePtr);
        dataStructurePtrs[1] = jobArray;
    }
    return dataStructurePtrs;

}

int display_menu(void **dataStructurePtrs, size_t *numOfElements) {

    Employee **employeeArray = dataStructurePtrs[0];
    Job **jobArray = dataStructurePtrs[1];


/*
    puts("TESTING ALL");

    for (size_t i = 0; i < numOfElements[1]; i++) {   //Iterate for each Job record...
        printf("%u\t%u\t%s\t%lu\t%lu\n", jobArray[i]->empNumber, jobArray[i]->jobNumber,
               jobArray[i]->customerName, (unsigned long)jobArray[i]->dueDate, (unsigned long)jobArray[i]->completionDate);
    }
*/

    //time_t now = time(NULL);
    //printf("TESTING %lu\n", (unsigned long)now);

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

    switch (userInput){
        case 1:{
            puts("Item 1");
            printf("%s", "Press [ENTER] to continue...");
            getchar();
            break;
        }
        case 2:{
            puts("Item 2");
            printf("%s", "Press [ENTER] to continue...");
            getchar();
            break;
        }
        case 3:{
            puts("Item 3");
            for (size_t i = 0; i < numOfElements[0]; i++) {   //Iterate for each Employee record...
                printf("%u\t%s, %s\n", employeeArray[i]->number, employeeArray[i]->empLastName,
                       employeeArray[i]->empFirstName);
            }
            printf("%s", "Press [ENTER] to continue...");
            getchar();
            break;
        }
        case 4:{
            puts("Item 4");
            printf("%s", "Press [ENTER] to continue...");
            getchar();
            break;
        }
        case 5:{
            puts("Item 5");
            printf("%s", "Press [ENTER] to continue...");
            getchar();
            break;
        }
        case 6:{
            puts("Item 6");
            for (size_t i = 0; i < numOfElements[1]; i++) {   //Iterate for each Job record...
                printf("%u\t%u\t%s\t%lu\t%lu\n", jobArray[i]->empNumber, jobArray[i]->jobNumber,
                       jobArray[i]->customerName, (unsigned long)jobArray[i]->dueDate, (unsigned long)jobArray[i]->completionDate);
            }
            printf("%s", "Press [ENTER] to continue...");
            getchar();
            break;
        }
        case 7:{
            puts("Item 7");
            printf("%s", "Press [ENTER] to continue...");
            getchar();
            break;
        }
        case 8:{
            puts("Item 8");
            printf("%s", "Press [ENTER] to continue...");
            getchar();
            break;
        }
        case 9:{
            puts("Item 9");
            printf("%s", "Press [ENTER] to continue...");
            getchar();
            break;
        }

        default: {
            break;
        }
    }

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