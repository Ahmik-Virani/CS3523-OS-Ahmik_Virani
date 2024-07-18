
# Operating Systems, Programming Assignment 2

This Project is made by : Ahmik Virani.  
Roll Number : ES22BTECH11001.  


This assignment is about matrix multiplication using thread affinity

### Input File

The input file is named "inp.txt"
The contents of this are
  1. First we have the size of the matrix : N
  2. Followed by the number of threads : K (This may be seperated by a space or a new line)
  3. Next, the number of cores : C (This may be seperated by a space or a new line)
  4. After that, we have the number of bounded threads : BT (This may be seperated by a space or a new line)
  3. Then the input matrix of size N*N : A
  
  
### Before Running
  1. If you want to run the code as in assignment 1 (b = 0), i.e. no bounded threads
      a. In the Mixed Src file, uncomment line 73 and line 227, and comment line 72 and 226
      b. In the Chunk Src file, uncomment line 72 and line 226, and comment line 73 and line 227
  2. Do the opposite if you want to run the program otherwise
  3. Please do keep in mind that all the variables, (N, K, BT) are taken as powers of 2 in this assignment, more on this has been mentioned in the report
  
## Deployment

I have solved this problem in two ways

1. Chunk method

To deploy the chunk method project on a Linux terminal use

```bash
g++ Assgn2_Chunk_Src-ES22BTECH11001.cpp -o Assgn2_Chunk_Src-ES22BTECH11001
```
Followed by 
```bash
./Assgn2_Chunk_Src-ES22BTECH11001
```

2. Mixed method

To deploy the Mixed method project on a Linux terminal use

```bash
g++ Assgn2_Mixed_Src-ES22BTECH11001.cpp -o Assgn2_Mixed_Src-ES22BTECH11001
```

Followed by

```bash
./Assgn2_Mixed_Src-ES22BTECH11001
```

### Output file

All the codes will output the answer matrix into the same file names "out.txt".
The output matrix also contains the time taken for the matrix multiplication process

### Output in terminal

I have even displayed the average time taken for bounded and normal threads (as specified in experiment 2) in the terminal itself.

### Final Note

For experiment 1, you can just run the code manipulating the input.txt file and the output will be written in the out.txt file. (Make sure to change values of b as per the condition)

For experiment 2, you have to run the code twice, first keep value of b = K/C in the code, this will give time for bounded and normal threads, next run the code keeping b = 0 as mentioned above, this will give time for completely unbounded case as in assignment 1.

Please also remember to not keep BT = 0, but keep b = 0 whenever you want to run the program corresponding to assignment 1.



