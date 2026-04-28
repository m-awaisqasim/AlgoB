#include <iostream>
#include <fstream>
#include <sstream>

using namespace std;

int main()
{
    ifstream file("bitcoin_data.csv");
    string line,date,volume,open,high,close,low;
    if (!file) 
    {cout << "Error: file cannot open" << endl;        
    }
    else 
    {getline(file, line);
    }
    int rowCount = 0;
    while (getline(file, line)) {
        stringstream ss(line);
        getline(ss, date, ',');
        getline(ss, open, ',');
        getline(ss, high, ',');
        getline(ss, low, ',');
        getline(ss, close, ',');
        getline(ss, volume, ',');

        cout << "Date: " << date << endl;
        cout << "Open: " << open << endl;
        cout << "High: " << high << endl;
        cout << "Low: " << low << endl;
        cout << "Close: " << close << endl;
        cout << "Volume: " << volume << endl;
        cout << "------------" << endl;
        rowCount++;
    }

    cout << "\nTotal rows: " << rowCount << endl;    
    file.close();
    return 0;
}
