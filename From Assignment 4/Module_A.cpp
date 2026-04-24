#include <iostream>
#include <string>
using namespace std;

double** loadMarketData(string filename, int &rows, int &cols) {
    cout << "\n[Module A] How many days of market data? ";
    cin >> rows;
    cols = 5;

    double** priceMatrix = new double*[rows];
    for (int i = 0; i < rows; i++) {
        priceMatrix[i] = new double[cols];
    }

    cin.ignore();
    cout << "\n[System] You can now PASTE your data rows below:\n";
    for (int i = 0; i < rows; i++) {
        string line;
        cout << "Enter Day " << i + 1 << ": ";
        getline(cin, line);
        
        int col = 0, start = 0;
        for (int k = 0; k <= (int)line.length() && col < 5; k++) {
            if (k == (int)line.length() || line[k] == ',' || line[k] == ' ') {
                if (k > start) {
                    string part = "";
                    for (int m = start; m < k; m++) part += line[m];
                    if (part != "") {
                        priceMatrix[i][col++] = stod(part);
                    }
                }
                start = k + 1;
            }
        }
    }

    return priceMatrix;
}

void editMarketData(double** matrix, int rows) {
    char choice;
    cout << "\n[Module A] Do you want to edit any data? (y/n): ";
    cin >> choice;

    if (choice == 'y' || choice == 'Y') {
        int day, col;
        cout << "Enter Day number to edit (1-" << rows << "): ";
        cin >> day;
        cout << "Enter Column (1:Open, 2:High, 3:Low, 4:Close, 5:Vol): ";
        cin >> col;

        if (day >= 1 && day <= rows && col >= 1 && col <= 5) {
            cout << "Current value: " << matrix[day - 1][col - 1] << endl;
            cout << "Enter new value: ";
            cin >> matrix[day - 1][col - 1];
            cout << "[Success] Data updated!" << endl;
        } else {
            cout << "[Error] Invalid day or column!" << endl;
        }
   }
}

void displayMarketTable(double** matrix, int rows) {
    if (matrix == NULL) return;
    cout << "\nDay\tOpen\tHigh\tLow\tClose\tVolume" << endl;
    
    int previewRows = (rows > 5) ? 5 : rows;
    for (int i = 0; i < previewRows; i++) {
        cout << i + 1 << "\t";
        for (int j = 0; j < 5; j++) cout << matrix[i][j] << "\t";
        cout << endl;
    }
}

void unloadMarketData(double** matrix, int rows) {
    if (matrix == NULL) return;
    for (int i = 0; i < rows; i++) delete[] matrix[i];
    delete[] matrix;
}

