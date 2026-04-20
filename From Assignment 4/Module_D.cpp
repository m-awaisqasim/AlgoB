#include <iostream>
#include <string>
using namespace std;

double capital, finalVal, totalReturn, annualReturn, sharpe, drawdown, winRate, avgProfit, avgLoss, profitFactor;
int totalTrades, wins, losses;
double* p_history = 0;
int h_count = 0;

void setModuleCData(double* metrics, double* hist, int cnt) {
    if (metrics == 0) return;
    p_history = hist;
    h_count = cnt;

    const int IDX_INITIAL_CAPITAL = 0;
    const int IDX_FINAL_VALUE = 1;
    const int IDX_TOTAL_RETURN_PCT = 2;
    const int IDX_ANNUAL_RETURN_PCT = 3;
    const int IDX_SHARPE = 4;
    const int IDX_DRAWDOWN_PCT = 5;
    const int IDX_TOTAL_TRADES = 6;
    const int IDX_WIN_RATE_PCT = 7;
    const int IDX_AVG_PROFIT = 8;
    const int IDX_AVG_LOSS = 9;
    const int IDX_PROFIT_FACTOR = 10;

    capital = metrics[IDX_INITIAL_CAPITAL];
    finalVal = metrics[IDX_FINAL_VALUE];
    totalReturn = metrics[IDX_TOTAL_RETURN_PCT];
    annualReturn = metrics[IDX_ANNUAL_RETURN_PCT];
    sharpe = metrics[IDX_SHARPE];
    drawdown = metrics[IDX_DRAWDOWN_PCT];
    totalTrades = (int)metrics[IDX_TOTAL_TRADES];
    winRate = metrics[IDX_WIN_RATE_PCT];
    avgProfit = metrics[IDX_AVG_PROFIT];
    avgLoss = metrics[IDX_AVG_LOSS];
    profitFactor = metrics[IDX_PROFIT_FACTOR];

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
    string* labels = new string[3]{"Returns ", "Win Rate", "Risk    "};
    double* values = new double[3]{totalReturn, winRate, (100.0 - drawdown)};
    
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
    
    delete[] labels;
    delete[] values;
}

void drawEquityCurve(double* history, int count) {
    cout << "\n--- Equity Curve (Growth Path) ---" << endl;
    double minVal = history[0], maxVal = history[0];
    for (int i = 0; i < count; i++) {
        if (history[i] < minVal) minVal = history[i];
        if (history[i] > maxVal) maxVal = history[i];
    }
    double range = maxVal - minVal;
    if (range < 1) range = 1; 

    int rows = 11;
    int cols = 31;
    char** equityGrid = new char*[rows];
    for(int i = 0; i < rows; i++) {
        equityGrid[i] = new char[cols];
        for(int j = 0; j < cols; j++) equityGrid[i][j] = ' ';
    }

    for (int col = 0; col < cols; col++) {
        int index = (col * (count - 1)) / (cols - 1);
        double val = history[index];
        int valueRow = (int)(((val - minVal) / range) * 10);
        if(valueRow >= 0 && valueRow < rows) equityGrid[valueRow][col] = '*';
    }

    for (int row = rows - 1; row >= 0; row--) {
        double priceRow = minVal + (range * (row / 10.0));
        cout << "$" << (int)priceRow << "\t|";
        for (int col = 0; col < cols; col++) cout << equityGrid[row][col];
        cout << endl;
    }
    
    for(int i = 0; i < rows; i++) delete[] equityGrid[i];
    delete[] equityGrid;

    cout << "\t+-------------------------------" << endl;
    cout << "\t  Start Period          End Period" << endl;
}

void startChatbot() {
    string userCmd;
    bool isActive = true;

    int historyCapacity = 5;
    int historyCount = 0;
    string* cmdHistory = new string[historyCapacity];

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
    cout << "  |  history      - Show dynamically logged command history   |" << endl;
    cout << "  |  help         - Show this help menu                     |" << endl;
    cout << "  |  exit         - Exit the chat                           |" << endl;
    cout << "  +---------------------------------------------------------+" << endl;
    cout << endl;
    cout << "\nYou: ";
    
    while (isActive && cin >> userCmd) {
        
        if (historyCount == historyCapacity) {
            int newCapacity = historyCapacity * 2; 
            string* newHistory = new string[newCapacity]; 
            
            for (int i = 0; i < historyCount; i++) {
                newHistory[i] = cmdHistory[i];
            }
            
            delete[] cmdHistory;  
            cmdHistory = newHistory; 
            historyCapacity = newCapacity;
        }

        cmdHistory[historyCount] = userCmd;
        historyCount++;

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
        else if (userCmd == "equity") drawEquityCurve(p_history, h_count);
        else if (userCmd == "compare") showVisualChart();
        else if (userCmd == "history") {
            cout << "\n--- Session Command Logs (Capacity: " << historyCapacity << ") ---" << endl;
            for (int i = 0; i < historyCount; i++) {
                cout << "  " << i + 1 << ": " << cmdHistory[i] << endl;
            }
            cout << "------------------------------------------" << endl;
        }
        else if (userCmd == "help") cout << "Available: totalreturn, profit, sharpe, drawdown, losing, winning, tradecount, winrate, compare, equity, history, help, exit" << endl;
        else cout << "Type 'help' for the list of commands." << endl;
        
        if (isActive) cout << "\nYou: ";
    }

    delete[] cmdHistory;
}
