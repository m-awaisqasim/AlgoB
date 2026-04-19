#include <iostream>
using namespace std;

// --- Module B: Indicators & Signals (Ayan's missing logic) ---
// This module takes the dynamic array of prices and generates Trade Signals 
// using Pointers and Dynamic Array memory allocation.

int* generateSignals(double* prices, int daysCount) {
    
    // Dynamically allocate an array for signals (1=Buy, -1=Sell, 0=Hold)
    int* signals = new int[daysCount];
    
    // We will use a fast 3-day Moving Average vs 7-day Moving Average 
    // instead of 20/50 because our dataset only has 30 days.
    
    for (int day = 0; day < daysCount; day++) {
        
        // Before day 7, we don't have enough data to calculate the 7-day average
        if (day < 7) {
            signals[day] = 0; // HOLD
            continue;
        }
        
        // Calculate 3-day short average
        double sumShort = 0;
        for (int i = 0; i < 3; i++) {
            sumShort += prices[day - i];
        }
        double avgShort = sumShort / 3.0;

        // Calculate 7-day long average
        double sumLong = 0;
        for (int i = 0; i < 7; i++) {
            sumLong += prices[day - i];
        }
        double avgLong = sumLong / 7.0;

        // Apply Trading Rule
        if (avgShort > avgLong) {
            signals[day] = 1;  // BUY signal
        } else if (avgShort < avgLong) {
            signals[day] = -1; // SELL signal
        } else {
            signals[day] = 0;  // HOLD
        }
    }
    
    cout << "[Module B] Generated " << daysCount << " trading signals using MA Crossover Strategy." << endl;
    
    // Return the dynamic array pointer
    return signals;
}
