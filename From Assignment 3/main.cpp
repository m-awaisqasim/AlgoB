#include <iostream>
using namespace std;

#include "Module_D_Awais_Part3.cpp"

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

    receiveData();
    displaySummary();
    drawEquityCurve();
    showVisualChart();
    startChatbot();

    cout << "\nProgram terminated. Goodbye, Have a Nice Trading Journey!" << endl;
    
    return 0;
}
