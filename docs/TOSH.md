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
Tosh is a custom command line shell built for FogOS. 

<!-- FILES ADDED -->
### **📁➕ Files Added**
user:
- tosh.c

<!-- FEATURES -->
### **✨ Commands in Tosh**
| Command        | Description                                                                  | 
| :---------:    | :-----------:                                                                | 
| echo [string]  | Displays text to the terminal                                                | 
| ls [/dir]      | List directory contents                                                      |   
| cd /dir        | Change the current directory                                                 | 
| mkdir dir_name | Make directories                                                             | 
| history        | Display the history list of the last 100 commands with their command numbers | 
| !num           | A history execution that will re-run command number num                      | 
| !!             | A history execution that re-runs the last command that was entered           | 
| exit           | Exit the shell                                                               | 

<!-- EXAMPLES -->
### **📋 Examples**
 **Example 1: The Basic Unix/Linux Commands**
 * `echo "mwahahaha"`
 * Expected Output: mwahahaha 
 
 **Example 2: Scripting Support**
 
 **Example 3: Basic Built In Unix/Linux Commands**

 **Example 4: History**
 * `history `
 * Expected Output:

 **Example 5: Pipe & Redirection**

<!-- TEST -->
### **🧑‍🔬 Tests**
| Command      | Description                                                               | 
| :---------:  | :-----------:                                                             | 
| tosh test.sh | Runs a series of automated tests to validate the behavior of the program. | 
