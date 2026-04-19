#include <iostream>
using namespace std;

#include "Module_D_Awais_Part4.cpp"

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
    
// Passing simulated results from Module C using an Array
    double finalMetrics[10];
    finalMetrics[0] = 10000.0;     // Initial Capital
    finalMetrics[1] = 13636.4;     // Final Value
    finalMetrics[2] = 36.3636;     // Total Return
    finalMetrics[3] = 36.3636;     // Annualized Return
    finalMetrics[4] = 1.0;         // Sharpe Ratio
    finalMetrics[5] = 0.0;         // Drawdown
    finalMetrics[6] = 100.0;       // Win Rate
    finalMetrics[7] = 3636.36;     // Average Profit
    finalMetrics[8] = 0.0;         // Average Loss
    finalMetrics[9] = 0.0;         // Profit Factor
    
    int trades = 1;

    // Call the integrated function using pointers (arrays pass as pointers natively)
    setModuleCData(finalMetrics, trades);
    displaySummary();
    drawEquityCurve();
    showVisualChart();
    startChatbot();
    
    cout << "\nProgram terminated. Goodbye, Have a Nice Trading Journey!" << endl;
    
    return 0;
}
