#include <iostream>
using namespace std;

const int MAX_DAYS = 1000;

double price_data[MAX_DAYS];
int sma_short = 6, sma_long = 15, rsi_period = 5;

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
        
        if (s_val > l_val) {
            sma_sig = 1;
        }
        else if (s_val < l_val) {
            sma_sig = -1;
        }

        double rsi_val = calculateRSI(price_data, day, rsi_period);
        if (rsi_val < 30) {
            rsi_sig = 1;
        }
        else if (rsi_val > 70) {
            rsi_sig = -1;
        }

        if (sma_sig == 1 && rsi_sig >= 0) {
            signals[day] = 1;       
        }
        else if (rsi_sig == 1 && sma_sig >= 0) {
            signals[day] = 1;       
        }
        else if (sma_sig == -1 && rsi_sig <= 0) {
            signals[day] = -1;      
        }
        else if (rsi_sig == -1 && sma_sig <= 0) {
            signals[day] = -1;      
        }
    }
}

void configureStrategy(int totalDays) {
    char choice;
    cout << "\n[Module B] Use default strategy parameters? (SMA: 6/15, RSI: 5) (y/n): ";
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