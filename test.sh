#/**
# * File: test.sh
# * Author: Nino Estrada
# * Date: September 23, 2024
# *
# * Description:
# * This script runs the command 'test' and checks its output
# * against the expected result. It supports multiple test cases.
# *
# * Usage:
# * Compile: make qemu
# * Run: sh test.sh
# *
# * Results:
# * The expected results should be 0 for true, 1 for false, and 2 for error or syntax error.
# * Note: Everytime there is an error the help text is printing out.
# *
# * Note: This script only runs if you add shell scripting support.
# * https://github.com/USF-OS/FogOS/pull/71
# */


#!/bin/sh


#/**
# * Tests valid number of arguments
# *
# * Test 1: Check if valid number of arguments for any test implementation
# */
echo -------------------- Testing if No Arguments were Provided --------------------
echo Input: test
echo ***** Expected Result *****
echo Result: 2
echo ***** Actual Result *****
test


#/**
# * Tests the help flag
# *
# * Test 1: Check if help flag is outputting the implementations for the "test" command
# */
echo -------------------- Testing Help Flag --------------------
echo Input: test -h
echo ***** Expected Result *****
echo A list of commands with their respective description
echo ***** Actual Result *****
test -h


#/**
# * Tests the file flags
# *
# * Test 1: Check if file is directory (output=1)
# * Test 2: Check if file exist (output=0)
# * Test 3: Check if non-existing file exist (output=1)
# * Test 4: Check if empty file exist (output=0)
# * Test 5: Check if file is a regular file (output=0)
# * Test 6: Check if valid file flag (output=2)
# * Test 7: Check if there are enough arguments (output=2)
# */
echo -------------------- Testing File Flags --------------------
echo ---------- "-d" ----------
echo Input: test -d cat
echo ***** Expected Result *****
echo Result: 1
echo ***** Actual Result *****
test -d cat

echo ----- "-e" -----
echo Input: test -e cat
echo ***** Expected Result *****
echo Result: 0
echo ***** Actual Result *****
test -e cat

echo ----- "-e for non-existing" -----
echo Input: test -e dog
echo ***** Expected Result *****
echo Result: 1
echo ***** Actual Result *****
test -e dog

echo ----- "-e for empty file" -----
echo > empty.txt
echo Input: test -e empty.txt
echo ***** Expected Result *****
echo Result: 0
echo ***** Actual Result *****
test -e empty.txt

echo ---------- "-f" ----------
echo Input: test -f cat
echo ***** Expected Result ***** 
echo Result: 0
echo ***** Actual Result *****
test -f cat

echo ---------- invalid flag ----------
echo Input: test -g cat 
echo ***** Expected Result *****
echo Result: 2
echo ***** Actual Result *****
test -g cat

echo ---------- not enough arguments ----------
echo Input: test -f
echo ***** Expected Result *****
echo Result: 2
echo ***** Actual Result *****
test -f


#/**
# * Tests the string expressions
# *
# * Test 1: Check if two strings are equal (output=0)
# * Test 2: Check if two strings are equal (output=1)
# * Test 3: Check if two strings are not equal (output=1)
# * Test 4: Check if two strings are not equal (output=0)
# * Test 5: Check if valid string operator (output=2)
# * Test 6: Check strings with spaces are equal (output=0)
# * Test 7: Check strings with spaces are equal (output=1)
# * Test 8: Check strings with spaces are not equal (output=0)
# * Test 9: Check strings with spaces are not equal (output=1)
# * Test 10: Check if there are enough arguments (output=2)
# */
echo -------------------- Testing String Operators --------------------
echo ---------- "=" ----------
echo Input: test "hello" = "hello"
echo ***** Expected Result *****
echo Result: 0
echo ***** Actual Result *****
test "hello" = "hello"

echo ----- "=" -----
echo Input: test "hello" = "hell"
echo ***** Expected Result *****
echo Result: 1
echo ***** Actual Result *****
test "hello" = "hell"

echo ---------- "!=" ----------
echo Input: test "hello" != "hello"
echo ***** Expected Result *****
echo Result: 1
echo ***** Actual Result *****
test "hello" != "hello"

echo ---------- "!=" ----------
echo Input: test "hello" != "hell"
echo ***** Expected Result *****
echo Result: 0
echo ***** Actual Result *****
test "hello" != "hell"

echo ---------- invalid operator ----------
echo Input: test "hello" += "hello"
echo ***** Expected Result *****
echo Result: 2
echo ***** Actual Result *****
test "hello" += "hello"

echo ---------- strings with spaces: "=" ----------
echo Input: test "hi hello" = "hi hello"
echo ***** Expected Result *****
echo Result: 0
echo ***** Actual Result *****
test "hi hello" = "hi hello"

echo ---------- strings with spaces "=" ----------
echo Input: test "hi hello" = "hi hell"
echo ***** Expected Result *****
echo Result: 1
echo ***** Actual Result *****
test "hi hello" = "hi hell"

echo ---------- strings with spaces "!=" ----------
echo Input: test "hi hello" != "hi hell"
echo ***** Expected Result *****
echo Result: 0
echo ***** Actual Result *****
test "hi hello" != "hi hell"

echo ---------- strings with spaces "!=" ----------
echo Input: test "hi hello" != "hi hello"
echo ***** Expected Result *****
echo Result: 1
echo ***** Actual Result *****
test "hi hello" != "hi hello"

echo ---------- no closing quote ----------
echo Input: test "hi hi" != "hi hi
echo ***** Expected Result *****
echo Result: 2
echo ***** Actual Result *****
test "hi hi" != "hi hi

echo ---------- not enough arguments ----------
echo Input: test "hell"
echo ***** Expected Result *****
echo Result: 2
echo ***** Actual Result *****
test "hell"


#/**
# * Tests the number expressions
# *
# * Test 1: Check if num1 is less than num2 (output=0)
# * Test 2: Check if num1 is less than num2 (output=1)
# * Test 3: Check if num1 is greater than num2 (output=0)
# * Test 4: Check if num1 is greater than num2 (output=1)
# * Test 5: Check if valid number operator (output=2)
# * Test 6: Check if there are enough arguments (output=2)
# * Test 7: Check if argument is a valid number to compare: num1 (output=2)
# * Test 8: Check if argument is a valid number to compare: num2 (output=2)
# */
echo -------------------- Testing Number Operators --------------------
echo ---------- "-lt" ----------
echo Input: test 23 -lt 32
echo ***** Expected Result *****
echo Result: 0
echo ***** Actual Result *****
test 23 -lt 32

echo ----- "-lt" -----
echo Input: test 32 -lt 23
echo ***** Expected Result *****
echo Result: 1
echo ***** Actual Result *****
test 32 -lt 23

echo ---------- "-gt" ----------
echo Input: test 32 -gt 23
echo ***** Expected Result *****
echo Result: 0
echo ***** Actual Result *****
test 32 -gt 23

echo ---------- "-gt" ----------
echo Input: test 23 -gt 32
echo ***** Expected Result *****
echo Result: 1
echo ***** Actual Result *****
test 23 -gt 32

echo ---------- invalid operator ----------
echo Input: test 23 -g 32
echo ***** Expected Result *****
echo Result: 2
echo ***** Actual Result *****
test 23 -g 32

echo ---------- not enough arguments ----------
echo Input: test 23
echo ***** Expected Result *****
echo Result: 2
echo ***** Actual Result *****
test 23

echo ---------- valid number to compare ----------
echo Input: test 23a5 -lt 45
echo ***** Expected Result *****
echo Result: 2
echo ***** Actual Result *****
test 23a5 -lt 45

echo ---------- valid number to compare ----------
echo Input: test 23 -lt 45ed7
echo ***** Expected Result *****
echo Result: 2
echo ***** Actual Result *****
test 23 -lt 45ed7


#/**
# * Tests the file comparisons
# *
# * Test 1: Check if file exist and refers to the same file (output=0)
# * Test 2: Check if there are enough arguments (output=2)
# */
echo -------------------- Testing File Comparisons --------------------
echo ---------- "-ef" ----------
echo Input: test cat -ef cat
echo ***** Expected Result *****
echo Result: 0
echo ***** Actual Result *****
test cat -ef cat

echo ---------- not enough arguments ----------
echo Input: test cat -ef
echo ***** Expected Result *****
echo Result: 2
echo ***** Actual Result *****
test cat -ef
