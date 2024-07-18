/*
Name : Ahmik Virani
Roll No. : ES22BTECH11001
*/

#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <sys/shm.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <sys/wait.h>
#include <stdbool.h>

/*
Listed below are the globa variables
*/
int N, K;               //N and K are as deined in the question
int *array;             //This is an array which will store all the values, if they are tetrahedral or not


/*
The below function checks if a number is tetrahedral or not
It returns true if tetrahedral 
It returns false if not tetrahedral
*/
bool isTetrahedralNumber(int x) {
    for(int i = 1; i<=x; i++)
    {
        if(x == ( i * (i + 1) * (i + 2) / 6) )
        {
            return true;
        }
    }

    return false;

}

/*
This is the main function
Here all the file IO is done
Also, we do all the processes here
*/
int main(int argc, char const *argv[])
{    
    FILE *fileIn = fopen("InFile.txt", "r");        //Open the input file named "InFile.txt" in read only mode

    if(fileIn == NULL)                              //Check if the file in NULL
    {
        perror("error opening file\n");             //If it is NULL, which means it does not exist, throwing an error message
        return 1;                                   //A value more than zero is returned to show that something went wrong in the execution
    }

    /*
    The following two lines scan the values of N and M from the input file
    The first value scanned is N
    The second value scanned is M
    */
    fscanf(fileIn, "%d", &N);                       //value N is scanned as an integer from the input file
    fscanf(fileIn, "%d", &K);                       //value M is scanned as an integer from the input file

    fclose(fileIn);                                 //Once all the input values are taken, we close the input file

    const char *name = "OS";                        //Define the name of the shared memory as OS                    
    const int SIZE = 4096;                          //Define size of shared memory

    int shm_fileDescriptor;                         //Shared memory file descriptor
    void *ptr;                                      //Pointer to shared memory object

    array = (int *)calloc(N, sizeof(int));          //Dynamically allocating memory to the global variable array

    shm_fileDescriptor = shm_open(name, O_CREAT | O_RDWR, 0666);    //Create the shared memory object
    ftruncate(shm_fileDescriptor, SIZE);                            //Configure the size of the shared memory object

    ptr = mmap(0, SIZE, PROT_WRITE, MAP_SHARED, shm_fileDescriptor, 0);     //Memory map to the shared memory object

    /*
    Below is the loop to execute K processes simultaneously
    */
    for(int i= 0; i<K; i++)
    {
        pid_t pid;                              
        pid = fork();                               //Fork a child process

        if(pid < 0)                                 //Error occured
        {
            fprintf(stderr, "Fork Failed");         //Print an error message
            return 1;
        }

        else if(pid == 0)                           //Child Process
        {

            int localBuffer[N/K + 1];               //Create a local buffer of size N/K + 1

            for(int j = 0; j< N/K + 1; j++)
            {
                localBuffer[j] = 0;                 //Initialize all the values of the buffer to 0
            }

            int z = 0;                              //This counts the number of tetrahedral numbers in the range this thread calculates

            /*
            The loop below checks each number and stores the tetradehral numbers in the local buffer
            */
            for(int j = i; j < N; j+=K)
            {
                if( isTetrahedralNumber( j + 1 ))       
                {
                    localBuffer[z] = j + 1;
                    array[j] = localBuffer[z];
                    z++;
                }
            }

            /*
            Below loop is for writing to the shared memory from the local buffer
            */
            for(int j = 0; j < z ; j++)
            {
                int length = sprintf((char *)ptr, "%d", localBuffer[j]);        
                ptr += length;
            }

            char output[50];
            sprintf(output, "OutFile%d.txt", i + 1);
            FILE *fileOut = fopen(output, "w");                 //Open the output file in write mode

            if(fileOut == NULL)                                 //If file does not exist throw an error message
            {
                perror("Error opening file\n");
                return 1;
            }

            /*
            The loop below is for printing to the output file
            */
            for(int j = i; j < N; j+=K)
            {
                if(array[j] != 0)
                {
                    fprintf(fileOut, "%d: a tetrahedral number\n", j+1);
                }

                else
                {
                    fprintf(fileOut, "%d: Not a tetrahedral number\n", j+1);
                }
            }

            fclose(fileOut);                                        //Closing the file
            exit(0);
        }
    }

    shm_unlink(name);                       //Remove the shared memory object


    return 0;
}