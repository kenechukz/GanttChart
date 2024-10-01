#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#define NUM_OF_MONTHS 12
#define MAX_NAME_LENGTH 20
#define MAX_NUM_TASKS 10

#ifdef _WIN32
    #define CLEAR_SCREEN "cls"
#else
    #define CLEAR_SCREEN "clear"
#endif

enum months
{
    JAN,
    FEB,
    MAR,
    APR,
    MAY,
    JUN,
    JUL,
    AUG,
    SEP,
    OCT,
    NOV,
    DEC
};

struct Row 
{
    char taskName[20];
    int startMonth, endMonth;
    bool months[NUM_OF_MONTHS];
    int dependencies[MAX_NUM_TASKS-1];
};


void fillMonths(int start, int end, bool monthBool[])
{
    for (int j = 0; j < NUM_OF_MONTHS; j++)
        {
            monthBool[j] = false;
        }
        for (int j = start - 1; j < end; j++)
        {
            monthBool[j] = true;
        }
}

void printline() 
{
    puts("|----------------------|-------|-------|-------|-------|-------|-------|-------|-------|-------|-------|-------|-------|---------------------|");
}

void printheader() 
{
    // use an enum to store the months
    printf("|  Task Name:          |");
    for (enum months i = JAN; i <= DEC; i++)
    {
        switch(i)
        {
            case JAN: printf("  JAN  |"); break;
            case FEB: printf("  FEB  |"); break;
            case MAR: printf("  MAR  |"); break;
            case APR: printf("  APR  |"); break;
            case MAY: printf("  MAY  |"); break;
            case JUN: printf("  JUN  |"); break;
            case JUL: printf("  JUL  |"); break;
            case AUG: printf("  AUG  |"); break;
            case SEP: printf("  SEP  |"); break;
            case OCT: printf("  OCT  |"); break;
            case NOV: printf("  NOV  |"); break;
            case DEC: printf("  DEC  |"); break;
        }
    }
    printf("  Dependencies       |\n");
}

void printrow(struct Row allInfo[], int amountOfTasks) 
{
    system(CLEAR_SCREEN);
    puts("\n");
    printline();
    printheader();
    for (int i = 0; i < amountOfTasks; i++)
    {
        printline();
        printf("|  %-20s", allInfo[i].taskName);

        for (int j = 0; j < NUM_OF_MONTHS; j++) {
            printf("|");

            if (allInfo[i].months[j] == true) {
                printf("  XXX  ");
            } else {
                printf("       ");
            }
        }
        printf("|  ");
        for (int j = 1; j <= allInfo[i].dependencies[0]; j++)
        {
            printf("%d ", allInfo[i].dependencies[j]);
        }
        for (int j = 2*(allInfo[i].dependencies[0]); j < 19; j++)
        {
            printf(" ");
        }
        puts("|");
    }
    printline();
    puts("");
}


void taskFormat(int i, struct Row taskList[])
{
    puts("Enter a task name, fewer than 20 characters, with NO spaces (Example: Task_1): ");
    fflush(stdout);
    scanf("%s", taskList[i].taskName);
    puts("Enter start month and end month of the task (Example: 2, 5): ");
    fflush(stdout);
    scanf("%d, %d", &taskList[i].startMonth, &taskList[i].endMonth);
    fillMonths(taskList[i].startMonth, taskList[i].endMonth, taskList[i].months);
    puts("How many dependencies does this task have?");
    fflush(stdout);
    // first index is equal to length of array??
    scanf("%d", &taskList[i].dependencies[0]);
    for(int j = 1; j <= taskList[i].dependencies[0]; j++)
    {
        printf("Enter task dependency number %d:", j);
        fflush(stdout);
        scanf("%d", &taskList[i].dependencies[j]);
        if (taskList[i].dependencies[j] > 9)
        {
            puts("Too many dependencies, number of dependencies set to max (9)");
            fflush(stdout);
            taskList[i].dependencies[j] = 9;
        }
    }
}

// Checks if circular dependency occurs for a particular task
int checkCircDependency (struct Row allInfo[], int dependentTaskId, int currentRow, int visitedTasks[]){

    int length = allInfo[currentRow-1].dependencies[0];


    printf("( !!!!! Check if Circular !!!!! )\n");

    // base case: return if the task has no dependencies

    if (length == 0){

        printf("no circular dependency found....\n");
        return 0;
    }




    // return '1' if task has already been visited
    if (visitedTasks[dependentTaskId]){

        printf("( !!!!!! Circular dependency found !!!!!! )\n");
        //printf("task: %c\n", allInfo[currentRow]);
        //printf("dependent task: %d\n", dependentTaskId+1);
        visitedTasks[dependentTaskId] = 0;
        visitedTasks[currentRow] = 0;
        return 1;

    }





    // reset visited tasks at the end
    visitedTasks[dependentTaskId] = 0;
    printf(" No circular dependency found...\n");
    return 0; // no circular dependency found

}

void displayDependentTasks (struct Row allInfo[], int taskNum, int visitedTasks[]){


    // ISSUE: Lists of dependencies all have 10 values and starts at index one with index zero having random values
    // Solved: Realised length is at first index
    int length = allInfo[taskNum].dependencies[0];
    int circDependencyFound = 0;


    printf("%s -> ", allInfo[taskNum].taskName);


    visitedTasks[taskNum] = 1;






    for (int i = 0; i < length; i++){

        // index of dependent task
        int dependentTaskId = allInfo[taskNum].dependencies[i+1] - 1;

        // if task hasn't been visted display it
        if (visitedTasks[dependentTaskId] == 0){

            displayDependentTasks(allInfo, dependentTaskId, visitedTasks);

            }


        else {
            printf("( !!!! warning potential circular dependency !!!! ) \n");
            if(circDependencyFound = checkCircDependency(allInfo, dependentTaskId, taskNum, visitedTasks) == 1){
                return;
            }
            }


    }

}



int main() {
    //hardcode in an test one
    srand(2004);
    int userChoice;
    int testTask;
    int numOfTasks = 10;
    int visitedTasks[] = {0,0,0,0,0,0,0,0,0,0};
    struct Row taskList[numOfTasks];
    puts("Welcome to the Gantt Generator: \nType 1 to Create Your Own or 2 to View a Randomised Demo.");
    fflush(stdout);
    scanf("%d", &userChoice);

    // Custom Gantt Chart
    if (userChoice == 1)
    {
        puts("Enter number of tasks: ");
        fflush(stdout);
        scanf("%d", &numOfTasks);
        if (numOfTasks > 9)
        {
            puts("Too many tasks, number of tasks set to max (9)");
            fflush(stdout);
            numOfTasks = 9;
        }
        for (int i = 0; i < numOfTasks; i++)
        {
            taskFormat(i, taskList);
        }
    }
    // Demo Gantt Chart
    else
    {
        for (int i = 0; i < numOfTasks; i++) 
        {
            // ERROR: Not printing taskName correctly, only first task's taskName gets printed (SOLVED)

            snprintf(taskList[i].taskName, 20, "%d", i + 1);
            // TROUBLESHOOTING
            //printf("%d ", taskList[i].taskName);
            taskList[i].endMonth = (rand() % NUM_OF_MONTHS) + 1;
            taskList[i].startMonth = (rand() % taskList[i].endMonth) + 1;
            fillMonths(taskList[i].startMonth, taskList[i].endMonth, taskList[i].months);
        }
        taskList[0].dependencies[0] = 0;
            // taskList[0].dependencies[1] = 2;

        taskList[1].dependencies[0] = 1;
            taskList[1].dependencies[1] = 1;

        taskList[2].dependencies[0] = 2;
            taskList[2].dependencies[1] = 1;
            taskList[2].dependencies[2] = 2;

        taskList[3].dependencies[0] = 1;
            taskList[3].dependencies[1] = 3;


        taskList[4].dependencies[0] = 1;
        taskList[4].dependencies[1] = 9;


        taskList[5].dependencies[0] = 2;
            taskList[5].dependencies[1] = 4;
            taskList[5].dependencies[2] = 5;

        taskList[6].dependencies[0] = 1;
            taskList[6].dependencies[1] = 6;

        taskList[7].dependencies[0] = 1;
            taskList[7].dependencies[1] = 7;

        taskList[8].dependencies[0] = 1;
            taskList[8].dependencies[1] = 5;

        taskList[9].dependencies[0] = 2;
            taskList[9].dependencies[1] = 8;
            taskList[9].dependencies[2] = 9;



    }

    printrow(taskList, numOfTasks);
    userChoice = 0;
    testTask = 10;
    puts("\nType 1 if you wish to edit or Type 2 if you would like to run a test:");
    fflush(stdout);
    scanf("%d", &userChoice);
    while (userChoice == 1)
    {
        puts("Which task numerically would you like to edit?");
        fflush(stdout);
        scanf("%d", &userChoice);
        taskFormat(userChoice - 1, taskList);
        printrow(taskList, numOfTasks);
        puts("\nType 1 if you wish to edit or Type 2 if you would like to run a test:");
        fflush(stdout);
        scanf("%d", &userChoice);
    }

    if (userChoice == 2){
        // By default run test with last task#
        puts("Which task would you like to run on a test on? (give index of task, from 1 upwards):");
        fflush(stdout);
        scanf("%d", &testTask);
        displayDependentTasks(taskList, testTask-1, visitedTasks);

    }

    return 0;
}
/*
       .......                                                                                                 ...-..    ......
     ..-++**+=:.::::::::::::::::::::::::::::::::.:-------------------------------.::::::::::::::::::::::::::::::--+=-  ..-++=:.
   .:+*==+**+++*==*############################+.%@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@.=##%%%%%%%#########%%%%%%%%%%%***-=..:+*##:..
 .-+*++*+---=+*+++==#######**##**#*##*#**##*###+.%@@@@@%-..=%@@@@@@@@%+-=@@@@@@@@.=###%%%#++#*+++#*+#*+#%%%%%%%%*+**-.-=#**.
 .=#*+:+==--=+=++**+#######:.::.-+.##.+:.-*.###+.%@@@@@-.*=.+@@@@@@@@#*.-@@@@@@@@.=####%=.**#=-*--*.:.+#%%%%%%%%%%=*#++==-:.
..-=-. -==*++-+*-===#######:-+*=-+.++.+:-=..###+.%@@@@@:.#+.+@@+::+@@@@.-@@@@@@@@.=#####=.#*#=...+##.+###%%%%%%%#=-==*=:.-:.
.:==-. -:+#*=-+#-=-=*######+*##**#*++*#**#*+###+.%@@@@@%-..=%@@@@@@@@+.   %@@@@@@.=######*++#**%+*##+####%%%%%%%##===+=-=--.
 ..*#+.=:-*+=-==##-############################+.%@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@.=########%%%%%%%%%#####%%%%%%=:+%%%*==:.....:
 ..-*#+*++*++++*#=::::::::::::::::::::::::::::::.%@@@@@@@@%%%@@%@@@@@@@@@@@@@@@@@..::::::::::::::::::::::::::::::.::-:==-....:.
   ..-+*++++*+*=...                             .%@@@#-.+-**=#==*=-=-+-%=*#+*@@@@.                             -+++=::-=-:-++++..
      ..-====:...                               .+@@@%%*%*%%%@%*%%##%##@#%@*#@@@+.                             .+====++=+======.
                                                ..-%@@@@@@@@@@@@@@@@@@@@@@@@@@%-.                                  ...:::.. ...
                                                    ..--==================--:..
*/

/*

     __                    .___                    ________  ________
    |__|  ____ _______   __| _/_____     ____      \_____  \ \_____  \
    |  | /  _ \\_  __ \ / __ | \__  \   /    \      /  ____/   _(__  <
    |  |(  <_> )|  | \// /_/ |  / __ \_|   |  \    /       \  /       \
/\__|  | \____/ |__|   \____ | (____  /|___|  /    \_______ \/______  /
\______|                    \/      \/      \/             \/       \/


*/
