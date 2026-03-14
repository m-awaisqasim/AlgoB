# Product Requirements Document (PRD)

## Algorithmic Trading Strategy Backtester with Rule-Based Chatbot

| **Document Version** | 1.0 |
|----------------------|-----|
| **Date**             | February 20, 2026 |
| **Author**           | [Your Name] |
| **Course**           | Programming for Business (C++) |
| **Instructor**       | [Instructor's Name] |
| **Project Type**     | Semester Project |

---

## 1. Introduction

### 1.1 Purpose

The purpose of this document is to define the requirements for the **Algorithmic Trading Strategy Backtester with Rule-Based Chatbot** project. This application will allow users to load historical stock market data, run backtests of a moving average crossover trading strategy, view performance metrics, and interact with a simple rule-based chatbot to query the results.

### 1.2 Scope

The project will be developed entirely in **procedural C++**, using only the topics covered in the course syllabus: arrays, functions, pointers, dynamic memory allocation, strings (char arrays), and file I/O. No classes, object-oriented programming, or external libraries (except standard C libraries) will be used. The system will operate entirely via a console interface.

### 1.3 Definitions and Acronyms

| Term | Definition |
|------|------------|
| **Backtest** | Simulation of a trading strategy on historical data. |
| **OHLC** | Open, High, Low, Close – daily price data. |
| **SMA** | Simple Moving Average. |
| **EMA** | Exponential Moving Average. |
| **RSI** | Relative Strength Index. |
| **Sharpe Ratio** | Risk-adjusted return metric. |
| **Drawdown** | Peak-to-trough decline in portfolio value. |

---

## 2. Overall Description

### 2.1 Product Perspective

This is a standalone console application. It reads historical price data from a CSV file, processes it, and outputs results to the console. The user interacts via keyboard input.

### 2.2 User Characteristics

The target users are students and individuals interested in algorithmic trading. They are expected to have basic familiarity with financial concepts but no programming expertise is required to operate the application.

### 2.3 Operating Environment

- **Platform:** Windows / Linux / macOS (console application)
- **Compiler:** Any C++ compiler supporting C++98/11 (e.g., GCC, Clang, MSVC)
- **No GUI, no web interface – purely text-based.**

### 2.4 Design and Implementation Constraints

- **No classes, no OOP.**
- **No STL containers** (e.g., `std::vector`, `std::string`). Use C-style arrays and `char` arrays.
- **No external libraries** (except standard C libraries: `<cstdio>`, `<cstring>`, `<cmath>`).
- **Memory management:** Use static arrays with predefined maximum sizes, or optionally dynamic allocation with `new`/`delete` for variable-sized data (e.g., trade list). If dynamic, must be freed properly.
- **All code must be procedural** – organized into functions, grouped by module.

### 2.5 Assumptions and Dependencies

- Historical price data is provided in CSV format with the following columns: Date, Open, High, Low, Close, Volume.
- The CSV file contains at least 200 days of data to produce meaningful backtest results.
- User input for chatbot queries will be short, simple English phrases.
- The application will not connect to the internet or any live data feed.

---

## 3. Functional Requirements

The product is divided into two main modules: **Backtester Module** and **Chatbot Module**.

### 3.1 Backtester Module

#### FR1: Data Loading

- The program shall read a CSV file specified by the user (hardcoded or via input).
- It shall parse and store the following fields for each day:
  - Date (as `char[11]` array, format YYYY-MM-DD)
  - Open, High, Low, Close (as `float`)
  - Volume (as `long`)
- Data shall be stored in a global array of `struct Bar` with a predefined maximum size (e.g., 1000 days). The actual number of days loaded is tracked.

#### FR2: Technical Indicator Calculation

- The program shall provide functions to calculate:
  - Simple Moving Average (SMA) for a given period and day.
  - Exponential Moving Average (EMA) – requires previous EMA value.
  - Relative Strength Index (RSI) – optional, can be added as an extra.
- All indicator functions shall operate on the global price array and return a `float`.

#### FR3: Signal Generation

- The default strategy shall be **Moving Average Crossover** (short period = 20 days, long period = 50 days).
- For each day (starting after the long period), generate a signal:
  - **BUY** when the short SMA crosses above the long SMA.
  - **SELL** when the short SMA crosses below the long SMA.
  - **HOLD** otherwise.
- Signals shall be stored in a global integer array `signals[days]` with values: 1 (buy), -1 (sell), 0 (hold).

#### FR4: Trade Simulation

- Starting with an initial capital (e.g., $10,000), simulate trades based on signals.
- **Buying:** Use all available cash to buy as many shares as possible (integer shares, fractional not allowed). Record entry date, price, shares.
- **Selling:** Sell all held shares at current price. Record exit date, price, profit/loss.
- Maintain a global array of `struct Trade` to store completed trades.
- Maintain a global array of `struct PortfolioSnapshot` to record portfolio value at each day (cash + shares * close price).

#### FR5: Performance Metrics Calculation

- After simulation, compute the following metrics:
  - **Total Return (%)**
  - **Annualized Return (%)** (assuming 252 trading days per year)
  - **Sharpe Ratio** (annualized, using daily returns)
  - **Maximum Drawdown (%)**
  - **Number of Trades**
  - **Winning Trades / Losing Trades**
  - **Win Rate (%)**
  - **Average Win ($)**
  - **Average Loss ($)**
  - **Profit Factor** (gross profit / gross loss)
- Store all metrics in a global `struct Metrics` variable.

#### FR6: Display Summary

- After backtest, the program shall display a summary table with key metrics in a formatted console output.

### 3.2 Chatbot Module

#### FR7: Interactive Query Loop

- After backtest, the program shall enter an interactive mode where the user can type queries.
- The loop continues until the user types `exit`.
- Prompt character: `>`

#### FR8: Query Parsing (Keyword Matching)

- User input shall be converted to lowercase for easier matching.
- The program shall check for predefined keywords using `strstr` (case-insensitive after conversion).
- Supported queries (minimum set):
  - `"total return"`, `"profit"` → display total and annual return.
  - `"sharpe"` → display Sharpe ratio.
  - `"drawdown"`, `"max drawdown"` → display maximum drawdown.
  - `"losing trades"`, `"losses"` → list all losing trades with details.
  - `"winning trades"`, `"wins"` → list all winning trades.
  - `"trade count"`, `"number of trades"` → display total trades.
  - `"win rate"` → display win rate.
  - `"help"` → display list of available queries.
  - `"exit"` → exit chat.

#### FR9: Response Generation

- For each query, the program shall retrieve the relevant data from global structures and print a formatted answer.
- Responses should be concise and informative.

#### FR10: Unrecognized Queries

- If no keyword matches, the program shall print a friendly message: `"Sorry, I don't understand. Type 'help' for options."`

---

## 4. Non-Functional Requirements

### NFR1: Performance

- Loading a CSV file with up to 1000 rows should complete within 2 seconds.
- Backtest simulation (looping through days) should complete within 1 second for 1000 days.
- Chatbot responses must be instantaneous (no noticeable delay).

### NFR2: Usability

- The console output shall be well-formatted with clear headings, tables, and separators.
- Error messages shall be descriptive (e.g., "Failed to open file", "Insufficient data for selected period").
- The chatbot shall handle both uppercase and lowercase input.

### NFR3: Reliability

- The program shall validate input data (e.g., negative prices, missing values) and handle errors gracefully (print error and exit or prompt again).
- No memory leaks: all dynamically allocated memory shall be freed.

### NFR4: Maintainability

- Code shall be organized into separate `.c`/`.h` files for each module (data_loader.h/c, indicators.h/c, backtester.h/c, chatbot.h/c, main.c).
- Functions shall be small and focused (single responsibility).
- Global variables shall be clearly documented.

---

## 5. Technical Stack

| Component | Technology |
|-----------|------------|
| Language | C++ (C++98/11 style, no classes) |
| Compiler | GCC / Clang / MSVC |
| Standard Libraries | `<cstdio>`, `<cstring>`, `<cmath>`, `<cctype>` |
| File Format | CSV (comma-separated values) |
| Build System | Manual compilation (g++ *.c -o backtester) |

---

## 6. System Architecture

### 6.1 High-Level Block Diagram

```
┌─────────────────┐     ┌─────────────────┐     ┌─────────────────┐
│   Data Loader   │────▶│   Backtester    │────▶│   Chatbot       │
│   (CSV parser)  │     │   Engine        │     │   Interface     │
└─────────────────┘     └─────────────────┘     └─────────────────┘
         │                       │                        │
         ▼                       ▼                        ▼
    ┌─────────────────────────────────────────────────────────┐
    │                 Global Data Structures                   │
    │  - price_data[] (Bar)                                    │
    │  - trades[] (Trade)                                      │
    │  - equity_curve[] (PortfolioSnapshot)                    │
    │  - metrics (Metrics)                                     │
    └─────────────────────────────────────────────────────────┘
```

### 6.2 Data Flow

1. User runs the program (optionally provides CSV filename).
2. Data Loader reads CSV, populates `price_data`.
3. Backtester computes indicators, generates signals, simulates trades, fills `trades`, `equity_curve`, and `metrics`.
4. Backtester prints summary.
5. Chatbot module enters interactive loop, reading user queries and accessing global data to answer.

---

## 7. Data Structures

All structures defined in `structures.h`.

```c
// structures.h
#ifndef STRUCTURES_H
#define STRUCTURES_H

#define MAX_DAYS 1000
#define MAX_TRADES 500

struct Bar {
    char date[11];      // YYYY-MM-DD
    float open;
    float high;
    float low;
    float close;
    long volume;
};

struct Trade {
    char entry_date[11];
    char exit_date[11];
    float entry_price;
    float exit_price;
    int shares;
    float profit;       // in dollars
    float profit_pct;
};

struct PortfolioSnapshot {
    char date[11];
    float cash;
    int shares_held;
    float portfolio_value;
};

struct Metrics {
    float total_return;
    float annual_return;
    float sharpe_ratio;
    float max_drawdown;
    int total_trades;
    int winning_trades;
    int losing_trades;
    float win_rate;
    float avg_win;
    float avg_loss;
    float profit_factor;
};

#endif
```

Global variables (declared in `globals.h` with `extern`, defined in `main.c`):

```c
// globals.h
#ifndef GLOBALS_H
#define GLOBALS_H

#include "structures.h"

extern Bar price_data[MAX_DAYS];
extern int days_count;

extern Trade trades[MAX_TRADES];
extern int trade_count;

extern PortfolioSnapshot equity_curve[MAX_DAYS];
extern int snapshots_count;

extern Metrics perf_metrics;

#endif
```

---

## 8. Module Descriptions

### 8.1 Data Loader Module (`data_loader.c`, `data_loader.h`)

- `int load_csv(const char* filename)`: Reads CSV, populates `price_data` and sets `days_count`. Returns 0 on success, -1 on error.
- `void print_data_sample(int n)`: (optional) prints first n rows for verification.

### 8.2 Indicators Module (`indicators.c`, `indicators.h`)

- `float sma(int day, int period)`: Returns SMA for given day.
- `float ema(int day, int period, float prev_ema)`: Returns EMA (requires previous value).
- `float rsi(int day, int period)`: Calculates RSI (optional).

### 8.3 Backtester Engine (`backtester.c`, `backtester.h`)

- `void generate_signals(int short_period, int long_period)`: Populates signals array (global static).
- `void run_backtest(float initial_capital)`: Simulates trades, fills `trades` and `equity_curve`.
- `void compute_metrics(float initial_capital)`: Calculates all performance metrics.
- `void print_summary()`: Prints formatted summary table.

### 8.4 Chatbot Module (`chatbot.c`, `chatbot.h`)

- `void chat_loop()`: Main interactive loop.
- `void process_query(const char* query)`: Parses query and calls appropriate response function.
- `void to_lower(char* str)`: Converts string to lowercase.
- Response functions: `print_total_return()`, `print_sharpe()`, `print_max_drawdown()`, `print_losing_trades()`, `print_winning_trades()`, `print_trade_count()`, `print_win_rate()`, `print_help()`.

### 8.5 Main Module (`main.c`)

- Orchestrates the flow:
  - Load data.
  - Run backtest.
  - Print summary.
  - Enter chat loop.
- Handles command-line arguments (optional filename).

---

## 9. User Interface

### 9.1 Console Output Style

- Use box-drawing characters (`╔`, `╗`, `║`, `═`) for headers and tables.
- Use consistent spacing and alignment.
- Use symbols: `✓`, `⚠️`, `ℹ️` for visual cues (optional, can use ASCII alternatives).

### 9.2 Sample Output (as previously designed)

```
╔══════════════════════════════════════════════════════════════════╗
║           ALGORITHMIC TRADING BACKTESTER - RESULTS              ║
║           =====================================                  ║
║ Strategy: MA Crossover (20/50) | AAPL 2024                      ║
║ Initial Capital: $10,000                                         ║
╚══════════════════════════════════════════════════════════════════╝

TRADE LOG:
┌──────┬────────────┬────────┬──────────┬────────┬──────────┬─────────┐
│ Trade│ Date       │ Action │ Price    │ Shares │ Value    │ P&L     │
├──────┼────────────┼────────┼──────────┼────────┼──────────┼─────────┤
│ 1    │ 2024-02-15 │ BUY    │ $182.50  │ 50     │ $9,125   │ --      │
│ 2    │ 2024-03-22 │ SELL   │ $195.30  │ 50     │ $9,765   │ +$640   │
...

PERFORMANCE METRICS:
┌──────────────────────────────┬─────────────────────────────────┐
│ Metric                       │ Value                           │
├──────────────────────────────┼─────────────────────────────────┤
│ Total Return                 │ 28.47%                          │
│ Annualized Return            │ 14.2%                           │
│ Sharpe Ratio                 │ 1.24                            │
│ Maximum Drawdown             │ 8.32%                           │
│ Total Trades                 │ 23                              │
│ Win Rate                     │ 58.3%                           │
│ Profit Factor                │ 1.85                            │
└──────────────────────────────┴─────────────────────────────────┘

Chat mode activated. Type 'help' for commands.

> show losing trades
Losing trades (10):
  2024-03-15 to 2024-04-10: loss $342.50 (-4.2%)
  2024-05-22 to 2024-06-05: loss $665.20 (-6.1%)
...

> exit
Goodbye!
```

### 9.3 Input Handling

- User inputs for chatbot are read via `fgets()`.
- Commands are case-insensitive.
- Empty input is ignored (prompt again).

---

## 10. Assumptions and Dependencies

- The CSV file exists and is correctly formatted.
- Dates are in YYYY-MM-DD format.
- There are no missing or invalid data points (e.g., zero prices). If there are, the program may crash – validation is minimal.
- The user will not enter extremely long queries (buffer size 256 is sufficient).
- The program does not need to support multiple stocks simultaneously (only one at a time).

---

## 11. Constraints

- Maximum number of days: 1000 (static array). If more needed, can increase constant.
- Maximum number of trades: 500.
- No support for short selling (only long positions).
- No transaction costs or slippage (simplified model).
- Only one strategy implemented (MA crossover); others can be added later.

---

## 12. Future Enhancements

- Add more strategies (RSI, Bollinger Bands) selectable via menu.
- Allow user to input parameters (short/long periods) interactively.
- Implement transaction costs and slippage for realism.
- Export results to CSV file.
- Add more chatbot queries (e.g., "plot equity curve" – simple ASCII chart).
- Support multiple stocks and portfolio-level backtesting.

---

## 13. Appendices

### Appendix A: CSV Format Example

```
Date,Open,High,Low,Close,Volume
2024-01-02,185.50,187.00,184.25,186.75,85000000
2024-01-03,186.50,188.20,185.80,187.90,92000000
...
```

### Appendix B: Glossary of Financial Terms

- **Moving Average Crossover:** A trend-following strategy that generates buy/sell signals when a faster-moving average crosses above/below a slower-moving average.
- **Sharpe Ratio:** (Portfolio Return – Risk-Free Rate) / Standard Deviation of Returns. Measures risk-adjusted return.
- **Drawdown:** The decline from a historical peak in the portfolio value.

---

## 14. Document History

| Version | Date | Author | Changes |
|---------|------|--------|---------|
| 1.0 | 2026-02-20 | [Your Name] | Initial draft |

---

**End of PRD**
