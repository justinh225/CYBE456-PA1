# CYBE-465 Programming Assignment #1
## Environment Specification and Compliation:
This program was written in a Ubuntu 24.04.1 LTS virtual machine ran using WSL 2 for Windows. The program files were compiled using GCC 13.3.0.

The program was compiled and ran by navigation the the source directory at and running the command ```gcc access.c app.c commands.c acl_actions.c -o access``` in order to compile and produce the output file ```access```.

The program can be ran using the command ```./access <path_to_test_input>```, where the path to the test input file is provided as a command line argument.



## Directory Structure and Included Files:
### src:
Contains all source and header files. The files included are as follows:
- ```access.c```, ```./headers/access.h```
  - Main program entrypoint
- ```app.c```, ```./headers/app.h```
  - Contains implementations and definitions for general commands for the application
- ```commands.c```, ```./headers/commands.h```
  - Includes implementations and definitions of each command
- ```acl_actions.c```, ```./headers/acl_actions.h```
  - Contains implementations and definitions for access control
- ```./headers/structures.h```
  - Contains definitions for data structures

### test:
Contains all test files used during development and testing. Input commands are contained in ```input.txt``` and example output files from previous executions are contained in the ```./<test_case>/output``` directory.
- ```./test_case_1```, ```./test_case_2```
  - Default test cases provided by the instructor
- ```./test_case_3```, ```./test_case_4```
  - Custom test cases written during development
   


