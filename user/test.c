/**
 * File: test.c
 * Author: Nino Estrada
 * Date: September 23, 2024
 *
 * Description:
 * The "test" command evaluates a conditional expression. It is an if statement
 * that will return zero (true) or 1 (false), or greater than 1 (error).
 * The "test" command will check if any flags were inputted and, based on the expression, 
 * will return a truthy or falsy value.
 *
 * Usage: 
 * Compile: make qemu
 * Run: test [conditional expression]
 * 
 * Examples:
 * // Example 1: File Analysis
 * test -f cat
 * // Expected Output: 0 
 *
 * // Example 2: String Operators
 *  test "hello" = "hell"
 * // Expected Output: 1
 *
 * // Example 3: Number Operators
 * test 23 -gt 32 
 * // Expected Output: 1 
 */

 
#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/fcntl.h"
#include <stdbool.h>
#include <stddef.h>


const char *usageString = "Command			Description\n"
"-d file			True if file exists and is a directory.\n"
"-e file			True if file exists (regardless of type).\n"
"-f file			True if file exists and is a regular file.\n"
"file1 -ef file2		True if file1 and file2 exist and refer to the same file.\n"
"s1 = s2			True if the strings s1 and s2 are identical.\n"
"s1 != s2		True if the strings s1 and s2 are not identical.\n"
"n1 -gt n2		True if the integer n1 is algebraically greater than the integer n2.\n"
"n1 -lt n2		True if the integer n1 is algebraically less than the integer n2.\n";


/**
 * Checks if the string can be converted to a number
 *
 * @param str The string to check if it can be converted to a number
 * @returns a boolean value representing whether a string is an integer or not
 */
bool isInt(char *str)
{
	
	for (int i = 0; i < strlen(str); i += 1)
	{
		if (str[i] < '0' || str[i] > '9')
		{
			return false;
		}
	}

	return true;
}


/**
 * Opens a file and retrieves the metadata for the file
 *
 * @param file The file to be open
 * @param fileInfo The struct to be filled in with the metadata or null if not required
 * @return The file descriptor
 */
int openFile(char *file, struct stat *fileInfo)
{
	int fd = open(file, O_RDONLY);

	// Non Existing File
	if (fd < 0)
	{
		printf("Couldn't open file: %s\n", file);
	}
	// File Exist
	else if (fileInfo != NULL)
	{
		// Populate the File Info in the struct
		fstat(fd, fileInfo);
	}

	close(fd);
	return fd;
}


/**
 * Implements the file flags conditions
 *
 * @param flag The file flag
 * @param file The file argument for the unary operator
 * @return int 0 for true, 1 for false and 2 for error
 */
int fileFlags(char *flag, char *file)
{
	char *flags[] = {"-d", "-e", "-f"};

	int i;

	// Check if Passed in Flag is in the Flags Array
	for (i = 0; i < 3; i += 1)
	{
		if (strcmp(flag, flags[i]) == 0)
		{
			break;
		}
	}

	// Error Handling: Checking if User put in a Valid File Flag
	if (i == 3)
	{
		printf("Unknown file flag: %s \n\n%s \n", flag, usageString);
		return 2;
	} 

	// Open File and Store its Metadata in a struct
	struct stat fileInfo;
	int fd = openFile(file, &fileInfo);

	// File Doesn't Exist
	if (fd < 0)
	{
		return 1;
	}

	// is directory
	if (strcmp(flag, "-d") == 0)
	{
		if (fileInfo.type == 1)
		{
			printf("Type: %d\n", fileInfo.type);
			return 0;
		}
	}
	// file exists
	else if (strcmp(flag, "-e") == 0)
	{
		return 0;
	}
	// file exist and is a regular file type
	else if (strcmp(flag, "-f") == 0)
	{
		if (fileInfo.type == 2)
		{
			return 0;
		}
	}
	else 
	{
		printf("Unknown file flag: %s \n\n%s \n", flag, usageString);
		return 2;
	}

	return 1;
}


/**
 * Implements the string expressions
 *
 * @param string1 The first string to compare
 * @param string2 The second string to compare
 * @param operator The operator which can be either "=" or "!="
 * @return int 0 for true, 1 for false and 2 for error
 */
int stringOperators(char *string1, char *string2, char *operator)
{
	if (strcmp(operator, "=") == 0)
	{
		if (strcmp(string1, string2) == 0)
		{
			return 0;
		}
	} 
	else if (strcmp(operator, "!=") == 0)
	{
		if (strcmp(string1, string2) != 0)
		{
			return 0;
		}
	}
	else 
	{
		printf("Unknown string comparison: %s \n\n%s \n", operator, usageString);
		return 2;
	}

	return 1;	
}


/**
 * Implements the number expressions
 *
 * @param num1 The string representing the first number to compare
 * @param num2 The string representing the second number to compare
 * @param flag The comparison flag which can be either "-lt" (<) or "-gt" (>)
 * @return int 0 for true, 1 for false and 2 for error
 */
int numberOperators(char *num1, char *num2, char *flag)
{
	// Error Handling: Checking if the First String can be Converted to a Number
	if (!isInt(num1))
 	{
 		printf("Not a number: %s \n\n%s \n", num1, usageString);
 		return 2;
 	} 

 	// Error Handling: Checking if the Second String can be Converted to a Number
 	if (!isInt(num2))
 	{
 		printf("Not a number: %s \n\n%s \n", num2, usageString);
 		return 2;
 	} 

	// Convert the Strings to Numbers
 	int firstNum = atoi(num1);
 	int secondNum = atoi(num2);

 	if (strcmp(flag, "-lt") == 0)
 	{
 		return firstNum < secondNum ? 0 : 1;
 	} 
 	else if (strcmp(flag, "-gt") == 0)
 	{
 		return firstNum > secondNum ? 0 : 1;
 	}
 	else
 	{
 		printf("Unknown number flag: %s \n\n%s \n", flag, usageString);
 		return 2;
 	}
}

/**
 * Parses the arguments because FogOS doesn't respect quotes when creating the argv array
 * 
 * For example, when pass in "hi hello", it is treated as two separate arguments.
 * So, we have to build the string one word at a time.
 *
 * @param argc The number of command line arguments
 * @param argv The array of command line argumetns
 * @return int The number of parse arguments (the new argc) or -1 for error
 */
int parseArgs(int argc, char **argv, char** newArgs)
{
	char buf[128]; // String Builder
	char *ptrBuf; // Should Always Point to the Current End of the String being Built ('buf')
	int newIndex = 0; // Keeping Track of where we are in the Arguments

	// Loop through the Arguments
	for (int i = 0; i < argc; i += 1)
	{
		//  Argument is a String
		if (argv[i][0] == '"')
		{
			ptrBuf = buf;

			int len = strlen(argv[i]);

			// Keep Adding Words to the String Builder
			while (argv[i][len - 1] != '"' && i < argc)
			{
				// Copy Current Word into the Buffer 
				strcpy(ptrBuf, argv[i]);

				// Point 'ptrBuf' at the End of the Buffer
				ptrBuf += len;
				*ptrBuf = ' '; // Add Space

				// Since we Added a Space, Need to Increment Pointer to be At the End of Buf
				ptrBuf += 1;
			
				// Reset "len" for the Next Word
				i += 1;
				len = strlen(argv[i]);
			}

			// Error Handling: Checking if User is Missing the Closing Quote
			if (i == argc)
			{
				printf("Invalid syntax: missing closing quote\n\n%s \n", usageString);
				return -1;
			}

			// Copy Final Word into Buf
			strcpy(ptrBuf, argv[i]);

			// Setting up NewArgs to be Able to Store Buf with the Null Terminator Character
			newArgs[newIndex] = (char *)malloc(strlen(buf) + 1);

			// Copying Buf into the New Arguments
			strcpy(newArgs[newIndex], buf);
			newIndex += 1;
		}
		// Any Arguments without Quotes
		else
		{
			newArgs[newIndex] = argv[i];
			newIndex += 1;
		}
	}

	return newIndex;
}


/**
 * The entry point of the program
 *
 * @param argc The number of command line arguments
 * @param argv The array of command line arguments
 * @return 0 on success or 1 on error
 */
int main(int argc, char **argv)
{
	// Corrected Arguments
	char **newArgs = (char **)malloc(argc *sizeof(char *));
	int newArgc = 0;

	// Return Value
	int result = 1;
	
	// Error Handling: Checking if Any Arguments were Provided
	if (argc < 2)
	{	
		printf("No arguments provided. \n\n%s \n", usageString);
		result = 2;
	}
	// Help Flag
	else if (strcmp(argv[1], "-h") == 0)
	{
		printf("Usage: \n\n%s \n", usageString);
		exit(0);
	}
	// Error Handling: Checking if Enough Arguments were Provided
	else if (argc < 3)
	{	
		printf("Not enough arguments provided. \n\n%s \n", usageString);
		result = 2;
	}
	// Error Handling: Missing Closing Quote
	else if ((newArgc = parseArgs(argc, argv, newArgs)) < 0)
	{
		result = 2;
	}
	// File Flags
	else if (newArgs[1][0] == '-')
	{
		if (newArgc < 3)
	 	{
	 		printf("File expression requires 2 arguments. \n\n%s \n", usageString);
	 		result = 2;
	 	} 
	 	else
	 	 {
	 		result = fileFlags(newArgs[1], newArgs[2]);
	 	}
	}
	// String Operators
	else if (strcmp(newArgs[2], "=") == 0 || strcmp(newArgs[2], "!=") == 0)
	{
		if (newArgc < 4)
	 	{
	 		printf("String comparisons requires 3 arguments. \n\n%s \n", usageString);
	 		result = 2;
	 	}
	 	else
	 	{
			result = stringOperators(newArgs[1],  newArgs[3],  newArgs[2]);
		}
	}
	// File Comparisons
	else if (strcmp(newArgs[2], "-ef") == 0) 
	{
		if (newArgc < 4)
	 	{
	 		printf("File comparisons requires 3 arguments. \n\n%s \n", usageString);
	 		result = 2;
	 	}
	 	else
	 	{
	 		// Open Files and Store their Metadata in their Respective structs
			struct stat fileInfo;
			struct stat fileInfo2;
			int fd = openFile(newArgs[1], &fileInfo);
			int fd2 = openFile(newArgs[3], &fileInfo2);

			if (fd < 0 || fd2 < 0)
			{
				result = 2;
			} 
			else if (fileInfo.ino == fileInfo2.ino)
			{
				result = 0;
			}
		}
	}
	// Number Operators
	 else if (strcmp(newArgs[2], "-gt") == 0 || strcmp(newArgs[2], "-lt") == 0)
	 {
	 	if (newArgc < 4)
	 	{
	 		printf("Number comparisons requires 3 arguments. \n\n%s \n", usageString);
	 		result = 2;
	 	} 
	 	else
	 	{
	 		result = numberOperators(newArgs[1], newArgs[3], newArgs[2]);
	 	}
	 }
	 else
	 {
	 	printf("Unrecognized operation \n\n%s \n", usageString);
	 	result = 2;
	 }

	printf("Result: %d\n", result);
	
	exit(0);
}
