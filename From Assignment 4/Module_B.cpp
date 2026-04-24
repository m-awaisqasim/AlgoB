#include <iostream>
using namespace std;

double* price_data;
int sma_short = 5, sma_long = 20, rsi_period = 14;

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
        if (change > 0) {
            gain += change; 
        }
        else {
            loss += (-change);
        }
    }
    double avg_gain = gain / period; 
    double avg_loss = loss / period;
    if (avg_loss == 0){
        return 100;
    }
    double rs = avg_gain / avg_loss;
    return 100 - (100 / (1 + rs));
}

int* generateSignals(double* prices, int daysCount) {
    price_data = prices;
    int* final_signals = new int[daysCount];
    for (int i = 0; i < daysCount; i++) {
        final_signals[i] = 0;
    }
    int start_day = (sma_long > rsi_period) ? sma_long : rsi_period;
    start_day++;
    for (int day = start_day; day < daysCount; day++) {
        double prev_short = SMA(day - 1, sma_short); 
        double prev_long = SMA(day - 1, sma_long);
        double curr_short = SMA(day, sma_short); 
        double curr_long = SMA(day, sma_long);
        int sma_sig = 0;
        if (prev_short <= prev_long && curr_short > curr_long) {
            sma_sig = 1;
        }
        else if (prev_short >= prev_long && curr_short < curr_long) {
            sma_sig = -1;
        }
        double rsi_val = RSI(day, rsi_period);
        int rsi_sig = 0;
        if (rsi_val < 30) {
            rsi_sig = 1; 
        }
        else if (rsi_val > 70) {
            rsi_sig = -1;
        }
        if ((sma_sig == 1 || rsi_sig == 1) && !(sma_sig == -1 || rsi_sig == -1)) {
            final_signals[day] = 1;
        }
        else if ((sma_sig == -1 || rsi_sig == -1) && !(sma_sig == 1 || rsi_sig == 1)) {
            final_signals[day] = -1;
        }
    }
    return final_signals;
}

void configureStrategy(int totalDays) {
    char choice;
    cout << "\n[Module B] Use default strategy parameters? (SMA: 5/20, RSI: 14) (y/n): ";
    cin >> choice;
    if (choice == 'n' || choice == 'N') {
        bool valid = false;
        while (!valid) {
            cout << "Enter SMA Short Period: "; 
            cin >> sma_short;
            cout << "Enter SMA Long Period: ";  
            cin >> sma_long;
            cout << "Enter RSI Period: ";      
            cin >> rsi_period;
            if (sma_short <= 0 || sma_long <= 0 || rsi_period <= 0) {
                cout << " >> Error: All periods must be positive." << endl;
            }
            else if (sma_short >= sma_long) {
                cout << " >> Error: SMA Short must be less than SMA Long." << endl;
            }
            else if (sma_long >= totalDays || rsi_period >= totalDays) {
                cout << " >> Error: Periods cannot exceed data days (" << totalDays << ")." << endl;
            }
            else {
                valid = true;
            }
            if (!valid) {
                cout << "Please try again.\n";
            }
        }
    }
}