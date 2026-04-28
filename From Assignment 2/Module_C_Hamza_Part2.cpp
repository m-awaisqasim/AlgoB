#include <iostream>
#include <cmath>
using namespace std;

int main()
{
    int days_count, day = 1 , signal, total_trades = 0, wins = 0, losses = 0;

    double cash,shares, price, portfolio_value, previous_portfolio,
           peak, max_drawdown = 0, total_return, annualized_return,
           mean_return, variance, daily_return, sum_returns = 0, sum_squared = 0,initial_capital,
           final_value, std_dev, sharpe_ratio = 0,
           total_profit = 0, total_loss = 0, buy_price = 0,
           win_rate = 0, avg_profit = 0, avg_loss = 0, profit_factor = 0;

    cout << "Enter the Initial Capital: $";
    cin >> cash;
    initial_capital = cash;
    
    cout<<"Enter Initial Amount of Shares: ";
    cin>>shares;
    
    cout << "Enter the Value of Previous Portfolio: $";
    cin >> previous_portfolio;
    
    peak =  previous_portfolio;
    
    cout << "Enter the Number of Days (From price_data in Modula A): ";
    cin >> days_count;

    cout << "\n----- Backtesting Simulation -----\n";

    while(day <= days_count)
        {
            cout << "\nDay " << day << endl;
            cout << "Enter closing price(From price_data in Modula A): $";
            cin >> price;
            cout << "Enter Signal from Module 2 (1=Buy, -1=Sell, 0=Hold): ";
            cin >> signal;
    
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
                    cout << "Trade Profit/Loss: " << profit << endl;
                    
                     shares = 0;  // to reset shares after selling
                }
    
            else // When to HOLD
                {
                    cout << "Action: HOLD\n";
                }
    
            portfolio_value = cash + (shares * price);
    
            cout << "Portfolio Value: $" << portfolio_value << endl;
    
            if(previous_portfolio != 0) // to prevent division by 0 of prev portfolio
                { 
                 daily_return = (portfolio_value - previous_portfolio) / previous_portfolio; // Percentage change
                }
            else
                {
                    daily_return = 0;
                }
    
            sum_returns += daily_return; //This will add up daily returns
            sum_squared += daily_return * daily_return; //To calculate the variance
    
            previous_portfolio = portfolio_value; // This will update the previous portfolio value for the next day
    
            if(portfolio_value > peak)
                peak = portfolio_value;
    
            double drawdown = (peak - portfolio_value) / peak; // to get percentage drop from peak value
    
            if(drawdown > max_drawdown)
                max_drawdown = drawdown; // This will update max_drawdown if the current drawdown exceeds the previous max
    
            day++;
        }

    final_value = portfolio_value; // = cash + shares * price

    total_return = (final_value / initial_capital) - 1;

    if(days_count >= 30) // only annualize for a month or longer
    annualized_return = pow((1 + total_return), (365.0 / days_count)) - 1;
   else
    annualized_return = total_return; // for short periods, just show total return

    mean_return = sum_returns / days_count;

    variance = (sum_squared / days_count) - (mean_return * mean_return);

    std_dev = sqrt(variance);

    if(std_dev != 0)
        sharpe_ratio = mean_return / std_dev;

    if(total_trades > 0)
        win_rate = (double)wins / total_trades;

    if(wins > 0)
        avg_profit = total_profit / wins;

    if(losses > 0)
        avg_loss = total_loss / losses;

    if(total_loss > 0)
        profit_factor = total_profit / total_loss;
        
        cout<<"-------------------------";

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

    // ----- New: Copy-Paste String for Module D -----
    cout << "\n-------------------------------------------------------" << endl;
    cout << "DATA FOR MODULE D (Copy the line below):" << endl;
    cout << initial_capital << " " << final_value << " " << total_return * 100 << " " 
         << annualized_return * 100 << " " << sharpe_ratio << " " << max_drawdown * 100 << " "
         << total_trades << " " << win_rate * 100 << " " << avg_profit << " " 
         << avg_loss << " " << profit_factor << endl;
    cout << "-------------------------------------------------------" << endl;

    return 0;
}