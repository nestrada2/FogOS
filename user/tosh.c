/**
 * Tosh - The Operating Shell (v1.0)
 * 
 * 
 * File: tosh.c
 * Author: Nino Estrada
 * Date: October 16, 2024
 *
 * 
 * Description:
 * Tosh is a custom command-line shell with features such as command execution, built-in commands, I/O redirection, 
 * and background job management, providing an interactive user experience. Enhanced functionality with scripting mode 
 * (including shebang support), dynamic prompts displaying exit status, command history, and current working directory. 
 * Integrating custom path execution for seamless interaction with Unix-like environments. Tosh Implements advanced features 
 * such as executable script support and background job handling, optimizing user interaction with the operating system.
 * 
 *
 * Usage: 
 * Run: tosh [options] [script]
 *
 *
 *
 *
 *    TABLE OF CONTENTS
 *    -----------------
 *    1. IMPORTS                  : Line 47
 *    2. COLORS & TEXT STYLING    : Line 58
 *    3. DATA STRUCTURE           : Line 82
 *    4. GLOBAL VARIABLES         : Line 109
 *    5. FUNCTION DECLARATIONS    : Line 177
 *    5. HELPER FUNCTIONS         : Line 268
 *    6. MAIN FUNCTION HELPERS    : Line 443
 *    7. EXECUTION FUNCTIONS      : Line 514
 *    8. COMMAND FUNCTIONS        : Line 875
 *    9. EXECUTION HELPERS        : Line 1248
 *   10. OPTIONS                  : Line 1393
 *   11. SPECIAL FEATURES         : Line 1575
 *   12. MAIN FUNCTION            : Line 1933
 *   13. REFERENCES               : Line 2028
 *
 */


// -------------------------------------------------- IMPORTS --------------------------------------------------


#include "kernel/fcntl.h"
#include "kernel/types.h"
#include "user/user.h"
#include <stdbool.h>
#include <stddef.h>


// -------------------------------------------------- COLORS & TEXT STYLING --------------------------------------------------


/**
 * Colors
 */
#define CYAN "\033[38;2;174;242;241m"
#define WHITE "\033[38;2;255;245;240m"
#define RED "\033[38;5;217m"
#define YELLOW "\033[38;5;229m"
#define ORANGE "\033[38;2;255;179;102m"
#define BLUE "\033[38;2;48;116;143m"
#define BLUE_BACKGROUND "\033[48;2;48;116;143m"


/**
 * Text Styling
 */
 #define ITALIC "\033[3m"
 #define BOLD "\033[1m"
 #define UNDERLINE "\033[4m"
 #define RESET "\033[0m"


// -------------------------------------------------- DATA STRUCTURES --------------------------------------------------


/**
 * Structure to store information about a command within the history
 */
struct cmd_info
{
	int cmd_number; /* The number representing where the command is in relation to the history */
	char *cmd; /* The command itself */
	uint time; /* The time it took to execute the command */
};


/**
 * Represents a command in a pipeline.
 */
struct command 
{
  char **tokens; /* An array of strings that describe the command to run */
  bool stdout_pipe; /* Set to true if this command's output should be written to pipe */ 
  char *stdout_file; /*Set to a file name if this command's output should be written */
  char *stdin_file; /* set to a file name if this command's output should be written  */
  bool append_file; /* set to true if this command's output should be appended to a file */
};


// -------------------------------------------------- GLOBAL VARIABLES --------------------------------------------------


/**
 * Global Variables
 */
char cwd[128]; /* Current Working Directory */
char *arguments[128]; /* Tokens parse for the current command */
struct cmd_info history[100]; /* User's command history */
int history_end = 0; /* Kepp track of the starting point of the history array */
int command_number = 1; /* Counter of commands enter so far in this session of 'tosh' */
bool private_mode = false; /* Private mode option */
char *error_msg = WHITE "-tosh: " RED UNDERLINE "error" RESET RED ":" WHITE; /* The Beginning of every error message */


/**
 * Usage Strings
 */
const char *options = WHITE BOLD "		" BLUE_BACKGROUND "OPTIONS" RESET "\n\n"
	WHITE UNDERLINE "Option" RESET "				" WHITE UNDERLINE "Description\n" RESET
	CYAN BOLD "-" RESET WHITE "v" RESET WHITE ", " CYAN BOLD "--" RESET WHITE "version			Displays shell version\n" RESET
	CYAN BOLD "--" RESET WHITE "about				Displays an about section of the shelln\n" RESET
	CYAN "--" RESET WHITE "license			Shows the license\n" RESET
	CYAN BOLD "-" RESET WHITE "lc" RESET WHITE ", " CYAN BOLD "--" RESET WHITE "list" CYAN BOLD "-" RESET WHITE "commands		Shows a list of all the commands\n" RESET
	CYAN BOLD "-" RESET WHITE "p" RESET WHITE ", " CYAN BOLD "--" RESET WHITE "private			Private mode is turned on\n" RESET WHITE;


const char *built_in_cmds = WHITE BOLD "		" BLUE_BACKGROUND "BUILT-IN COMMANDS" RESET "\n\n" RESET
	WHITE UNDERLINE "Command" RESET "				" WHITE UNDERLINE "Description\n" RESET
	CYAN "cat" RESET WHITE " filename			Displays file content\n" RESET
	CYAN "echo"RESET WHITE " [" ORANGE "string" RESET WHITE "]			Displays text to the terminal\n" RESET
	CYAN "ls" RESET WHITE " [" ORANGE "/dir" RESET WHITE "]			List directory contents\n" RESET
	CYAN "cd" RESET WHITE " /dir 		        Change the current directory\n" RESET
	CYAN "mkdir" RESET WHITE " dir_name			Make directories\n" RESET
	CYAN "pwd" RESET WHITE " 		                Prints the working directory\n" RESET
	CYAN "wc" RESET WHITE " filename 		        Shows the number of lines, words, and bytes in a file\n" RESET
	CYAN "history" RESET WHITE " [" ORANGE "-t" RESET WHITE "]		        Display the history list of the last 100 commands with their command numbers\n" RESET
	CYAN "test" RESET WHITE " 				Evaluates a conditonal expression\n" RESET
	CYAN "uname" RESET WHITE " [" ORANGE "-a" RESET WHITE "]			Displays system information\n" RESET
	CYAN "joke" RESET WHITE " 				Tells a programming-related joke\n" RESET
	CYAN "fact" RESET WHITE " 				Shares a fun programming-related fact\n" RESET
	CYAN "quote" RESET WHITE " 				Provides a programming-related quote\n" RESET
	CYAN "menu"  WHITE " 				Display the main menu\n" RESET
	CYAN "ref" RESET WHITE " 				Provides a list of commands with their descriptions\n" RESET
	CYAN "reset" RESET WHITE " 				Resets the shell\n" RESET
	CYAN "exit" RESET WHITE " 		                Exit the shell\n";


const char *history_execution = WHITE BOLD "		" BLUE_BACKGROUND "HISTORY EXECUTION" RESET "\n\n" RESET
	WHITE UNDERLINE "Command" RESET "				" WHITE UNDERLINE "Description\n" RESET
	CYAN "!" WHITE "num 		                A history execution that will re-run command number num\n" RESET
	CYAN "!!" WHITE " 		                A history execution that re-runs the last command that was entered\n" RESET
	CYAN "!" WHITE "prefix 		        A history execution that re-runs the last command with that prefix\n" RESET;


const char *io_redirection = WHITE BOLD "		" BLUE_BACKGROUND "I/O REDIRECTION" RESET "\n\n" RESET
	WHITE UNDERLINE "Command" RESET "				" WHITE UNDERLINE "Description\n" RESET
	WHITE "command1 " CYAN "|" WHITE " command2 		Allows the output of one command serves as the input for another command\n" RESET
	WHITE "command " CYAN ">" WHITE " filename 		Redirects the output of a command to a file\n" RESET
	WHITE "filename " CYAN "<" WHITE " command 		Redirects the content of a file to be input for a command\n" RESET
	WHITE "command " CYAN ">>" WHITE " filename 		Append the output of a command to the end of a file\n" RESET WHITE;


const char *job_control = WHITE BOLD "		" BLUE_BACKGROUND "JOB CONTROL" RESET "\n\n" RESET
	WHITE UNDERLINE "Command" RESET "				" WHITE UNDERLINE "Description\n" RESET
	WHITE "command " CYAN "&" WHITE " 			Runs a command in the background\n" RESET WHITE;


// -------------------------------------------------- FUNCTION DECLARATIONS --------------------------------------------------


/**
 * String Helper Functions
 */
uint strspn(const char *str, const char *chars);
uint strcspn(const char *str, const char *chars);
int strncmp2(const char *p, const char *q, uint n);
bool isInt2(char *str);
void lowercase(char *str);


/**
 * Parsing Arguments Helper Functions
 */
char *next_token(char **str_ptr, const char *delim);
int tokenize(char *str, char **tokens);


/**
 * Main() Helper Functions
 */
void initialize_history();
void reset_history();
void shell_initial_state();
bool too_many_arguments_error(int num, char *cmd, char *usage);


/**
 * Execution Functions
 */
 int execvp(char *pathname, char **args);
 void execute_pipeline(struct command *cmd);
 int execute(char *cmd);


/**
 * Command Functions
 */
 bool is_built_in_cmd(char *cmd);
 int exit_cmd();
 int history_cmd();
 int pwd_cmd(struct cmd_info *cur_cmd);
 int uname_cmd(struct cmd_info *cur_cmd);
 int cd_cmd(struct cmd_info *cur_cmd);
 struct cmd_info *repeat_last_cmd();
 struct cmd_info *repeat_cmd_number();
 struct cmd_info *repeat_cmd_prefix();
 struct cmd_info *redo_cmd();
 int reset_cmd();
 

/**
 * Execution Helper Functions
 */
void reset_arguments();
void truncate_str_at_comment(char *cmd);
int handle_comments_and_empty_string(char *cmd);
void populate_history_struct(struct cmd_info *cur_cmd, char *cmd_copy);
void parse_and_configure_pipeline(struct command *cmds, int argc);


/**
 * Options
 */
int tosh_options(char *option);
void version_opt();
void about_opt();
void license_opt();
void list_cmds_opt();
void private_opt();
void standard_opt();

  
/**
 * Special Features
 */
void splash_screen();
int main_menu_cmd(struct cmd_info *cur_cmd);
int reference_cmd(struct cmd_info *cur_cmd);
int joke_cmd(struct cmd_info *cur_cmd);
int fact_cmd(struct cmd_info *cur_cmd);
int quote_cmd(struct cmd_info *cur_cmd);
void off_message();


// -------------------------------------------------- HELPER FUNCTIONS --------------------------------------------------


/**
 * Counts how many characters from the start of str are in chars
 *
 * @param str The string we want to validate
 * @param chars The string we want to match
 * @return the number representing how many prefix characters in string are in chars 
 */
uint strspn(const char *str, const char *chars)
{
  uint i, j;
  for (i = 0; str[i] != '\0'; i++) {
    for (j = 0; chars[j] != str[i]; j++) {
      if (chars[j] == '\0')
        return i;
    }
  }
  return i;
}


/**
 * Counts how many characters in str are NOT in chars 
 *
 * @param str The string we want to validate
 * @param chars The string we want to match
 * @return the number representing how many characters in string are NOT in chars
 */
uint strcspn(const char *str, const char *chars)
{
  const char *p, *sp;
  char c, sc;
  for (p = str;;) {
    c = *p++;
    sp = chars;
    do {
      if ((sc = *sp++) == c) {
        return (p - 1 - str);
      }
    } while (sc != 0);
  }
}


/**
 * Compares two strings based on a specified portion of the strings
 *
 * @param p The first string to compare
 * @param q The second string to compare
 * @return 0 (equal), > 0 (p > q), < 0 (p < q) 
 */
int strncmp2(const char *p, const char *q, uint n)
{
  while(n > 0 && *p && *p == *q)
    n--, p++, q++;
  if(n == 0)
    return 0;
  return (uchar)*p - (uchar)*q;
}


/**
 * Reads the terminal input up until the next instance of delim and whatever it gets it is considered one token
 *
 * @param str_ptr Pointing to the next token to be parsed
 * @param delim An array of possible separators between tokens
 * @return a pointer to the end of the token that was just parsed, so we can start there next time
 */
char *next_token(char **str_ptr, const char *delim)
{
  if (*str_ptr == NULL) {
    return NULL;
  }

  uint tok_start = strspn(*str_ptr, delim);
  uint tok_end = strcspn(*str_ptr + tok_start, delim);

  /* Zero length token. We must be finished. */
  if (tok_end  == 0) {
    *str_ptr = NULL;
    return NULL;
  }


  /* Take note of the start of the current token. We'll return it later. */
  char *current_ptr = *str_ptr + tok_start;

  /* Shift pointer forward (to the end of the current token) */
  *str_ptr += tok_start + tok_end;

  if (**str_ptr == '\0') {
    /* If the end of the current token is also the end of the string, we
         * must be at the last token. */
    *str_ptr = NULL;
  } else {
    /* Replace the matching delimiter with a NUL character to terminate the
         * token string. */
    **str_ptr = '\0';

    /* Shift forward one character over the newly-placed NUL so that
         * next_pointer now points at the first character of the next token. */
    (*str_ptr)++;
  }

  return current_ptr;
}


/**
 * Parses all tokens in the given string 
 *
 * @param str The string from which to parse tokens
 * @param tokens An array where tokens will be returned
 * @return the number of tokens parsed
 */
int tokenize(char *str, char **tokens)
{
	char *next_tok = str;
	char *curr_tok;
	int n_tokens = 0;
	
	// Tokenize
	while ((curr_tok = next_token(&next_tok, " \n")) != NULL) 
	{
		if (strcmp(curr_tok, "#") == 0)
		{
			break;
		}
		
		tokens[n_tokens++] = curr_tok;
	}

	return n_tokens;
}


/**
 * Checks if the string can be converted to a number
 *
 * @param str The string to check if it can be converted to a number
 * @returns a boolean value representing whether a string is an integer or not
 * 	 - `true` if the string is an integer
 * 	 - `-false` if a string is not an integer
 */
bool isInt2(char *str)
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
 * Lowercases the string based on its ASCII value
 *
 * e.g. To convert 'A' (65) to 'a' (97) we add 32.
 */
void lowercase(char *str)
{
	for (int i = 0; i < strlen(str); i += 1)
	{
		if (str[i] >= 'A' && str[i] <= 'Z') 
		{ 
			// Convert to Lowercase
			str[i] = str[i] + 32;
		}
	}
}


// -------------------------------------------------- MAIN FUNCTION HELPERS --------------------------------------------------


/**
 * Initializes all of the history's command numbers to be -1 to indicate they have not been used yet
 */
void initialize_history()
{
	for (int i = 0; i < 100; i += 1)
	{
		history[i].cmd_number = -1;
	}
}


/**
 * Sets the history to its initial state
 */
void reset_history()
{
	initialize_history();
	history_end = 0; /* Kepp track of the starting point of the history array */
	command_number = 1; /* Counter of commands enter so far in this session of 'tosh' */
}


/**
 * Restoring the shell to its initial state
 */
void shell_initial_state()
{
	private_mode = false;
	reset_history();
	reset_arguments();
	chdir("/");
	getcwd(cwd, 128);
}


/**
 * Evaluates if a command or option has been given more arguments than expected
 *
 * @param num The limit of arguments for the command or option
 * @param cmd The command or option 
 * @param usage The string representing the correct usage of the command or option
 * @returns a boolean value representing whether the number of arguments exceeds the command's or option's limit
 * 	 - `true` if there are too many arguments
 * 	 - `-false` if arguments are within the command's or option's constraints
 */
bool too_many_arguments_error(int num, char *cmd, char *usage)
{
	// Error Handling
	if (arguments[num] != NULL)
	{
		fprintf(2, "%s %s: too many arguments\n", error_msg, cmd);
		printf("\n");
		printf(WHITE "Usage: \n");
		printf("%s\n", usage);
		return true;
	}

	return false;
}


// -------------------------------------------------- EXECUTION FUNCTIONS --------------------------------------------------


/**
 * Determines where the binary files are located and then runs exec
 *
 * @param pathname The executable or binary file to run
 * @param args The list of arguments to that command
 * @return an exit status
 * 	 - `0` on success
 * 	 - `-1` on error
 */
int execvp(char *pathname, char **args)
{
	char *fullpath;
	
	// Option 1: Checking if it is an Absolute or Relative Path
	if (pathname[0] == '/' || (pathname[0] == '.' && pathname[1] == '/') || (pathname[0] == '.' && pathname[1] == '.' && pathname[2] == '/'))
	{
		fullpath = pathname;
	} 
	else
	{
		// Option 2: Search the Root Directory
		fullpath = (char*)malloc(strlen(pathname) + 2); // extra space for slash and null terminator character
		fullpath[0] = '/';
		strcpy(&fullpath[1], pathname);
		
		int fd = open(fullpath, O_RDONLY);
		
		// Non Existing File in the Root Directory
		if (fd < 0)
		{
			// Option 3: Search in the Current Directory
			strcpy(fullpath, pathname);
			fd = open(fullpath, O_RDONLY);

			// Non Existing File in the Current Directory
			if (fd < 0)
			{
				fprintf(2, "%s Command not found: " ITALIC "%s\n" RESET WHITE, error_msg, pathname);
				printf("\n");
				printf("Usage:\n");
				printf("%s\n", built_in_cmds);
				return -1;
			}
		}

		close(fd);
	}
	
	return exec(fullpath, args);
}


/**
 * Execute a list of commands where the output of the previous becomes the input of the next
 *
 * @param cmd An array of structs that represents a command in a pipeline
 */
void
execute_pipeline(struct command *cmd)
{
	int i = 0;

	// Creates a pipe.
	int fd[2];
	
	// Keep Looping through Commands until Don't get a stdout (Implying on the Last Command)
	while (cmd[i].stdout_pipe)
	{
		// Error Handling
		if (pipe(fd) == -1) 
		{
		  fprintf(2, "%s Could not create pipe\n" RESET, error_msg);
		  return;
		}

		int pid = fork();

		// Error Handling
		if (pid == -1) 
		{
		  fprintf(2, "%s Fork failed \n" RESET, error_msg);
		  return;
		} 
		else if (pid == 0) 
		{
		  // Child
		  close(fd[0]); // Close Read End of the Pipe
		  close(1); // STDOUT
		  dup(fd[1]); // STDOUT now Comes from the Pipe

		  // Execute Current Command
		  execvp(cmd[i].tokens[0], cmd[i].tokens);

		  // CLose the Pipe
		  close(fd[1]);
		} 
		else 
		{
		  // Parent
		  
		  close(fd[1]); // Close the Write end of the Pipe (Next Command)

		  // Next Command will Read from the Pipe
		  close(0); // STDIN
		  dup(fd[0]); // STDIN now goes to the Pipe
		}

		i += 1;
	}

	int out_fd = -1, in_fd = -1;

	// Checking if there is an Output File to Redirect
	if (cmd[i].stdin_file != NULL)
	{
		int open_flags = O_RDONLY;
		
		in_fd = open(cmd[i].stdin_file, open_flags);

		if (in_fd == -1) 
		{
	    	fprintf(2, "%s Could not open file: " ITALIC "%s\n" RESET, error_msg, cmd[i].stdin_file);
	    	return;
	    }

		close(0); // close stdin

		if (dup(in_fd) == -1) 
		{
	    	fprintf(2, "%s dup failed\n" RESET, error_msg);
	    	return;
	  	}
	 }

	// Checking if there is an Output File to Redirect
	if (cmd[i].stdout_file != NULL)
	{
		int open_flags = O_RDWR | O_CREATE;

		if (cmd[i].append_file)
		{
			open_flags = open_flags | O_APPEND;
		}
		else
		{
			open_flags = open_flags | O_TRUNC;
		}
		
		out_fd = open(cmd[i].stdout_file, open_flags);

		if (out_fd == -1) 
		{
	    	fprintf(2, "%s Could not open file\n" RESET, error_msg);
	    	return;
	 	 }

		close(1); // close stdout

		if (dup(out_fd) == -1) 
		{
	    	fprintf(2, "%s dup failed\n" RESET, error_msg);
	    	return;
	  	}
	 }
  	
	// 1 More Command Left: Doesn't Write to a Pipe
	execvp(cmd[i].tokens[0], cmd[i].tokens);

	if (out_fd >= 0)
	{
		// Close the Output File
		close(out_fd);
	}

	if (in_fd >= 0)
	{
		close(in_fd);
	}

	// Close the Pipe
	close(fd[0]);
}


/**
 * Executes the given commands
 *
 * @param cmd The given command
 * @return an exit status
 * 	 - `0` for success
 * 	 - `1` for error
 * 	 - `-1` for exit
 */
int execute(char *cmd)
{	
	int exit_status = 0;

	// Strings prefixed with a comment ('#') or empty strings will be ignored by the shell
	if (handle_comments_and_empty_string(cmd) == 0) 
	{
		 return 0;
  	}

	// Resets the Arguments List to Avoid Reusing Old Commands
  	reset_arguments();

	// Ignore Inline Comments
  	truncate_str_at_comment(cmd);

  	// Make a Copy of the Command
  	char *cmd_copy = (char*)malloc(strlen(cmd) + 1);
  	strcpy(cmd_copy, cmd);

  	// Tokenize
  	int argc = tokenize(cmd, arguments);
  	lowercase(arguments[0]);

	// Buil-In Commands
	if (strcmp(arguments[0], "exit") == 0)
	{
		return exit_cmd();
	}
	else if (strcmp(arguments[0], "history") == 0)
	{
		return history_cmd();
	 }
	 else if (arguments[0][0] == '!')
	 {
	 	// History Execution
	 	struct cmd_info *repeat_cmd = redo_cmd();

		// Error Handling
	 	if (repeat_cmd == NULL)
	 	{
	 		return 1;
	 	}
	 
		// Copy of the Command
	 	char *cmd = (char*)malloc(strlen(repeat_cmd->cmd) + 1);
	 		 		
		// Copy Command 
		strcpy(cmd, repeat_cmd->cmd);
		execute(cmd);
	 }
	 else
	 {
		// Current Command
	 	struct cmd_info *cur_cmd = &history[history_end];

	 	// Popuate History Struct
	 	populate_history_struct(cur_cmd, cmd_copy);
	 	
	 	// Execute Command
	 	if (strcmp(arguments[0], "cd") == 0)
	 	{
	 		return cd_cmd(cur_cmd);
		 }
		 else if (strcmp(arguments[0], "pwd") == 0)
		 {
		 	return pwd_cmd(cur_cmd);
		 }
		 else if (strcmp(arguments[0], "uname") == 0)
		 {
		 	return uname_cmd(cur_cmd);
		 }
		 else if (strcmp(arguments[0], "ref") == 0)
		 {
		 	return reference_cmd(cur_cmd);
		 }
		 else if (strcmp(arguments[0], "joke") == 0)
		 {
		 	return joke_cmd(cur_cmd);
		 }
		 else if (strcmp(arguments[0], "fact") == 0)
		 {
		 	return fact_cmd(cur_cmd);
		 }
		 else if (strcmp(arguments[0], "quote") == 0)
		 {
		 	return quote_cmd(cur_cmd);
		 }
		 else if (strcmp(arguments[0], "menu") == 0)
		 {
		 	return main_menu_cmd(cur_cmd);
		 }
		 else if (strcmp(arguments[0], "reset") == 0)
		 {
		 	return reset_cmd();
		 }
		 else
		 {
		 	struct command cmds[16] = { 0 };
		  	
		  	cmds[0].tokens = arguments;
		  	bool background = strcmp(arguments[argc - 1], "&") == 0;

		  	if (background)
		  	{
		  		arguments[argc - 1] = NULL;
		  	}

			// Set up Commands for Pipeline Execution
		  	parse_and_configure_pipeline(cmds, argc);

		 	// Executing all Regular Commands: (NOT cd, history, !, comments, exit)
			int child = fork();
			
			if (child == -1) 
			{
			  /* Something went wrong */
			  fprintf(2, "%s fork failed\n" RESET, error_msg);
			} 
			else if (child == 0) 
			{	
				// Run Command
				execute_pipeline(cmds);
			} 
			else 
			{
				// Parent: Shell

				if (!background)
				{
					uint64 elapsed = 0;
					
					// Timing Current Commmand
					uint64 start = gettime();

					int finished_pid = 0;

					while (finished_pid != child)
					{
						finished_pid = wait(&exit_status);
					
						// Error Handling
						if (finished_pid < 0)
						{
							fprintf(2, "Wait failed\n" RESET, error_msg);
							break;
						}
					}
					
					uint64 end = gettime();
					elapsed = end - start;
	
					// Storing Time in the History
					cur_cmd->time = elapsed;
				}
				else
				{
					// Background Job 
				}
			}
		}
	}

	return exit_status;
}


// -------------------------------------------------- COMMAND FUNCTIONS --------------------------------------------------


/**
 * Displays the history
 *
 * @return an exit status
 * 	 - `0` for success
 * 	 - `1` for error
 */
int history_cmd()
{
	if (private_mode)
	{
		printf("🕵️\n");
		printf(BOLD WHITE "Private mode is on. " UNDERLINE BOLD "No history is being recorded.\n" RESET);
		return 0;
	}

	// Error Handling
	if (strcmp(arguments[1], "-t") == 0)
	{
		// Error Handling
		if (too_many_arguments_error(2, "-t", "history -t"))
		{
			return 1;
		}
	}
	else if (arguments[1] != NULL)
	{
		fprintf(2, "%s Unrecognize flag: " ITALIC "%s\n" RESET WHITE, error_msg, arguments[1]);
		printf("\n");
		printf("Usage:\n");
		printf("history -t\n");
		return 1;
	}
	
	// Loop through the History (Circular Buffer)
	for (int i = 0; i < 100; i += 1)
	{
		struct cmd_info *item = &history[(i + history_end) % 100];

		// Once an item hasn't been used, break out printing out the history
		if (item->cmd_number < 0)
		{
			continue;
		}

		if (strcmp(arguments[1], "-t") == 0) 
		{
			// Print History Info of the Current Item with Time
			printf(WHITE BOLD "[" ORANGE "%d" WHITE BOLD "|" RESET ORANGE "%dms" RESET WHITE BOLD "]" RESET CYAN" %s" RESET, item->cmd_number, item->time / 1000000, item->cmd);
		}
		else
		{
			// Print History Info of the Current Item
			printf(WHITE " %d" RESET CYAN " %s" RESET, item->cmd_number, item->cmd);
		}
	}

	return 0;
}


/**
 * Prints the working directory name
 *
 * @return an exit status
 * 	 - `0` for success
 * 	 - `1` for error
 */
int pwd_cmd(struct cmd_info *cur_cmd)
{
	// Error Handling
	if (too_many_arguments_error(1, "pwd", "pwd"))
	{
		return 1;
	}
	
	char cwd[128];
	uint64 elapsed = 0;
		
	// Timing the 'pwd' Command
	uint64 start = gettime();
	
	// Get Current Working Directory
	getcwd(cwd, 128);
	
	uint64 end = gettime();
	elapsed = end - start;

	if (!private_mode)
	{
		// Storing Time in History 
		cur_cmd->time = elapsed;
	}

	// Print the Working Directory Name
	printf(WHITE ITALIC "%s\n"RESET, cwd);
	return 0;
}


/**
 * Prints the operating system name and information
 *
 * @return an exit status
 * 	 - `0` for success
 * 	 - `1` for error
 */
int uname_cmd(struct cmd_info *cur_cmd)
{
	uint64 elapsed = 0;
		
	// Timing the 'pwd' Command
	uint64 start = gettime();

	if (strcmp(arguments[1], "-a") == 0)
	{
		// Error Handling
		if (too_many_arguments_error(2, "-a", "uname -a"))
		{
			return 1;
		}
		
		// Print the Working Directory Name
		printf(CYAN BOLD "OS: " RESET WHITE ITALIC "FogOS " CYAN "(" WHITE "based on xv6" CYAN ")\n" RESET);
		printf(CYAN BOLD "Host: " RESET WHITE ITALIC "Gojira\n" RESET);
		printf(CYAN BOLD "Kernel: " RESET WHITE ITALIC "xv6\n" RESET);
		printf(CYAN BOLD "Shell: " RESET WHITE ITALIC "Tosh v1" CYAN "." WHITE "0\n" RESET);
	}
	else if (arguments[1] != NULL)
	{
		fprintf(2, "%s Unrecognize flag:" WHITE " " ITALIC "%s\n" RESET WHITE, error_msg, arguments[1]);
		printf("\n");
		printf("Usage: \n");
		printf("uname -a\n");
		return 1;
	}
	else
	{
		// Print the Working Directory Name
		printf(WHITE ITALIC "FogOS " CYAN "(" WHITE "based on xv6" CYAN ")\n" RESET WHITE);
	}
	
	uint64 end = gettime();
	elapsed = end - start;
	
	if (!private_mode)
	{
		// Storing Time in History 
		cur_cmd->time = elapsed;
	}

	return 0;
}


/**
 * Changes the directory
 *
 * @return an exit status
 * 	 - `0` for success
 * 	 - `1` for error
 */
int cd_cmd(struct cmd_info *cur_cmd)
{
	uint64 elapsed = 0;
	
	// Timing the 'cd' Command
	uint64 start = gettime();
	int status = chdir(arguments[1]);
	
	// Error Handling
	if (status < 0)
	{
		fprintf(2, "%s chdir: no such file or directory: " ITALIC "%s\n" RESET WHITE, error_msg, arguments[1]);
		return 1;
	}

	uint64 end = gettime();
	elapsed = end - start;

	if (!private_mode)
	{
		// Storing Time in History 
		cur_cmd->time = elapsed;
	}

	return 0;
}


/**
 * Resets the shell
 *
 * @return an exit status
 * 	 - `0` for success
 * 	 - `1` for error
 */
int reset_cmd()
{
	// Error Handling
	if (too_many_arguments_error(1, "reset", "reset"))
	{
		return 1;
	}
	
	// Resetting Message
	printf("\n");
	printf("Resetting " ITALIC "Tosh\n" RESET);
	printf("\n");

	// Loading Message
	printf(WHITE ITALIC "\n");
	printf("Loading");
	sleep(6);
	printf(CYAN);
	printf(".");
	sleep(7);
	printf(".");
	sleep(8);
	printf(".");
	sleep(9);
	printf("\n");
	printf("\n" WHITE);

	// Restoring the Shell to its Initial State
	shell_initial_state();
	splash_screen();
	
	return 0;
}


/**
 * Exits the shell
 *
 * @return an exit status
 * 	 - `-1` for success
 * 	 - `1` for error
 */
int exit_cmd()
{
	// Error Handling
	if (too_many_arguments_error(1, "exit", "exit"))
	{
		return 1;
	}
	
	return -1;
}


/**
 * Repeat the last command 
 *
 * @return a pointer to the `struct cmd_info` representing the last command to repeat,
  *         or NULL if there was no last command found in the history.
 */
struct cmd_info *repeat_last_cmd()
{
	int last_cmd_index = (history_end == 0) ? 99 : history_end - 1;
	struct cmd_info *repeat_cmd = &history[last_cmd_index];

	// Checking If There was a Last Command
	if (repeat_cmd->cmd_number < 0)
	{
		fprintf(2, "%s There was no last command\n" RESET WHITE, error_msg);
		printf("\n");
		printf("Usage:\n");
		printf("%s\n", history_execution);
	 	return NULL;
	}

	return repeat_cmd;
}


/**
 * Repeat command based on a given number 
 *
 * @return a pointer to the `struct cmd_info` representing the command number to repeat,
  *         or NULL if no matching command is found in the history. 
 */
struct cmd_info *repeat_cmd_number()
{
	int i = 0;
	
	// Convert String to a Number
	int idx = atoi(&arguments[0][1]);

	// Check if Repeated Command is in the History
	for  (i = 0; i < 100; i += 1)
	{
		if (history[i].cmd_number == idx)
	 	{
	 		return &history[i];
	 	}
	}

	fprintf(2, "%s Command number " ITALIC "'%d'" RESET WHITE " is not in the history\n", error_msg, idx);
	printf(WHITE"\n");
	printf("Usage:\n");
	printf("%s\n", history_execution);
	return NULL;
}


/**
 * Repeat command based on the prefix 
 *
 * @return a pointer to the `struct cmd_info` representing the command prefix to repeat,
  *         or NULL if there was no command prefix found in the history.
 */
struct cmd_info *repeat_cmd_prefix()
{
	int i = 0;
	
	int prefix_length = strlen(arguments[0]) - 1;

	struct cmd_info *repeat_cmd = NULL;
	
	// Loop through the History Struct 
	for (i = 0; i < 100; i += 1)
	{
		// Check Current Command in the History
		repeat_cmd = &history[(history_end - i + 99) % 100];

		// Prefix is in the History Struct
	 	if (strncmp2(repeat_cmd->cmd, &arguments[0][1], prefix_length) == 0)
	 	{
	 		return repeat_cmd;
	 	}
	}

	fprintf(2, "%s Command " ITALIC "'%s'" RESET WHITE " is not in the history\n", error_msg, &arguments[0][1]);
	printf(WHITE"\n");
	printf("Usage:\n");
	printf("%s\n", history_execution);
	return NULL;
}


/**
 * Repeat a command 
 *
 * @return a pointer to the `struct cmd_info` representing the command to repeat,
  *         or NULL if there was no matching command found.
 */
struct cmd_info *redo_cmd()
{
	struct cmd_info *repeat_cmd = NULL;
	
	// Last Command
	if (strcmp(arguments[0], "!!") == 0)
	{
		repeat_cmd = repeat_last_cmd();
	}
	//  Number (e.g. !23)
	else if (isInt2(&arguments[0][1]))
	{
		repeat_cmd = repeat_cmd_number();
	}
	// Prefix (e.g. !c)
	else
	{
		repeat_cmd = repeat_cmd_prefix();
	}

	return repeat_cmd;
}


// -------------------------------------------------- EXECUTION HELPERS --------------------------------------------------


/**
 * Parses the commands and configures them for pipeline execution
 *
 * @param cmds An array of structs that represents a command in a pipeline
 * @param argc The number of command line arguments
 */
void parse_and_configure_pipeline(struct command *cmds, int argc)
{
	int cmds_idx = 0;
	
	// Check if There are Pipes
	for (int i = 0; i < argc; i += 1)
	{
		// Last Command
		if (i == argc - 1)
		{
			cmds[cmds_idx].stdout_pipe = false; /* Last command so set stdout_pipe = false */	
		}

		// Found a Pipe
		if (strcmp(arguments[i], "|") == 0)
		{
			cmds[cmds_idx].stdout_pipe = true;
			cmds[cmds_idx].stdout_file = NULL; /* This command is not writing to a file. */
			arguments[i] = NULL; // Remove the '|'
			cmds_idx += 1;

			// Next Token starts 1 position after the Pipe
			cmds[cmds_idx].tokens = &arguments[i + 1];
		}
		else if (strcmp(arguments[i], "<") == 0)
		{
			arguments[i] = NULL; // Removes the '<'
			cmds[cmds_idx].stdin_file = arguments[i + 1]; /* Input Redirection */
			i += 1;
			arguments[i] = NULL; // Removes the filename
		}
		else if (strcmp(arguments[i], ">") == 0)
		{
			arguments[i] = NULL; // Removes the '>'
			cmds[cmds_idx].stdout_file = arguments[i + 1]; /* Output Redirection */
			cmds[cmds_idx].append_file = false;
			i += 1;
			arguments[i] = NULL; // Removes the filename
		}
		else if (strcmp(arguments[i], ">>") == 0)
		{
			arguments[i] = NULL; // Removes the '>>'
			cmds[cmds_idx].stdout_file = arguments[i + 1]; /* Output Redirection to a File */
			cmds[cmds_idx].append_file = true;
			i += 1;
			arguments[i] = NULL; /* Removes the filename */
		}
	}
}


/**
 * Resets the arguments list to avoid reusing old commands
 */
void reset_arguments()
{
	for (int i = 0; i < 128; i += 1)
	{
		arguments[i] = NULL;
	}
}


/**
 * Stores data in the history struct
 */
void populate_history_struct(struct cmd_info *cur_cmd, char *cmd_copy)
{
	uint64 elapsed = 0;
	
	// Overwrite the Next Item in the History List
	if (cur_cmd->cmd_number >= 0)
	{
		free(cur_cmd->cmd);
	 }
	 
	// Add Data to the History Struct
	cur_cmd->cmd = cmd_copy;
	cur_cmd->cmd_number = command_number; /* Storing the command counter for this session of 'tosh' */
	cur_cmd->time = elapsed; /* Time it took to execute current command */

	// Wrap Around to the Top of the History Array
	history_end = (history_end + 1) % 100; 

	// Increment Command Number Counter
	command_number += 1;
}


/**
 * Truncates the string at the comment ('#')
 */
void truncate_str_at_comment(char *cmd)
{
	// Truncate the String at the '#'
	for (int i = 0; i < strlen(cmd); i += 1)
	{
		// Found Comment
		if (cmd[i] == '#')
		{
			cmd[i] = '\n'; /* Ensures that the next command is in a new line */
	  		cmd[i + 1] = '\0';
	  	}
	}
}


/**
 * Strings prefixed with a comment ('#') or empty strings will be ignored by the shell
 *
 * @param cmd The given command
 * @return an exit status
 * 	 - `0` for success if a comment or empty string was found
 * 	 - `1` otherwise
 */
int handle_comments_and_empty_string(char *cmd)
{
	// # (comments): strings prefixed with # will be ignored by the shell / Empty strings will be ignoredy
	if (cmd[0] == '#' || strlen(cmd) == 1)
	{
		return 0;
	}

	for (int i = 0; i < strlen(cmd); i += 1)
	{
		if (cmd[i] != ' ' && cmd[i] != '\t' && cmd[i] != '\n')
		{
			return 1;
		}
	}

	return 0;
}


// -------------------------------------------------- OPTIONS --------------------------------------------------


/**
 * Identifies which option to execute
 *
 * @return an option type
 * 	 - `0` for regular option
 * 	 - `1` standalone option
 */
int tosh_options(char *option)
{
	if (strcmp(option, "-v") == 0 || strcmp(option, "--version") == 0)
	{
		version_opt();
		return 1;
	}
	else if (strcmp(option, "--about") == 0)
	{
		about_opt();
		return 1;
	}
	else if (strcmp(option, "--license") == 0)
	{
		license_opt();
		return 1;
	}
	else if (strcmp(option, "-lc") == 0 || strcmp(option, "--list-commands") == 0)
	{
		list_cmds_opt();
		return 1;
	}
	else if (strcmp(option, "-p") == 0 || strcmp(option, "--private") == 0)
	{
		private_opt();
		return 0;
	}
	else
	{	
		fprintf(2, "%s Unrecognize option: " ITALIC "%s\n" RESET WHITE, error_msg, option);
		printf(WHITE"\n");
		printf("Usage:\n");
		printf("%s\n" RESET WHITE, options);
		return 1;
	}
}


/**
 * Shows which version of 'Tosh'
 */
void version_opt()
{
	// Error Handling
	if (too_many_arguments_error(2, "-v, --version", "tosh -v | --version"))
	{
		return;
	}
	
	printf(ITALIC WHITE "Tosh v1" CYAN "." WHITE "0\n" RESET WHITE);
}


/**
 * Displays the xv6 license
 */
void license_opt()
{
	// Error Handling
	if (too_many_arguments_error(2, "--license", "tosh --license"))
	{
		return;
	}
	
	printf(WHITE"\n");
	
	printf("The xv6 software is:\n");
	printf("\n");
	printf("Copyright (c) 2006-2019 Frans Kaashoek, Robert Morris, Russ Cox,\n\t\t\tMassachusetts Institute of Technology\n");
	printf("\n");
	printf("Permission is hereby granted, free of charge, to any person obtaining\n");
	printf("a copy of this software and associated documentation files (the\n");
	printf("\"Software\"), to deal in the Software without restriction, including\n");
	printf("without limitation the rights to use, copy, modify, merge, publish,\n");
	printf("distribute, sublicense, and/or sell copies of the Software, and to\n");
	printf("permit persons to whom the Software is furnished to do so, subject to\n");
	printf("the following conditions:\n");
	printf("\n");
	printf("\n");
	printf("The above copyright notice and this permission notice shall be\n");
	printf("included in all copies or substantial portions of the Software.\n");
	printf("\n");
	printf("\n");
	printf("THE SOFTWARE IS PROVIDED \"AS IS\", WITHOUT WARRANTY OF ANY KIND,\n");
	printf("EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF\n");
	printf("MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND\n");
	printf("NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE\n");
	printf("LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION\n");
	printf("OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION\n");
	printf("WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.\n");

	printf("\n");
}


/**
 * Prints out an about section for 'Tosh'
 */
void about_opt()
{
	// Error Handling
	if (too_many_arguments_error(2, "--about", "tosh --about"))
	{
		return;
	}

	printf(ITALIC WHITE "Tosh v1" CYAN "." WHITE "0\n");
	printf( "Created by " BOLD "NINO ESTRADA\n" RESET ITALIC WHITE);
	printf("" CYAN "©" WHITE " 2024\n" RESET WHITE);
	printf("\n");
	printf("" ITALIC "Tosh" RESET WHITE " was built to bring simplicity and creativity to the shell" CYAN ".\n" RESET WHITE);
	printf("\n");
	printf("The name " CYAN "'" WHITE "Tosh" CYAN "'" WHITE " was inspired by an operating systems class where the professor created a shell and called it " CYAN "'" WHITE ITALIC "Trash" RESET CYAN ".'" WHITE " I wanted to come up with a just as bold name" CYAN "," WHITE " so I chose " CYAN "'" WHITE ITALIC "Tosh" RESET CYAN "'—" WHITE "a British slang word meaning " CYAN "'" WHITE "rubbish" CYAN "'" WHITE " or " CYAN "'" WHITE "nonsense" CYAN ".'\n" WHITE);
	printf("\n");
	printf("" ITALIC "Tosh" RESET WHITE " was built to bridge the gap between practicality and personality" CYAN "," WHITE " hopefully helping users enjoyably explore their system" CYAN ".\n" WHITE);
	printf("\n");
	printf("Thank you for using " ITALIC "Tosh" RESET CYAN "!\n" RESET WHITE);
}


/**
 * Displays a list of all the commands in 'Tosh'
 */
void list_cmds_opt()
{
	// Error Handling
	if (too_many_arguments_error(2, "-lc, --list-commands", "tosh -lc | --list-commands"))
	{
		return;
	}
	
	const char *list_of_cmds = WHITE UNDERLINE "Command\n" RESET CYAN
	"cat\n"
	"echo\n"
	"ls\n"
	"cd\n" 
	"mkdir\n"
	"pwd\n"
	"wc\n" 
	"history\n"
	"test\n"
	"uname\n"
	"joke\n"
	"fact\n"
	"quote\n"
	"menu\n"
	"ref\n"
	"exit\n";

	printf("\n%s \n" WHITE, list_of_cmds);
}


/**
 * Sets the shell to private mode where commands are not stored in the history
 */
void private_opt()
{
	// Error Handling
	if (too_many_arguments_error(2, "-p, --private", "tosh -p | --private"))
	{
		return;
	}
	
	if (private_mode)
	{
		printf("Private mode is already on" CYAN ".\n" RESET WHITE);
		return;	
	}
	
	private_mode = true;
	printf("🕵️ \n");
	printf("Prvate mode is on" CYAN ".\n" RESET WHITE);
}


// -------------------------------------------------- SPECIAL FEATURES --------------------------------------------------


/**
 * The first screen that appears when tosh is booted up
 */
void splash_screen()
{
	printf(WHITE ITALIC "\n");
	printf("Okay");
	sleep(1);
	printf(CYAN ", " WHITE);
	sleep(3);
	printf("3");
	sleep(3);
	printf(CYAN ", " WHITE);
	sleep(3);
	printf("2");
	sleep(3);
	printf(CYAN ", " WHITE);
	sleep(3);
	printf("1");
	sleep(3);
	printf(CYAN ", " WHITE);
	sleep(3);
	printf(UNDERLINE "let" CYAN "'" WHITE "s jam");
	sleep(3);
	printf("\n\n\n" RESET);
	sleep(2);
	printf(BLUE);
	printf("		████████╗ ██████╗ ███████╗██╗  ██╗\n");
	sleep(2);
    printf("		╚══██╔══╝██╔═══██╗██╔════╝██║  ██║\n");
    sleep(2);
    printf("		   ██║   ██║   ██║███████╗███████║\n");
    sleep(2);
    printf("		   ██║   ██║   ██║╚════██║██╔══██║\n");
    sleep(2);
    printf("		   ██║   ╚██████╔╝███████║██║  ██║\n");
    sleep(2);
    printf("		   ╚═╝    ╚═════╝ ╚══════╝╚═╝  ╚═╝\n");
    sleep(2);
	printf(ITALIC "   			 the operating shell\n" RESET);
	sleep(2);
	printf(YELLOW "\n");
	sleep(2);
	printf("		   📝 Change Folders     cd\n");
	sleep(2);
	printf("		   📂 Make Folder	 mkdir\n");
	sleep(2);
	printf("		   📑 List Content	 ls\n");
	sleep(2);
	printf("		   📢 Output 		 echo\n");
	sleep(2);
	printf("		   ⏳ History 		 history\n");
	sleep(2);
	printf("		   📚 Reference 	 ref\n");
	sleep(2);
	printf("		   🚪 Quit		 exit\n");
	sleep(2);
	printf("\n"RESET);
	printf(BLUE ITALIC "   		      created by NINO ESTRADA\n" RESET);
	sleep(2);
}


/**
 * The main menu of tosh highlighting key features
 *
 * @return an exit status
 * 	 - `0` for success
 * 	 - `1` for error
 */
int main_menu_cmd(struct cmd_info *cur_cmd)
{
	// Error Handling
	if (too_many_arguments_error(1, "menu", "menu"))
	{
		return 1;
	}
	
	uint64 elapsed = 0;

	// Timing the 'menu' Command
	uint64 start = gettime();

	printf("\n\n");
	printf(BLUE);
	printf("		████████╗ ██████╗ ███████╗██╗  ██╗\n");
    printf("		╚══██╔══╝██╔═══██╗██╔════╝██║  ██║\n");
    printf("		   ██║   ██║   ██║███████╗███████║\n");
    printf("		   ██║   ██║   ██║╚════██║██╔══██║\n");
    printf("		   ██║   ╚██████╔╝███████║██║  ██║\n");
    printf("		   ╚═╝    ╚═════╝ ╚══════╝╚═╝  ╚═╝\n");;
	printf(ITALIC "   			 the operating shell\n" RESET);
	printf(YELLOW "\n");
	printf("		   📝 Change Folders     cd\n");
	printf("		   📂 Make Folder	 mkdir\n");
	printf("		   📑 List Content	 ls\n");
	printf("		   📢 Output 		 echo\n");
	printf("		   ⏳ History 		 history\n");
	printf("		   📚 Reference 	 ref\n");
	printf("		   🚪 Quit		 exit\n");
	printf("\n"RESET);
	printf(BLUE ITALIC "   		      created by NINO ESTRADA\n" RESET);

	uint64 end = gettime();
	elapsed = end - start;

	if (!private_mode)
	{
		// Storing Time in History 
		cur_cmd->time = elapsed;
	}

	return 0;
}


/**
 * The reference command displays all the available commands and their descriptions
 *
 * @return an exit status
 * 	 - `0` for success
 * 	 - `1` for error
 */
int reference_cmd(struct cmd_info *cur_cmd)
{
	// Error Handling
	if (too_many_arguments_error(1, "ref", "ref"))
	{
		return 1;
	}
	
	uint64 elapsed = 0;

	// Timing the 'help' Command
	uint64 start = gettime();

	const char *synopsis = WHITE BOLD "		" BLUE_BACKGROUND "SYNOPSIS" RESET "\n\n" RESET
	WHITE "tosh " BOLD "[" RESET ORANGE "options" BOLD WHITE "] [" RESET ORANGE "script" BOLD WHITE "]\n" RESET WHITE;

	const char *note = WHITE BOLD "Note" RESET WHITE": Parameters in square brackets are optional.";

	printf(WHITE BOLD UNDERLINE "Reference Guide" RESET " \n\n%s \n%s \n\n%s \n\n%s \n\n%s \n\n%s \n\n%s \n" RESET WHITE, synopsis, note, options, built_in_cmds, history_execution, io_redirection, job_control);

	uint64 end = gettime();
	elapsed = end - start;

	if (!private_mode)
	{
		// Storing Time in History 
		cur_cmd->time = elapsed;
	}

	return 0;
}


/**
 * Displays a programming-related joke to the terminal
 *
 * @return an exit status
 * 	 - `0` for success
 * 	 - `1` for error
 */
int joke_cmd(struct cmd_info *cur_cmd)
{
	// Error Handling
	if (too_many_arguments_error(1, "joke", "joke"))
	{
		return 1;
	}
	
	uint64 elapsed = 0;

	// Timing the 'joke' Command
	uint64 start = gettime();
	
	char *jokes[20];
	jokes[0] = "How many computer programmers does it take to change a light bulb" CYAN "?\n🤔..." WHITE "\nNone" CYAN "," WHITE " that" CYAN "'" WHITE "s a hardware problem" CYAN "!" RESET WHITE;
	jokes[1] = "There are 10 types of people in the world" CYAN "." WHITE " Those who understand binary" CYAN "," WHITE " and those who don" CYAN "'" WHITE "t" CYAN "." RESET WHITE;
	jokes[2] = "My dog ate my computer sceince project" CYAN "." WHITE " It only took him a few bytes" CYAN "!" RESET WHITE;
	jokes[3] = "I don" CYAN "'" WHITE "t talk about computer science at home" CYAN "." WHITE " It makes my mother board" CYAN "!" RESET WHITE;
	jokes[4] = "I don" CYAN "'" WHITE "t like computer science jokes" CYAN "." WHITE " Not one bit" CYAN "!" RESET WHITE; 
	jokes[5] = "Why couldn" CYAN "'" WHITE "t the computer science student read his text book" CYAN "?\n🤔..." WHITE "\nHe couldn" CYAN "'" WHITE "t find page 404" CYAN "!" RESET WHITE;
	jokes[6] = "What do you get if you cross computer science with a pond" CYAN "?\n🤔..." WHITE "\nAlgae rhythms" CYAN "!" RESET WHITE;
	jokes[7] = "Which farm animal likes computing the best" CYAN "?\n🤔..." WHITE "\nThe RAM" CYAN "!" RESET WHITE;
	jokes[8] = "Why was the computer programmer fired from M15" CYAN "?\n🤔..." WHITE "\nHe couldn" CYAN "'" WHITE "t hack it" CYAN "!" RESET WHITE;
	jokes[9] = "Why should you be scared of computers" CYAN "?\n🤔..." WHITE "\nThey byte" CYAN "!" RESET WHITE;
	jokes[10] = "Where do computer programmers go for fun" CYAN "?\n🤔..." WHITE "\nThe disc" CYAN "-" WHITE "o" CYAN "!" RESET WHITE;
	jokes[11] = "Why do computer scientists prefer the metric system" CYAN "?\n🤔..." WHITE "\nThey are all pro" CYAN "-" WHITE "gram" CYAN "-" WHITE "ers" CYAN "!" RESET WHITE;
	jokes[12] = "What" CYAN "'" WHITE "s a computer scientist" CYAN "'" WHITE "s favorite type of coffee" CYAN "?\n🤔..." WHITE "\nJava" CYAN "!" RESET WHITE;
	jokes[13] = "What does a baby computer call its dad" CYAN "?\n🤔..." WHITE "\nData" CYAN "!" RESET WHITE;
	jokes[14] = "What" CYAN "'" WHITE "s a computer programmer" CYAN "'" WHITE "s house made of" CYAN "?\n🤔..." WHITE "\nFirewalls" CYAN "!" RESET WHITE;
	jokes[15] = "Why did the computer programmer quit" CYAN "?\n🤔..." WHITE "\nHe didn" CYAN "'" WHITE "t get arrays" CYAN "!" RESET WHITE;
	jokes[16] = "I could tell you a joke about UDP" CYAN "," WHITE " but I don" CYAN "'" WHITE "t know if you" CYAN "'" WHITE "d get it" CYAN "." RESET WHITE;
	jokes[17] = "Who" CYAN "'" WHITE "s a computer scientist" CYAN "'" WHITE "s favorite Star Wars character" CYAN "?\n🤔..." WHITE "\nAdobe Wan Kenobi" CYAN "!" RESET WHITE;
	jokes[18] = "What kind of money do computer sientists use" CYAN "?\n🤔..." WHITE "\nCache" CYAN "!" RESET WHITE;
	jokes[19] = "Why was the computer scientist bad at driving" CYAN "?\n🤔..." WHITE "\nThey kept crashing" CYAN "!" RESET WHITE;

	// Print a Joke Randomly
	printf(WHITE ITALIC "%s\n" RESET WHITE, jokes[cur_cmd->cmd_number%20]);

	uint64 end = gettime();
	elapsed = end - start;

	if (!private_mode)
	{
		// Storing Time in History 
		cur_cmd->time = elapsed;
	}

	return 0;
}


/**
 * Displays a fun programming-related fact to the terminal
 *
 * @return an exit status
 * 	 - `0` for success
 * 	 - `1` for error
 */
int fact_cmd(struct cmd_info *cur_cmd)
{
	// Error Handling
	if (too_many_arguments_error(1, "fact", "fact"))
	{
		return 1;
	}
	
	uint64 elapsed = 0;

	// Timing the 'fact' Command
	uint64 start = gettime();
	
	char *facts[21];
	facts[0] = "The first computer virus was created in 1986" CYAN "." RESET WHITE;
	facts[1] = "Ada Lovelace" CYAN "," WHITE" a woman" CYAN "," WHITE " was the world" CYAN "'" WHITE "s first computer programmer" CYAN "." RESET WHITE;
	facts[2] = "The world" CYAN "'" WHITE "s first electronic computer" CYAN "," WHITE " the Colossus" CYAN "," WHITE " was built in 1943" CYAN "." RESET WHITE;
	facts[3] = "The first webcam was invented at the University of Cambridge to monitor the coffee pot in the computer science department" CYAN "." RESET WHITE;
	facts[4] = "The first email was sent in 1971 by Ray Tomlinson" CYAN "." RESET WHITE;
	facts[5] = "The first computer game" CYAN ", '" WHITE "Spacewar" CYAN "!'," WHITE " was invented in 1962" CYAN "." RESET WHITE;
	facts[6] = "The first computer" CYAN "," WHITE " known as the ENIAC" CYAN "," WHITE " weighed 30 tons and took up an entire room" CYAN "." RESET WHITE;
	facts[7] = "The first website was created in 1991 by Tim Berners" CYAN "-" WHITE "Lee" CYAN "." RESET WHITE;
	facts[8] = "There are over 700 programming languages" CYAN "!" RESET WHITE;
	facts[9] = "At least 30k websites are hacked daily" CYAN "!" RESET WHITE;
	facts[10] = "Just 10" CYAN "%" WHITE " of the money in the world is physical currency" CYAN "," WHITE " the remaining being computer currency" CYAN "!" RESET WHITE;
	facts[11] = "A teenager " CYAN "(" WHITE "Johnathan James" CYAN ")" WHITE " once hacked NASA" CYAN "'" WHITE "s computer systems" CYAN "!" RESET WHITE;
	facts[12] = "Domain names were free until 1995" CYAN "!" RESET WHITE;
	facts[13] = "People blink less when they use computers" CYAN "." RESET WHITE;
	facts[14] = "More than 80" CYAN "%" WHITE " of daily emails in the U" CYAN "." WHITE "S" CYAN "." WHITE " are spam" CYAN "." RESET WHITE;
	facts[15] = "The password of the computers in charge of controlling the nuclear missiles of the United States Army for years was 0000000000" CYAN "." RESET WHITE;
	facts[16] = "The three most commonly used passwords in the world are 123456" CYAN "," WHITE " password" CYAN "," WHITE " and 12345" CYAN "." RESET WHITE;
	facts[17] = "NASA still uses programs from the 70s in their spacecraft" CYAN "." RESET WHITE;
	facts[18] = "It took less code to send a man to the Moon than to run a smartphone" CYAN "." RESET WHITE;
	facts[19] = "Computer Programming played a vital role in the ending of World War II" CYAN "." RESET WHITE;
	facts[20] = "JAVA was born out of a coffee session" CYAN "." RESET WHITE;

	// Print a Fact
	printf(WHITE ITALIC "%s\n" RESET WHITE, facts[cur_cmd->cmd_number%21]);

	uint64 end = gettime();
	elapsed = end - start;

	if (!private_mode)
	{
		// Storing Time in History 
		cur_cmd->time = elapsed;
	}

	return 0;
}


/**
 * Displays a programming-related quote to the terminal
 *
 * @return an exit status
 * 	 - `0` for success
 * 	 - `1` for error
 */
int quote_cmd(struct cmd_info *cur_cmd)
{
	// Error Handling
	if (too_many_arguments_error(1, "quote", "quote"))
	{
		return 1;
	}
	
	uint64 elapsed = 0;

	// Timing the 'quote' Command
	uint64 start = gettime();
	
	char *quotes[21];
	quotes[0] = "\"Code is like humor" CYAN "." RESET WHITE ITALIC " When you have to explain it" CYAN"," RESET WHITE ITALIC " it" CYAN "'" RESET WHITE ITALIC "s bad" CYAN "." RESET WHITE ITALIC "\" " RESET CYAN BOLD "-" RESET WHITE " Cory House";
	quotes[1] = CYAN "\"" WHITE "Coding is like poetry should be short and concise" CYAN ".\" " RESET CYAN BOLD "-" RESET WHITE " Santosh Kalwar";
	quotes[2] = CYAN "\"" WHITE "Software is like sex" CYAN ";" WHITE " it" CYAN"'" WHITE "s better when it" CYAN "'" WHITE "s free" CYAN".\" " RESET CYAN BOLD "-" RESET WHITE " Linus Torvalds";
	quotes[3] = CYAN "\"" WHITE "If we want users to like our software" CYAN "," WHITE " we should design it to behave like a likable person" CYAN ".\" " RESET CYAN BOLD "-" RESET WHITE " Alan Cooper";
	quotes[4] = CYAN "\"" WHITE "Software and cathedrals are much the same " CYAN BOLD "—" RESET WHITE ITALIC" first we build them" CYAN "," WHITE " then we pray" CYAN ".\"" RESET WHITE;
	quotes[5] = CYAN "\"" WHITE "When I wrote this code" CYAN "," WHITE " only God and I understood what I did" CYAN "." WHITE " Now only God knows" CYAN ".\"  " RESET CYAN BOLD "–" RESET WHITE " Anonymous";
	quotes[6] = CYAN "\"" WHITE "You might not think that programmers are artists" CYAN "," WHITE " but programming is an extremely creative profession" CYAN "." WHITE" It" CYAN "’" WHITE "s logic" CYAN "-" WHITE "based creativity" CYAN ".\" " RESET CYAN BOLD "–" RESET WHITE" John Romero";
	quotes[7] = CYAN "\"" WHITE "If debugging is the process of removing bugs" CYAN "," WHITE " then programming must be the process of putting them in" CYAN ".\" " RESET CYAN BOLD "–" RESET WHITE" Sam Redwine";
	quotes[8] = CYAN "\"" WHITE "The most dangerous phrase in the language is" CYAN ", ‘" WHITE "We" CYAN "’" WHITE "ve always done it this way" CYAN ".'\" " RESET CYAN "-" RESET WHITE " Grace Hopper";
	quotes[9] = CYAN "\"" WHITE "Code is read much more often than it is written" CYAN ".\" " RESET CYAN BOLD "-" RESET WHITE " Guido Van Rossum";
	quotes[10] = CYAN "\"" WHITE "This was one of the best parts of being a coder" CYAN "," WHITE " and an artist" CYAN ":" WHITE " the thrill of being in the middle of creating something delightful" CYAN "." WHITE " It" CYAN "’" WHITE "s like the anticipation of eating freshly baked bread after its aroma fills the room" CYAN ".\" " RESET CYAN BOLD "-" RESET WHITE " Dr. Joy Buolamwini";
	quotes[11] = CYAN "\"" WHITE "Every great developer you know got there by solving problems they were unqualified to solve until they actually did it" CYAN ".\" " RESET CYAN BOLD "-" RESET WHITE " Patrick McKenzie";
	quotes[12] = CYAN "\"" WHITE "The code you write makes you a programmer" CYAN "." WHITE " The code you delete makes you a good one" CYAN "." WHITE " The code you don" CYAN "’" WHITE "t have to write makes you a great one" CYAN ".\" " RESET CYAN BOLD "-" RESET WHITE " Mario Fusco";
	quotes[13] = CYAN "\"" WHITE "Any fool can write code that a computer can understand" CYAN "." WHITE " Good programmers write code that humans can understand" CYAN ".\" " RESET CYAN BOLD "-" RESET WHITE " Martin Fowler";
	quotes[14] = CYAN "\"" WHITE "The function of good software is to make the complex appear to be simple" CYAN ".\" " RESET CYAN BOLD "-" RESET WHITE " Grady Booch";
	quotes[15] = CYAN "\"" WHITE "Simplicity is the soul of efficiency" CYAN ".\" " RESET CYAN BOLD "-" RESET WHITE " Austin Freeman";
	quotes[16] = CYAN "\"" WHITE "It" CYAN "'" WHITE "s not a bug " CYAN "–" WHITE " it" CYAN "'" WHITE "s an undocumented feature" CYAN ".\"" RESET WHITE;
	quotes[17] = CYAN "\"" WHITE "In order to understand recursion" CYAN "," WHITE " one must first understand recursion" CYAN ".\" " RESET CYAN BOLD "-" RESET WHITE " Mouna Ben Ali";
	quotes[18] = CYAN "\"" WHITE "Code never lies" CYAN "," WHITE " comments sometimes do" CYAN ".\" " RESET CYAN BOLD "-" RESET WHITE " Ron Jeffries";
	quotes[19] = CYAN "\"" WHITE "Don" CYAN "’" WHITE "t comment bad code " RESET CYAN BOLD "–" RESET WHITE ITALIC " rewrite it" CYAN ".\" " RESET CYAN BOLD "-" RESET WHITE " Brian Kernighan";
	quotes[20] = CYAN "\"" WHITE "Debugging becomes significantly easier if you first admit that you are the problem" CYAN ".\" " RESET CYAN BOLD "-" RESET WHITE " William Laeder";

	// Print a Quote
	printf(WHITE ITALIC "%s\n" RESET WHITE, quotes[cur_cmd->cmd_number%21]);

	uint64 end = gettime();
	elapsed = end - start;

	if (!private_mode)
	{
		// Storing Time in History 
		cur_cmd->time = elapsed;
	}

	return 0;
}


/**
 * The last screen that appears when tosh is shutdown
 */
void off_message()
{
	printf("\n\n");
	printf(BLUE);
	printf("		██╗███╗   ███╗     ██████╗ ███████╗███████╗\n");
	printf("		██║████╗ ████║    ██╔═══██╗██╔════╝██╔════╝\n");
	printf("		██║██╔████╔██║    ██║   ██║█████╗  █████╗  \n");
	printf("		██║██║╚██╔╝██║    ██║   ██║██╔══╝  ██╔══╝  \n");
	printf("		██║██║ ╚═╝ ██║    ╚██████╔╝██║     ██║██╗  \n");
	printf("		╚═╝╚═╝     ╚═╝     ╚═════╝ ╚═╝     ╚═╝╚═╝  \n");
	printf("		                                            \n" RESET);
}


// -------------------------------------------------- MAIN FUNCTION --------------------------------------------------


/**
 * The entry point of the program
 *
 * @param argc The number of command line arguments
 * @param argv The array of command line arguments
 * @return an exit status
 * 	 - `0` for success
 * 	 - `1` for error
 */
int main(int argc, char **argv)
{
	int exit_status = 0; 
	uint cmd_size = 512;
	char *cmd = (char *)malloc(cmd_size); /* buffer */
	int input_fd = 0; /* Receive commands from STDIN by default */
	bool show_splash = true;

	// Shell Script Support
	if (argc > 1)
	{
		// Option
		if (argv[1][0] == '-')
		{
			for (int i = 0; i < argc; i += 1)
			{
				arguments[i] = argv[i];	
			}
			
			if (tosh_options(argv[1]) == 1)
			{
				return 0;
			}
		}
		else
		{
			// Receive commands from specify script file
			input_fd = open(argv[1], 0);
	
			// Error Handling
			if (input_fd < 0)
			{
				fprintf(2, "%s " WHITE "cannot open: " ITALIC "%s" RESET "\n", error_msg, argv[1]);
				exit(1);
			}

			show_splash = false;
		}
	}

	if (show_splash)
	{
		splash_screen();
		printf("\n");
	}

	// Initilaize all the command numbers to be -1 to indicate they have not been used yet
	initialize_history();

	// Keep Retreiving Commands until User Exits or End of Script File has Reached
	while (exit_status != -1)
	{
		// STDIN
		if (input_fd == 0)
		{
			// Get Current Working Directory
			getcwd(cwd, 128);
			
			if (private_mode)
			{
				printf(WHITE BOLD "[" RESET WHITE "%d" BOLD "]" CYAN "-" BOLD WHITE "[" RESET WHITE "%s" BOLD "]" RESET CYAN "$ " WHITE, exit_status, cwd);
			}
			else
			{
				// Printing the Prompt
				printf(WHITE BOLD "[" RESET WHITE "%d" BOLD "]" CYAN "-" BOLD WHITE "[" RESET WHITE "%d" BOLD "]" CYAN "-" BOLD WHITE "[" RESET WHITE "%s" BOLD "]" RESET CYAN "$ " WHITE, exit_status, command_number, cwd);
			}		
		}

		// Getting the Next Command
		int read = getline(&cmd, &cmd_size, input_fd);

		// Run the Current Command
		exit_status = execute(cmd);

		// Reach the End of the Script File
		if (read < 1)
		{
			break;
		}
	}

	if (show_splash)
	{
		off_message();
	}

	return 0;
}


// -------------------------------------------------- REFERENCES --------------------------------------------------


/*
strncmp - https://www.tutorialspoint.com/c_standard_library/c_function_strncmp.htm
Avoiding Zombie Processes in Background Jobs - https://www.geeksforgeeks.org/zombie-processes-prevention/
Adding Colors to Terminal - https://www.theurbanpenguin.com/4184-2/
Colors - https://en.wikipedia.org/wiki/ANSI_escape_code
ASCII Table - https://www.cs.cmu.edu/~pattis/15-1XX/common/handouts/ascii.html
ASCII Font - https://patorjk.com/software/taag/#p=display&f=Graffiti&t=Type%20Something%20
Goodbye ASCII Art - https://emojicombos.com/bye-ascii-art
Computer Science Jokes - https://www.beano.com/jokes/science/computer-science-jokes
Fun Facts about Computer Science - https://www.idtech.com/blog/computer-science-facts-collection-of-interesting-statistics
Fun Facts about Computer Science 2 - https://mp.moonpreneur.com/blog/computer-science-facts-for-kids/
Fun Facts about Computer Science 3 - https://www.computercpr.com/computer-facts/
Fun Facts about Computer Science 4 - https://pandorafms.com/blog/computing-facts/
Fun Facts about Computer Science 5 - https://www.21kschool.com/us/blog/15-facts-about-coding/
Quotes - https://techvify-software.com/35-best-coding-programming-quotes/
Quotes 2 - https://www.codecademy.com/resources/blog/inspirational-coding-quotes/
Quotes 3 - https://www.geeksforgeeks.org/coding-quotes-for-software-engineers/
Quotes 4 - https://devtechnosys.com/insights/inspiring-programming-quotes-for-developer/
Random - https://wiki.osdev.org/Random_Number_Generator
*/
