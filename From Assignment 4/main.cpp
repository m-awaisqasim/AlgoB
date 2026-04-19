#include <iostream>
using namespace std;

const int METRICS_COUNT = 11;

double** loadMarketData(string filename, int& rows, int& cols);
void displayMarketTable(double** matrix, int rows);
void unloadMarketData(double** matrix, int rows);
int* generateSignals(double* prices, int daysCount);
void runBacktest(
    const double* prices,
    const int* signals,
    int daysCount,
    double initialCapital,
    double initialShares,
    double previousPortfolio,
    double* outMetrics
);
void setModuleCData(double* metrics);
void displaySummary();
void showVisualChart();
void drawEquityCurve();
void startChatbot();

#include "Module_A.cpp"
#include "Module_B.cpp"
#include "Module_C.cpp"
#include "Module_D.cpp"

int main() {
    system("chcp 65001 > nul");
    cout << "\t\t    ████╗  ██╗                 █████╗      " << endl;
    cout << "\t\t   ██╔═██╗ ██║  ████╗   ████╗  ██╔══██╗    " << endl;
    cout << "\t\t   ██████║ ██║ ██╔═██╗ ██╔═██╗ ██████╔╝    " << endl;
    cout << "\t\t   ██╔═██║ ██║ ██║ ██║ ██║ ██║ ██╔══██╗    " << endl;
    cout << "\t\t   ██║ ██║ ██║ ╚█████║ ╚████╔╝ ██████╔╝    " << endl;
    cout << "\t\t   ╚═╝ ╚═╝ ╚═╝  ╚══██║  ╚═══╝  ╚═════╝     " << endl;
    cout << "\t\t                 ████║                     " << endl;
    cout << "\t\t                 ╚═══╝                     " << endl;
    
    string filename;
    cout << "\n[System] Enter Market Data File (e.g., BitcoinYTD.csv): ";
    cin >> filename;

    int rows = 0;
    int cols = 0;
    double** marketData = loadMarketData(filename, rows, cols);
    if (marketData == NULL || rows <= 0) {
        cout << "Failed to load market data. Program exiting." << endl;
        return 1;
    }

    displayMarketTable(marketData, rows);

    double* closePrices = new double[rows];
    for (int i = 0; i < rows; i++) {
        closePrices[i] = marketData[i][3];
    }

    int* signals = generateSignals(closePrices, rows);

    double finalMetrics[METRICS_COUNT];
    double initialCapital = 10000.0;
    double initialShares = 0.0;
    double previousPortfolio = initialCapital;

    runBacktest(
        closePrices,
        signals,
        rows,
        initialCapital,
        initialShares,
        previousPortfolio,
        finalMetrics
    );

    setModuleCData(finalMetrics);
    displaySummary();
    drawEquityCurve();
    showVisualChart();
    startChatbot();

    delete[] signals;
    delete[] closePrices;
    unloadMarketData(marketData, rows);
    
    cout << "\nProgram terminated. Goodbye, Have a Nice Trading Journey!" << endl;
    
    return 0;
}
