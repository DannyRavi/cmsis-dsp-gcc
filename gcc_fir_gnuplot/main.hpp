#include <iostream>
#include <vector>
#include <cmath>
#include <cstdlib>
#include "../dsp/filtering_functions.h"
constexpr int MAX_COLS = 4;
constexpr int MAX_COL_LEN = 100;
constexpr int MAX_ROWS = 92774;
#include <chrono>

class Timer {
public:
    Timer() : start_time_(std::chrono::high_resolution_clock::now()) {}

    // Reset the timer
    void reset() {
        start_time_ = std::chrono::high_resolution_clock::now();
    }

    // Get the elapsed time in milliseconds
    double elapsed() const {
        auto end_time = std::chrono::high_resolution_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end_time - start_time_);
        return duration.count();
    }

private:
    std::chrono::time_point<std::chrono::high_resolution_clock> start_time_;
};


// Function to calculate the element-wise difference between two vectors and return the sum
double calculateSumOfDifferences(const std::vector<double>& vector1, const std::vector<double>& vector2) {
    if (vector1.size() != vector2.size()) {
        // Handle error: Vectors must be of the same size
        std::cerr << "Error: Vectors must be of the same size." << std::endl;
        return 0.0; // Return 0 in case of an error
    }

    // Calculate the sum of differences
    double sum = 0.0;
    for (size_t i = 0; i < vector1.size(); ++i) {
        sum += vector1[i] - vector2[i];
    }

    return sum;
}


void storeDataToFile(const std::vector<double>& x, const std::vector<double>& y1, const std::vector<double>& y2) {
    std::ofstream dataFile("store.txt");
    for (size_t i = 0; i < x.size(); ++i) {
        dataFile << x[i] << " " << y1[i] << " " << y2[i] << std::endl;
    }
    dataFile.close();
}




void readCSV(const std::string &filename, std::vector<std::vector<std::string>> &data, int &numRows, int &numCols)
{
    std::ifstream file(filename);
    if (!file.is_open())
    {
        std::cerr << "Error opening file: " << filename << std::endl;
        exit(EXIT_FAILURE);
    }

    std::string line;
    numRows = 0;
    numCols = 0;

    while (std::getline(file, line) && (numRows < MAX_ROWS))
    {
        std::istringstream ss(line);
        std::string token;
        int col = 0;

        while (std::getline(ss, token, ',') && (col < MAX_COLS))
        {
            // Remove leading and trailing whitespaces from the token
            size_t start = token.find_first_not_of(" \t\n");
            size_t end = token.find_last_not_of(" \t\n");
            token = (start != std::string::npos && end != std::string::npos) ? token.substr(start, end - start + 1) : "";

            // Push the token into the data vector
            data[numRows][col] = token;

            col++;
        }

        if (numRows == 0)
        {
            numCols = col;
        }
        else if (col != numCols)
        {
            std::cerr << "Error: Inconsistent number of columns in line " << numRows + 1 << std::endl;
            exit(EXIT_FAILURE);
        }

        numRows++;
    }

    file.close();
}





// Function to calculate mean and variance of a vector
std::pair<double, double> calculateMeanAndVariance(const std::vector<double>& data) {
    // Check if the vector is not empty
    if (data.empty()) {
        // Return a pair with NaN values indicating an error
        return std::make_pair(std::numeric_limits<double>::quiet_NaN(), std::numeric_limits<double>::quiet_NaN());
    }

    // Calculate mean
    double mean = 0.0;
    for (const double& value : data) {
        mean += value;
    }
    mean /= data.size();

    // Calculate variance
    double variance = 0.0;
    for (const double& value : data) {
        variance += std::pow(value - mean, 2);
    }
    variance /= data.size();

    // Return a pair with mean and variance
    return std::make_pair(mean, variance);
}

void plot_gnu(void)
{

    // Generate plot using Gnuplot
    FILE *gnuplotPipe = popen("gnuplot -persistent  ", "w");
    if (gnuplotPipe)
    {
        fprintf(gnuplotPipe, "set multiplot layout 2, 1 title 'Subplots'\n");
        fprintf(gnuplotPipe, "set xlabel 'time - sec'\n");
        fprintf(gnuplotPipe, "set ylabel 'amplitude'\n");
        fprintf(gnuplotPipe, "plot 'store.txt' using 1:2 with lines title 'sin(2wt)+sin(50wt)'\n");
        fprintf(gnuplotPipe, "plot 'store.txt' using 1:3 with lines title 'in(2wt)+sin(50wt) filtered'\n");
        fprintf(gnuplotPipe, "unset multiplot\n");
        fflush(gnuplotPipe);
    }
    else
    {
        std::cerr << "Error opening Gnuplot pipe" << std::endl;
        exit(EXIT_FAILURE);
    }
}

