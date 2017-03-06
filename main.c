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
void** load_data(void** dataStructurePtrs, size_t numOfElements[2]);
int display_menu(void** dataStructurePtrs, size_t numOfElements[2]);
Employee **addEmployee(Employee **employeeArray, size_t *numOfElements);
Job **addJob(Job **jobArray, Employee **employeeArray, size_t *numOfElements);
void listEmployees(Employee **employeeArray, size_t *numOfElements);
void employeeBubbleSort(Employee **unsortedArray, size_t arraySize, int sortType);
void jobBubbleSort(Job **unsortedArray, size_t arraySize, int sortType);
time_t getDateTimeFromUser();
char *getEmployeeName(Employee **employeeArray, size_t numOfElements, int employeeIdToFind);
void save_data(void **dataStructurePtrs, size_t numOfElements[2]);
void clrscr();
void pressEnterToContinue();

void printWelcome();

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
    while (userSelection != 10){    //Keep doing this until the user inputs the 'exit' code

        userSelection = display_menu(dataStructurePtrs, numOfElements);

        //If the user input is not between 1 - 10, then present an error message...
        if(userSelection < 1 || userSelection > 10){
            puts("Invalid Selection - Please try again.");
            printf("%s", "Press [ENTER] to continue...");
            getchar();  //Wait for the user to hit [ENTER]

        }//End of if
        clrscr();   //Clear the screen

    }//Back to while condition

    //User selected Option 10
    //Save data to files and exit....
    save_data(dataStructurePtrs, numOfElements);


    //Assuming successful completion of program...
    return 0;

}

/**
 * Function printWelcome
 * This function outputs a welcome message to the user
 */
void printWelcome() {
    puts("#########################################################");
    puts("###                 Welcome to FAMS!                  ###");
    puts("###          The Farm App Management System           ###");
    puts("###                                                   ###");
    puts("###                                                   ###");
    puts("###               Written by John Byrne               ###");
    puts("### Week 8 - Final Exam - Fundamentals of Programming ###");
    puts("#########################################################");
    puts("");
    puts("");

}//End of printWelcome()


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

        }//End of for
        fclose(inputFilePtr);   //Close the Jobs file

        printf("%u job records loaded.\n", (unsigned int) numOfElements[1]);

        dataStructurePtrs[1] = jobArray;    //Save the jobArray pointer in my pointers array.

    }//End if/else
    return dataStructurePtrs;   //Return a pointer to the array containing the pointers to my data structures.

}//End of function load_data

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
    Employee **employeeArray = dataStructurePtrs[0];
    Job **jobArray = dataStructurePtrs[1];


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

    printf("\n%s", "Please make a selection :");
    int userInput;
    scanf("%d", &userInput);
    while (fgetc(stdin) != '\n'); //Clear whatever is left of the stdin buffer

    switch (userInput){
        case 1:{
            clrscr();
            puts("########################");
            puts("### Add New Employee ###");
            puts("########################\n");

            employeeArray = addEmployee(employeeArray, numOfElements);
            dataStructurePtrs[0] = employeeArray;
            puts("");

            pressEnterToContinue();
            break;
        }
        case 2:{
            clrscr();
            puts("###################");
            puts("### Add New Job ###");
            puts("###################\n");

            jobArray = addJob(jobArray, employeeArray, numOfElements);
            dataStructurePtrs[1] = jobArray;
            puts("");

            pressEnterToContinue();
            break;
        }
        case 3:{
            clrscr();
            puts("#####################");
            puts("### Employee List ###");
            puts("#####################\n");

            listEmployees(employeeArray, numOfElements);
            puts("");

            pressEnterToContinue();
            break;
        }
        case 4:{
            clrscr();
            puts("##############################");
            puts("### Sort Employees by Name ###");
            puts("##############################\n");

            employeeBubbleSort(employeeArray, numOfElements[0], 2);
            puts("");

            pressEnterToContinue();
            break;
        }
        case 5:{
            clrscr();
            puts("#############################");
            puts("### Sort Jobs by Due Date ###");
            puts("#############################\n");

            jobBubbleSort(jobArray, numOfElements[1], 4);
            puts("");

            pressEnterToContinue();
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
            //If the user enters anything that is not 1 - 9...
            //Do nothing. I let the main() function handle this eventuality
            break;
        }
    }

    return userInput;
}

/**
 * Function listEmployees
 * This function outputs a list of employees, in a tabular format
 *
 * @param employeeArray - The employeeArray data structure
 * @param numOfElements - The size_t array holding the number of elements in each array. We're interested in index[0].
 */
void listEmployees(Employee **employeeArray, size_t *numOfElements) {
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

        if(index != 0 && (index % 15 == 0)){
            puts("More records to display...");
            pressEnterToContinue();
            puts("");
            printf("%8s\n", "Employee");
            printf("%8s    %-20s\n", "Number", "Employee Name");
            printf("%8s    %-20s\n", "********", "********************");
        }
    }

}

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
Employee **addEmployee(Employee **employeeArray, size_t *numOfElements) {

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
}//End of function addEmployee

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
Job **addJob(Job **jobArray, Employee **employeeArray, size_t *numOfElements) {

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
            listEmployees(employeeArray, numOfElements);    //List available employees

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


        printf("%s", "Customer's name :");
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
}//End of function addJob

/**
 * Function bubbleSort
 * This function sorts an array in Ascending order and reports performance statistics to the user
 *
 * @param unsortedArray
 * @param numOfElements
 * @param sortType - 1 will sort by Employee Number; 2 will sort by Employee Name
 */
void employeeBubbleSort(Employee **unsortedArray, size_t arraySize, int sortType) {

    //size_t arraySize = numOfElements[0];
    clock_t startTime = clock();    //Variable to hold the number of ticks at the start of this function
    unsigned int swapCount = 0;     //Counter to track the number of data item swaps required to complete this task.
    unsigned int comparisonCount = 0;   //Counter to track the number of data item comparisons required to complete this task.

    size_t pass;    //Passes counter
    size_t i;       //Comparisons counter
    Employee *temp;     //Temporary location used to swap array elements

    // loop to control number of passes
    for (pass = 1; pass < arraySize; pass++) {

        // loop to control number of comparisons per pass
        for (i = 0; i < arraySize - 1; i++) {
            //For every comparison, increment the comparisonCount counter
            comparisonCount++;

            if(sortType == 1) {//Sort by number

                // compare adjacent elements and swap them if first
                // element is greater than second element
                if (unsortedArray[i]->number > unsortedArray[i + 1]->number) {
                    //For every swap, increment the swapCount counter
                    swapCount++;

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
                    swapCount++;

                    //Do the Swap...
                    temp = unsortedArray[i];                    //Store item[i] in temp.
                    unsortedArray[i] = unsortedArray[i + 1];    //Put item[i + 1] into location [i]
                    unsortedArray[i + 1] = temp;                //Put temp in to location [i + 1]

                } // end if
            }
        } // end inner for
    } // end outer for

    clock_t finishTime = clock();   //Variable to hold the number of ticks at the end of this function.

    //Print statistics...
    printf("Bubble Sort completed after %f seconds,\n\twith %u data swaps and %u data comparisons.\n",
           (double) (finishTime - startTime) / CLOCKS_PER_SEC, swapCount, comparisonCount);

}//End of function employeeBubbleSort

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

    clock_t startTime = clock();    //Variable to hold the number of ticks at the start of this function
    unsigned int swapCount = 0;     //Counter to track the number of data item swaps required to complete this task.
    unsigned int comparisonCount = 0;   //Counter to track the number of data item comparisons required to complete this task.

    size_t pass;    //Passes counter
    size_t i;       //Comparisons counter
    Job *temp;     //Temporary location used to swap array elements

    // loop to control number of passes
    for (pass = 1; pass < arraySize; pass++) {

        // loop to control number of comparisons per pass
        for (i = 0; i < arraySize - 1; i++) {
            //For every comparison, increment the comparisonCount counter
            comparisonCount++;

            switch(sortType){

                case 1: {//Sort by Job Number

                    // compare adjacent elements and swap them if first
                    // element is greater than second element
                    if (unsortedArray[i]->jobNumber > unsortedArray[i + 1]->jobNumber) {
                        //For every swap, increment the swapCount counter
                        swapCount++;

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
                        swapCount++;

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
                        swapCount++;

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
                        swapCount++;

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

    clock_t finishTime = clock();   //Variable to hold the number of ticks at the end of this function.

    //Print statistics...
    printf("Bubble Sort completed after %f seconds,\n\twith %u data swaps and %u data comparisons.\n",
           (double) (finishTime - startTime) / CLOCKS_PER_SEC, swapCount, comparisonCount);

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
}

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
        //Year 2038 problem?? 2 digit entry?? Todo
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