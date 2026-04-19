#include <iostream>
using namespace std;

int* generateSignals(double* prices, int daysCount) {
    
    int* signals = new int[daysCount];
    
    for (int day = 0; day < daysCount; day++) {
        
        if (day < 7) {
            signals[day] = 0; 
            continue;
        }
        
        double sumShort = 0;
        for (int i = 0; i < 3; i++) {
            sumShort += prices[day - i];
        }
        double avgShort = sumShort / 3.0;

        double sumLong = 0;
        for (int i = 0; i < 7; i++) {
            sumLong += prices[day - i];
        }
        double avgLong = sumLong / 7.0;

        if (avgShort > avgLong) {
            signals[day] = 1;  
        } else if (avgShort < avgLong) {
            signals[day] = -1; 
        } else {
            signals[day] = 0;  
        }
    }
    
    cout << "[Module B] Generated " << daysCount << " trading signals using MA Crossover Strategy." << endl;
    
    return signals;
}
