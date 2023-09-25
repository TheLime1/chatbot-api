#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <algorithm>
#include <sstream>

int main() {
    // Specify the paths for the input text and output CSV files
    std::string txtFilePath = "input.txt";
    std::string csvFilePath = "output.csv";

    // Open the input text file for reading
    std::ifstream txtFile(txtFilePath);
    if (!txtFile.is_open()) {
        std::cerr << "Failed to open text file: " << txtFilePath << std::endl;
        return 1;
    }

    // Open the output CSV file for writing
    std::ofstream csvFile(csvFilePath);
    if (!csvFile.is_open()) {
        std::cerr << "Failed to open CSV file: " << csvFilePath << std::endl;
        return 1;
    }

    // Read the one-line text
    std::string text;
    std::getline(txtFile, text);

    //find the number of "," between "#HEADER#" and the first "#RECORD#"
    std::string header = "#HEADER#";
    std::string record = "#RECORD#";
    std::string record2 = "RECORD";
    int headerPos = text.find(header);
    int recordPos = text.find(record);
    int numSep = std::count(text.begin() + headerPos, text.begin() + recordPos, ',');

    //save the number as a variable called TotalNumSep
    int totalNumSep = numSep;

    //remove the word "#HEADER#"
    text.erase(headerPos, header.length());

    //replace every "#RECORD#" with "\n" 
    std::replace(text.begin(), text.end(), '#', '\n');
    //remove every "#RECORD#" or "RECORD" until there is no "#RECORD#" or "RECORD"
    while (text.find(record) != std::string::npos) {
        text.erase(text.find(record), record.length());
    }
    while (text.find(record2) != std::string::npos) {
        text.erase(text.find(record2), record2.length());
    }

    //print the string to the new csv file in loop
    std::istringstream iss(text);
    std::string line;
    int currentNumSep = 0;
    while (std::getline(iss, line)) {
        csvFile << line;
        currentNumSep += std::count(line.begin(), line.end(), ',');
        if (currentNumSep == totalNumSep) {
            csvFile << "\n";
            currentNumSep = 0;
        }
    }

    // Close the files
    txtFile.close();
    csvFile.close();

    std::cout << "One-line text to CSV conversion successful." << std::endl;

    return 0;
}