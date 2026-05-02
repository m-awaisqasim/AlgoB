# Module D: Performance UI & Chatbot - Dry Run Analysis

This document provides a detailed, line-by-line dry run of **Module D** from the `main.cpp` file. This module handles the interpretation of backtest results and provides an interactive terminal interface for the user.

---

## 1. `setModuleCData(double* results)`
**Lines 276–281**
This function acts as a data mapper between the computational engine (Module C) and the user interface.

- **Purpose:** Unpacks the results array into global variables.
- **Mapping:**
  - `results[0]` → `capital`
  - `results[1]` → `finalVal`
  - `results[2]` → `totalReturn`
  - ... and so on for all 11 metrics.
- **Logic Highlight (Line 280):** 
  - `wins = (int)((winRate / 100.0) * totalTrades + 0.5)`
  - The `+ 0.5` is used to ensure that when the double is cast to an integer, it rounds to the nearest whole number instead of always rounding down (floor).

---

## 2. `formatPKR(double val)`
**Lines 283–287**
A formatting utility to improve readability of large financial figures.

- **Logic:**
  - **Millions (Line 284):** If value $\ge 1,000,000$, it returns the value divided by $10^6$ followed by "M".
  - **Thousands (Line 285):** If value $\ge 1,000$, it returns the value divided by $1,000$ followed by "K".
  - **Standard (Line 286):** Returns the raw integer value as a string.
- **Example:** `500000` becomes `"500K"`.

---

## 3. `displaySummary()`
**Lines 289–313**
The primary output function for the strategy's final performance.

- **Status Check (Line 290):** Uses a ternary operator to set the strategy status to `"PROFITABLE"` if `totalReturn >= 0`, otherwise `"LOSS MAKING"`.
- **UI Construction (Lines 292-312):** 
  - Uses `cout` with fixed-width formatting (manually aligned with spaces).
  - Integrates `formatPKR()` to display `capital` and `finalVal`.
  - Prints all risk metrics (Drawdown, Sharpe, Profit Factor) and trade statistics.

---

## 4. `showVisualChart()`
**Lines 315–335**
A terminal-based graphical representation of key performance indicators.

- **Data Selection (Lines 316-317):** 
  - Labels: "Returns", "Win Rate", "Safety" (Calculated as $100 - \text{Drawdown}$).
- **Scaling Logic (Line 325):** 
  - `int barLength = (int)(values[i] / 2.0);`
  - This scales a percentage (0-100) down to a 0-50 character width to fit standard terminal windows.
- **Pattern Cycling (Line 329):** 
  - `bars[i][j % 4]`
  - Each metric uses a specific character (e.g., `>>>>`, `####`). The modulo operator cycles through the 4-character string to fill the bar.

---

## 5. `findKeyword(string text, string key)`
**Lines 337–347**
A manual substring search algorithm.

- **Validation (Line 338):** Immediately returns `false` if the search key is longer than the input text.
- **Search Logic (Lines 339-346):**
  - **Outer Loop:** Iterates through every possible starting position in `text`.
  - **Inner Loop:** Compares each character of the `key` against the current segment of `text`.
  - **Efficiency:** Uses a `match` boolean that breaks early if a mismatch is found, saving CPU cycles.

---

## 6. `startChatbot(double matrix[MAX_DAYS][5], int& rows)`
**Lines 349–425**
The command-driven interface for post-analysis exploration.

- **Command Normalization (Lines 388-390):**
  - Converts all user input to lowercase by iterating through the string and adding `32` to any character in the `'A'-'Z'` range.
- **Keyword Routing:**
  - **Exit/Quit:** Sets `isActive = false` to terminate the loop.
  - **Advice (Lines 399-406):** Contains a mini "expert system" that analyzes current metrics:
    - If `totalReturn < 0`, it suggests increasing SMA Long.
    - If `winRate < 45`, it suggests increasing the RSI period.
  - **Data Integration (Lines 407-415):** Allows the user to jump back into Data Management (Module A) or Strategy Config (Module B) directly from the chat.
- **Loop Control (Line 416-417):**
  - If the user types "retest", "update", or "run", the function returns `true`.
  - This tells the `main()` function to restart the backtesting process with the new values.
