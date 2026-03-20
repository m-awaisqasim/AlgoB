// Module D - Result Reporting & Chat-bot
// Name: Awais

#include <iostream>
#include <string>
using namespace std;

int main() {

    // Variables used in Module C (Hamza)
    double initial_capital, final_value, total_return, annualized_return, 
           sharpe_ratio, max_drawdown, win_rate, avg_profit, 
           avg_loss, profit_factor, total_profit, total_loss;
    int total_trades, wins, losses;

    string input;
    bool chat_active;
    int i;

    // ==================== START MODULE D ==========================
    cout << endl;
    cout << "+======================================================+" << endl;
    cout << "|         ALGORITHMIC TRADING BACKTESTER                |" << endl;
    cout << "|     Module D - Result Reporting & Chat-bot            |" << endl;
    cout << "|              Awais (25i-5647)                         |" << endl;
    cout << "+======================================================+" << endl;
    cout << endl;

    // These values come from Module C's output
    cout << "--- Paste the Result String from Module C Below ---" << endl;
    cout << "> ";
    cin >> initial_capital >> final_value >> total_return >> annualized_return 
        >> sharpe_ratio >> max_drawdown >> total_trades >> win_rate 
        >> avg_profit >> avg_loss >> profit_factor;

    total_return /= 100.0;
    annualized_return /= 100.0;
    max_drawdown /= 100.0;

    wins = (int)((win_rate / 100.0) * total_trades + 0.5); 
    losses = total_trades - wins;
    total_profit = wins * avg_profit;
    total_loss = losses * avg_loss;
    win_rate /= 100.0; 

    cin.ignore(1000, '\n');

    // ==================== PRINT SUMMARY ===========================
    cout << endl;
    cout << "+======================================================+" << endl;
    cout << "|           BACKTESTING PERFORMANCE SUMMARY             |" << endl;
    cout << "+======================================================+" << endl;
    cout << endl;
    cout << "  --- Portfolio Overview ---" << endl;
    cout << "  Initial Capital         : $" << initial_capital << endl;
    cout << "  Final Portfolio Value   : $" << final_value << endl;
    cout << endl;
    cout << "  --- Performance Metrics ---" << endl;
    cout << "  Total Return (%)        : " << total_return * 100 << "%" << endl;
    cout << "  Annualized Return (%)   : " << annualized_return * 100 << "%" << endl;
    cout << "  Sharpe Ratio            : " << sharpe_ratio << endl;
    cout << "  Max Drawdown (%)        : " << max_drawdown * 100 << "%" << endl;
    cout << endl;
    cout << "  --- Trade Statistics ---" << endl;
    cout << "  Total Trades            : " << total_trades << endl;
    cout << "  Winning Trades          : " << wins << endl;
    cout << "  Losing Trades           : " << losses << endl;
    cout << "  Win Rate (%)            : " << win_rate * 100 << "%" << endl;
    cout << "  Average Profit (Wins)   : $" << avg_profit << endl;
    cout << "  Average Loss (Losses)   : $" << avg_loss << endl;
    cout << "  Profit Factor           : " << profit_factor << endl;

    // ==================== INITIALIZE CHATBOT ====================
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
    cout << "  |  loss         - Show losing trade stats          |" << endl;
    cout << "  |  winning      - Show winning trade stats         |" << endl;
    cout << "  |  win          - Show winning trade stats         |" << endl;
    cout << "  |  tradecount   - Show number of trades            |" << endl;
    cout << "  |  winrate      - Show win rate percentage         |" << endl;
    cout << "  |  help         - Show this help menu              |" << endl;
    cout << "  |  exit         - Exit the chat                    |" << endl;
    cout << "  +--------------------------------------------------+" << endl;
    cout << endl;
    cout << "  Enter a keyword from the list above:" << endl;
    cout << endl;

    cout << "> ";
    cin >> input;

    if (input == "totalreturn" || input == "profit") {
        cout << "\n  --- Total Return ---" << endl;
        cout << "  Total Return      : " << total_return * 100 << "%" << endl;
        cout << "  Annualized Return : " << annualized_return * 100 << "%" << endl;
        cout << "  Average Profit (Wins): "<< avg_profit << endl;
    }
    else if (input == "sharpe") {
        cout << "\n  --- Sharpe Ratio ---" << endl;
        cout << "  Sharpe Ratio : " << sharpe_ratio << endl;
    }
    else if (input == "drawdown") {
        cout << "\n  --- Max Drawdown ---" << endl;
        cout << "  Max Drawdown : " << max_drawdown * 100 << "%" << endl;
    }
    else if (input == "losing" || input == "loss") {
        cout << "\n  --- Losing Trades ---" << endl;
        cout << "  Number of Losing Trades : " << losses << endl;
        cout << "  Total Loss Amount       : $" << total_loss << endl;
        cout << "  Average Loss per Trade  : $" << avg_loss << endl;
    }
    else if (input == "winrate") {
        cout << "\n  --- Win Rate ---" << endl;
        cout << "  Win Rate : " << win_rate * 100 << "%" << endl;
    }
    else if (input == "winning" || input == "win") {
        cout << "\n  --- Winning Trades ---" << endl;
        cout << "  Number of Winning Trades : " << wins << endl;
        cout << "  Total Profit Amount      : $" << total_profit << endl;
        cout << "  Average Profit per Trade : $" << avg_profit << endl;
    }
    else if (input == "tradecount" || input == "nooftrades") {
        cout << "\n  --- Trade Count ---" << endl;
        cout << "  Total Trades : " << total_trades << endl;
        cout << "  Wins         : " << wins << endl;
        cout << "  Losses       : " << losses << endl;
    }
    else {
        cout << "\n  Keyword not recognized." << endl;
    }

    cout << endl;
    cout << "  Analysis Complete. Goodbye!" << endl;

    return 0;
}
