#include <iostream>
using namespace std;

const int METRICS_COUNT = 11;
const int MAX_DAYS = 1000;

// ============================================================================
// GLOBAL Varaibles
// ============================================================================
double capital, finalVal, totalReturn, annualReturn, sharpe, drawdown, winRate, avgProfit, avgLoss, profitFactor;
int totalTrades, wins, losses;
double price_data[MAX_DAYS];
int sma_short = 6, sma_long = 15, rsi_period = 5;

// ============================================================================
// MODULE A: DATA MANAGEMENT
// ============================================================================

int getLength(string s) {
    int count = 0;
    while (s[count] != '\0') count++;
    return count;
}

void loadMarketData(double matrix[MAX_DAYS][5], int &rows, int &cols) {
    do {
        cout << "\n[Module A] How many days of market data? (Minimum 2, Max " << MAX_DAYS << "): ";
        cin >> rows;
        if (rows < 2) cout << " >> Error: You need at least 2 days of data for analysis." << endl;
        if (rows > MAX_DAYS) cout << " >> Error: Max " << MAX_DAYS << " days allowed." << endl;
    } while (rows < 2 || rows > MAX_DAYS);

    cols = 5;
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
                        matrix[i][col++] = stod(part);
                    }
                }
                start = k + 1;
            }
        }
    }
}

void editMarketData(double matrix[MAX_DAYS][5], int rows) {
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

void displayMarketTable(double matrix[MAX_DAYS][5], int rows) {
    cout << endl;
    cout << "+-----+---------------+---------------+---------------+---------------+---------------+" << endl;
    cout << "| Day |     Open      |     High      |      Low      |     Close     |    Volume     |" << endl;
    cout << "+-----+---------------+---------------+---------------+---------------+---------------+" << endl;
    int previewRows;
    previewRows = (rows > 5) ? 5 : rows;
    for (int i = 1; i < previewRows; i++) {
        cout << "|  " << i + 1;
        cout << "  |";
        for (int j = 0; j < 5; j++) {
            cout << "\t" << matrix[i][j] << "\t";
        }
        cout << endl;
    }
    cout << "+-----+---------------+---------------+---------------+---------------+---------------+" << endl;
    if (rows > 5) cout << "  ... (" << rows - 5 << " more rows not shown)" << endl;
}

// ============================================================================
// MODULE B: SIGNAL GENERATION
// ============================================================================

double calculateSMA(double prices[], int currentIndex, int period) {
    double sum = 0;
    for (int i = currentIndex - period + 1; i <= currentIndex; i++) {
        sum = sum + prices[i];
    }
    return sum / period;
}

double calculateRSI(double prices[], int currentIndex, int period) {
    double gain = 0;
    double loss = 0;
    int startIndex = currentIndex - period + 1;
    for (int i = startIndex; i <= currentIndex; i++) {
        double change = prices[i] - prices[i - 1];
        if (change > 0) {
            gain = gain + change;
        } else {
            loss = loss - change;
        }
    }
    if (loss == 0) {
        return 100;
    }
    double rs = gain / loss;
    return 100 - (100 / (1 + rs));
}

void generateSignalsManual(double prices[MAX_DAYS], int daysCount, int signals[MAX_DAYS]) {
    for (int i = 0; i < daysCount; i++) {
        price_data[i] = prices[i];
        signals[i] = 0;
    }
    int start_day = (sma_long > rsi_period) ? sma_long : rsi_period;
    start_day++;

    for (int day = start_day; day < daysCount; day++) {
        int sma_sig = 0, rsi_sig = 0;
        double s_val = calculateSMA(price_data, day, sma_short);
        double l_val = calculateSMA(price_data, day, sma_long);
        if (s_val > l_val) sma_sig = 1;
        else if (s_val < l_val) sma_sig = -1;

        double rsi_val = calculateRSI(price_data, day, rsi_period);
        if (rsi_val < 30) rsi_sig = 1;
        else if (rsi_val > 70) rsi_sig = -1;

        if (sma_sig == 1 && rsi_sig >= 0) signals[day] = 1;
        else if (rsi_sig == 1 && sma_sig >= 0) signals[day] = 1;
        else if (sma_sig == -1 && rsi_sig <= 0) signals[day] = -1;
        else if (rsi_sig == -1 && sma_sig <= 0) signals[day] = -1;
    }
}

void configureStrategy(int totalDays) {
    char choice;
    cout << "\n[Module B] Use default strategy parameters? (SMA: 6/15, RSI: 5) (y/n): ";
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

void performFinalCalculations(int days_count, double portfolio_value, double initial_capital, double sum_returns, double sum_squared, int total_trades, int wins, int losses, double total_profit, double total_loss, double &final_value, double &total_return, double &annualized_return, double &sharpe_ratio, double &win_rate, double &avg_profit, double &avg_loss, double &profit_factor) {
    final_value = portfolio_value;
    total_return = (final_value / initial_capital) - 1;
    
    if (days_count >= 30) {
        double base = (1 + total_return);
        double Exp = 365.0 / days_count;
        annualized_return = 1;
        for (int i = 1; i <= Exp; i++) {
            annualized_return = annualized_return * base;  
        }
        annualized_return = annualized_return - 1;
    } else {
        annualized_return = total_return;  
    }
    
    double mean_return = sum_returns / days_count;    
    double variance = (sum_squared / days_count) - (mean_return * mean_return);  
    double std_dev = variance; 
    for (int i = 0; i < 20; i++) {
        if (std_dev > 0) 
            std_dev = 0.5 * (std_dev + variance / std_dev);
    } 

    if (std_dev != 0) 
        sharpe_ratio = mean_return / std_dev; 
    if (total_trades > 0) 
        win_rate = (double)wins / total_trades;
    if (wins > 0) 
        avg_profit = total_profit / wins;
    if (losses > 0) 
        avg_loss = total_loss / losses;
    if (total_loss > 0) 
        profit_factor = total_profit / total_loss;
}

void runBacktest(double prices[MAX_DAYS], int signals[MAX_DAYS], int daysCount, double initialCapital, double initialShares, double prevPortfolio, double results[]) {
    int days_count = daysCount, total_trades = 0, wins = 0, losses = 0;
    double cash = initialCapital, shares = initialShares, price, portfolio_value, previous_portfolio = prevPortfolio, max_drawdown = 0, total_return, annualized_return, sum_returns = 0, sum_squared = 0, initial_capital = initialCapital, peak = initial_capital, final_value, sharpe_ratio = 0, total_profit = 0, total_loss = 0, buy_price = 0, win_rate = 0, avg_profit = 0, avg_loss = 0, profit_factor = 0, daily_return;

    for (int day = 0; day < days_count; day++) {
        price = prices[day];      
        int signal = signals[day];  
        
        if (signal == 1 && shares == 0 && price > 0) {
            shares = cash / price; 
            cash = cash - shares * price; 
            buy_price = price; 
        } else if (signal == -1 && shares > 0) {
            cash = cash + shares * price; 
            double profit = (price - buy_price) * shares;
            total_trades = total_trades + 1;
            if (profit > 0) {
                wins = wins + 1;
                total_profit += profit;
            } else {
                losses = losses + 1;
                total_loss += -profit;
            }
            shares = 0;  
        }

        portfolio_value = cash + (shares * price);
        if (previous_portfolio != 0) daily_return = (portfolio_value - previous_portfolio) / previous_portfolio;
        else daily_return = 0;

        sum_returns += daily_return; 
        sum_squared += daily_return * daily_return; 
        previous_portfolio = portfolio_value; 

        if (portfolio_value > peak) peak = portfolio_value;
        double drawdown = (peak - portfolio_value) / peak; 
        if (drawdown > max_drawdown) max_drawdown = drawdown; 
    }
            
    performFinalCalculations(days_count, portfolio_value, initial_capital, sum_returns, sum_squared, total_trades, wins, losses, total_profit, total_loss, final_value, total_return, annualized_return, sharpe_ratio, win_rate, avg_profit, avg_loss, profit_factor);

    results[0] = initial_capital;
    results[1] = final_value;
    results[2] = total_return * 100;
    results[3] = annualized_return * 100;
    results[4] = sharpe_ratio;
    results[5] = max_drawdown * 100;
    results[6] = total_trades;
    results[7] = win_rate * 100;
    results[8] = avg_profit;
    results[9] = avg_loss;
    results[10] = profit_factor;
}

// ============================================================================
// MODULE D: PERFORMANCE UI & CHATBOT
// ============================================================================

void setModuleCData(double* results) {
    capital = results[0]; finalVal = results[1]; totalReturn = results[2]; annualReturn = results[3];
    sharpe = results[4]; drawdown = results[5]; totalTrades = (int)results[6]; winRate = results[7];
    avgProfit = results[8]; avgLoss = results[9]; profitFactor = results[10];
    wins = (int)((winRate / 100.0) * totalTrades + 0.5); losses = totalTrades - wins;
}

string formatPKR(double val) {
    if (val >= 1000000) return to_string((int)(val / 1000000)) + "M";
    if (val >= 1000) return to_string((int)(val / 1000)) + "K";
    return to_string((int)val);
}

void displaySummary() {
    string status = (totalReturn >= 0) ? "PROFITABLE" : "LOSS MAKING";

    cout << "\n  +------------------------------------------------------+" << endl;
    cout << "  |         STRATEGY PERFORMANCE SUMMARY                   " << endl;
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
    char bars[3][5] = {
        {'>', '>', '>', '>', '\0'},
        {'#', '#', '#', '#', '\0'},
        {'=', '=', '=', '=', '\0'}
    };
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

// bool findKeyword(string text, string key) {
//     int textLength = getLength(text);
//     int keyLength = getLength(key);

//     if (keyLength > textLength) return false;

//     for (int i = 0; i <= textLength - keyLength; i++) {
//         int j = 0;
//         while (j < keyLength && text[i + j] == key[j]) {
//             j++;
//         }
//         if (j == keyLength) return true;
//     }
//     return false;
// }

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
        else if (findKeyword(userCmd, "return") || findKeyword(userCmd, "profit")) 
            cout << "Analysis: Total Return is " << totalReturn << "% and Annual Profit is Rs." << formatPKR((totalReturn/100)*capital) << "." << endl;
        else if (findKeyword(userCmd, "sharpe")) cout << "Risk-Adjusted: Your Sharpe Ratio is " << sharpe << "." << endl;
        else if (findKeyword(userCmd, "drawdown") || findKeyword(userCmd, "risk")) cout << "Risk: Max Drawdown reached " << drawdown << "% during the test." << endl;
        else if (findKeyword(userCmd, "winrate") || findKeyword(userCmd, "accuracy")) cout << "Accuracy: Your Win Rate is " << winRate << "%." << endl;
        else if (findKeyword(userCmd, "tradecount") || findKeyword(userCmd, "trades")) cout << "Volume: A total of " << totalTrades << " trades were executed." << endl;
        else if (findKeyword(userCmd, "advice")) {
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
        else if (findKeyword(userCmd, "settings")) {
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
    cout << "     SMA + RSI Crossover Strategy Engine " << endl;
    cout << endl;
    
    double initialCapital;
    do {
        cout << "[System] Enter Initial Investment (PKR): ";
        cin >> initialCapital;
        if (initialCapital < 1000) cout << " >> Error: You need at least 1000 PKR to trade." << endl;
    } while (initialCapital < 1000);

    double marketData[MAX_DAYS][5];
    int rows, cols;
    loadMarketData(marketData, rows, cols);
    configureStrategy(rows);

    displayMarketTable(marketData, rows);

    bool redo;
    do {
        double closePrices[MAX_DAYS];
        for (int i = 0; i < rows; i++) closePrices[i] = marketData[i][3];

        int signals[MAX_DAYS];
        generateSignalsManual(closePrices, rows, signals);
        double results[METRICS_COUNT];

        runBacktest(closePrices, signals, rows, initialCapital, 0.0, initialCapital, results);
        setModuleCData(results);
        
        displaySummary();
        redo = startChatbot(marketData, rows);
    } while (redo);
    
    cout << "\nProgram terminated. Goodbye, Have a Nice Trading Journey!" << endl;
    
    return 0;
}
