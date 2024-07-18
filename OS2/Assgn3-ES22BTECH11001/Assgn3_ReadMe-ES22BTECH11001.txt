
# Operating Systems, Programming Assignment 3

This Project is made by : Ahmik Virani.  
Roll Number : ES22BTECH11001.  


This assignment is about Dynamic Matrix Squaring

### Input File

The input file is named "inp.txt"
The contents of this are
  1. First we have the size of the matrix : N
  2. Followed by the number of threads : K (This may be separated by a space)
  3. Next, the value of row increment : rowInc (This may be separated by a space)
  4. Then the input matrix of size N*N : A (This may be separated by a new line)
  
  
### Before Running
  1. Please ensure to change the values of N, K , rowInc, and the matrix A as per requirements

  
## Deployment

There are a total of 4 methods in this problem

1. Test-And-Set (TAS)

To deploy the chunk method project on a Linux terminal use

```bash
g++ Assgn3_TAS_Src-ES22BTECH11001.cpp
```
Followed by 
```bash
./a.out
```

2. Compare-And-Swap (CAS)

To deploy the Mixed method project on a Linux terminal use

```bash
g++ Assgn3_CAS_Src-ES22BTECH11001.cpp
```

Followed by

```bash
./a.out
```

3. Compare-And-Swap using Bounded Waiting (Bounded CAS)

To deploy the Mixed method project on a Linux terminal use

```bash
g++ Assgn3_Bounded_CAS_Src-ES22BTECH11001.cpp
```

Followed by

```bash
./a.out
```

4. Atomic Increment

To deploy the Mixed method project on a Linux terminal use

```bash
g++ Assgn3_Atomic_Increment_Src-ES22BTECH11001.cpp
```

Followed by

```bash
./a.out
```

### Output file

All the codes will output the answer matrix into the same file names "out.txt".
The output file contains : 
  1. The square matrix : C
  2. The time taken for execution of the square matrix

### Final Note


Experiments 1, 2 and 3 can be directly run using the provided codes
For experiment 4 (a) and (b) parts, I have used the code I have written in experiment 1, and the rest I have used the provided codes in this assignment


