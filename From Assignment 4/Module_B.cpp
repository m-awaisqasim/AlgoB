#include <iostream>
#include <string>
using namespace std;

double* price_data;

double SMA(int day, int period)
{
    double total = 0;
    for (int i = day - period + 1; i <= day; i++)
    {
        total = total + price_data[i];
    }
    return total / period;
}

double RSI(int day, int period)
{
    double gain = 0;
    double loss = 0;
    for (int i = day - period + 1; i <= day; i++)
    {
        double change = price_data[i] - price_data[i - 1];
        if (change > 0) gain = gain + change;
        else loss = loss + (-change);
    }
    double avg_gain = gain / period;
    double avg_loss = loss / period;
    if (avg_loss == 0) return 100;
    double rs = avg_gain / avg_loss;
    double rsi = 100 - (100 / (1 + rs));
    return rsi;
}

int generateSMASignal(int day, int short_period, int long_period)
{
    if (day <= long_period) return 0;
    double prev_short = SMA(day - 1, short_period);
    double prev_long = SMA(day - 1, long_period);
    double current_short = SMA(day, short_period);
    double current_long = SMA(day, long_period);

    if (prev_short <= prev_long && current_short > current_long)
    {
        return 1;   // Buy
    }
    else if (prev_short >= prev_long && current_short < current_long)
    {
        return -1;  // Sell
    }

    return 0;   // Hold
}

int generateRSISignal(double rsi)
{
    if (rsi < 30)
    {
        return 1;   // Buy
    }
    else if (rsi > 70)
    {
        return -1;  // Sell
    }

    return 0;   // Hold
}

int generateFinalSignal(int sma_signal, int rsi_signal)
{
    if ((sma_signal == 1 || rsi_signal == 1) && !(sma_signal == -1 || rsi_signal == -1)) return 1;
    else if ((sma_signal == -1 || rsi_signal == -1) && !(sma_signal == 1 || rsi_signal == 1)) return -1;
    return 0;
}

int* generateSignals(double* prices, int daysCount) 
{
    price_data = prices;
    int* final_signals = new int[daysCount];
    
    int short_period = 20;
    int long_period = 50;
    int rsi_period = 14;

    for (int i = 0; i < daysCount; i++) final_signals[i] = 0;

    if (daysCount > long_period) 
    {
        for (int day = long_period; day < daysCount; day++)
        {
            int sma_sig = generateSMASignal(day, short_period, long_period);
            double rsi_val = RSI(day, rsi_period);
            int rsi_sig = generateRSISignal(rsi_val);
            final_signals[day] = generateFinalSignal(sma_sig, rsi_sig);
        }
    }

    cout << "\n[Module B] Generated " << daysCount << " signals using Ayan's RSI/SMA logic." << endl;
    return final_signals;
}