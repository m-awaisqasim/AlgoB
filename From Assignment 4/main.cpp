#include <iostream>
#include <string>
#include <cmath>
using namespace std;

const int METRICS_COUNT = 11;

// ============================================================================
// GLOBAL STATE (MODULE D)
// ============================================================================
double capital, finalVal, totalReturn, annualReturn, sharpe, drawdown, winRate, avgProfit, avgLoss, profitFactor;
int totalTrades, wins, losses;
double* p_history = 0;
int h_count = 0;
double* price_data;
int sma_short = 20, sma_long = 50, rsi_period = 14;
bool startChatbot(double** matrix, int& rows);

// ============================================================================
// MODULE A: DATA MANAGEMENT
// ============================================================================

int getLength(string s) {
    int count = 0;
    while (s[count] != '\0') count++;
    return count;
}

double** loadMarketData(string filename, int &rows, int &cols) {
    do {
        cout << "\n[Module A] How many days of market data? (Minimum 2): ";
        cin >> rows;
        if (rows < 2) cout << " >> Error: You need at least 2 days of data for analysis." << endl;
    } while (rows < 2);

    cols = 5;
    double** priceMatrix = new double*[rows];
    for (int i = 0; i < rows; i++) {
        priceMatrix[i] = new double[cols];
    }

    cin.ignore();
    cout << "\n[System] You can now PASTE your data rows below:\n";
    for (int i = 0; i < rows; i++) {
        string line;
        cout << "Enter Day " << i + 1 << ": ";
        getline(cin, line);
        int col = 0, start = 0;
        for (int k = 0; k <= getLength(line) && col < 5; k++) {
            if (k == getLength(line) || line[k] == ',' || line[k] == ' ') {
                if (k > start) {
                    string part = "";
                    for (int m = start; m < k; m++) part += line[m];
                    if (part != "") {
                        priceMatrix[i][col++] = stod(part);
                    }
                }
                start = k + 1;
            }
        }
    }
    return priceMatrix;
}

void editMarketData(double** matrix, int rows) {
    char choice;
    cout << "\n[Module A] Do you want to edit any data? (y/n): ";
    cin >> choice;

    if (choice == 'y' || choice == 'Y') {
        int day, col;
        cout << "Enter Day number to edit (1-" << rows << "): ";
        cin >> day;
        cout << "Enter Column (1:Open, 2:High, 3:Low, 4:Close, 5:Vol): ";
        cin >> col;

        if (day >= 1 && day <= rows && col >= 1 && col <= 5) {
            cout << "Current value: " << matrix[day - 1][col - 1] << endl;
            cout << "Enter new value: ";
            cin >> matrix[day - 1][col - 1];
            cout << "[Success] Data updated!" << endl;
        } else {
            cout << "[Error] Invalid day or column!" << endl;
        }
    }
}

void displayMarketTable(double** matrix, int rows) {
    if (matrix == 0) return;
    cout << "\nDay\tOpen\tHigh\tLow\tClose\tVolume" << endl;
    int previewRows = (rows > 5) ? 5 : rows;
    for (int i = 0; i < previewRows; i++) {
        cout << i + 1 << "\t";
        for (int j = 0; j < 5; j++) cout << matrix[i][j] << "\t";
        cout << endl;
    }
}

void unloadMarketData(double** matrix, int rows) {
    if (matrix == 0) return;
    for (int i = 0; i < rows; i++) delete[] matrix[i];
    delete[] matrix;
}

// ============================================================================
// MODULE B: SIGNAL GENERATION
// ============================================================================

double SMA(int day, int period) {
    double total = 0;
    for (int i = day - period + 1; i <= day; i++) {
        total += price_data[i];
    }
    return total / period;
}

double RSI(int day, int period) {
    double gain = 0, loss = 0;
    for (int i = day - period + 1; i <= day; i++) {
        double change = price_data[i] - price_data[i - 1];
        if (change > 0) gain += change;
        else loss += (-change);
    }
    double avg_gain = gain / period;
    double avg_loss = loss / period;
    if (avg_loss == 0) return 100;
    double rs = avg_gain / avg_loss;
    return 100 - (100 / (1 + rs));
}

int* generateSignals(double* prices, int daysCount) {
    price_data = prices;
    int* final_signals = new int[daysCount];

    for (int i = 0; i < daysCount; i++) final_signals[i] = 0;
    int start_day = (sma_long > rsi_period) ? sma_long : rsi_period;
    start_day++;

    for (int day = start_day; day < daysCount; day++) {
        double prev_short = SMA(day - 1, sma_short);
        double prev_long = SMA(day - 1, sma_long);
        double curr_short = SMA(day, sma_short);
        double curr_long = SMA(day, sma_long);
        int sma_sig = 0;
        if (prev_short <= prev_long && curr_short > curr_long) sma_sig = 1;
        else if (prev_short >= prev_long && curr_short < curr_long) sma_sig = -1;

        double rsi_val = RSI(day, rsi_period);
        int rsi_sig = 0;
        if (rsi_val < 30) rsi_sig = 1;
        else if (rsi_val > 70) rsi_sig = -1;

        if ((sma_sig == 1 || rsi_sig == 1) && !(sma_sig == -1 || rsi_sig == -1)) final_signals[day] = 1;
        else if ((sma_sig == -1 || rsi_sig == -1) && !(sma_sig == 1 || rsi_sig == 1)) final_signals[day] = -1;
    }
    return final_signals;
}

void configureStrategy(int totalDays) {
    char choice;
    cout << "\n[Module B] Use default strategy parameters? (SMA: 20/50, RSI: 14) (y/n): ";
    cin >> choice;
    if (choice == 'n' || choice == 'N') {
        bool valid = false;
        while (!valid) {
            cout << "Enter SMA Short Period: "; cin >> sma_short;
            cout << "Enter SMA Long Period: ";  cin >> sma_long;
            cout << "Enter RSI Period: ";      cin >> rsi_period;

            if (sma_short <= 0 || sma_long <= 0 || rsi_period <= 0) {
                cout << " >> Error: All periods must be positive numbers." << endl;
            } else if (sma_short >= sma_long) {
                cout << " >> Error: SMA Short must be less than SMA Long." << endl;
            } else if (sma_long >= totalDays || rsi_period >= totalDays) {
                cout << " >> Error: Periods cannot be longer than the data days (" << totalDays << ")." << endl;
            } else {
                valid = true;
            }
        }
    }
}

// ============================================================================
// MODULE C: BACKTESTING ENGINE
// ============================================================================

void runBacktest(const double* prices, const int* signals, int daysCount, double initialCapital, double initialShares, double prevPortfolio, double results[], double history[]) {
    double cash = initialCapital, shares = initialShares, portfolio_value, previous_portfolio = prevPortfolio, max_drawdown = 0, sum_returns = 0, sum_squared = 0, peak = initialCapital, total_profit = 0, total_loss = 0, buy_price = 0;
    int total_trades = 0, wins = 0, losses = 0;

    for(int day = 0; day < daysCount; day++) {
        double price = prices[day];
        if(signals[day] == 1 && shares == 0 && price > 0) {
            shares = cash / price; cash = 0; buy_price = price;
        } else if(signals[day] == -1 && shares > 0) {
            cash = shares * price;
            double profit = (price - buy_price) * shares;
            total_trades++;
            if(profit > 0) { wins++; total_profit += profit; }
            else { losses++; total_loss += -profit; }
            shares = 0;
        }
        portfolio_value = cash + (shares * price);
        history[day] = portfolio_value;
        double daily_return = (previous_portfolio != 0) ? (portfolio_value - previous_portfolio) / previous_portfolio : 0;
        sum_returns += daily_return; sum_squared += daily_return * daily_return;
        previous_portfolio = portfolio_value;
        if(portfolio_value > peak) peak = portfolio_value;
        double dd = (peak - portfolio_value) / peak;
        if(dd > max_drawdown) max_drawdown = dd;
    }

    results[0] = initialCapital; results[1] = portfolio_value; results[2] = ((portfolio_value/initialCapital)-1)*100.0;
    results[3] = (daysCount >= 30) ? (pow((portfolio_value/initialCapital), (365.0/daysCount))-1)*100.0 : results[2];
    double mean_ret = sum_returns / daysCount;
    double std_dev = sqrt((sum_squared/daysCount) - (mean_ret*mean_ret));
    results[4] = (std_dev != 0) ? mean_ret/std_dev : 0;
    results[5] = max_drawdown * 100.0; results[6] = (double)total_trades;
    results[7] = (total_trades > 0) ? ((double)wins/total_trades)*100.0 : 0;
    results[8] = (wins > 0) ? total_profit/wins : 0; results[9] = (losses > 0) ? total_loss/losses : 0;
    results[10] = (total_loss > 0) ? total_profit/total_loss : 0;
}

// ============================================================================
// MODULE D: PERFORMANCE UI & CHATBOT
// ============================================================================

void setModuleCData(double* metrics, double* hist, int cnt) {
    if (metrics == 0) return;
    p_history = hist; h_count = cnt;
    capital = metrics[0]; finalVal = metrics[1]; totalReturn = metrics[2]; annualReturn = metrics[3];
    sharpe = metrics[4]; drawdown = metrics[5]; totalTrades = (int)metrics[6]; winRate = metrics[7];
    avgProfit = metrics[8]; avgLoss = metrics[9]; profitFactor = metrics[10];
    wins = (int)((winRate / 100.0) * totalTrades + 0.5); losses = totalTrades - wins;
}

string formatPKR(double val) {
    if (val >= 1000000) return to_string((int)(val / 1000000)) + "M";
    if (val >= 1000) return to_string((int)(val / 1000)) + "K";
    return to_string((int)val);
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

bool startChatbot(double** matrix, int& rows) {
    cout << "\n      ___   __             ________          __  __          __  \n";
    cout << "     /   | / /____ _____  / ____/ /_  ____ _/ /_/ /_  ____  / /_ \n";
    cout << "    / /| |/ / __ `/ __ \\ / /   / __ \\/ __ `/ __/ __ \\/ __ \\/ __/ \n";
    cout << "   / ___ / / /_/ / /_/ // /___/ / / / /_/ / /_/ /_/ / /_/ / /_   \n";
    cout << "  /_/  |_\\_\\__, /\\____/ \\____/_/ /_/\\__,_/\\__/_.___/\\____/\\__/   \n";

    string userCmd;
    bool isActive = true;

    int numCats = 4;
    string* catNames = new string[numCats]{"Returns", "Risk Metrics", "Trade Stats", "Settings & Data"};
    int* catCounts = new int[numCats]{2, 2, 5, 4};
    
    string** helpMap = new string*[numCats];
    helpMap[0] = new string[2]{"totalreturn - Show total return", "profit      - Show annual profit"};
    helpMap[1] = new string[2]{"sharpe      - Show Sharpe ratio", "drawdown    - Show max drawdown"};
    helpMap[2] = new string[5]{"losing      - Show losing trades", "winning     - Show winning trades", "tradecount  - Show trade count", "winrate     - Show win rate %", "advice      - Get strategy advice"};
    helpMap[3] = new string[4]{"edit        - Edit market data", "settings    - Change strategy params", "retest      - Update results", "exit        - Close the chat"};

    cout << "\n+---------------------------------------------------------+" << endl;
    cout << "|              DYNAMIC COMMAND HELP MENU                  |" << endl;
    cout << "+---------------------------------------------------------+" << endl;
    for (int i = 0; i < numCats; i++) {
        cout << " [" << catNames[i] << "]" << endl;
        for (int j = 0; j < catCounts[i]; j++) {
            cout << "  * " << helpMap[i][j] << endl;
        }
        cout << endl;
    }
    cout << "+---------------------------------------------------------+" << endl;

    cout << "\n > You: ";
    if (cin.peek() == '\n') cin.ignore();
    while (isActive && getline(cin, userCmd)) {
        for(int i=0; i<getLength(userCmd); i++) {
            if(userCmd[i]>='A' && userCmd[i]<='Z') userCmd[i]+=32;
        }
        
        if (findKeyword(userCmd, "exit") || findKeyword(userCmd, "quit") || findKeyword(userCmd, "bye")) isActive = false;
        else if (findKeyword(userCmd, "hello") || findKeyword(userCmd, "hi")) cout << "Hello! I am your AlgoB assistant. How can I help with your strategy today?" << endl;
        else if (findKeyword(userCmd, "return") || findKeyword(userCmd, "profit")) 
            cout << "Analysis: Total Return is " << totalReturn << "% and Annual Profit is Rs." << formatPKR((totalReturn/100)*capital) << "." << endl;
        else if (findKeyword(userCmd, "sharpe")) cout << "Risk-Adjusted: Your Sharpe Ratio is " << sharpe << "." << endl;
        else if (findKeyword(userCmd, "drawdown") || findKeyword(userCmd, "risk")) cout << "Risk: Max Drawdown reached " << drawdown << "% during the test." << endl;
        else if (findKeyword(userCmd, "win")) cout << "Accuracy: Your Win Rate is " << winRate << "% (" << wins << " wins vs " << losses << " losses)." << endl;
        else if (findKeyword(userCmd, "trade")) cout << "Volume: A total of " << totalTrades << " trades were executed." << endl;
        else if (findKeyword(userCmd, "advice") || findKeyword(userCmd, "suggest")) {
            cout << "\n[AlgoB Advice Engine]" << endl;
            if (totalReturn < 0) cout << " > Warning: Strategy is losing money. Try increasing SMA Long Period." << endl;
            if (drawdown > 15) cout << " > Warning: High Drawdown (" << drawdown << "%). Strategy is too aggressive." << endl;
            if (winRate < 45) cout << " > Tip: Low Win Rate. Try increasing RSI Period." << endl;
            if (profitFactor > 1.3 && totalReturn > 5) cout << " > Success: Solid metrics. This strategy shows good alpha!" << endl;
            if (totalTrades < 5) cout << " > Note: Low sample size. Add more days of data." << endl;
        }
        else if (findKeyword(userCmd, "edit") || findKeyword(userCmd, "change")) {
            editMarketData(matrix, rows);
            displayMarketTable(matrix, rows);
            cout << "\n[System] Type 'retest' to apply these changes." << endl;
        }
        else if (findKeyword(userCmd, "settings")) {
            configureStrategy(rows);
            cout << "\n[System] Settings updated. Type 'retest' to see impact." << endl;
        }
        else if (findKeyword(userCmd, "retest") || findKeyword(userCmd, "update") || findKeyword(userCmd, "run")) {
            for (int i = 0; i < numCats; i++) delete[] helpMap[i];
            delete[] helpMap; delete[] catNames; delete[] catCounts;
            return true; 
        }
        else if (findKeyword(userCmd, "compare") || findKeyword(userCmd, "chart")) showVisualChart();
        else cout << "I didn't quite get that. Try asking about 'return', 'win rate', 'risk', or 'advice'." << endl;
        
        if (isActive) cout << "\n > You: ";
    }

    for (int i = 0; i < numCats; i++) delete[] helpMap[i];
    delete[] helpMap; delete[] catNames; delete[] catCounts;
    return false;
}

// ============================================================================
// MAIN ORCHESTRATOR
// ============================================================================

int main() {
    cout << "           _    _             ____  " << endl;
    cout << "          / \\  | | __ _  ___ | __ ) " << endl;
    cout << "         / _ \\ | |/ _` |/ _ \\|  _ \\ " << endl;
    cout << "        / ___ \\| | (_| | (_) | |_) |" << endl;
    cout << "       /_/   \\_\\_|\\__, |\\___/|____/ " << endl;
    cout << "                  |___/             " << endl;
    cout << "          ALGORITHMIC BACKTESTER" << endl;
    cout << endl;
    
    double initialCapital;
    do {
        cout << "[System] Enter Initial Investment (PKR): ";
        cin >> initialCapital;
        if (initialCapital < 1000) cout << " >> Error: You need at least 1000 PKR to trade." << endl;
    } while (initialCapital < 1000);

    int rows = 0, cols = 0;
    double** marketData = loadMarketData("", rows, cols);
    configureStrategy(rows);

    displayMarketTable(marketData, rows);

    bool redo;
    do {
        double* closePrices = new double[rows];
        for (int i = 0; i < rows; i++) closePrices[i] = marketData[i][3];

        int* signals = generateSignals(closePrices, rows);
        double finalMetrics[METRICS_COUNT], *equityHistory = new double[rows];

        runBacktest(closePrices, signals, rows, initialCapital, 0.0, initialCapital, finalMetrics, equityHistory);
        setModuleCData(finalMetrics, equityHistory, rows);
        
        displaySummary();
        redo = startChatbot(marketData, rows);

        delete[] signals; delete[] closePrices; delete[] equityHistory;
    } while (redo);
    unloadMarketData(marketData, rows);
    
    cout << "\nProgram terminated. Goodbye, Have a Nice Trading Journey!" << endl;
    
    return 0;
}
