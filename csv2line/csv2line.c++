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

    // Read the one-line text
    std::string text;
    std::getline(txtFile, text);

    // Find the number of separators in the header
    std::string header = text.substr(text.find("#HEADER#") + 8, text.find("#RECORD#") - text.find("#HEADER#") - 8);
    int numSeparators = std::count(header.begin(), header.end(), ',');

    // Remove #HEADER# and #RECORD# from the text
    text = text.substr(text.find("#RECORD#") + 8);
    text.erase(std::remove(text.begin(), text.end(), '#'), text.end());
    text.erase(0, 7);

    // Split the text into records
    std::vector<std::string> records;
    std::string record;
    std::istringstream iss(text);
    while (std::getline(iss, record, ',')) {
        records.push_back(record);
    }

    // Open the output CSV file for writing
    std::ofstream csvFile(csvFilePath);
    if (!csvFile.is_open()) {
        std::cerr << "Failed to create or open CSV file: " << csvFilePath << std::endl;
        txtFile.close();
        return 1;
    }

    // Write the header to the CSV file
    csvFile << header << std::endl;

    // Write the records to the CSV file
    for (int i = 0; i < records.size(); i++) {
        csvFile << records[i];
        if ((i + 1) % numSeparators == 0) {
            csvFile << std::endl;
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