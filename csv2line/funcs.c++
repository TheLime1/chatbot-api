#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>

// Function to convert CSV to 1-line database
bool convertCsvToLine(const std::string& csvFilePath, const std::string& txtFilePath) {
    std::ifstream inputFile(csvFilePath);
    if (!inputFile.is_open()) {
        std::cerr << "Failed to open CSV file: " << csvFilePath << std::endl;
        return false;
    }

    std::ofstream outputFile(txtFilePath);
    if (!outputFile.is_open()) {
        std::cerr << "Failed to create or open text file: " << txtFilePath << std::endl;
        inputFile.close();
        return false;
    }

    // Write the header
    outputFile << "#HEADER#";
    std::string header;
    std::getline(inputFile, header);
    outputFile << header;

    // Write the records
    std::string record;
    while (std::getline(inputFile, record)) {
        outputFile << "#RECORD#" << record;
    }

    inputFile.close();
    outputFile.close();
    return true;
}

// Function to convert 1-line database to CSV
bool convertLineToCsv(const std::string& txtFilePath, const std::string& csvFilePath) {
    std::ifstream inputFile(txtFilePath);
    if (!inputFile.is_open()) {
        std::cerr << "Failed to open text file: " << txtFilePath << std::endl;
        return false;
    }

    std::ofstream outputFile(csvFilePath);
    if (!outputFile.is_open()) {
        std::cerr << "Failed to create or open CSV file: " << csvFilePath << std::endl;
        inputFile.close();
        return false;
    }

    std::string line;
    std::getline(inputFile, line);

    // Check if the file starts with #HEADER#
    if (line.find("#HEADER#") != 0) {
        std::cerr << "Invalid format: Missing #HEADER#" << std::endl;
        inputFile.close();
        outputFile.close();
        return false;
    }

    // Write the header without #HEADER#
    outputFile << line.substr(8) << std::endl;

    // Process records
    while (std::getline(inputFile, line)) {
        if (line.find("#RECORD#") == 0) {
            // Write the record without #RECORD#
            outputFile << line.substr(8) << std::endl;
        } else {
            std::cerr << "Invalid format: Missing #RECORD# before a record." << std::endl;
            inputFile.close();
            outputFile.close();
            return false;
        }
    }

    inputFile.close();
    outputFile.close();
    return true;
}

int main() {
    // Example usage:
    std::string input_csvFile = "exp.csv";
    std::string input_txtFile = "dbtxt.txt";
    std::string newTxtFile = "output.txt";
    std::string newCsvFile = "output.csv";

    // Convert CSV to 1-line database
    if (convertCsvToLine(input_csvFile, newTxtFile)) {
        std::cout << "CSV to 1-line database conversion successful." << std::endl;
    } else {
        std::cerr << "CSV to 1-line database conversion failed." << std::endl;
    }

    // Convert 1-line database to CSV
    if (convertLineToCsv(input_txtFile, newCsvFile)) {
        std::cout << "1-line database to CSV conversion successful." << std::endl;
    } else {
        std::cerr << "1-line database to CSV conversion failed." << std::endl;
    }

    return 0;
}
