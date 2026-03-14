# Project Proposal

## Algorithmic Trading Strategy Backtester with Chatbot

| **Project Title** | Algorithmic Trading Strategy Backtester with Chatbot |
|---|---|
| **Course** | Programming for Business (C++) |
| **Semester** | Spring 2026 |
| **Instructor** | Muhammad Tehaam |
| **Submission Date** | 28th February 2026 |

---

## Team Members

| **Member** | **Name**          | **Student ID** | **Role**                       |
| :--------- | :---------------- | :------------- | :----------------------------- |
| Member A   | Sarah Shabbir     | 25i-5554       | Data Loader & Structures       |
| Member B   | Syed Ayan Ali     | 25i-5639       | Indicators & Signal Generation |
| Member C   | Syed Hamza        | 25i-5558       | Backtest Engine & Metrics      |
| Member D   | Awais Qasim       | 25i-5647       | Reporting & Chatbot            |

---

## 1. What Are We Building?

We are building a **C++ program that test stock/crypto trading strategies** using historical data:

- You start with **$10,000 fake (only numbers) money**.
- The program looks at **past stock/crypto prices** (from a CSV file).
- It follows a simple rule to decide when to **buy** and when to **sell**.
- At the end, it shows you **how much money you made or lost**.
- You can also **ask questions** about the results, like "How many losing trades did I have?" and the program will answer.

This is called a **backtester** – we test a trading strategy on old data to see if it would have worked.

---

## 2. Why Are We Building It?

- To practice **C++ programming** using what we will learn in course (arrays, functions, pointers, file reading, etc.).
- To understand how **trading strategies** work in the real world.
- To create something **useful and interesting** that we can show in our portfolio.
- To work as a **team** and learn how to divide a project into smaller pieces.

---

## 3. What Will the Program Do?

Here is what the user will experience:

1. **Run the program** – it loads stock/crypto data from a file.
2. **Backtest runs automatically** – the program simulates trading for the entire period.
3. **See results** – a summary shows total profit, number of trades, win rate, etc.
4. **Chat mode starts** – the user can type questions and get answers.
5. **Type 'exit' to quit** – program ends.

---

## 4. How Will We Build It? (The Simple Version)

We will split the work into **4 modules**, one for each team member. Each part is like a puzzle piece that fits together.

### Module 1: Data Loader – *Sarah Shabbir*

**Job:** Read the stock/crypto data from a CSV file and store it neatly.

- Open a CSV file (like Excel) that contains dates and prices.
- Read each line and save the information OHLCV (date, open price, high price, low price, close price, volume) into an array.
- Make sure the data is correct (no missing values, dates in order).
- Provide helper functions so other members can easily get a price for any day.

---

### Module 2: Indicators & Signals – *Syed Ayan Ali*

**Job:** Look at the prices and decide when to buy and sell.

- Calculate **moving averages (Simple / Exponential)** – the average price over the last 20 days and last 50 days.
- Apply the **trading rule**:
  - When the 20-day average goes **above** the 50-day average → **BUY**.
  - When the 20-day average goes **below** the 50-day average → **SELL**.
  - Otherwise → **HOLD**.
- Store these decisions (buy, sell, hold) in an array for each day.

**NOTE:** We can make it dynamic by adding more strategies like RSI, MACD, etc.

---

### Module 3: Backtest Engine – *Syed Hamza*

**Job:** Simulate the trading and calculate how much money we made.

- Start with $10,000 cash and 0 shares.
- Go day by day:
  - If the signal says BUY and we have no shares → buy as many as we can with our cash.
  - If the signal says SELL and we have shares → sell all of them.
  - Keep track of cash, shares, and portfolio value each day.
- Record every completed trade (entry date, exit date, buy price, sell price, profit).
- At the end, calculate performance numbers:
  - **Total return** (how much money we made in $ / %)
  - **Sharpe ratio** (a measure of risk – higher is better)
  - **Maximum drawdown** (biggest drop from a peak)
  - **Win rate** (% of trades that made money)
  - And more...

---

### Module 4: Reporting & Chatbot – *Awais Qasim*

**Job:** Show the results nicely and let the user ask questions.

- Print a **summary table** with all the important numbers (like a report card).
- Use lines and boxes (`╔══╗`) to make it look professional.
- Create a **chat loop** that asks the user to type and waits for their question.
- Recognize keywords like "total return", "losing trades", "help", etc.
- For each question, print the answer by looking at the data from Module 3.
- If the question is not recognized, say "Sorry, I don't understand. Type 'help'."

**NOTE:** But, if possible we make *LLM model* to answer questions.

---

## 5. How the Modules Fit Together

```
Module A (Data Loader) 
    ↓
Module B (Signals)  – needs price data from Sarah
    ↓
Module C (Backtest) – needs price data from Sarah and signals from Ayan
    ↓
Module D (Chatbot)  – needs trade data and metrics from Hamza
```

We will work **in parallel** by using temporary fake data until the real modules are ready. Then we connect them one by one.

---

## 6. What We Will Learn

- How to **work in a team** on a real project.
- How to **split a problem** into smaller pieces.
- How to **read from files** and process data.
- How to **write functions** that do specific jobs.
- How to **use arrays** to store lots of information.
- How to **make a simple chatbot** that understands keywords.
- How to **present results** in a clean, professional way.

---

## 7. Final Words

This project is challenging but **doable**. Each person has a clear job, and we will help each other when someone gets stuck. By the end, we will Insha'Allah have a working program.

Let's build something awesome with the help of members!

---

## Simple Glossary (if you get confused)

| **Term**           | **Simple Meaning**                                                                 |
| :----------------- | :--------------------------------------------------------------------------------- |
| **Backtest**       | Testing a trading strategy on old data to see if it works before risking real money.                         |
| **Moving Average** | Average price over a certain number of days (smooths out ups and downs).           |
| **Signal**         | A decision: BUY, SELL, or HOLD.                                                    |
| **Sharpe Ratio**   | A number that tells you if profits are worth the risk. Higher is better.           |
| **Drawdown**       | How much your money dropped from its highest point.                                |
| **Win Rate**       | Percentage of trades that made money.                                              |
| **CSV File**       | A simple text file with values separated by commas (like a mini Excel sheet).      |

---
