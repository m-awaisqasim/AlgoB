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
    string status = (totalReturn >= 0) ? "PROFITABLE" : "LOSS MAKING";

    cout << "\n  +------------------------------------------------------+" << endl;
    cout << "  |         STRATEGY PERFORMANCE SUMMARY                   |" << endl;
    cout << "  +--------------------------------------------------------+" << endl;
    cout << "  |                                                        " << endl;
    cout << "  |  STATUS: " << status << "                              " << endl;
    cout << "  |                                                        " << endl;
    cout << "  |  [ OVERVIEW ]                                          " << endl;
    cout << "  |  Initial Capital : Rs." << formatPKR(capital) << "     " << endl; 
    cout << "  |  Final Value     : Rs." << formatPKR(finalVal) << "    " << endl;
    cout << "  |  Total Return    : " << totalReturn << "%              " << endl;
    cout << "  |                                                        " << endl;
    cout << "  |  [ RISK METRICS ]                                      " << endl;
    cout << "  |  Max Drawdown    : " << drawdown << "%                 " << endl;
    cout << "  |  Sharpe Ratio    : " << sharpe << "                    " << endl;
    cout << "  |  Profit Factor   : " << profitFactor << "              " << endl;
    cout << "  |                                                        " << endl;
    cout << "  |  [ TRADE STATS ]                                       " << endl;
    cout << "  |  Total Trades    : " << totalTrades << "               " << endl;
    cout << "  |  Win Rate        : " << winRate << "%                  " << endl;
    cout << "  |  Wins / Losses   : " << wins << " / " << losses << "   " << endl;
    cout << "  +--------------------------------------------------------+" << endl;
}

void showVisualChart() {
    string labels[3] = {"Returns ", "Win Rate", "Safety  "};
    double values[3] = {totalReturn, winRate, (100.0 - drawdown)};
    string bars[3] = {">>>>", "####", "===="};
    cout << endl;
    cout << "  +----------------------------------------------------+" << endl;
    cout << "  |         PERFORMANCE VISUALIZATION BOARD            |" << endl;
    cout << "  +----------------------------------------------------+" << endl;
    for (int i = 0; i < 3; i++) {
        cout << "  | " << labels[i] << " |";
        int barLength = (int)(values[i] / 2.0);
        if (barLength < 0) barLength = 0;
        if (barLength > 40) barLength = 40;
        for (int j = 0; j < 40; j++) {
            if (j < barLength) cout << bars[i][j % 4];
            else cout << " ";
        }
        cout << "| " << values[i] << "%" << endl;
    }
    cout << "  +----------------------------------------------------+" << endl;
}

bool startChatbot(double matrix[MAX_DAYS][5], int& rows) {
    cout << "\n      ___   __             ________          __  __          __  \n";
    cout << "     /   | / /____ _____  / ____/ /_  ____ _/ /_/ /_  ____  / /_ \n";
    cout << "    / /| |/ / __ `/ __ \\ / /   / __ \\/ __ `/ __/ __ \\/ __ \\/ __/ \n";
    cout << "   / ___ / / /_/ / /_/ // /___/ / / / /_/ / /_/ /_/ / /_/ / /_   \n";
    cout << "  /_/  |_\\_\\__, /\\____/ \\____/_/ /_/\\__,_/\\__/_.___/\\____/\\__/   \n";

    string userCmd;
    bool isActive = true;

    cout << endl;
    cout << "  +---------------------------------------------------------+" << endl;
    cout << "  |            ALGOCHATBOT - COMMAND REFERENCE              |" << endl;
    cout << "  +---------------------------------------------------------+" << endl;
    cout << "  |  [Returns]" << endl;
    cout << "  |    >> return    - Show total return & profit" << endl;
    cout << "  |    >> profit    - Show annual profit" << endl;
    cout << "  |" << endl;
    cout << "  |  [Risk Metrics]" << endl;
    cout << "  |    >> sharpe    - Show Sharpe ratio" << endl;
    cout << "  |    >> drawdown  - Show max drawdown" << endl;
    cout << "  |" << endl;
    cout << "  |  [Trade Stats]" << endl;
    cout << "  |    >> winrate   - Show win rate %" << endl;
    cout << "  |    >> tradecount- Show trade count" << endl;
    cout << "  |    >> advice    - Get strategy tips" << endl;
    cout << "  |    >> chart     - Visual comparison" << endl;
    cout << "  |    >> compare   - Performance bars" << endl;
    cout << "  |" << endl;
    cout << "  |  [Settings & Data]" << endl;
    cout << "  |    >> edit      - Edit market data" << endl;
    cout << "  |    >> settings  - Change SMA/RSI" << endl;
    cout << "  |    >> retest    - Re-run backtest" << endl;
    cout << "  |    >> exit      - Close chatbot" << endl;
    cout << "  |" << endl;
    cout << "  +---------------------------------------------------------+" << endl;

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
            if (totalTrades < 3) cout << " > Note: Low trade count. Strategy hasn't traded enough for a solid statistical conclusion." << endl;
            else if (totalTrades < 10) cout << " > Note: Small sample size. These results may not be consistent over long periods." << endl;
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
