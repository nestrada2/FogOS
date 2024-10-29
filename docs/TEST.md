<!-- COMMAND LOGO -->
<div align="center">
    🧪
    <h3 align="center" style="font-size: 2.1em; font-weight: bolder;">test</h3>

  <p align="center">
    Condition Evaluation Tool
    <br />
    <br />
    <a href="https://github.com/nestrada2/FogOS/issues/new?labels=bug&template=bug-report.md">Report Bug</a>
    ·
    <a href="https://github.com/nestrada2/FogOS/issues/new?labels=enhancement&template=feature-request.md">Request Feature</a>
  </p>
</div>

<!-- ABOUT THE COMMAND -->
## 📖 About the Command
The `test` command evaluates a conditional expression. It is an if statement that will return zero (true), 1 (false), or greater than 1 (error). The `test` command will check if any flags were inputted and, based on the expression, will return a truthy or falsy value. 

<!-- FILES ADDED -->
### **📁➕ Files Added**
user:
- test.c (new: call sys_open, pass a path to that and that will give us a file descriptor. With the file descriptor we call sys_fstat, and pass in the file descriptor, and a pointer to an empty stat struct which will be populated with the file's metadata)

<!-- FEATURES -->
### **✨ Expressions Implemented in Test**
| Command         | Description                                                           | 
| :---------:     | :-----------:                                                         | 
| -d file         | True if file exists and is a directory.                               | 
| -e file         |   True if file exists (regardless of type).                           |   
| -f file         |  True if file exists and is a regular file.                           | 
| file1 -ef file2 | True if file1 and file2 exist and refer to the same file.             | 
| s1 = s2         |  True if the strings s1 and s2 are identical.                         | 
|  s1 != s2       | True if the strings s1 and s2 are not identical.                      | 
| n1 -gt n2       |  True if the integer n1 is algebraically greater than the integer n2. | 
|  n1 -lt n2      | True if the integer n1 is algebraically less than the integer n2.     | 

<!-- EXAMPLES -->
### **📋 Examples**
 **Example 1: File Analysis**
 * `test -f cat`
 * Expected Output: 0 
 
 **Example 2: String Operators**
 *  `test "hello" = "hell"`
 * Expected Output: 1
 
 **Example 3: Number Operators**
 * `test 23 -gt 32 `
 * Expected Output: 1 

<!-- TEST -->
### **🔬 Tests**
| Command   | Description | 
| :---------: | :-----------:   | 
| sh test.sh      | Runs a series of automated tests to validate the behavior of the program.   | 

**⚠️ Note:** The test script only runs if you add shell scripting support.
