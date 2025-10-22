ims190003 Project 1.

3 files (logger.c, driver.c, and encrypt.c) are compiled using a makefile. 

"make" will compile the program, once it is compiled it can be run using ./driver followed by the name of your choosing for the log file.

encrypt.c was created first as a standalone program to make finding errors faster and easier, afterwards it was edited to work with other files. All inputs are converted to uppercase.
logger.c creates a log file and writes to it to track the actions taken during the programs run.
driver.c is the interface to access all commands. It accepts all user inputs and uses the other 2 programs to execute the commands.

Makefile compiles all 3 .c files then cleans the directory of .o files afterwards.
