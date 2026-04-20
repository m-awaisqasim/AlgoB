#include <iostream>
#include <cmath>
using namespace std;

void performFinalCalculations(int days_count, double portfolio_value, double initial_capital, double sum_returns, double sum_squared, int total_trades, int wins, int losses, double total_profit, double total_loss, double &final_value, double &total_return, double &annualized_return, double &sharpe_ratio, double &win_rate, double &avg_profit, double &avg_loss, double &profit_factor) 
    {
        final_value = portfolio_value;
        total_return = (final_value / initial_capital) - 1;
        
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

void runBacktest(const double* prices, const int* signals, int daysCount, double initialCapital, double initialShares, double prevPortfolio, double results[], double history[])
    {
        int days_count = daysCount, signal, total_trades = 0, wins = 0, losses = 0;
    
        double cash = initialCapital, shares = initialShares, price, portfolio_value, previous_portfolio = prevPortfolio, max_drawdown = 0, total_return, annualized_return, mean_return, variance, daily_return, sum_returns = 0, sum_squared = 0,initial_capital = initialCapital, peak=initial_capital, final_value, std_dev, sharpe_ratio = 0, total_profit = 0, total_loss = 0, buy_price = 0, win_rate = 0, avg_profit = 0, avg_loss = 0, profit_factor = 0;
    
        for(int day = 0; day < days_count; day++) 
        {
            price = prices[day];      
            int signal = signals[day];  
            
                if(signal == 1 && shares == 0)  
                    {
                        shares = cash / price; 
                        cash = cash - shares * price; 
                        buy_price = price; 
                    }
        
                else if(signal == -1 && shares > 0) 
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
                        
                         shares = 0;  
                    }
        
                portfolio_value = cash + (shares * price);
                history[day] = portfolio_value;
                if(previous_portfolio != 0) 
                    { daily_return = (portfolio_value - previous_portfolio) / previous_portfolio; } 
                else 
                    { daily_return = 0; }
        
                sum_returns += daily_return; 
                sum_squared += daily_return * daily_return; 
        
                previous_portfolio = portfolio_value; 
                if(portfolio_value > peak)
                    peak = portfolio_value;
        
                double drawdown = (peak - portfolio_value) / peak; 
                if(drawdown > max_drawdown)
                    max_drawdown = drawdown; 
            }
            
        performFinalCalculations(days_count, portfolio_value, initial_capital, sum_returns, sum_squared, total_trades, wins, losses, total_profit, total_loss, final_value, total_return, annualized_return, sharpe_ratio, win_rate, avg_profit, avg_loss, profit_factor);
            
        results[0] = initial_capital;
        results[1] = final_value;
        results[2] = total_return * 100.0;
        results[3] = annualized_return * 100.0;
        results[4] = sharpe_ratio;
        results[5] = max_drawdown * 100.0;
        results[6] = (double)total_trades;
        results[7] = win_rate * 100.0;
        results[8] = avg_profit;
        results[9] = avg_loss;
        results[10] = profit_factor;
    }
