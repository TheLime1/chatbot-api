#include <iostream>
#include <fstream>
#include <vector>
#include <string>

int main() {
    // Define the new comment for the first line
    std::string newComment = "#only speak french";

    // Specify the Python file to modify
    std::string pythonFileName = "chatgpt.py";

    // Read the content of the Python file into a vector of strings
    std::ifstream inputFile(pythonFileName);
    std::vector<std::string> lines;
    std::string line;

    while (std::getline(inputFile, line)) {
        lines.push_back(line);
    }

    // Modify the first line (assuming there is at least one line in the file)
    if (!lines.empty()) {
        lines[0] = newComment;
    }

    // Write the modified content back to the Python file
    std::ofstream outputFile(pythonFileName);
    for (const std::string& modifiedLine : lines) {
        outputFile << modifiedLine << std::endl;
    }

    // Close the files
    inputFile.close();
    outputFile.close();

    std::cout << "First line of " << pythonFileName << " has been changed to: " << newComment << std::endl;

    // Execute the "modal deploy main.py" command
    std::string deployCommand = "modal deploy main.py";
    std::cout << "Executing command: " << deployCommand << std::endl;
    int result = system(deployCommand.c_str());
    if (result == 0) {
        std::cout << "Command executed successfully." << std::endl;
    } else {
        std::cout << "Command execution failed." << std::endl;
    }

    return 0;
}
