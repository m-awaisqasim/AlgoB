#include <iostream>
#include <string>
using namespace std;

const int MAX_DAYS = 1000;

void editMarketData(double matrix[MAX_DAYS][5], int rows);
void displayMarketTable(double matrix[MAX_DAYS][5], int rows);
void configureStrategy(int totalDays);

double capital, finalVal, totalReturn, annualReturn, sharpe, drawdown, winRate, avgProfit, avgLoss, profitFactor;
int totalTrades, wins, losses;
double p_history[MAX_DAYS];
int h_count = 0;

int getLength(string s) {
    int count = 0;
    while (s[count] != '\0') count++;
    return count;
}

string formatPKR(double val) {
    if (val >= 1000000) return to_string((int)(val / 1000000)) + "M";
    if (val >= 1000) return to_string((int)(val / 1000)) + "K";
    return to_string((int)val);
}

bool findKeyword(string text, string key) {
    if (getLength(key) > getLength(text)) return false;
    for (int i = 0; i <= (getLength(text) - getLength(key)); i++) {
        bool match = true;
        for (int j = 0; j < getLength(key); j++) {
            if (text[i + j] != key[j]) { match = false; break; }
        }
        if (match) return true;
    }
    return false;
}

void setModuleCData(double* metrics, double* hist, int cnt) {
    if (metrics == 0) return;
    for (int i = 0; i < cnt; i++) p_history[i] = hist[i];
    h_count = cnt;
    capital = metrics[0];
    finalVal = metrics[1];
    totalReturn = metrics[2];
    annualReturn = metrics[3];
    sharpe = metrics[4];
    drawdown = metrics[5];
    totalTrades = (int)metrics[6];
    winRate = metrics[7];
    avgProfit = metrics[8];
    avgLoss = metrics[9];
    profitFactor = metrics[10];
    wins = (int)((winRate / 100.0) * totalTrades + 0.5);
    losses = totalTrades - wins;
}

void displaySummary() {
    cout << "========================================" << endl;
    cout << "     BACKTESTING PERFORMANCE RESULTS    " << endl;
    cout << "========================================" << endl;
    cout << "Initial Capital  : Rs." << formatPKR(capital) << endl;
    cout << "Final Portfolio  : Rs." << formatPKR(finalVal) << endl;
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
    cout << "\n--- Performance Visualization Board ---\n";
    for (int i = 0; i < 3; i++) {
        cout << labels[i] << " |";
        int barLength = (int)(values[i] / 2.0);
        if (barLength > 50) barLength = 50;
        for (int j = 0; j < 50; j++) {
            cout << (j < barLength ? "#" : " ");
        }
        cout << "| [" << values[i] << "%]\n";
    }
}

bool startChatbot(double matrix[MAX_DAYS][5], int& rows) {
    cout << "\n      ___   __             ________          __  __          __  \n";
    cout << "     /   | / /____ _____  / ____/ /_  ____ _/ /_/ /_  ____  / /_ \n";
    cout << "    / /| |/ / __ `/ __ \\ / /   / __ \\/ __ `/ __/ __ \\/ __ \\/ __/ \n";
    cout << "   / ___ / / /_/ / /_/ // /___/ / / / /_/ / /_/ /_/ / /_/ / /_   \n";
    cout << "  /_/  |_\\_\\__, /\\____/ \\____/_/ /_/\\__,_/\\__/_.___/\\____/\\__/   \n";

    string userCmd;
    bool isActive = true;

    const int NUM_CATS = 4;
    string catNames[NUM_CATS] = {"Returns", "Risk Metrics", "Trade Stats", "Settings & Data"};
    int catCounts[NUM_CATS] = {2, 2, 5, 4};
    string helpItems[NUM_CATS][5] = {
        {"totalreturn", "profit", "", "", ""},
        {"sharpe", "drawdown", "", "", ""},
        {"losing", "winning", "tradecount", "winrate", "advice"},
        {"edit", "settings", "retest", "exit", ""}
    };

    cout << "\n+---------------------------------------------------------+" << endl;
    cout << "|              DYNAMIC COMMAND HELP MENU                  |" << endl;
    cout << "+---------------------------------------------------------+" << endl;
    for (int i = 0; i < NUM_CATS; i++) {
        cout << " [" << *(catNames + i) << "]" << endl;
        for (int j = 0; j < *(catCounts + i); j++) {
            cout << "  * " << *(*(helpItems + i) + j) << endl;
        }
        cout << endl;
    }
    cout << "+---------------------------------------------------------+" << endl;

    cout << "\n > You: ";
    while (isActive && getline(cin, userCmd)) {
        for(int i=0; i<getLength(userCmd); i++) {
            if(userCmd[i]>='A' && userCmd[i]<='Z') userCmd[i]+=32;
        }
        if (findKeyword(userCmd, "exit") || findKeyword(userCmd, "quit") || findKeyword(userCmd, "bye")) isActive = false;
        else if (findKeyword(userCmd, "hello") || findKeyword(userCmd, "hi"))
            cout << "Hello! I am your AlgoB assistant. How can I help with your strategy today?" << endl;
        else if (findKeyword(userCmd, "return") || findKeyword(userCmd, "profit"))
            cout << "Analysis: Total Return is " << totalReturn << "% and Annual Profit is Rs." << formatPKR((totalReturn/100)*capital) << "." << endl;
        else if (findKeyword(userCmd, "sharpe"))
            cout << "Risk-Adjusted: Your Sharpe Ratio is " << sharpe << "." << endl;
        else if (findKeyword(userCmd, "drawdown") || findKeyword(userCmd, "risk"))
            cout << "Risk: Max Drawdown reached " << drawdown << "% during the test." << endl;
        else if (findKeyword(userCmd, "winrate") || findKeyword(userCmd, "accuracy"))
            cout << "Accuracy: Your Win Rate is " << winRate << "% (" << wins << " wins vs " << losses << " losses)." << endl;
        else if (findKeyword(userCmd, "tradecount") || findKeyword(userCmd, "trades"))
            cout << "Volume: A total of " << totalTrades << " trades were executed." << endl;
        else if (findKeyword(userCmd, "advice") || findKeyword(userCmd, "suggest")) {
            cout << "\n[AlgoB Advice Engine]" << endl;
            if (totalReturn < 0) cout << " > Warning: Strategy is losing money. Try increasing SMA Long Period." << endl;
            if (drawdown > 15) cout << " > Warning: High Drawdown (" << drawdown << "%). Strategy is too aggressive." << endl;
            if (winRate < 45) cout << " > Tip: Low Win Rate. Try increasing RSI Period." << endl;
            if (profitFactor > 1.3 && totalReturn > 5) cout << " > Success: Solid metrics. This strategy shows good alpha!" << endl;
            if (totalTrades < 5) cout << " > Note: Low sample size. Add more days of data." << endl;
        }
        else if (findKeyword(userCmd, "edit")) {
            editMarketData(matrix, rows);
            displayMarketTable(matrix, rows);
            cout << "\n[System] Type 'retest' to apply these changes." << endl;
        }
        else if (findKeyword(userCmd, "settings") || findKeyword(userCmd, "param")) {
            configureStrategy(rows);
            cout << "\n[System] Settings updated. Type 'retest' to see impact." << endl;
        }
        else if (findKeyword(userCmd, "retest") || findKeyword(userCmd, "update") || findKeyword(userCmd, "run")) {
            return true;
        }
        else if (findKeyword(userCmd, "compare") || findKeyword(userCmd, "chart")) showVisualChart();
        else cout << "I didn't quite get that. Try asking about 'return', 'win rate', 'risk', or 'advice'." << endl;
        if (isActive) cout << "\n > You: ";
    }
    return false;
}
