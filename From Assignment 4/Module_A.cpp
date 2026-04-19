#include <iostream>
#include <fstream>
#include <string>
using namespace std;

double** loadMarketData(string filename, int &rows, int &cols) {
    
    ifstream countFile(filename);
    rows = 0;
    cols = 5;

    if (!countFile.is_open()) {
        cout << "[Module A] Error: Could not find " << filename << endl;
        return NULL;
    }

    string line;
    while (getline(countFile, line)) {
        if (line.length() > 0) rows++;
    }
    countFile.close();

    double** priceMatrix = new double*[rows];
    for (int i = 0; i < rows; i++) {
        priceMatrix[i] = new double[cols];
    }

    ifstream dataFile(filename);
    int currentRow = 0;
    bool isHeader = true;

    while (getline(dataFile, line) && currentRow < rows) {
        if (isHeader || line.empty()) { 
            isHeader = false; 
            continue; 
        }

        size_t start = 0;
        size_t end = line.find(',');
        
        for (int segmentIdx = 0; segmentIdx < 6; segmentIdx++) {
            string segment;
            if (end != string::npos) {
                segment = line.substr(start, end - start);
                start = end + 1;
                end = line.find(',', start);
            } else {
                segment = line.substr(start);
            }
            
            if (segmentIdx == 0) continue;

            int matrixCol = segmentIdx - 1;

            bool isNumeric = !segment.empty();
            for(char c : segment) {
                if(!isdigit(c) && c != '.' && c != '-') {
                    isNumeric = false;
                    break;
                }
            }

            if (isNumeric) {
                priceMatrix[currentRow][matrixCol] = stod(segment);
            } else {
                priceMatrix[currentRow][matrixCol] = 0;
            }
        }
        currentRow++;
    }
    dataFile.close();

    rows = currentRow; 
    cout << "[Module A] Successfully loaded " << rows << " records from " << filename << endl;
    return priceMatrix;
}

void displayMarketTable(double** matrix, int rows) {
    if (matrix == NULL) return;
    cout << "\n--- Market Data (Preview) ---" << endl;
    cout << "Day\tOpen\tHigh\tLow\tClose\tVolume" << endl;
    cout << "--------------------------------------------------------" << endl;
    
    int previewRows = (rows > 5) ? 5 : rows;
    for (int i = 0; i < previewRows; i++) {
        cout << i + 1 << "\t";
        for (int j = 0; j < 5; j++) {
            cout << matrix[i][j] << "\t";
        }
        cout << endl;
    }
}

void unloadMarketData(double** matrix, int rows) {
    if (matrix == NULL) return;
    for (int i = 0; i < rows; i++) {
        delete[] matrix[i];
    }
    delete[] matrix;
    cout << "[Module A] Memory Released." << endl;
}
