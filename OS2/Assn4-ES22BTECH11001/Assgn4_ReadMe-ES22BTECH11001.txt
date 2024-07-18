# Operating Systems, Programming Assignment 4

This Project is made by : Ahmik Virani.  
Roll Number : ES22BTECH11001.  


This assignment is about Implement solutions to Readers-Writers (writer preference) and Fair Readers-Writers problems using Semaphores

### Input File

The input file is named "inp-params.txt"
The contents of this are
  1. First we have the number of writer threads : nw
  2. Followed by the number of reader threads : nr (This may be separated by a space)
  3. Next, the number of times each writer thread tries to enter the CS : kw (This may be separated by a space)
  4. Then the number of times each reader thread tries to enter the CS : kr (This may be separated by a space)
  5. μCS : required to calculate the randCSTime (This may be separated by a space)
  6. μRem : required to calculate the randRemTime (This may be separated by a space)
  
  
### Before Running
  1. Please ensure to change the values in the input file as per requirements
  2. I have written thread ID's as numbers as given by the question, but if you want actual ID's then you can uncomment the line 158, 224 in rw-ES22BTECH11001.cpp file and comment the line 156, 222
  3. Similar to 2, you can uncomment the line 147, 211 and comment the line 145, 209 in the file frw-ES22BTECH11001.cpp

  
## Deployment

There are a total of 2 source files in this problem

1. Writers preference solution

To deploy the chunk method project on a Linux terminal use

```bash
g++ rw-ES22BTECH11001.cpp
```

Followed by 
```bash
./a.out
```

2. Fair Readers Writers solution

To deploy the Mixed method project on a Linux terminal use

```bash
g++ frw-ES22BTECH11001.cpp
```

Followed by

```bash
./a.out
```


### Output file

We have 3 output files
  1. "RW-log.txt" which outputs the results of the writers preference solution
  2. "FairRW-log.txt" which outputs the results of the fair readers writers solution
  3. "Average_time.txt", which outputs the average time a thread takes to gain entry to the Critical Section

Please note that the file "Average_time.txt" is common to both the programs and will show results corresponding to which program to run
The file "Average_time.txt" contains 3 values : 
  1. Average waiting time for all threads
  2. Average waiting time for writers threads
  3. Average waiting time for readers threads

Also, the worst case times are printed in the terminal itself as the question did not specify any requirements for this



