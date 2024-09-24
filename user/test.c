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


char * usageString = "Command			Description\n"
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

	return fd;
}


/**
 * Implements the file flags conditions
 *
 * @param flag The file flag
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
 * The entry point of the program
 *
 * @param argc The number of command line arguments
 * @param argv The array of command line arguments
 * @return 0 on success or 1 on error
 */
int main(int argc, char** argv)
{
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
	// File Flags
	else if (argv[1][0] == '-')
	{
		if (argc < 3)
	 	{
	 		printf("File expression requires 2 arguments. \n\n%s \n", usageString);
	 		result = 2;
	 	} 
	 	else
	 	 {
	 		result = fileFlags(argv[1], argv[2]);
	 	}
	}
	// String Operators
	else if (argv[1][0] == '"')
	{
		if (argc < 4)
	 	{
	 		printf("String comparisons requires 3 arguments. \n\n%s \n", usageString);
	 		result = 2;
	 	}
	 	else
	 	{
			result = stringOperators( argv[1],  argv[3],  argv[2]);
		}
	}
	// File Comparisons
	else if (strcmp(argv[2], "-ef") == 0) 
	{
		if (argc < 4)
	 	{
	 		printf("File comparisons requires 3 arguments. \n\n%s \n", usageString);
	 		result = 2;
	 	}
	 	else
	 	{
	 		// Open Files and Store their Metadata in their Respected struct
			struct stat fileInfo;
			struct stat fileInfo2;
			int fd = openFile(argv[1], &fileInfo);
			int fd2 = openFile(argv[3], &fileInfo2);

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
	 else
	 {
	 	if (argc < 4)
	 	{
	 		printf("Number comparisons requires 3 arguments. \n\n%s \n", usageString);
	 		result = 2;
	 	} 
	 	else
	 	{
	 		result = numberOperators(argv[1], argv[3], argv[2]);
	 	}
	 }

	printf("Result: %d\n", result);
	
	exit(0);
}
