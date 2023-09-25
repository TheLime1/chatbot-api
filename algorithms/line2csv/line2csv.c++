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

    // Find the number of "," between "#HEADER#" and the first "#RECORD#"
    std::string header = "#HEADER#";
    std::string record = "#RECORD#";
    size_t headerPos = text.find(header);
    size_t recordPos = text.find(record);
    if (headerPos == std::string::npos || recordPos == std::string::npos) {
        std::cerr << "Failed to find header or record in text." << std::endl;
        return 1;
    }
    std::string headerToRecord = text.substr(headerPos + header.length(), recordPos - headerPos - header.length());
    int totalNumSep = std::count(headerToRecord.begin(), headerToRecord.end(), ',');
    totalNumSep++; // Add one for the last column

    // Remove every "#HEADER#" and every "#RECORD#" from the string
    text.erase(headerPos, header.length());
    text.erase(recordPos - record.length(), record.length());

    // Print the string to the new csv file in loop
    std::istringstream iss(text);
    std::string token;
    int currentNumSep = 0;
    while (std::getline(iss, token, ',')) {
        csvFile << token;
        currentNumSep++;
        if (currentNumSep == totalNumSep) {
            csvFile << "\n";
            currentNumSep = 0;
        } else {
            csvFile << ",";
        }
    }

    // Close the files
    txtFile.close();
    csvFile.close();

    std::cout << "One-line text to CSV conversion successful." << std::endl;

    return 0;
}