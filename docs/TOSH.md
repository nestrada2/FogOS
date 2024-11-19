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

<!-- TABLE OF CONTENTS -->
<details>
  <summary>Table of Contents</summary>
  <ol>
    <li>
      <a href="#-about-the-shell">About the Shell</a>
    </li>
    <li>
      <a href="#-file-changes-overview">File Changes Overview</a>
      <ul>
        <li><a href="#-files-added">Files Added</a></li>
        <li><a href="#-files-modified">Files Modified</a></li>
      </ul>
    </li>
    <li>
        <a href="#-synopsis">Synopsis</a>
        <ul>
          <li><a href="#-options">Options</a></li>
          <li><a href="#-built-in-commands">Built-In Commands</a></li>
          <li><a href="#-history-execution">History Execution</a></li>
          <li><a href="#-i/o-redirection">I/O Redirection</a></li>
          <li><a href="#-job-control">Job Control</a></li>
        </ul>
    </li>
    <li>
      <a href="#-examples">Examples</a>
        <ul>
          <li><a href="#options">Options</a></li>
          <li><a href="#basic-unix/linux-commands">Basic Unix/Linux Commands</a></li>
          <li><a href="#history-executions">History Executions</a></li>
          <li><a href="#scripting-support">Scripting Support</a></li>
          <li><a href="#i/o-redirection">I/O Redirection</a></li>
          <li><a href="#job-control">Job Control</a></li>
        </ul>
    </li>
    <li>
      <a href="#-special-features">Special Features</a>
    </li>
    <li>
      <a href="#-tests">Tests</a>
    </li>
    <li>
      <a href="#-running-the-program">Running the Program</a>
    </li>
  </ol>
</details>

<!-- ABOUT THE SHELL -->
## 📖 About the Shell
<h3 align="center"><b>Tosh - The Operating Shell</b></h3>
Tosh is a custom command-line shell with features such as command execution, built-in commands, I/O redirection, and background job management, providing an interactive user experience. Enhanced functionality with scripting mode (including shebang support), dynamic prompts displaying exit status, command history, and current working directory. Integrating custom path execution for seamless interaction with Unix-like environments. Tosh Implements advanced features such as executable script support and background job handling, optimizing user interaction with the operating system.

<br />

<!-- FILE CHANGES OVERVIEW -->
## 📁 File Changes Overview
<!-- FILES ADDED -->
### **📁➕ Files Added**
user:
- **tosh.c**

<!-- FILES MODIIFIED -->
### **📁🖋️ Files Modified**
kernel:
- **fcntl.h**
- **fs.c** 
- **sysfile.c**
- **defs.h**
- **syscall.c**
- **syscall.h**

user:
- **user.h**
- **usys.pl**

<br />

<!-- SYNOPSIS -->
## 📜 Synopsis
  ```sh
  tosh [options] [script]
  ```

  **⚠️ Note:** Parameters in square brackets are optional.

<!-- OPTIONS -->
### **⚙️ Options**
| Option               | Description                            | 
| :---------:          | :-----------:                          |
| -v, --version        | Displays shell version                 |
| --about              | Displays an about section of the shell | 
| --license            | Shows the license                      |   
| -lc, --list-commands | Shows a list of all the commands       |
| -p, --private        | Private mode is turned on              |

<!-- BUILT-IN COMMANDS -->
### **🔧 Built-In Commands**
| Command        | Description                                                                  | 
| :---------:    | :-----------:                                                                |
| cat filename   | Displays file content                                                        |
| echo [string]  | Displays text to the terminal                                                | 
| ls [/dir]      | List directory contents                                                      |   
| cd /dir        | Change the current directory                                                 | 
| mkdir dir_name | Make directories                                                             |
| pwd            | Prints the working directory                                                 |
| wc filename    | Shows the number of lines, words, and bytes in a file                        |
| history [-t]   | Display the history list of the last 100 commands with their command numbers | 
| test           | Evaluates a conditional expression                                           |
| uname [-a]     | Displays system information                                                  |
| joke           | Tells a programming joke                                                     | 
| fact           | Shares a fun programming-related fact                                        |
| quote          | Provides a programming-related quote                                         |
| menu           | Display the main menu                                                        |
| ref            | Provides a list of commands with their descriptions                          |
| reset          | Resets the shell                                                             |
| exit           | Exit the shell                                                               | 

<!-- HISTORY EXECUTION -->
### **⏳ History Execution**
| Command     | Description                                                        | 
| :---------: | :-----------:                                                      |
| !num        | A history execution that will re-run command number num            | 
| !!          | A history execution that re-runs the last command that was entered |
| !prefix     | A history execution that re-runs the last command with that prefix | 

<!-- I/O REDIRECTION -->
### **🔀 I/O Redirection**
| Command              | Description                                                              | 
| :---------:          | :-----------:                                                            |
| command1 \| command2 | Allows the output of one command serves as the input for another command | 
| command > filename   | Redirects the output of a command to a file                              | 
| filename < command   | Redirects the content of a file to be input for a command                | 
| command >> filename  | Append the output of a command to the end of a file                      |

<!-- JOB CONTROL -->
### **👷‍♂️ Job Control**
| Command     | Description                      | 
| :---------: | :-----------:                    |
| command &   | Runs a command in the background |  

<br />

<!-- EXAMPLES -->
## **📋 Examples**
 **Options**
 ```sh
 tosh -v
 tosh --about
 tosh -p
 ```

 **Basic Unix/Linux Commands**
 ```sh
 cat append.txt 
 echo "mwahahaha"       
 mkdir dog         
 cd dog        
 pwd
 ```

  **History Executions**
 ```sh
 !!
 !23
 !l
 ```
 
 **Scripting Support**
 ```sh
 tosh history_test1.sh
 ```

 **I/O Redirection**
 ```sh
 cat input.txt | toLower | fnr the teh a 4 e 3 i ! | fnr l 1 o 0 s 5 > output.txt
 toLower < input.txt >> append.txt
 ```

 **Job Control**
 ```sh
 cat README.md &
 toLower < input.txt >> append.txt &
 ```

<br />

 <!-- SPECIAL FEATURES -->
## **✨ Special Features**
| Feature                              | Description                                                                                                        |  
| :---------:                          | :-----------:                                                                                                      |
| Splash Screen                        | Displays a welcome screen with the shell's name and key features                                                   |
| Color Coding                         | Uses different colors to differentiate between commands, output, and errors for improved readability and usability | 
| Text Formatiing                      | Add text styling like italics, underline, and bold for clearer, more improved terminal display                     |   
| Case Insensitive (Built-In Commands) | Allows commands to be entered in any letter case for a more user-friendly experience                               |
| Private Mode                         | Empowers users to interact with the shell incognito, ensuring commands are not recorded in the history             | 

<br />

<!-- TEST -->
## **🧑‍🔬 Tests**
| Command            | Description                                                                        | 
| :---------:        | :-----------:                                                                      | 
| tosh tosh_test1.sh | Runs a series of automated tests to validate the behavior of the built-in commands |                  
| tosh tosh_test2.sh | Similar test series for built-in commands                                          | 
| tosh tosh_test3.sh | Similar test series for built-in commands                                          | 
| tosh tosh_test4.sh | Similar test series for built-in commands                                          |

<br />

<!-- RUNNING THE PROGRAM -->
## **▶️ Running the Program**
1. **Build FogOS**
   ```sh
   make
   ```
2. **Run FogOS with QEMU**
   ```sh
   make qemu
   ```
3. **Run Tosh**
   ```sh
   tosh [options] [script]
   ```
