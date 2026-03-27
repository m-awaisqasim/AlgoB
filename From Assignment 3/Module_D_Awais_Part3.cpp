// Roll No: i255647
// Name: Awais
// Assignment 3 - Functions & Nested Loops 

#include <iostream>
#include <string>
using namespace std;

// --- Global Variables (Easy to access from any function) ---
double capital, finalVal, totalReturn, annualReturn, sharpe, drawdown, winRate, avgProfit, avgLoss, profitFactor;
int totalTrades, wins, losses;

// --- Function 2: Input Data ---
void receiveData() {
    cout << "Paste the data line from Module C: " << endl;
    
    // Breaking down the input into blocks for simplicity
    cin >> capital >> finalVal;
    cin >> totalReturn >> annualReturn;
    cin >> sharpe >> drawdown;
    cin >> totalTrades >> winRate;
    cin >> avgProfit >> avgLoss >> profitFactor;
    
    // Basic calculation for wins/losses
    wins = (int)((winRate / 100.0) * totalTrades + 0.5);
    losses = totalTrades - wins;
}

// --- Function 3: Show Summary Table ---
void displaySummary() {
    cout << "========================================" << endl;
    cout << "     BACKTESTING PERFORMANCE RESULTS    " << endl;
    cout << "========================================" << endl;
    
    cout << "Initial Capital  : $" << capital << endl;
    cout << "Final Portfolio  : $" << finalVal << endl;
    cout << "Total Return     : " << totalReturn << "%" << endl;
    cout << "Sharpe Ratio     : " << sharpe << endl;
    cout << "Drawdown         : " << drawdown << "%" << endl;
    cout << "Total Trades     : " << totalTrades << endl;
    cout << "Win Rate         : " << winRate << "%" << endl;
    cout << "Profit Factor    : " << profitFactor << endl;
    
    cout << "----------------------------------------" << endl;
}

// --- Function 4: Rating System (Nested Loops) ---
void calculateRating() {
    string criteria[4] = {"Win Rate", "Sharpe", "Returns", "Risk"};
    int points[4];

    // Simple Scoring Criteria
    // 1. Win Rate Score
    if (winRate >= 60) {
        points[0] = 5;
    } else if (winRate >= 50) {
        points[0] = 3;
    } else {
        points[0] = 1;
    }

    // 2. Sharpe Score
    if (sharpe >= 1.5) {
        points[1] = 5;
    } else if (sharpe >= 1.0) {
        points[1] = 3;
    } else {
        points[1] = 1;
    }

    // 3. Return Score
    if (totalReturn >= 30) {
        points[2] = 5;
    } else if (totalReturn >= 10) {
        points[2] = 3;
    } else {
        points[2] = 1;
    }

    // 4. Risk (Drawdown) Score - Lower drawdown is better
    if (drawdown <= 10) {
        points[3] = 5;
    } else if (drawdown <= 20) {
        points[3] = 3;
    } else {
        points[3] = 1;
    }

    cout << "\n--- Strategy Rating Board ---" << endl;
    
    // Outer loop to go through each criteria
    for (int i = 0; i < 4; i++) {
        cout << criteria[i] << " \t: ";
        
        // Inner loop to print stars (Nested Loop)
        for (int j = 0; j < 5; j++) {
            if (j < points[i]) {
                cout << "*";
            } else {
                cout << "-";
            }
        }
        cout << " (" << points[i] << "/5)" << endl;
    }
}

// --- Function 5: Visual Performance Chart ---
void showVisualChart() {
    string labels[3] = {"Returns ", "Win Rate", "Risk    "};
    double values[3] = {totalReturn, winRate, (100.0 - drawdown)};
    
    cout << "\n--- Performance Visualization Board ---" << endl;
    
    for (int i = 0; i < 3; i++) {
        cout << labels[i] << " |";
        int barLength = (int)(values[i] / 2.0);
        if (barLength > 50) barLength = 50; 
        for (int j = 0; j < 50; j++) {
            if (j < barLength) cout << "#";
            else cout << " ";
        }
        cout << "| [" << values[i] << "%]" << endl;
    }
}

// --- Function 6: Equity Curve Graph (2D Nested Loop) ---
void drawEquityCurve() {
    cout << "\n--- Equity Curve (Growth Path) ---" << endl;
    
    // Determine the vertical height based on performance
    double minVal = (capital < finalVal) ? capital : finalVal;
    double maxVal = (capital > finalVal) ? capital : finalVal;
    double range = maxVal - minVal;
    if (range == 0) range = 1; // Prevent division by zero

    // 10 Rows for Price Levels
    for (int row = 10; row >= 0; row--) {
        // Print the scale/price axis
        double priceAtRow = minVal + (range * (row / 10.0));
        cout << "$" << (int)priceAtRow << "\t|";

        // 30 Columns for Time Steps (Nested Loop)
        for (int col = 0; col <= 30; col++) {
            // Calculate what the portfolio value "would be" at this time step
            double progress = col / 30.0;
            double estimatedVal = capital + (progress * (finalVal - capital));
            
            // Check if this estimated value fits in the current row
            int valueRow = (int)(( (estimatedVal - minVal) / range ) * 10);
            
            if (valueRow == row) {
                cout << "*"; // Plot the point
            } else {
                cout << " ";
            }
        }
        cout << endl;
    }
    cout << "\t+-------------------------------" << endl;
    cout << "\t  Start Period          End Period" << endl;
}

// --- Function 7: Chatbot Implementation ---
void startChatbot() {
    string userCmd;
    bool isActive = true;

    cout << endl;
    cout << "+======================================================+" << endl;
    cout << "|         INTERACTIVE PERFORMANCE CHAT                  |" << endl;
    cout << "+======================================================+" << endl;
    cout << endl; 
    cout << "  +--------------------------------------------------+" << endl;
    cout << "  |      ONLY TYPE THE AVAILABLE COMMANDS            |" << endl;
    cout << "  +--------------------------------------------------+" << endl;
    cout << "  |  totalreturn  - Show total & annual return       |" << endl;
    cout << "  |  profit       - Show total & annual return       |" << endl;
    cout << "  |  sharpe       - Show Sharpe ratio                |" << endl;
    cout << "  |  drawdown     - Show max drawdown                |" << endl;
    cout << "  |  losing       - Show losing trade stats          |" << endl;
    cout << "  |  winning      - Show winning trade stats         |" << endl;
    cout << "  |  tradecount   - Show number of trades            |" << endl;
    cout << "  |  winrate      - Show win rate percentage         |" << endl;
    cout << "  |  compare      - Show visual bar chart            |" << endl;
    cout << "  |  equity       - Show equity curve graph          |" << endl;
    cout << "  |  help         - Show this help menu              |" << endl;
    cout << "  |  exit         - Exit the chat                    |" << endl;
    cout << "  +--------------------------------------------------+" << endl;
    cout << endl;

    while (isActive) {
        cout << "\nYou: ";
        cin >> userCmd;

        for (int i = 0; i < userCmd.length(); i++) {
            if (userCmd[i] >= 'A' && userCmd[i] <= 'Z') userCmd[i] += 32;
        }

        if (userCmd == "exit") {
            isActive = false;
        } 
        else if (userCmd == "totalreturn" || userCmd == "profit") {
            cout << "Stats: Total Return is " << totalReturn << "% and Annual is " << annualReturn << "%." << endl;
        } 
        else if (userCmd == "sharpe") {
            cout << "Sharpe Ratio: " << sharpe << endl;
        }
        else if (userCmd == "drawdown") {
            cout << "Max Drawdown: " << drawdown << "%" << endl;
        }
        else if (userCmd == "losing") {
            cout << "Losing Trades: " << losses << " (Avg Loss: $" << avgLoss << ")" << endl;
        }
        else if (userCmd == "winning") {
            cout << "Winning Trades: " << wins << " (Avg Profit: $" << avgProfit << ")" << endl;
        }
        else if (userCmd == "tradecount") {
            cout << "Total Trades: " << totalTrades << endl;
        }
        else if (userCmd == "winrate") {
            cout << "Win Rate: " << winRate << "%" << endl;
        }
        else if (userCmd == "equity") {
            drawEquityCurve();
        }
        else if (userCmd == "compare") {
            showVisualChart();
        }
        else if (userCmd == "rating") {
            calculateRating();
        } 
        else if (userCmd == "help") {
            cout << "Available: totalreturn, profit, sharpe, drawdown, losing, winning, tradecount, winrate, compare, equity, help, exit" << endl;
        } 
        else {
            cout << "Type 'help' for the list of commands." << endl;
        }
    }
}



