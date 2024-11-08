<!-- SHELL LOGO -->
<div align="center">
    🐚
    <h3 align="center" style="font-size: 2.1em; font-weight: bolder;">Tosh</h3>

  <p align="center">
    Command Line Shell
    <br />
    <br />
    <a href="https://github.com/nestrada2/FogOS/issues/new?labels=bug&template=bug-report.md">Report Bug</a>
    ·
    <a href="https://github.com/nestrada2/FogOS/issues/new?labels=enhancement&template=feature-request.md">Request Feature</a>
  </p>
</div>

<!-- ABOUT THE SHELL -->
## 📖 About the Shell
<h3 align="center"><b>(In Development)</b></h3>
Tosh is a custom command-line shell with features such as command execution, built-in commands, I/O redirection, and background job management, providing an interactive user experience. Enhanced functionality with scripting mode (including shebang support), dynamic prompts displaying exit status, command history, and current working directory. Integrating custom path execution for seamless interaction with Unix-like environments. Tosh Implements advanced features such as executable script support and background job handling, optimizing user interaction with the operating system.

<!-- FILES ADDED -->
### **📁➕ Files Added**
user:
- tosh.c
- tosh.helpers.h
- tosh_helpers.c
- tosh_features.h
- tosh_features.c

kernel:
- fcntl.h
- fs.c 
- sysfile.c

- **Synopsis**
    ```sh
    tosh [script]
    ```

    **⚠️ Note:** Parameters in square brackets are optional.

<!-- BUILT-IN COMMANDS -->
### **🔧 Built-In Commands in Tosh**
| Command             | Description                                                                  | 
| :---------:         | :-----------:                                                                |
| cat filename        | concatenate and print files                                                  |
| echo [string]       | Displays text to the terminal                                                | 
| ls [/dir]           | List directory contents                                                      |   
| cd /dir             | Change the current directory                                                 | 
| mkdir dir_name      | Make directories                                                             |
| pwd                 | Prints the working directory                                                 |
| history [-t]        | Display the history list of the last 100 commands with their command numbers | 
| !<num>              | A history execution that will re-run command number num                      | 
| !!                  | A history execution that re-runs the last command that was entered           |
| !<prefix>           | A history execution that re-runs the last command with that prefix           | 
| command1 | command2 | Allows the output of one command serves as the input for another command     | 
| command > filename  | Redirects the output of a command to a file                                  | 
| filename < command  | Redirects the content of a file to be input for a command                    | 
| command >> filename | Append the output of a command to the end of a file                          |
| exit                | Exit the shell                                                               | 

<!-- EXAMPLES -->
### **📋 Examples**
 **Basic Unix/Linux Commands**
 ```sh
 cat README.md
 echo "mwahahaha"
 mkdir dog
 cd dog
 pwd
 !!
 ```
 
 **Scripting Support**
 ```sh
 tosh hisotry_test1.sh
 ```

 **History**
 ```sh
 history
 history -t
 ```

 **Pipe & Redirection**
 ```sh
 cat input.txt | toLower | fnr the teh a 4 e 3 i ! | fnr l 1 o 0 s 5 > output.txt
 ```

 <!-- SPECIAL FEATURES -->
### **✨ Special Features in Tosh**
| Feature                              | Description                                                                                                        | 
| :---------:                          | :-----------:                                                                                                      |
| Splash Screen                        | Displays a welcome screen with the shell's name and key features                                                   |
| Color Coding                         | Uses different colors to differentiate between commands, output, and errors for improved readability and usability | 
| Text Formatiing                      | Add text styling like italics, and bold for clearer, more improved terminal display                                |   
| Case Insensitive (Built-In Commands) | Allows commands to be entered in any letter case for a more user-friendly experience                               | 


<!-- TEST -->
### **🧑‍🔬 Tests**
| Command               | Description                                                                        | 
| :---------:           | :-----------:                                                                      | 
| tosh history_test.sh  | Runs a series of automated tests to validate the behavior of the built-in commands | 
| tosh history_test1.sh | Similar test series for built-in commands                                          |                  
| tosh history_test2.sh | Similar test series for built-in commands                                          | 
| tosh history_test3.sh | Similar test series for built-in commands                                          | 
| tosh history_test4.sh | Similar test series for built-in commands                                          | 
