# Module A: Data Loader Explanation

This document provides a line-by-line explanation of the `Module_A.cpp` source code for Assignment 4.

## 1. Setup and Counting (Lines 1–23)
*   **Lines 1–3:** Includes basic libraries. `<iostream>` for console output, `<fstream>` for file handling, and `<string>` for text processing.
*   **Line 7:** Definition of the Data Loader. It uses `&rows` and `&cols` (pass-by-reference) to send the size of the file back to the `main()` function.
*   **Line 10:** Opens the `BitcoinYTD.csv` file using the constructor.
*   **Lines 14–17:** Safety check. If the CSV file isn't in the folder, the function stops and returns `NULL`.
*   **Lines 20–22:** A `while` loop that reads the file line-by-line using `getline` just to count how many days of data are in the file.
*   **Line 23:** Closes the file so we can restart from the beginning later.

## 2. Memory Allocation (Lines 25–28)
*   **Line 25:** **The Double Pointer.** We create an array that holds memory addresses (pointers) for each row.
*   **Lines 26–28:** A loop that goes through each row and creates 5 columns (for Open, High, Low, Close, Volume). This is "Dynamic Allocation."

## 3. Data Parsing (Lines 30–76)
*   **Line 30:** Re-opens the file to actually read the numbers.
*   **Line 35:** Skips the first line of the file (the header labels) so we don't try to turn the word "Date" into a number.
*   **Line 41:** Finds the first comma in the row.
*   **Lines 44–55:** A loop that cuts the line into segments. If `segmentIdx` is 0, we **skip** it (Line 55) because that is the Date string.
*   **Lines 58–72:** It checks if the text segment is numeric. If it is, `stod()` (Line 69) converts the string into a `double` and saves it into the matrix.

## 4. Result and Cleanup (Lines 78–114)
*   **Line 80:** Returns the address of the 2D matrix back to the main program.
*   **Lines 83–97:** A utility function to print a table on the screen for you to verify the data.
*   **Line 102:** **Memory Release.** A loop that deletes every row to prevent the program from hogging your RAM (Memory Leak).
*   **Line 104:** Deletes the main pointer to the array.
*   **Line 108:** The `main()` function ties everything together: Calls the loader -> Displays it -> Cleans it up.

---

### Key Assignment 4 Concepts Used:
1.  **Double Pointers (`double**`):** Used to create a 2D memory structure.
2.  **Dynamic Allocation (`new`):** Used to allocate memory on the heap based on file size.
3.  **Manual Parsing:** Avoids advanced libraries to stay within course scope.
4.  **Memory Management (`delete[]`):** Essential for preventing memory leaks in C++.
