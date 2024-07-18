# Operating Systems Assignment 2

This Project is made by : Ahmik Virani.
Roll Number : ES22BTECH11001.

A brief description of what this project does:

## Instructions

-   Before running the program please ensure that the name of the input
    file is InFile.txt
-   Also ensure that the first value is the value of N and the second
    value is the value of M, and that they are separated by a space

## Deployment

To host this project on an ubuntu

``` bash
  gcc Assgn2Src-ES22BTECH11001.c -o Assgn2Src-ES22BTECH11001 -lm
```

followed by

``` bash
  ./Assgn2Src-ES22BTECH11001
```

## Features

#### Taking Input

-   Takes two values of input from input file named InFile.txt
-   The two inputs are the values of N followed by value of M.
-   The value of N is the number of numbers (i.e. the first N numbers) we are checking for (if it is vampire or
    not).
-   The value of M is the number of threads.
-   In the input file, the values are separated by a space.

#### Output File

-   The output file is named OutFile.txt
-   It prints the values which are vampire numbers
-   It also shows which vampire number was found by which thread.
-   At the end it shows the total number of vampire numbers
    corresponding to the value of N.
