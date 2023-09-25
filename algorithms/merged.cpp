#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <algorithm>
#include <sstream>

// Function to convert CSV to one-line text
bool convertCsvToOneLine(const std::string& csvFilePath, const std::string& txtFilePath) {
    // Open the CSV file for reading
    std::ifstream csvFile(csvFilePath);
    if (!csvFile.is_open()) {
        std::cerr << "Failed to open CSV file: " << csvFilePath << std::endl;
        return false;
    }

    // Open the output text file for writing
    std::ofstream txtFile(txtFilePath);
    if (!txtFile.is_open()) {
        std::cerr << "Failed to create or open text file: " << txtFilePath << std::endl;
        csvFile.close();
        return false;
    }

    // Write the header
    std::string header;
    std::getline(csvFile, header);
    txtFile << "#HEADER#" << header << "#RECORD#";

    // Write the records
    std::string record;
    while (std::getline(csvFile, record)) {
        txtFile << record << "#RECORD#";
    }

    // Close the files
    csvFile.close();
    txtFile.close();

    std::cout << "CSV to one-line text conversion successful." << std::endl;
    return true;
}

// Function to convert one-line text to CSV
bool convertOneLineToCsv(const std::string& txtFilePath, const std::string& csvFilePath) {
    // Open the input text file for reading
    std::ifstream txtFile(txtFilePath);
    if (!txtFile.is_open()) {
        std::cerr << "Failed to open text file: " << txtFilePath << std::endl;
        return false;
    }

    // Open the output CSV file for writing
    std::ofstream csvFile(csvFilePath);
    if (!csvFile.is_open()) {
        std::cerr << "Failed to open CSV file: " << csvFilePath << std::endl;
        return false;
    }

    // Read the one-line text
    std::string text;
    std::getline(txtFile, text);

    // Find the number of "," between "#HEADER#" and the first "#RECORD#"
    std::string header = "#HEADER#";
    std::string record = "#RECORD#";
    std::string record2 = "RECORD";
    int headerPos = text.find(header);
    int recordPos = text.find(record);
    int numSep = std::count(text.begin() + headerPos, text.begin() + recordPos, ',');

    // Save the number as a variable called TotalNumSep
    int totalNumSep = numSep;

    // Remove the word "#HEADER#"
    text.erase(headerPos, header.length());

    // Replace every "#RECORD#" with "\n" 
    std::replace(text.begin(), text.end(), '#', '\n');
    // Remove every "#RECORD#" or "RECORD" until there is no "#RECORD#" or "RECORD"
    while (text.find(record) != std::string::npos) {
        text.erase(text.find(record), record.length());
    }
    while (text.find(record2) != std::string::npos) {
        text.erase(text.find(record2), record2.length());
    }

    // Print the string to the new CSV file in a loop
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
    return true;
}

int main() {
    // Example usage:
    std::string input_csvFile = "input.csv";
    std::string input_txtFile = "input.txt";
    std::string newTxtFile = "output.txt";
    std::string newCsvFile = "output.csv";

    // Convert CSV to one-line text
    if (convertCsvToOneLine(input_csvFile, newTxtFile)) {
        std::cout << "CSV to one-line text conversion successful." << std::endl;
    } else {
        std::cerr << "CSV to one-line text conversion failed." << std::endl;
    }

    // Convert one-line text to CSV
    if (convertOneLineToCsv(input_txtFile, newCsvFile)) {
        std::cout << "One-line text to CSV conversion successful." << std::endl;
    } else {
        std::cerr << "One-line text to CSV conversion failed." << std::endl;
    }

    return 0;
}
