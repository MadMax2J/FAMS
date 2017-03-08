//Access to the C Standard Library
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <ctype.h>

//Preprocessor constant definitions
#define EMPLOYEE_FILE "employee.dat"
#define JOB_FILE "job.dat"
#define EMP_REPORT_FILE "_Employee_Report.txt"
#define JOB_REPORT_FILE "_Job_Report.txt"
#define VERSION 0.6

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

//// Function Prototypes ////
//File Manipulation...
void** load_data(void** dataStructurePtrs, size_t numOfElements[2]);
void save_data(void **dataStructurePtrs, size_t numOfElements[2]);

//Interface and Menus...
void printWelcome();
void topBanner();
int display_menu(void** dataStructurePtrs, size_t numOfElements[2]);
int employeeSubMenu(void **dataStructurePtrs, size_t *numOfElements);
int jobSubMenu(void **dataStructurePtrs, size_t *numOfElements);
int reportsSubMenu(void **dataStructurePtrs, size_t *numOfElements);

//Employee Functions...
Employee **add_new_employee(Employee **employeeArray, size_t *numOfElements);
void view_employees(Employee **employeeArray, size_t *numOfElements);
void employeeBubbleSort(Employee **unsortedArray, size_t arraySize, int sortType);
char *getEmployeeName(Employee **employeeArray, size_t numOfElements, int employeeIdToFind);
void findEmployeeByNumber(Employee **employeeArray, size_t numOfElements);

//Job Functions...
Job **add_new_job(Job **jobArray, Employee **employeeArray, size_t *numOfElements);
void listJobs(Employee **employeeArray, Job **jobArray, size_t *numOfElements, int boolIncComplete);
void completeJob(Employee **employeeArray, Job **jobArray, size_t *numOfElements);
void jobBubbleSort(Job **unsortedArray, size_t arraySize, int sortType);

//Report Functions...
void generateEmployeeReportFile(Employee **employeeArrayCopy, size_t arraySize);
void generateJobReportFile(Employee **employeeArray, Job **jobArray, size_t *numOfElements);

//Utility Functions...
time_t getDateTimeFromUser();
void clrscr();
void pressEnterToContinue();
//// End of Function Prototypes ////

int main() {

    void** dataStructurePtrs = {NULL};  //A pointer to an array of pointers, storing the locations of my data structures
                                        //dataStructurePtrs[0] will store the address of the employeeArray
                                        //dataStructurePtrs[1] will store the address of the jobArray
    size_t numOfElements[2] = {0};      //An array to store the number of Employees[0] and Jobs[1]

    printWelcome();

    //Load data from files into my data structures and put the
    // addresses of my data structures into the dataStructuresPtrs array...
    dataStructurePtrs = load_data(dataStructurePtrs, numOfElements);
    puts("");

    pressEnterToContinue();
    clrscr();

    //Menu System
    int userSelection = 0;  //A variable to hold the user input
    while (userSelection != 4){    //Keep doing this until the user inputs the 'exit' code

        userSelection = display_menu(dataStructurePtrs, numOfElements);

        //If the user input is not between 1 - 4, then present an error message...
        if(userSelection < 1 || userSelection > 4){
            puts("Invalid Selection - Please try again.");
            pressEnterToContinue();

        }//End of if
        clrscr();   //Clear the screen

    }//Back to while condition

    //User selected Option 10
    //Save data to files and exit....
    save_data(dataStructurePtrs, numOfElements);


    //Assuming successful completion of program...
    return 0;

}//End of function main()
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


///////////////////////////////
//// File Manipulation... ////
/////////////////////////////
/**
 * Function load_data
 * This function reads in data from my data files, initializes the data structures with
 * the correct amount of memory and returns a pointer array containing a pointer to each of my data structures.
 *
 * @param dataStructurePtrs - An array of pointers, with a pointers to each of my data arrays
 * @param numOfElements - The number of elements in each array
 * @return - A pointer to my array of pointers :)
 */
void** load_data(void **dataStructurePtrs, size_t numOfElements[2]) {

    FILE *inputFilePtr; //A pointer for the input data file being processed.

    //Bring in a local reference to my data structures...
    Employee **employeeArray = NULL;
    Job **jobArray = NULL;


    //Initialize my array of pointers, which will hold a pointer to each of my data structures...
    dataStructurePtrs = calloc(2, sizeof(void*));
    if (!dataStructurePtrs){    //Check to ensure the memory was allocated successfully
        puts("FATAL ERROR - Insufficient memory for Data. Exiting...");
        exit(-1);
    }

    //// Read in data from Employees Data file...
    if ((inputFilePtr = fopen(EMPLOYEE_FILE, "r")) == NULL) {
        printf("%s", "No existing Employee Data found. Creating new Employee Data file...");
        if ((inputFilePtr = fopen(EMPLOYEE_FILE, "w")) == NULL) {
            puts("\nFATAL ERROR - Unable to create Employee Data file. Exiting...");
            fclose(inputFilePtr);
            exit(-1);
        }
        //Created a new File
        puts(" Done!");

    } else { //Only run this section if there was an existing file that was successfully opened.
        //Existing file opened
        char temp[BUFSIZ] = ""; //A temporary holder for each record. ## BUFSIZ is declared in stdio.h as 512 ##

        //1st line of file stores the number of records
        fscanf(inputFilePtr, "%s", temp);
        numOfElements[0] = strtoul(temp, NULL, 10);

        //Allocate space for an appropriate number of Employee pointers...
        employeeArray = calloc(numOfElements[0], sizeof(Employee*));
        if (!employeeArray){
            puts("FATAL ERROR - Insufficient memory for Employee Data. Exiting...");
            exit(-1);
        }

        for (size_t i = 0; i < numOfElements[0]; i++) {   //Iterate for each Employee record...

            //Allocate memory for an Employee struct
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

        }//End of for
        fclose(inputFilePtr);   //Close the file

        printf("%u employee records loaded.\n", (unsigned int) numOfElements[0]);

        dataStructurePtrs[0] = employeeArray;   //Save the employeeArray pointer in my pointers array.
    }//End of if/else

    //// Read in data from Jobs Data file...
    if ((inputFilePtr = fopen(JOB_FILE, "r")) == NULL) {
        printf("%s", "No existing Job Data found. Creating new Job Data file...");
        if (NULL == (inputFilePtr = fopen(JOB_FILE, "w"))) {
            puts("\nFATAL ERROR - Unable to create Job Data file. Exiting...");
            fclose(inputFilePtr);
            exit(-1);
        }
        //Created a new File
        puts(" Done!");

    } else { //Only run this section if there was an existing file that was successfully opened.
        //Existing file opened
        char temp[BUFSIZ] = ""; //A temporary holder for each record. ## BUFSIZ is declared in stdio.h as 512 ##

        //1st line of file stores the number of records
        fscanf(inputFilePtr, "%s", temp);
        numOfElements[1] = strtoul(temp, NULL, 10);

        //Allocate space for an appropriate number of Employee pointers...
        jobArray = calloc(numOfElements[1], sizeof(Job*));
        if (!jobArray){
            puts("FATAL ERROR - Insufficient memory for Job Data. Exiting...");
            exit(-1);
        }
        for (size_t i = 0; i < numOfElements[1]; i++) {   //Iterate for each Employee record...

            //Allocate memory for a Job struct
            jobArray[i] = calloc(1, sizeof(Job));   //int empNumber;
                                                    //int jobNumber;
                                                    //char* customerName;
                                                    //time_t dueDate;
                                                    //time_t completionDate;
            if (!jobArray[i]){
                puts("FATAL ERROR - Insufficient memory for Job Data. Exiting...");
                exit(-1);
            }
            //File data is in the format '1 1 PizzaHut 23412341234 NULL'
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

        }//End of for
        fclose(inputFilePtr);   //Close the Jobs file

        printf("%u job records loaded.\n", (unsigned int) numOfElements[1]);

        dataStructurePtrs[1] = jobArray;    //Save the jobArray pointer in my pointers array.

    }//End if/else
    return dataStructurePtrs;   //Return a pointer to the array containing the pointers to my data structures.

}//End of function load_data

/**
 * Function save_data
 * This function writes the content of my data arrays to the employee and job files.
 *
 * @param dataStructurePtrs - Pointers to my data arrays
 * @param numOfElements - The number of elements in each array
 */
void save_data(void **dataStructurePtrs, size_t numOfElements[2]) {
    //Bring in a local reference to my data structures...
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
    }//Back to while condition

    //Sort Employee data by number before saving to file.
    employeeBubbleSort(employeeArray, numOfElements[0], 1);

    //Continuing if fopen was successful...
    fprintf(outputFilePtr, "%d", (int)numOfElements[0]);  //Top line of the file is the Number of Employees
    for (size_t x = 0; x < numOfElements[0]; x++){             //Iterate through the rest of the dataArray...
        //Send a 'newline' and the next employee record to file
        fprintf(outputFilePtr, "\n%d %s %s",
                employeeArray[x]->number, employeeArray[x]->empLastName, employeeArray[x]->empFirstName);
        // I do it this way so that I'm not left with an empty line at the end of my file.
    }//End of for

    fclose(outputFilePtr); //Close the file


    //// Do the same for my Job data


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
    }//Back to while condition

    //Sort Job data by number before saving to file.
    jobBubbleSort(jobArray, numOfElements[1], 1);

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

}//End of function save_data
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


/////////////////////////////////
//// Interface and Menus... ////
///////////////////////////////
/**
 * Function printWelcome
 * This function outputs a welcome message to the user
 */
void printWelcome() {
    puts("###############################################################################");
    puts("###                            Welcome to FAMS!                             ###");
    puts("###                     The Farm App Management System                      ###");
    puts("###                                                                         ###");
    puts("###                                                                         ###");
    puts("###                          Written by John Byrne                          ###");
    puts("###            Week 8 - Final Exam - Fundamentals of Programming            ###");
    puts("###############################################################################");
    puts("");
    puts("");

}//End of printWelcome()

void topBanner(){
    puts("###############################################################################");
    printf("###   Farm App Management System - v%0.1f                     by John Byrne   ###\n", VERSION);
    puts("###############################################################################");
    puts("");
    puts("");
}

/**
 * Function display_menu
 * This function displays the menu of options to the user and passes control to other functions based on the user's input.
 *
 * @param dataStructurePtrs - Pointers to my data arrays
 * @param numOfElements - The number of elements in each array
 * @return - returns the userInput to the main() function. Primarily for error handling.
 */
int display_menu(void **dataStructurePtrs, size_t *numOfElements) {
    //Bring in a local reference to my data structures...
    //Employee **employeeArray = dataStructurePtrs[0];
    //Job **jobArray = dataStructurePtrs[1];
    int userSelection = 0;  //A variable to hold the user input
    topBanner();
    puts("#################");
    puts("### Main Menu ###");
    puts("#################\n");

    puts("1. Employee Menu");
    puts("2. Job Menu");
    puts("3. Reports Menu\n");

    puts("4. Exit");

    printf("\n%s", "Please make a selection :");
    int userInput;
    scanf("%d", &userInput);
    while (fgetc(stdin) != '\n'); //Clear whatever is left of the stdin buffer

    switch (userInput){
        case 1:{
            clrscr();

            while (userSelection != 5){    //Keep doing this until the user inputs the 'exit' code of Employee Menu

                userSelection = employeeSubMenu(dataStructurePtrs, numOfElements);

                //If the user input is not between 1 - 5, then present an error message...
                if(userSelection < 1 || userSelection > 5){
                    puts("Invalid Selection - Please try again.");
                    pressEnterToContinue();

                }//End of if
                clrscr();   //Clear the screen

            }//Back to while condition

            break;
        }
        case 2:{
            clrscr();

            while (userSelection != 7){    //Keep doing this until the user inputs the 'exit' code of Job Menu

                userSelection = jobSubMenu(dataStructurePtrs, numOfElements);

                //If the user input is not between 1 - 5, then present an error message...
                if(userSelection < 1 || userSelection > 7){
                    puts("Invalid Selection - Please try again.");
                    pressEnterToContinue();

                }//End of if
                clrscr();   //Clear the screen

            }//Back to while condition

            break;
        }
        case 3:{
            clrscr();

            while (userSelection != 3){    //Keep doing this until the user inputs the 'exit' code of Report Menu

                userSelection = reportsSubMenu(dataStructurePtrs, numOfElements);

                //If the user input is not between 1 - 3, then present an error message...
                if(userSelection < 1 || userSelection > 3){
                    puts("Invalid Selection - Please try again.");
                    pressEnterToContinue();

                }//End of if
                clrscr();   //Clear the screen

            }//Back to while condition

            break;
        }
        default: {
            //If the user enters anything that is not 1 - 3...
            //Do nothing. I let the main() function handle this eventuality
            break;
        }
    }

    return userInput;
}

int employeeSubMenu(void **dataStructurePtrs, size_t *numOfElements){
    //Bring in a local reference to my data structures...
    Employee **employeeArray = dataStructurePtrs[0];
    topBanner();
    puts("#####################");
    puts("### Employee Menu ###");
    puts("#####################\n");

    puts("1. Add an Employee");
    puts("2. List all Employees by Name");
    puts("3. List all Employees by Number");
    puts("4. Find Employee by Number\n");

    puts("5. Back to the Main Menu");

    printf("\n%s", "Please make a selection :");
    int userInput;
    scanf("%d", &userInput);
    while (fgetc(stdin) != '\n'); //Clear whatever is left of the stdin buffer

    switch (userInput){
        case 1:{
            clrscr();
            topBanner();
            puts("########################");
            puts("### Add New Employee ###");
            puts("########################\n");

            employeeArray = add_new_employee(employeeArray, numOfElements);
            dataStructurePtrs[0] = employeeArray;
            puts("");

            pressEnterToContinue();
            break;
        }
        case 2:{
            clrscr();
            topBanner();
            puts("##############################");
            puts("### List Employees by Name ###");
            puts("##############################\n");

            employeeBubbleSort(employeeArray, numOfElements[0], 2);
            view_employees(employeeArray, numOfElements);
            puts("");

            pressEnterToContinue();
            break;
        }
        case 3:{
            clrscr();
            topBanner();
            puts("################################");
            puts("### List Employees by Number ###");
            puts("################################\n");

            employeeBubbleSort(employeeArray, numOfElements[0], 1);
            view_employees(employeeArray, numOfElements);
            puts("");

            pressEnterToContinue();
            break;
        }
        case 4:{
            clrscr();
            topBanner();
            puts("###############################");
            puts("### Find Employee by Number ###");
            puts("###############################\n");

            findEmployeeByNumber(employeeArray, numOfElements[0]);
            puts("");

            pressEnterToContinue();
            break;
        }
        default:{
            //If the user enters anything that is not 1 - 4...
            //Do nothing. To be handled by display_menu function
        }
    }//End of Switch
    return userInput;

}//End of function employeeSubMenu

int jobSubMenu(void **dataStructurePtrs, size_t *numOfElements){
    //Bring in a local reference to my data structures...
    Employee **employeeArray = dataStructurePtrs[0];
    Job **jobArray = dataStructurePtrs[1];
    topBanner();
    puts("################");
    puts("### Job Menu ###");
    puts("################\n");

    puts("1. Add a Job");
    puts("2. List outstanding Jobs to be completed by {date}");
    puts("3. List all Jobs by Customer");
    puts("4. Sort by 'Due Date' and list");
    puts("5. Set Job as 'Completed'");
    puts("6. List Jobs by employee\n");

    puts("7. Back to the Main Menu");

    printf("\n%s", "Please make a selection :");
    int userInput;
    scanf("%d", &userInput);
    while (fgetc(stdin) != '\n'); //Clear whatever is left of the stdin buffer

    switch (userInput){
        case 1:{
            clrscr();
            topBanner();
            puts("###################");
            puts("### Add New Job ###");
            puts("###################\n");

            employeeBubbleSort(employeeArray, numOfElements[0], 2);
            jobArray = add_new_job(jobArray, employeeArray, numOfElements);
            dataStructurePtrs[1] = jobArray;
            puts("");

            pressEnterToContinue();
            break;
        }
        case 2:{
            clrscr();
            topBanner();
            puts("##################################################");
            puts("### Outstanding Jobs to be completed by {date} ###");
            puts("##################################################\n");

            jobBubbleSort(jobArray, numOfElements[1], 4);
            listJobs(employeeArray, jobArray, numOfElements, 0);
            puts("");

            pressEnterToContinue();
            break;
        }
        case 3:{
            clrscr();
            topBanner();
            puts("#################################");
            puts("### List all Jobs by Customer ###");
            puts("#################################\n");

            jobBubbleSort(jobArray, numOfElements[1], 3);
            listJobs(employeeArray, jobArray, numOfElements, 1);
            puts("");

            pressEnterToContinue();
            break;
        }
        case 4:{
            clrscr();
            topBanner();
            puts("###################################");
            puts("### Sort by 'Due Date' and list ###");
            puts("###################################\n");

            jobBubbleSort(jobArray, numOfElements[1], 4);
            listJobs(employeeArray, jobArray, numOfElements, 1);
            puts("");

            pressEnterToContinue();
            break;
        }
        case 5:{
            clrscr();
            topBanner();
            puts("##############################");
            puts("### Set Job as 'Completed' ###");
            puts("##############################\n");

            completeJob(employeeArray, jobArray, numOfElements);
            puts("");

            pressEnterToContinue();
            break;
        }
        case 6:{
            clrscr();
            topBanner();
            puts("#############################");
            puts("### List Jobs by employee ###");
            puts("#############################\n");

            //ToDo ADD FUNCTION

            puts("");

            pressEnterToContinue();
            break;
        }
        default:{
            //If the user enters anything that is not 1 - 2...
            //Do nothing. To be handled by display_menu function
        }
    }//End of Switch
    return userInput;

}//End of function jobSubMenu

int reportsSubMenu(void **dataStructurePtrs, size_t *numOfElements){

    //Bring in a local reference to my data structures...
    Employee **employeeArray = dataStructurePtrs[0];
    Job **jobArray = dataStructurePtrs[1];

    topBanner();
    puts("####################");
    puts("### Reports Menu ###");
    puts("####################\n");

    puts("1. All current Employees, sorted by Name");
    puts("2. All Jobs, sorted by Customer\n");

    puts("3. Back to the Main Menu");

    printf("\n%s", "Please make a selection :");
    int userInput;
    scanf("%d", &userInput);
    while (fgetc(stdin) != '\n'); //Clear whatever is left of the stdin buffer

    switch (userInput){
        case 1:{
            clrscr();
            topBanner();
            puts("#############################################");
            puts("### All current Employees, sorted by Name ###");
            puts("#############################################\n");
            generateEmployeeReportFile(employeeArray, numOfElements[0]);

            puts("");

            pressEnterToContinue();
            break;
        }
        case 2: {
            clrscr();
            topBanner();
            puts("####################################");
            puts("### All Jobs, sorted by Customer ###");
            puts("####################################\n");
            generateJobReportFile(employeeArray, jobArray, numOfElements);

            puts("");

            pressEnterToContinue();
            break;
        }
        default:{
            //If the user enters anything that is not 1 - 4...
            //Do nothing. To be handled by display_menu function
        }
    }//End of Switch
    return userInput;

}//End of function reportsSubMenu

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


///////////////////////////////
//// Employee Functions... ////
///////////////////////////////
/**
 * Function addEmployee
 * This function adds an employee to the employeeArray data structure. But before it can do that, we need to increase
 * the memory allocated to the employeeArray. We'll reallocate a sufficient block of memory to the employeeArray, then
 * allocate memory for the Employee struct and then to the strings within the struct.
 *
 * @param employeeArray - The current employeeArray data structure
 * @param numOfElements - The size_t array holding the number of elements in each array. We're interested in index[0].
 * @return - A pointer to the location of the employeeArray, as this might change during the course of this function.
 */
Employee **add_new_employee(Employee **employeeArray, size_t *numOfElements) {

    //Format of Employee record...
    //int empID
    //char* empFName
    //char* empLName

    //Increment the employee counter and attempt to reallocate sufficient memory for the additional employee...
    Employee** originalEmployeeArrayAddress = employeeArray; //Just in-case I need to roll back.
    employeeArray = realloc(employeeArray, ++numOfElements[0] * sizeof(Employee*));
    if(employeeArray == NULL){
        //Reallocation Fail...
        puts("ERROR - Insufficient memory for new Employee. Cancelling...");
        --numOfElements[0];
        employeeArray = originalEmployeeArrayAddress;
        return employeeArray;

    }//End if
    //Allocation of new memory for additional Employee*, successful...

    //Allocate memory for the new Employee struct and save the
    // pointer in the new, last element of the employee pointer array.
    employeeArray[numOfElements[0] - 1] = calloc(1, sizeof(Employee));
    if(!(employeeArray[numOfElements[0] - 1])){
        puts("ERROR - Insufficient memory for new Employee. Cancelling...");

        //Return employeeArray to its original size and return it...
        employeeArray = realloc(employeeArray, --numOfElements[0] * sizeof(Employee*)); //No reason to think this might fail.
        return employeeArray;
    }//End of if

    //Allocation of new memory for additional Employee struct, successful...
    printf("Employee Number :%u\n", (unsigned int)numOfElements[0]);
    employeeArray[numOfElements[0] - 1]->number = (unsigned int)numOfElements[0];

    char userInput[30] = "";
    printf("\n%s", "Employee First Name :");
    scanf("%s", userInput);
    while (fgetc(stdin) != '\n'); //Clear whatever is left of the stdin buffer
    employeeArray[numOfElements[0] - 1]->empFirstName = calloc(strlen(userInput) + 1, sizeof(char));
    strcpy(employeeArray[numOfElements[0] - 1]->empFirstName, userInput);

    printf("%s", "Employee Last Name  :");
    scanf("%s", userInput);
    while (fgetc(stdin) != '\n'); //Clear whatever is left of the stdin buffer
    employeeArray[numOfElements[0] - 1]->empLastName = calloc(strlen(userInput) + 1, sizeof(char));
    strcpy(employeeArray[numOfElements[0] - 1]->empLastName, userInput);

    puts("\n\nNew Employee Added...\n");

    printf("%8s\n", "Employee");
    printf("%8s    %-20s\n", "Number", "Employee Name");
    printf("%8s    %-20s\n", "********", "********************");

    char displayName[21] = "";
    strcpy(displayName, employeeArray[numOfElements[0] - 1]->empLastName);
    strcat(displayName, ", ");
    strcat(displayName, employeeArray[numOfElements[0] - 1]->empFirstName);

    printf("%8u    %-20s\n", employeeArray[numOfElements[0] - 1]->number, displayName);

    return employeeArray; //Return the potentially updated employeeArray pointer
}//End of function add_new_employee

/**
 * Function listEmployees
 * This function outputs a list of employees, in a tabular format
 *
 * @param employeeArray - The employeeArray data structure
 * @param numOfElements - The size_t array holding the number of elements in each array. We're interested in index[0].
 */
void view_employees(Employee **employeeArray, size_t *numOfElements) {
    size_t index;
    char displayName[21] = "";

    printf("%8s\n", "Employee");
    printf("%8s    %-20s\n", "Number", "Employee Name");
    printf("%8s    %-20s\n", "********", "********************");
    for (index = 0; index < numOfElements[0]; index++) {   //Iterate for each Employee record...

        strcpy(displayName, employeeArray[index]->empLastName);
        strcat(displayName, ", ");
        strcat(displayName, employeeArray[index]->empFirstName);
        printf("%8u    %-20s\n", employeeArray[index]->number, displayName);

        if(index != 0 && (index % 13 == 0)){
            puts("More records to display...");
            pressEnterToContinue();
            puts("");
            printf("%8s\n", "Employee");
            printf("%8s    %-20s\n", "Number", "Employee Name");
            printf("%8s    %-20s\n", "********", "********************");
        }
    }

}//End of function view_employees

/**
 * Function employeeBubbleSort
 * This function sorts an array in Ascending order and reports performance statistics to the user
 *
 * @param unsortedArray
 * @param numOfElements
 * @param sortType - 1 will sort by Employee Number; 2 will sort by Employee Name
 */
void employeeBubbleSort(Employee **unsortedArray, size_t arraySize, int sortType) {

    //size_t arraySize = numOfElements[0];
    //clock_t startTime = clock();    //Variable to hold the number of ticks at the start of this function
    //unsigned int swapCount = 0;     //Counter to track the number of data item swaps required to complete this task.
    //unsigned int comparisonCount = 0;   //Counter to track the number of data item comparisons required to complete this task.

    size_t pass;    //Passes counter
    size_t i;       //Comparisons counter
    Employee *temp;     //Temporary location used to swap array elements

    // loop to control number of passes
    for (pass = 1; pass < arraySize; pass++) {

        // loop to control number of comparisons per pass
        for (i = 0; i < arraySize - 1; i++) {
            //For every comparison, increment the comparisonCount counter
            //comparisonCount++;

            if(sortType == 1) {//Sort by number

                // compare adjacent elements and swap them if first
                // element is greater than second element
                if (unsortedArray[i]->number > unsortedArray[i + 1]->number) {
                    //For every swap, increment the swapCount counter
                    //swapCount++;

                    //Do the Swap...
                    temp = unsortedArray[i];                    //Store item[i] in temp.
                    unsortedArray[i] = unsortedArray[i + 1];    //Put item[i + 1] into location [i]
                    unsortedArray[i + 1] = temp;                //Put temp in to location [i + 1]

                } // end if

            }else if(sortType == 2) {//Sort by Employee Name

                // compare adjacent elements and swap them if first
                // element is greater than second element
                if (strcmp(getEmployeeName(unsortedArray, arraySize, unsortedArray[i]->number),
                           getEmployeeName(unsortedArray, arraySize, unsortedArray[i + 1]->number)) > 0) {
                    //For every swap, increment the swapCount counter
                    //swapCount++;

                    //Do the Swap...
                    temp = unsortedArray[i];                    //Store item[i] in temp.
                    unsortedArray[i] = unsortedArray[i + 1];    //Put item[i + 1] into location [i]
                    unsortedArray[i + 1] = temp;                //Put temp in to location [i + 1]

                } // end if
            }
        } // end inner for
    } // end outer for

    //clock_t finishTime = clock();   //Variable to hold the number of ticks at the end of this function.

    //Print statistics...
    //printf("Bubble Sort completed after %f seconds,\n\t with %u data swaps and %u data comparisons.\n",
    //       (double) (finishTime - startTime) / CLOCKS_PER_SEC, swapCount, comparisonCount);

}//End of function employeeBubbleSort

/**
 * Function getEmployeeName
 * This function returns a char* containing the full name of the employeeIdToFind
 *
 * @param employeeArray - The employeeArray data structure.
 * @param numOfElements - The size_t array holding the number of elements in each array. We're interested in index[0].
 * @param employeeIdToFind - The employeeId of the employee we're interested in.
 * @return - a char* containing the full name of the employee.
 */
char *getEmployeeName(Employee **employeeArray, size_t numOfElements, int employeeIdToFind) {
//    printf("TESTING - Looking for employee number %d.", employeeIdToFind);

    char *returnName;
    size_t targetIndex = numOfElements;  //Valid size_t, but out of scope!

    for(size_t index = 0; index < numOfElements; index++){
        if(employeeArray[index]->number == employeeIdToFind){
            targetIndex = index;
        }
    }

    if(targetIndex == numOfElements){ //targetIndex was not changed, so No Match Found
        returnName = "Employee Not Found!";

    }else {

        //Allocate memory for the concatenated Employee name with extra space for ', ' and NULL
        returnName = calloc(
                strlen(employeeArray[targetIndex]->empLastName) + strlen(employeeArray[targetIndex]->empFirstName) + 3,
                sizeof(char));

        strcpy(returnName, employeeArray[targetIndex]->empLastName);
        strcat(returnName, ", ");
        strcat(returnName, employeeArray[targetIndex]->empFirstName);
    }

    return returnName;
}//End of function getEmployeeName

/**
 * Function getEmployeeName
 * This function returns a char* containing the full name of the employeeIdToFind
 *
 * @param employeeArray - The employeeArray data structure.
 * @param numOfElements - The size_t array holding the number of elements in each array. We're interested in index[0].
 * @param employeeIdToFind - The employeeId of the employee we're interested in.
 * @return - a char* containing the full name of the employee.
 */
void findEmployeeByNumber(Employee **employeeArray, size_t numOfElements) {

    int employeeIdToFind;
    do {
        printf("Employee ID to find (1 - %d):", (int) numOfElements);
        scanf("%d", &employeeIdToFind);
        while (fgetc(stdin) != '\n'); //Clear whatever is left of the stdin buffer

        if (employeeIdToFind < 1 || employeeIdToFind > numOfElements) {
            puts("\nThis is not a valid or active Employee number!");
        }
    }while(employeeIdToFind < 1 || employeeIdToFind > numOfElements);


    //char *returnName;
    size_t targetIndex = numOfElements;  //Valid size_t, but out of scope!

    for(size_t index = 0; index < numOfElements; index++){
        if(employeeArray[index]->number == employeeIdToFind){
            targetIndex = index;
        }
    }

    if(targetIndex == numOfElements){ //targetIndex was not changed, so No Match Found
        puts("Employee Not Found!");

    }else {

        puts("\n\nEmployee Found...\n");

        printf("%8s\n", "Employee");
        printf("%8s    %-20s\n", "Number", "Employee Name");
        printf("%8s    %-20s\n", "********", "********************");

        char *displayName = calloc(
                strlen(employeeArray[targetIndex]->empLastName) + strlen(employeeArray[targetIndex]->empFirstName) + 3,
                sizeof(char));
        strcpy(displayName, employeeArray[targetIndex]->empLastName);
        strcat(displayName, ", ");
        strcat(displayName, employeeArray[targetIndex]->empFirstName);

        printf("%8u    %-20s\n", employeeArray[targetIndex]->number, displayName);


    }

    //return returnName;
}//End of function getEmployeeName

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


///////////////////////////
//// Job Functions... ////
/////////////////////////
/**
 * Function addJob
 * This function adds a Job to the jobArray data structure. But before it can do that, we need to increase
 * the memory allocated to the jobArray. We'll reallocate a sufficient block of memory to the jobArray, then
 * allocate memory for the new Job struct and then to the string within the struct.
 *
 * @param jobArray - The current jobArray data structure
 * @param numOfElements - The size_t array holding the number of elements in each array. We're interested in index[1].
 * @return - A pointer to the location of the jobArray, as this might change during the course of this function.
 */
Job **add_new_job(Job **jobArray, Employee **employeeArray, size_t *numOfElements) {

    //Format of Job record...
    //int empID
    //int jobID
    //char* customerName
    //time_t dueDateTime
    //time_t completionDateTime

    if(numOfElements[0] > 0) {   //Is there at least one employee to do the job?

        //Increment the job counter and attempt to reallocate sufficient memory for the additional employee...
        Job **originalJobArrayAddress = jobArray; //Just in-case I need to roll back.
        jobArray = realloc(jobArray, ++numOfElements[1] * sizeof(Job *));
        if (jobArray == NULL) {
            //Reallocation Fail...
            puts("ERROR - Insufficient memory for new Job. Cancelling...");
            --numOfElements[1];
            jobArray = originalJobArrayAddress;
            return jobArray;


        }//End if
        //Allocation of new memory for additional Job*, successful...

        //Allocate memory for the new Job struct and save the
        // pointer in the new, last element of the employee pointer array.
        jobArray[numOfElements[1] - 1] = calloc(1, sizeof(Job));
        if (!(jobArray[numOfElements[1] - 1])) {
            puts("ERROR - Insufficient memory for new Job. Cancelling...");

            //Return jobArray to its original size and return it...
            jobArray = realloc(jobArray, --numOfElements[1] * sizeof(Job *)); //No reason to think this might fail.
            return jobArray;
        }//End of if

        //Allocation of new memory for additional Job struct, successful...
        printf("Job Number :%u\n", (unsigned int) numOfElements[1]);
        jobArray[numOfElements[1] - 1]->jobNumber = (unsigned int) numOfElements[1];

        char userInput[30] = "";    //User Input buffer

        int userInputEmployeeNumber = 0;
        while (userInputEmployeeNumber < 1 || userInputEmployeeNumber > numOfElements[0]) {
            puts("\nWhich employee is assigned to this job ...\n");
            view_employees(employeeArray, numOfElements);    //List available employees

            printf("\nEmployee Number (1 - %u):", (unsigned int)numOfElements[0]);
            scanf("%s", userInput);
            while (fgetc(stdin) != '\n'); //Clear whatever is left of the stdin buffer
            userInputEmployeeNumber = (int) strtoul(userInput, NULL, 10);

            if (userInputEmployeeNumber < 1 || userInputEmployeeNumber > numOfElements[0]) {
                puts("\nError - This is not a valid or active Employee Number. Try again...\n");
                pressEnterToContinue();
            }//End of if
        }//Back to while condition
        //Valid Employee number selected
        jobArray[numOfElements[1] - 1]->empNumber = userInputEmployeeNumber;


        printf("%s", "\nCustomer's name :");
        scanf("%s", userInput);    //Get all of the String - might include Spaces!! Need to test this //ToDo
        while (fgetc(stdin) != '\n'); //Clear whatever is left of the stdin buffer
        jobArray[numOfElements[1] - 1]->customerName = calloc(strlen(userInput) + 1, sizeof(char));
        strcpy(jobArray[numOfElements[1] - 1]->customerName, userInput);


        time_t currentTime = 0;
        time_t proposedDateTime;
        do {
            puts("\nPlease enter due date and time...");
            proposedDateTime = getDateTimeFromUser();
            currentTime = time(NULL);

            if (difftime(proposedDateTime, currentTime) < 0) {
                puts("\nThis Date / Time is in the Past! Please try again...");
            }
        } while (difftime(proposedDateTime, currentTime) < 0);
        //Valid future Due Date
        jobArray[numOfElements[1] - 1]->dueDate = proposedDateTime; //Assign Due Date to Job Struct

        //Set completion date to ZERO for the moment...
        jobArray[numOfElements[1] - 1]->completionDate = 0;


        //display Summary...
        //Generate a readable date / time in the format "Thu Apr 20, 2017 @ 12:00"
        char *engDueDate = calloc(27, sizeof(char));
        strftime(engDueDate, 26, "%a %b %d, %Y @ %H:%M", localtime(&jobArray[numOfElements[1] - 1]->dueDate));

        puts("\n\nNew Job Added...\n");

        printf("%10s%20s%17s%28s\n", "Job Number", "Assigned Employee", "Customer Name", "Due Date / Time");
        printf("%10s%20s%17s%28s\n", "**********", "*****************", "*************", "************************");
        printf("%10d%20s%17s%28s\n", jobArray[numOfElements[1] - 1]->jobNumber,
               getEmployeeName(employeeArray, numOfElements[0], jobArray[numOfElements[1] - 1]->empNumber),
               jobArray[numOfElements[1] - 1]->customerName,
               engDueDate);
    }else{
        puts("Error - Need to have at least one employee.");
        puts("Please add an employee and try again.");
    }

    return jobArray;
}//End of function add_new_job

/**
 * Function listJobs
 * This function outputs a list of jobs, in a tabular format
 *
 * @param employeeArray - The employeeArray data structure
 * @param jobArray - The jobArray data structure
 * @param numOfElements - The size_t array holding the number of elements in each array.
 */
void listJobs(Employee **employeeArray, Job **jobArray, size_t *numOfElements, int boolIncComplete) {
    size_t index;
    char *employeeDisplayName;
    char *engDueDate = calloc(27, sizeof(char)); //"Apr 20, 2017 @12:00" + NULL
    char *engCompleteDate = calloc(19, sizeof(char)); //"Thu Apr 20, 2017" + NULL

    if(boolIncComplete) {
        printf("%6s%15s%19s%21s%18s\n", "Job  ", "Customer  ", "Employee    ", "Due          ", "Completed    ");
        printf("%6s%15s%19s%21s%18s\n", "Number", "Name    ", "Name      ", "Date and Time   ", "Date      ");
        printf("%6s%15s%19s%21s%18s\n", "******", "*************", "*****************", "*******************",
               "****************");
    }else{
        printf("%6s%15s%19s%21s\n", "Job  ", "Customer  ", "Employee    ", "Due          ");
        printf("%6s%15s%19s%21s\n", "Number", "Name    ", "Name      ", "Date and Time   ");
        printf("%6s%15s%19s%21s\n", "******", "*************", "*****************", "*******************");
    }

    int boolSkip;
    int count;
    for (index = 0; index < numOfElements[1]; index++) {   //Iterate for each Job record...
        boolSkip = 0;
        count = 0;
        if (jobArray[index]->completionDate == 0){
            strcpy(engCompleteDate, "In progress...");
        }else {
            boolSkip = 1;
            //Generate a readable date / time in the format "Thu Apr 20, 2017 @ 12:00"
            strftime(engCompleteDate, 18, "%a %b %d, %Y", localtime(&jobArray[index]->completionDate));
        }
        if(!boolIncComplete && boolSkip){
            //Skip this record altogether
        }else {
            count++;
            employeeDisplayName = getEmployeeName(employeeArray, numOfElements[0], jobArray[index]->empNumber);

            //Generate a readable date / time in the format "Thu Apr 20, 2017 @ 12:00"
            strftime(engDueDate, 26, "%b %d, %Y @%H:%M", localtime(&jobArray[index]->dueDate));


            printf("%6d  %-13s  %-17s%21s  %-16s\n", jobArray[index]->jobNumber, jobArray[index]->customerName,
                   employeeDisplayName, engDueDate, boolIncComplete ? engCompleteDate : "");
        }
        if(count != 0 && (count % 13 == 0)){
            puts("More records to display...");
            pressEnterToContinue();
            puts("");
            if(boolIncComplete) {
                printf("%6s%15s%19s%21s%18s\n", "Job  ", "Customer  ", "Employee    ", "Due          ", "Completed    ");
                printf("%6s%15s%19s%21s%18s\n", "Number", "Name    ", "Name      ", "Date and Time   ", "Date      ");
                printf("%6s%15s%19s%21s%18s\n", "******", "*************", "*****************", "*******************",
                       "****************");
            }else{
                printf("%6s%15s%19s%21s\n", "Job  ", "Customer  ", "Employee    ", "Due          ");
                printf("%6s%15s%19s%21s\n", "Number", "Name    ", "Name      ", "Date and Time   ");
                printf("%6s%15s%19s%21s\n", "******", "*************", "*****************", "*******************");
            }
        }
    }

}//End of function listJobs

/**
 * Function completeJob
 * This function is used to mark a job as complete, by assigning a job a valid completion time_t value.
 * We display a list of un-completed jobs and ask the user to select which one they'd like to complete.
 * We then ask for a date of completion and assign it to the job.
 *
 * @param employeeArray - The employee data structure
 * @param jobArray - The job data structure
 * @param numOfElements - An array storing the number of elements in my data structures
 */
void completeJob(Employee **employeeArray, Job **jobArray, size_t *numOfElements){

    size_t targetIndex = numOfElements[1];  //Valid size_t, but out of scope!
    int jobId;
    do {
        jobId = 0;
        listJobs(employeeArray, jobArray, numOfElements, 0);
        printf("\nWhich job has been completed (1 - %d) :", (int) numOfElements[1]);
        scanf("%d", &jobId);
        while (fgetc(stdin) != '\n'); //Clear whatever is left of the stdin buffer

        for (size_t index = 0; index < numOfElements[1]; index++) {
            if (jobArray[index]->jobNumber == jobId) {
                targetIndex = index;
            }
        }

        if (jobArray[targetIndex]->completionDate != 0 || targetIndex == numOfElements[1]) {
            //The job selected was either already completed, or targetIndex was not changed, so No Match Found
            puts("\nThis is not a valid selection or the job has already been completed!\n");
            pressEnterToContinue();
            puts("");

        }
    }while(jobArray[targetIndex]->completionDate != 0 || targetIndex == numOfElements[1]);
    puts("");

    //Generate a readable date / time in the format "Thu Apr 20, 2017 @ 12:00"
    char *engDueDate = calloc(27, sizeof(char));
    strftime(engDueDate, 26, "%a %b %d, %Y @ %H:%M", localtime(&jobArray[targetIndex]->dueDate));

    puts("You have selected the following job to complete...");
    printf("%10s%20s%17s%28s\n", "Job Number", "Assigned Employee", "Customer Name", "Due Date / Time");
    printf("%10s%20s%17s%28s\n", "**********", "*****************", "*************", "************************");
    printf("%10d%20s%17s%28s\n", jobArray[targetIndex]->jobNumber,
           getEmployeeName(employeeArray, numOfElements[0], jobArray[targetIndex]->empNumber),
           jobArray[targetIndex]->customerName,
           engDueDate);

    //I could have asked the user to enter a completion date / time...
    /*
    puts("\nPlease enter the completion Date and Time...");

    time_t currentTime;
    do {
        jobArray[targetIndex]->completionDate = getDateTimeFromUser();
        currentTime = time(NULL);
        if (jobArray[targetIndex]->completionDate > currentTime) {
            puts("You can't set a job as complete with a date in the future!\n");

        }
    }while(jobArray[targetIndex]->completionDate > currentTime);
    */

    //...but the requirement is just to use the Current Date/Time.
    jobArray[targetIndex]->completionDate = time(NULL);

    if(jobArray[targetIndex]->completionDate < jobArray[targetIndex]->dueDate){
        //Complete ahead of schedule
        puts("\nThank you! Well done for getting this order completed early!\nExtra 5% commission credited!");

    }else if(jobArray[targetIndex]->completionDate > jobArray[targetIndex]->dueDate){
        //Order was completed late
        puts("\nThank you!\nThis order was late and as a result, a 5% discount has been applied!");

    }else {
        //Order was completed on time to the second!
        //Or if user a user entered dateTime, in the 30min window that it was due.
        puts("\nThank you! Job Complete on-time!");
    }


}

int findJobIndexByNumber(Job **jobArray, size_t numOfElements) {

    int jobIdToFind;

    do {
        printf("Job ID to find (1 - %d):", (int) numOfElements);
        scanf("%d", &jobIdToFind);
        while (fgetc(stdin) != '\n'); //Clear whatever is left of the stdin buffer

        if (jobIdToFind < 1 || jobIdToFind > numOfElements) {
            puts("\nThis is not a valid or active Job number!");
        }
    }while(jobIdToFind < 1 || jobIdToFind > numOfElements);


    //char *returnName;
    size_t targetIndex = numOfElements;  //Valid size_t, but out of scope!

    for(size_t index = 0; index < numOfElements; index++){
        if(jobArray[index]->jobNumber == jobIdToFind){
            targetIndex = index;
        }
    }

    if(targetIndex == numOfElements){ //targetIndex was not changed, so No Match Found
        puts("Job Not Found!");
        return -1;

    }else {
        return (int)targetIndex;
    }

    //return returnName;
}//End of function getEmployeeName

/**
 * Function jobBubbleSort
 * This function sorts an array in Ascending order and reports performance statistics to the user
 *
 * @param unsortedArray
 * @param numOfElements
 * @param sortType -    1 will sort by Job Number
 *                      2 will sort by Employee Number
 *                      3 will sort by Customer
 *                      4 will sort by Due Date
 */
void jobBubbleSort(Job **unsortedArray, size_t arraySize, int sortType) {

    //clock_t startTime = clock();    //Variable to hold the number of ticks at the start of this function
    //unsigned int swapCount = 0;     //Counter to track the number of data item swaps required to complete this task.
    //unsigned int comparisonCount = 0;   //Counter to track the number of data item comparisons required to complete this task.

    size_t pass;    //Passes counter
    size_t i;       //Comparisons counter
    Job *temp;     //Temporary location used to swap array elements

    // loop to control number of passes
    for (pass = 1; pass < arraySize; pass++) {

        // loop to control number of comparisons per pass
        for (i = 0; i < arraySize - 1; i++) {
            //For every comparison, increment the comparisonCount counter
            //comparisonCount++;

            switch(sortType){

                case 1: {//Sort by Job Number

                    // compare adjacent elements and swap them if first
                    // element is greater than second element
                    if (unsortedArray[i]->jobNumber > unsortedArray[i + 1]->jobNumber) {
                        //For every swap, increment the swapCount counter
                        //swapCount++;

                        //Do the Swap...
                        temp = unsortedArray[i];                    //Store item[i] in temp.
                        unsortedArray[i] = unsortedArray[i + 1];    //Put item[i + 1] into location [i]
                        unsortedArray[i + 1] = temp;                //Put temp in to location [i + 1]

                    } // end if
                    break;
                }

                case 2: {//Sort by Employee

                    // compare adjacent elements and swap them if first
                    // element is greater than second element
                    if (unsortedArray[i]->empNumber > unsortedArray[i + 1]->empNumber) {
                        //For every swap, increment the swapCount counter
                        //swapCount++;

                        //Do the Swap...
                        temp = unsortedArray[i];                    //Store item[i] in temp.
                        unsortedArray[i] = unsortedArray[i + 1];    //Put item[i + 1] into location [i]
                        unsortedArray[i + 1] = temp;                //Put temp in to location [i + 1]

                    } // end if
                    break;
                }

                case 3: {//Sort by Customer Name
                    // compare adjacent elements and swap them if first
                    // element is greater than second element
                    if (strcmp(unsortedArray[i]->customerName, unsortedArray[i + 1]->customerName) > 0) {
                        //For every swap, increment the swapCount counter
                        //swapCount++;

                        //Do the Swap...
                        temp = unsortedArray[i];                    //Store item[i] in temp.
                        unsortedArray[i] = unsortedArray[i + 1];    //Put item[i + 1] into location [i]
                        unsortedArray[i + 1] = temp;                //Put temp in to location [i + 1]

                    }
                        break;
                }
                case 4:{//Sort by Due Date
                    // compare adjacent elements and swap them if first
                    // element is greater than second element
                    if (unsortedArray[i]->dueDate > unsortedArray[i + 1]->dueDate) {
                        //For every swap, increment the swapCount counter
                        //swapCount++;

                        //Do the Swap...
                        temp = unsortedArray[i];                    //Store item[i] in temp.
                        unsortedArray[i] = unsortedArray[i + 1];    //Put item[i + 1] into location [i]
                        unsortedArray[i + 1] = temp;                //Put temp in to location [i + 1]

                    } // end if
                    break;
                }
                default:{
                    //Do nothing!
                }
            }
        } // end inner for
    } // end outer for

    //clock_t finishTime = clock();   //Variable to hold the number of ticks at the end of this function.

    //Print statistics...
    //printf("Bubble Sort completed after %f seconds,\n\t with %u data swaps and %u data comparisons.\n",
    //       (double) (finishTime - startTime) / CLOCKS_PER_SEC, swapCount, comparisonCount);

}//End of function employeeBubbleSort
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

///////////////////////////////
//// Report Functions... ////
/////////////////////////////

/**
 * Function generateEmployeeReportFile
 * This function is will product a report detailing the current Employees, sorted by name and output to a file.
 * @param employeeArrayCopy - The Employee data Structure
 * @param arraySize - The number of elements in the data Structure
 */
void generateEmployeeReportFile(Employee **employeeArray, size_t arraySize){

    Employee **employeeArrayCopy = calloc(arraySize, sizeof(Employee*));
    if (!employeeArrayCopy){
        puts("FATAL ERROR - Insufficient memory to Copy Employee Data. Cancelling...");
        //return;
    }else {

        //Make a copy of the Employ Struct 'Pointers'
        memcpy(employeeArrayCopy, employeeArray, arraySize * sizeof(Employee*));

        //Sort the employeeArrayCopy pointers, in order of Employee name
        employeeBubbleSort(employeeArrayCopy, arraySize, 2);

        /////////////////////////

        FILE *outputFilePtr = NULL;//A pointer to my output file.

        //Generate a date / time prefix that I'll use as part of my output filename
        time_t currentDateTime = time(NULL);
        char *employeeReportFile = calloc(8 + strlen(EMP_REPORT_FILE) + 1, sizeof(char)); //20170308 + EMP_REPORT_FILE + NULL char
        strftime(employeeReportFile, 9, "%Y%m%d", localtime(&currentDateTime));
        strcat(employeeReportFile, EMP_REPORT_FILE);

        char *fullDateTime = calloc(50, sizeof(char));
        strftime(fullDateTime, 49, "%A, %B %d, %Y @ %I:%M%p", localtime(&currentDateTime));

        //Check that I can open the file...
        if ((outputFilePtr = fopen(employeeReportFile, "w")) == NULL) { //If opening fails...
            fprintf(stderr, "Error opening output file '%s'. Cancelling...\n", employeeReportFile);

        }else {
            //Continuing if fopen was successful...
            printf("Generating Report, Stand By...");
            //Report Header of the file...
            fprintf(outputFilePtr,
                    "###############################################################################\n"
                    "###             FAMS - Employee Report, sorted by Employee Name             ###\n"
                    "###############################################################################\n\n\n"
                            "Report generated :%s.\n\n", fullDateTime);


            char displayName[64] = "";

            fprintf(outputFilePtr, "%8s", "Employee");
            fprintf(outputFilePtr, "\n%8s    %-20s", "Number ", "Employee Name");
            fprintf(outputFilePtr, "\n%8s    %-20s", "********", "********************");
            for (size_t index = 0; index < arraySize; index++) {   //Iterate for each Employee record...

                strcpy(displayName, employeeArrayCopy[index]->empLastName);
                strcat(displayName, ", ");
                strcat(displayName, employeeArrayCopy[index]->empFirstName);
                fprintf(outputFilePtr, "\n%8u    %-20s", employeeArrayCopy[index]->number, displayName);

            }

        }//End of if / else

        fclose(outputFilePtr); //Close the file

        printf("Complete!\nReport saved to '%s'\n", employeeReportFile);


    }//End of For (Allocating memory to Employee**
}

/**
 * Function generateEmployeeReportFile
 * This function is will product a report detailing the current Employees, sorted by name and output to a file.
 * @param employeeArrayCopy - The Employee data Structure
 * @param numOfElements - The number of elements in the data Structure
 */
void generateJobReportFile(Employee **employeeArray, Job **jobArray, size_t *numOfElements){

    Job **jobArrayCopy = calloc(numOfElements[1], sizeof(Job*));
    if (!jobArrayCopy){
        puts("FATAL ERROR - Insufficient memory to Copy Employee Data. Cancelling...");
        //return;
    }else {

        //Make a copy of the Employ Struct 'Pointers'
        memcpy(jobArrayCopy, jobArray, numOfElements[1] * sizeof(Job*));

        //Sort the jobArrayCopy pointers, in order of Employee name
        jobBubbleSort(jobArrayCopy, numOfElements[1], 3);

        /////////////////////////

        FILE *outputFilePtr = NULL;//A pointer to my output file.

        //Generate a date / time prefix that I'll use as part of my output filename
        time_t currentDateTime = time(NULL);
        char *jobReportFile = calloc(8 + strlen(JOB_REPORT_FILE) + 1, sizeof(char)); //20170308 + JOB_REPORT_FILE + NULL char
        strftime(jobReportFile, 9, "%Y%m%d", localtime(&currentDateTime));
        strcat(jobReportFile, JOB_REPORT_FILE);

        char *fullDateTime = calloc(50, sizeof(char));
        strftime(fullDateTime, 49, "%A, %B %d, %Y @ %I:%M%p", localtime(&currentDateTime));

        //Check that I can open the file...
        if ((outputFilePtr = fopen(jobReportFile, "w")) == NULL) { //If opening fails...
            fprintf(stderr, "Error opening output file '%s'. Cancelling...\n", jobReportFile);

        }else {
            //Continuing if fopen was successful...
            printf("Generating Report, Stand By...");
            //Report Header of the file...
            fprintf(outputFilePtr,
                    "###############################################################################\n"
                            "###               FAMS - Job Report, sorted by Customer Name                ###\n"
                            "###############################################################################\n\n\n"
                            "Report generated :%s.\n\n", fullDateTime);


            size_t index;
            char *employeeDisplayName;
            char *engDueDate = calloc(20, sizeof(char)); //"Apr 20, 2017 @12:00" + NULL
            char *engCompleteDate = calloc(17, sizeof(char)); //"Thu Apr 20, 2017" + NULL


            fprintf(outputFilePtr, "%6s%15s%19s%21s%18s", "Job  ", "Customer  ", "Employee    ", "Due          ", "Completed    ");
            fprintf(outputFilePtr, "\n%6s%15s%19s%21s%18s", "Number", "Name    ", "Name      ", "Date and Time   ", "Date      ");
            fprintf(outputFilePtr, "\n%6s%15s%19s%21s%18s", "******", "*************", "*****************", "*******************",
                   "****************");


            for (index = 0; index < numOfElements[1]; index++) {   //Iterate for each Job record...
                if (jobArray[index]->completionDate == 0){
                    strcpy(engCompleteDate, "In progress...");
                }else {
                    //Generate a readable date / time in the format "Thu Apr 20, 2017 @ 12:00" ToDo
                    strftime(engCompleteDate, 18, "%a %b %d, %Y", localtime(&jobArray[index]->completionDate));
                }

                    employeeDisplayName = getEmployeeName(employeeArray, numOfElements[0], jobArray[index]->empNumber);

                    //Generate a readable date / time in the format "Thu Apr 20, 2017 @ 12:00"
                    strftime(engDueDate, 26, "%b %d, %Y @%H:%M", localtime(&jobArray[index]->dueDate));


                    fprintf(outputFilePtr, "\n%6d  %-13s  %-17s%21s  %-16s", jobArray[index]->jobNumber, jobArray[index]->customerName,
                           employeeDisplayName, engDueDate, engCompleteDate);


            }

        }//End of if / else

        fclose(outputFilePtr); //Close the file

        printf("Complete!\nReport saved to '%s'\n", jobReportFile);


    }//End of For (Allocating memory to Employee**
}




///////////////////////////////
//// Utility Functions... ////
/////////////////////////////
/**
 * Function getDateTimeFromUser
 * This function asks the users for the year, month, day, hour and half-hour, and returns an appropriate time_t representation.
 *
 * @return - a time_t representation of the user input date and time.
 */
time_t getDateTimeFromUser() {
    //Get Due Year
    //Get Due Month
    //Get Due Day
    //Get Due Hour
    //Get Due Minute (in 30min chunks)
    char *month[] = {"Spacer", "January", "February", "March", "April", "May", "June",
                     "July", "August", "September", "October", "November", "December"};

    time_t proposedDateTime = 0;

    //Resolve Year
    int proposedYear = 0;
    do {
        printf("%s", "Year (YYYY) :");
        scanf("%d", &proposedYear);
        while (fgetc(stdin) != '\n'); //Clear whatever is left of the stdin buffer
        if (proposedYear < 1900 || proposedYear > 2037) {
            puts("This year is out of range!");
        }

    } while (proposedYear < 1900 || proposedYear > 2037);

    //Resolve Month
    int proposedMonth = -1;
    do {
        printf("%s", "Month (1 - 12) :");
        scanf("%d", &proposedMonth);
        while (fgetc(stdin) != '\n'); //Clear whatever is left of the stdin buffer
        if (proposedMonth < 1 || proposedMonth > 12) {
            puts("This month is out of range!");
        }
    } while (proposedMonth < 1 || proposedMonth > 12);

    //Resolve Day
    int proposedDay = -1;
    int validMaxDay = (proposedMonth == 2 ?
                       (proposedYear % 4 ? 28 : (proposedYear % 100 ? 29 : (proposedYear % 400 ? 28 : 29))) :
                       ((proposedMonth - 1) % 7 % 2 ? 30 : 31)); //From Week2 Discussion! :)
    do {
        printf("Day (1 - %d) :", validMaxDay);
        scanf("%d", &proposedDay);
        while (fgetc(stdin) != '\n'); //Clear whatever is left of the stdin buffer
        if (proposedDay < 1) {
            puts("This day is out of range!");
        } else if (proposedDay > validMaxDay) {
            if(proposedMonth == 2) { //February
                printf("The month of %s in the year %d, does not have %d days!\n", month[proposedMonth],
                       proposedYear, proposedDay);
            }else{  //Any other month
                printf("The month of %s, does not have %d days!\n", month[proposedMonth], proposedDay);
            }
        }
    } while (proposedDay < 1 || proposedDay > validMaxDay);

    //Resolve Hour
    int proposedHour = -1;
    do {
        printf("%s", "Hour (0 - 23) :");
        scanf("%d", &proposedHour);
        while (fgetc(stdin) != '\n'); //Clear whatever is left of the stdin buffer
        if (proposedHour < 0 || proposedHour > 23) {
            puts("This hour is out of range!");
        }
    } while (proposedHour < 0 || proposedHour > 23);

    //Resolve Minute
    int proposedMinute = -1;
    do {
        printf("%s", "Minute - Half hour interval(0 or 30) :");
        scanf("%d", &proposedMinute);
        while (fgetc(stdin) != '\n'); //Clear whatever is left of the stdin buffer
        if (proposedMinute != 0 && proposedMinute != 30) {
            puts("This is not a Half hour interval!");
        }
    } while (proposedMinute != 0 && proposedMinute != 30);

    struct tm proposedTimeStruct;
    proposedTimeStruct.tm_year = proposedYear - 1900;
    proposedTimeStruct.tm_mon = proposedMonth - 1; //April
    proposedTimeStruct.tm_mday = proposedDay;
    proposedTimeStruct.tm_hour = proposedHour;
    proposedTimeStruct.tm_min = proposedMinute;

    proposedTimeStruct.tm_isdst = -1;
    proposedTimeStruct.tm_sec = 0;
    proposedTimeStruct.tm_wday = -1;
    proposedTimeStruct.tm_yday = -1;

    proposedDateTime = mktime(&proposedTimeStruct);

    return proposedDateTime;

}//End of function getDateTimeFromUser

/**
 * Function clrscr
 * This function simply clears the content of the console window
 */
void clrscr(){
    system("@cls||clear");
}//End of function clrscr

/**
 * Function pressEnterToContinue
 * This function simply pauses the execution of the program, until the user hits the [ENTER] key.
 */
void pressEnterToContinue(){
    printf("%s", "Press [ENTER] to continue...");
    getchar();
}//End of function pressEnterToContinue