#include <iostream>
#include <string>
using namespace std;

double capital, finalVal, totalReturn, annualReturn, sharpe, drawdown, winRate, avgProfit, avgLoss, profitFactor;
int totalTrades, wins, losses;

void receiveData() {
    cout << "Paste the data line from Module C: " << endl;
    cin >> capital >> finalVal;
    cin >> totalReturn >> annualReturn;
    cin >> sharpe >> drawdown;
    cin >> totalTrades >> winRate;
    cin >> avgProfit >> avgLoss >> profitFactor;
    wins = (int)((winRate / 100.0) * totalTrades + 0.5);
    losses = totalTrades - wins;
}

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

void showVisualChart() {
    string labels[3] = {"Returns ", "Win Rate", "Risk    "};
    double values[3] = {totalReturn, winRate, (100.0 - drawdown)};
    cout << "\n--- Performance Visualization Board ---" << endl;
    for (int i = 0; i < 3; i++) {
        cout << labels[i] << " |";
        int barLength = (int)(values[i] / 2.0);
        if (barLength > 50) barLength = 50; 
        for (int j = 0; j < 50; j++) {
            if (j < barLength) cout << "#"; else cout << " ";
        }
        cout << "| [" << values[i] << "%]" << endl;
    }
}

void drawEquityCurve() {
    cout << "\n--- Equity Curve (Growth Path) ---" << endl;
    double peakVal = (capital > finalVal) ? capital : finalVal;
    double dipVal = peakVal * (1.0 - (drawdown / 100.0));
    double minVal = (capital < finalVal) ? capital : finalVal;
    if (dipVal < minVal) minVal = dipVal;
    double maxVal = peakVal;
    double range = maxVal - minVal;
    if (range < 1) range = 1; 

    for (int row = 10; row >= 0; row--) {
        double priceAtRow = minVal + (range * (row / 10.0));
        cout << "$" << (int)priceAtRow << "\t|";
        for (int col = 0; col <= 30; col++) {
            double progress = col / 30.0;
            double estimatedVal;
            if (progress < 0.4) {
                estimatedVal = capital + (progress * 2.5 * (peakVal - capital));
                if (estimatedVal > peakVal) estimatedVal = peakVal;
            } else if (progress < 0.6) {
                double dipProgress = (progress - 0.4) / 0.2;
                estimatedVal = peakVal - (dipProgress * (peakVal - dipVal));
            } else {
                double recProgress = (progress - 0.6) / 0.4;
                estimatedVal = dipVal + (recProgress * (finalVal - dipVal));
            }
            int valueRow = (int)(( (estimatedVal - minVal) / range ) * 10);
            if (valueRow == row) cout << "*"; else cout << " ";
        }
        cout << endl;
    }
    cout << "\t+-------------------------------" << endl;
    cout << "\t  Start Period          End Period" << endl;
}

void startChatbot() {
    string userCmd;
    bool isActive = true;
    cout << endl;
    cout << "      ___   __             ________          __  __          __  " << endl;
    cout << "     /   | / /____ _____  / ____/ /_  ____ _/ /_/ /_  ____  / /_ " << endl;
    cout << "    / /| |/ / __ `/ __ \\ / /   / __ \\/ __ `/ __/ __ \\/ __ \\/ __/ " << endl;
    cout << "   / ___ / / /_/ / /_/ // /___/ / / / /_/ / /_/ /_/ / /_/ / /_   " << endl;
    cout << "  /_/  |_\\_\\__, /\\____/ \\____/_/ /_/\\__,_/\\__/_.___/\\____/\\__/   " << endl;
    cout << "          /____/                                                 " << endl;
    cout << endl; 
    cout << "  +---------------------------------------------------------+" << endl;
    cout << "  |      ONLY TYPE THE AVAILABLE COMMANDS                   |" << endl;
    cout << "  +---------------------------------------------------------+" << endl;
    cout << "  |  Hello/Hi     - I am AlgoB, your backtesting assistant. |" << endl;
    cout << "  |  totalreturn  - Show total & annual return              |" << endl;
    cout << "  |  profit       - Show total & annual return              |" << endl;
    cout << "  |  sharpe       - Show Sharpe ratio                       |" << endl;
    cout << "  |  drawdown     - Show max drawdown                       |" << endl;
    cout << "  |  losing       - Show losing trade stats                 |" << endl;
    cout << "  |  winning      - Show winning trade stats                |" << endl;
    cout << "  |  tradecount   - Show number of trades                   |" << endl;
    cout << "  |  winrate      - Show win rate percentage                |" << endl;
    cout << "  |  compare      - Show visual bar chart                   |" << endl;
    cout << "  |  equity       - Show equity curve graph                 |" << endl;
    cout << "  |  help         - Show this help menu                     |" << endl;
    cout << "  |  exit         - Exit the chat                           |" << endl;
    cout << "  +---------------------------------------------------------+" << endl;
    cout << endl;
    cout << "\nYou: ";
    while (isActive && cin >> userCmd) {
        for (int i = 0; i < (int)userCmd.length(); i++) {
            if (userCmd[i] >= 'A' && userCmd[i] <= 'Z') userCmd[i] += 32;
        }
        if (userCmd == "exit") isActive = false;
        else if (userCmd == "hello" || userCmd == "hi") cout << "Hello! I am AlgoB, your backtesting assistant." << endl;
        else if (userCmd == "totalreturn" || userCmd == "profit") cout << "Stats: Total Return is " << totalReturn << "% and Annual is " << annualReturn << "%." << endl;
        else if (userCmd == "sharpe") cout << "Sharpe Ratio: " << sharpe << endl;
        else if (userCmd == "drawdown") cout << "Max Drawdown: " << drawdown << "%" << endl;
        else if (userCmd == "losing") cout << "Losing Trades: " << losses << " (Avg Loss: $" << avgLoss << ")" << endl;
        else if (userCmd == "winning") cout << "Winning Trades: " << wins << " (Avg Profit: $" << avgProfit << ")" << endl;
        else if (userCmd == "tradecount") cout << "Total Trades: " << totalTrades << endl;
        else if (userCmd == "winrate") cout << "Win Rate: " << winRate << "%" << endl;
        else if (userCmd == "equity") drawEquityCurve();
        else if (userCmd == "compare") showVisualChart();
        else if (userCmd == "help") cout << "Available: totalreturn, profit, sharpe, drawdown, losing, winning, tradecount, winrate, compare, equity, help, exit" << endl;
        else cout << "Type 'help' for the list of commands." << endl;
        if (isActive) cout << "\nYou: ";
    }
}
