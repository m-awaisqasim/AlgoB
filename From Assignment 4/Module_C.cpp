#include <iostream>
#include <cmath>
using namespace std;

double get_positive_value(string);
void allocateData(double** &data, int days);
void collectData(double** &price_signal_data, int days_count);
void performFinalCalculations(int days_count, double portfolio_value, double initial_capital, double sum_returns, double sum_squared, int total_trades, int wins, int losses, double total_profit, double total_loss, double &final_value, double &total_return, double &annualized_return, double &sharpe_ratio, double &win_rate, double &avg_profit, double &avg_loss, double &profit_factor);
void display_result(double final_value, double total_return, double annualized_return, double sharpe_ratio, double max_drawdown, double total_trades, double  win_rate, double avg_profit, double avg_loss, double profit_factor);

int main()
    {
        int days_count, signal, total_trades = 0, wins = 0, losses = 0;
    
        double cash,shares, price, portfolio_value, previous_portfolio, max_drawdown = 0, total_return, annualized_return, mean_return, variance, daily_return, sum_returns = 0, sum_squared = 0,initial_capital, peak=initial_capital, final_value, std_dev, sharpe_ratio = 0, total_profit = 0, total_loss = 0, buy_price = 0, win_rate = 0, avg_profit = 0, avg_loss = 0, profit_factor = 0;
    
        cout << "+==============================================+" << endl;
        cout << "         ALGORITHMIC TRADING BACKTESTER            " << endl;
        cout << "          Module C - Result Reporting            " << endl;
        cout << "+==============================================+" << endl;
        cout << endl;
        
    // ==================== Ini Capital, Shares, Prev Prtfolio, Days Count & Error Handling ==================   
        cash = get_positive_value("Enter the Initial Capital: $");
        initial_capital = cash;
        shares = get_positive_value("Enter the Initial Amount of Shares: ");
        previous_portfolio = get_positive_value("Enter the Value of Previous Portfolio: $");
        days_count = (int)get_positive_value("Enter the Number of Days (From Module A): ");
        
            // === DATA COLLECTION PHASE ===
        double** price_signal_data;
        allocateData(price_signal_data, days_count);
        collectData(price_signal_data, days_count);
    
        // === BACKTESTING SIMULATION PHASE ===
        cout << "\n----- BACKTESTING SIMULATION -----\n";
        
        for(int day = 0; day < days_count; day++) 
        {
            price = price_signal_data[day][0];      // Get stored price
            int signal = (int)price_signal_data[day][1];  // Get stored signal
            
            cout << "\nDay " << (day + 1) << " (Price: $" << price << ", Signal: " << signal << ")\n";
        
                if(signal == 1 && shares == 0)  //This is for BUY
                    {
                        shares = cash / price; // your're buying shares with available cash
                        cash = cash - shares * price; // cash will be adjusted here
                        buy_price = price; // the buyig price is saved to calculate profit or loss later
            
                        cout << "Action: BUY " << shares << " shares\n";
                    }
        
                else if(signal == -1 && shares > 0) // This is the case to SELL
                    {
                        cash = cash + shares * price; 
                        double profit = (price - buy_price) * shares;
                        total_trades=total_trades + 1;
            
                        if(profit > 0)
                            {
                                wins = wins + 1;
                                total_profit += profit;
                            }
                        else
                            {
                                losses = losses + 1;
                                total_loss += -profit;
                            }
                
                        cout << "Action: SELL shares\n";
                        cout << "Trade Profit/Loss: $" << profit << endl;
                        
                         shares = 0;  // to reset shares after selling
                    }
        
                else // When to HOLD
                    { cout << "Action: HOLD\n"; }
                    
                // ==================== Portfolio Value & Daily Return Calculation ===========================
                portfolio_value = cash + (shares * price);
                cout << "Portfolio Value: $" << portfolio_value << endl;
                if(previous_portfolio != 0) // to prevent division by 0 of prev portfolio
                    { daily_return = (portfolio_value - previous_portfolio) / previous_portfolio; } // Percentage change 
                else 
                    { daily_return = 0; }
        
                sum_returns += daily_return; //This will add up daily returns
                sum_squared += daily_return * daily_return; //To calculate the variance
        
                previous_portfolio = portfolio_value; // This will update the previous portfolio value for the next day
                if(portfolio_value > peak)
                    peak = portfolio_value;
        
                double drawdown = (peak - portfolio_value) / peak; // to get percentage drop from peak value
                if(drawdown > max_drawdown)
                    max_drawdown = drawdown; // This will update max_drawdown if the current drawdown exceeds the previous max
            }
            
        // ==================== Final Calculations ===========================    
        performFinalCalculations(days_count, portfolio_value, initial_capital, sum_returns, sum_squared, total_trades, wins, losses, total_profit, total_loss, final_value, total_return, annualized_return, sharpe_ratio, win_rate, avg_profit, avg_loss, profit_factor);
            
            cout<<"-------------------------";
    
        display_result(final_value, total_return, annualized_return, sharpe_ratio, max_drawdown, total_trades, win_rate, avg_profit, avg_loss, profit_factor);
        
         // === Memory Cleanup ===
        for(int i = 0; i < days_count; i++) 
            { delete[] price_signal_data[i]; }
        delete[] price_signal_data;
        
        return 0;
    }
    
void allocateData(double** &data, int days)
    {
        data = new double* [days]; //allocates rows/pointers
        for(int i=0; i<days; i++)
            { data[i]=new double [2]; } // Each row has 2 columns: [price, signal]
    }
    
void collectData(double** &price_signal_data, int days_count) 
    {
        cout << "\n----- DATA COLLECTION PHASE -----\n";
        for(int day = 0; day < days_count; day++) 
            {
                cout << "\nDay " << (day + 1) << " Data:\n";
                
                // Column 0: Closing Price
                cout << "Enter closing price: $";
                cin >> price_signal_data[day][0];
                while(price_signal_data[day][0] <= 0) 
                    {
                        cout << "Warning: Invalid Price. Re-enter: $";
                        cin >> price_signal_data[day][0];
                    }
                
                // Column 1: Signal
                cout << "Enter Signal (1=Buy, -1=Sell, 0=Hold): ";
                cin >> price_signal_data[day][1];
                while(price_signal_data[day][1] != 1 && 
                      price_signal_data[day][1] != 0 && 
                      price_signal_data[day][1] != -1) 
                        {
                            cout << "Warning: Invalid Signal. Enter 1, 0, or -1: ";
                            cin >> price_signal_data[day][1];
                        }
                cout << "Data stored: Price=$" << price_signal_data[day][0] 
                     << ", Signal=" << (int)price_signal_data[day][1] << endl;
            }
        cout << "\n----- Data collection complete! -----\n";
    }
    
void performFinalCalculations(int days_count, double portfolio_value, double initial_capital, double sum_returns, double sum_squared, int total_trades, int wins, int losses, double total_profit, double total_loss, double &final_value, double &total_return, double &annualized_return, double &sharpe_ratio, double &win_rate, double &avg_profit, double &avg_loss, double &profit_factor) 
    {
        final_value = portfolio_value;
        total_return = (final_value / initial_capital) - 1;
        
        // If backtesting period is 30 days or longer, only then calculate annualized return. Otherwise for short periods, just use the total return to avoid over values.
        if (days_count >= 30) annualized_return = pow((1 + total_return), (365.0 / days_count)) - 1;
        else annualized_return = total_return;
    
        double mean_return = sum_returns / days_count;
        double variance = (sum_squared / days_count) - (mean_return * mean_return);
        double std_dev = sqrt(variance);
    
        if (std_dev != 0) sharpe_ratio = mean_return / std_dev;
        if (total_trades > 0) win_rate = (double)wins / total_trades;
        if (wins > 0) avg_profit = total_profit / wins;
        if (losses > 0) avg_loss = total_loss / losses;
        if (total_loss > 0) profit_factor = total_profit / total_loss;
    }

double get_positive_value(string text)
    {
        double value=0;
        cout<<text;
        cin>>value;
        while(value<0)
            { 
              cout <<"Invalid Entry! The Amount cannot be Negative.\n";
              cout<<"Re-Enter the Amount: ";
              cin>>value;
            }
        return value;
    }
    
void display_result(double final_value, double total_return, double annualized_return, double sharpe_ratio, double max_drawdown, double total_trades, double  win_rate, double avg_profit, double avg_loss, double profit_factor)
    {
        cout << "\nResults:\n";
    
        cout << "Final Portfolio Value = $" << final_value << endl;
        cout << "Total Return (%) = " << total_return * 100 << endl;
        cout << "Annualized Return (%) = " << annualized_return * 100 << endl;
        cout << "Sharpe Ratio = " << sharpe_ratio << endl;
        cout << "Max Drawdown (%) = " << max_drawdown * 100 << endl;
    
        cout << "\nTrade Statistics:\n";
    
        cout << "Total Trades = " << total_trades << endl;
        cout << "Win Rate (%) = " << win_rate * 100 << endl;
        cout << "Average Profit = " << avg_profit << endl;
        cout << "Average Loss = " << avg_loss << endl;
        cout << "Profit Factor = " << profit_factor << endl;
    }
