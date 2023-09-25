#include <iostream>
#include <fstream>
#include <string>

int main() {
    // Specify the paths for the CSV and output text files
    std::string csvFilePath = "input.csv";
    std::string txtFilePath = "output.txt";

    // Open the CSV file for reading
    std::ifstream csvFile(csvFilePath);
    if (!csvFile.is_open()) {
        std::cerr << "Failed to open CSV file: " << csvFilePath << std::endl;
        return 1;
    }

    // Open the output text file for writing
    std::ofstream txtFile(txtFilePath);
    if (!txtFile.is_open()) {
        std::cerr << "Failed to create or open text file: " << txtFilePath << std::endl;
        csvFile.close();
        return 1;
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

    return 0;
}
