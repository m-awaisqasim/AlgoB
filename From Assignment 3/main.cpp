#include <iostream>
using namespace std;

#include "Module_D_Awais_Part3.cpp"

int main() {
    string cyan = "\033[36m"; 
    string gold = "\033[33m"; 
    string reset = "\033[0m"; 

    cout << cyan << endl;
    system("chcp 65001 > nul");
    cout << "\t\t    ████╗  ██╗                 █████╗                    " << endl;
    cout << "\t\t   ██╔═██╗ ██║  ████╗   ████╗  ██╔══██╗                   " << endl;
    cout << "\t\t   ██████║ ██║ ██╔═██╗ ██╔═██╗ ██████╔╝                   " << endl;
    cout << "\t\t   ██╔═██║ ██║ ██║ ██║ ██║ ██║ ██╔══██╗                   " << endl;
    cout << "\t\t   ██║ ██║ ██║ ╚█████║ ╚████╔╝ ██████╔╝                   " << endl;
    cout << "\t\t   ╚═╝ ╚═╝ ╚═╝  ╚══██║  ╚═══╝  ╚═════╝                    " << endl;
    cout << "\t\t                 ████║                                    " << endl;
    cout << "\t\t                 ╚═══╝                                   " << endl;
    cout << "\t\t" << gold << "      ALGORITHMIC BACKTESTER (V3.0) " << cyan << "" << endl;
    cout << reset << endl;

    receiveData();
    displaySummary();
    calculateRating();
    drawEquityCurve();
    startChatbot();

    cout << "\nProgram terminated. Goodbye, Have a Nice Trading Journey!" << endl;
    
    return 0;
}
