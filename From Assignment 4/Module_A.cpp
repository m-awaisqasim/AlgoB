#include <iostream>
#include <string>
using namespace std;

const int MAX_DAYS = 1000;

int getLength(string s) {
    int count = 0;
    while (s[count] != '\0') count++;
    return count;
}

void loadMarketData(double matrix[MAX_DAYS][5], int &rows, int &cols) {
    do {
        cout << "\n[Module A] How many days of market data? (Minimum 2, Max " << MAX_DAYS << "): ";
        cin >> rows;
        if (rows < 2) cout << " >> Error: You need at least 2 days of data for analysis." << endl;
        if (rows > MAX_DAYS) cout << " >> Error: Max " << MAX_DAYS << " days allowed." << endl;
    } while (rows < 2 || rows > MAX_DAYS);

    cols = 5;
    cin.ignore();
    cout << "\n[System] You can now PASTE your data rows below:\n";
    for (int i = 0; i < rows; i++) {
        string line;
        cout << "Enter Day " << i + 1 << ": ";
        getline(cin, line);
        int col = 0, start = 0;
        for (int k = 0; k <= getLength(line) && col < 5; k++) {
            if (k == getLength(line) || line[k] == ',' || line[k] == ' ') {
                if (k > start) {
                    string part = "";
                    for (int m = start; m < k; m++) part += line[m];
                    if (part != "") {
                        matrix[i][col++] = stod(part);
                    }
                }
                start = k + 1;
            }
        }
    }
}

void editMarketData(double matrix[MAX_DAYS][5], int rows) {
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

void displayMarketTable(double matrix[MAX_DAYS][5], int rows) {
    cout << endl;
    cout << "+-----+---------------+---------------+---------------+---------------+---------------+" << endl;
    cout << "| Day |     Open      |     High      |      Low      |     Close     |    Volume     |" << endl;
    cout << "+-----+---------------+---------------+---------------+---------------+---------------+" << endl;
    int previewRows;
    if (rows > 5) {
        previewRows = 5;
    }
    else {
        previewRows = rows;
    }
    for (int i = 0; i < previewRows; i++) {
        cout << "|  " << i + 1;
        if (i + 1 < 10) cout << "  |";
        else if (i + 1 < 100) cout << " |";
        else cout << "|";
        for (int j = 0; j < 5; j++) {
            cout << "\t" << matrix[i][j] << "\t";
        }
        cout << endl;
    }
    cout << "+-----+---------------+---------------+---------------+---------------+---------------+" << endl;
    if (rows > 5) cout << "  ... (" << rows - 5 << " more rows not shown)" << endl;
}
