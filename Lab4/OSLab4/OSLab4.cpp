#include <iostream>
#include <stdlib.h>
#include <time.h>
#include <string>
#include <iomanip>
#include <iostream>
using namespace std;

class Planner {

private: int taskNumber;
         int  processorNumber;
         int** arr;
         bool* processedTask;
         void generateArr();
         bool isOneResourseToOneTask(int& columnIndex, int& rowIndex);
         void resetColumn(int columnIndex, int rowIndex);
         void resetRow(int columnIndex, int rowIndex);
         int findWidestTier();
         void printResult();
         bool isFindPairing();
         void findPairing();
         void printReport();

public:
    Planner(int tasks, int processors) {
       
        taskNumber = tasks;
        processorNumber = processors;

        arr = new int* [taskNumber];
        for (int i = 0; i < taskNumber; i++)
        {
            arr[i] = new int[processorNumber];
        }

        processedTask = new bool[taskNumber];
        for (int i = 0; i < taskNumber; i++) {
            processedTask[i] = false;
        }

        generateArr();
        findPairing();
        printReport();
    }
};



int main()
{
    srand(time(NULL));
    const int tasks= 30;
    const int processors = 30;

    Planner planner = Planner(tasks, processors);

}


void Planner::generateArr()
{
    for (int i = 0; i < taskNumber; i++)
    {
        for (int j = 0; j < processorNumber; j++)
        {
            arr[i][j] = rand() % 2;
        }
    }
    printResult();
}


bool Planner::isOneResourseToOneTask(int& columnIndex, int& rowIndex)
{
    for (int i = 0; i < taskNumber; i++)
    {
        if (processedTask[i])
            continue;
        int rowSum = 0;
        for (int j = 0; j < processorNumber; j++)
        {
            rowSum += arr[i][j];
            if (arr[i][j] == 1) {          
                columnIndex = j;
                rowIndex = i;
            }
        }
        if (rowSum == 1) {
            processedTask[i] = true;
            return true;
        }
        else {
            columnIndex = -1;
            rowIndex = -1;
        }
    }
    return false;
}


void Planner::resetColumn(int columnIndex, int rowIndex)
{
    for (int i = 0; i < taskNumber; i++)
    {
        if(i != rowIndex)
            arr[i][columnIndex] = 0;
    }

  
}


void Planner::resetRow(int columnIndex, int rowIndex)
{
    for (int i = 0; i < processorNumber; i++)
    {
        if (i != columnIndex)
            arr[rowIndex][i] = 0;
    }
}

int Planner::findWidestTier()
{
    int maxRowSum = 0;
    int result = -1;
    for (int i = 0; i < taskNumber; i++)
    {
        int rowSum = 0;
        for (int j = 0; j < processorNumber; j++)
        {
            rowSum += arr[i][j];
        }
        if (rowSum > maxRowSum) {
            maxRowSum = rowSum;
            result = i;
        }
    }
    return result;
}


void Planner::printResult()
{
    for (int i = 0; i < taskNumber; i++)
    {
        for (int j = 0; j < processorNumber; j++)
        {
            cout << arr[i][j] << " ";
        }
        cout << endl;
    }
    cout << endl;
}


bool Planner::isFindPairing()
{
    for (int i = 0; i < taskNumber; i++)
    {
        int rowSum = 0;
        int columnSum = 0;
        for (int j = 0; j < processorNumber; j++)
        {
            rowSum += arr[i][j];
            columnSum += arr[j][i];
        }
        if (rowSum > 1 || columnSum > 1)
            return false;
    }

    return true;
}


void Planner::findPairing()
{
    while (!isFindPairing()) {

        cout << "new" << endl;

        while (true) {
            
            int columnIndex;
            int rowIndex;
 
            if (isOneResourseToOneTask(columnIndex, rowIndex)) {
                resetColumn(columnIndex, rowIndex);
                printResult();
            }
            else
                break;
        }

        int rowIndex = findWidestTier();


        for (int j = 0; j < processorNumber; j++)
        {
            if (arr[rowIndex][j] == 1) {
                resetColumn(j, rowIndex);
                resetRow(j, rowIndex);
            }
            processedTask[rowIndex] = true;
        }

        printResult();
    }
}

void Planner::printReport()
{
    cout << setw(10) << "Task" << setw(5);
    cout << "|";
    cout << setw(10) << "Processor" << endl;

    for (int i = 0; i < taskNumber; i++)
    {
        int processor = -1;
        for (int j = 0; j < processorNumber; j++)
        {
            if(arr[i][j] ==1)
            {
                processor = j;
         
            }
        }
        cout << setw(9) << i << setw(6);
        cout << "|";
        if (processor == -1)
            cout << setw(7) << "-" << endl;
        else
            cout << setw(7) << processor << endl;
    }
}




