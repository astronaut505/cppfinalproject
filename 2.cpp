#include <unordered_map>
#include <vector>
#include <cmath>
#include <stdexcept> // Include for std::invalid_argument
#include <iostream>
#include <algorithm> // For max function

// Define M_PI if not already defined
#ifndef M_PI
    #define M_PI 3.14159265358979323846
#endif

class Option {
public:
    enum class Type { Call, Put };
    Type type;
    double strike;
    double timeToExpiry; // In years
    double position;     // +1 for long, -1 for short

    Option(Type type, double strike, double timeToExpiry, double position)
        : type(type), strike(strike), timeToExpiry(timeToExpiry), position(position) {}
};

class VarianceCalculator {
private:
    const double underlyingValue = 5000;
    const double underlyingStdDev = 0.2; // 20%
    const double annualReturn = 0.05;    // 5%

    std::vector<Option> portfolio;
    double portfolioVariance;

    double calculateOptionVariance(const Option& option);
    double calculatePortfolioVariance();

public:
    VarianceCalculator() : portfolioVariance(0) {}

    void addOption(const Option& option);
    void resetPortfolio();

    struct VarianceInfo {
        double currentVariance;
        double newVariance;
    };

    VarianceInfo getVarianceIfPurchased(const Option& option);
};

// Helper function for standard normal cumulative distribution function
double normcdf(double value) {
    double x = value;
    double k = 1 / (1 + 0.2316419 * x);
    double k_sum = k * (0.319381530 + k * (-0.356563782 + k * (1.781477937 + k * (-1.821255978 + 1.330274429 * k))));

    if (x >= 0.0) {
        return (1.0 - (1.0 / (sqrt(2 * M_PI))) * exp(-0.5 * x * x) * k_sum);
    } else {
        return 1.0 - normcdf(-x);
    }
}

// Function to calculate the Black-Scholes delta for an option
double blackScholesDelta(const Option& option, double underlyingPrice, double volatility, double riskFreeRate) {
// Ensure that time to expiry and strike price are valid
if (option.timeToExpiry <= 0 || option.strike <= 0) {
throw std::invalid_argument("Invalid time to expiry or strike price");
}

double d1 = (log(underlyingPrice / option.strike) + (riskFreeRate + 0.5 * pow(volatility, 2)) * option.timeToExpiry) / (volatility * sqrt(option.timeToExpiry));
if (option.type == Option::Type::Call) {
    return exp(-riskFreeRate * option.timeToExpiry) * normcdf(d1);
} else {
    return -exp(-riskFreeRate * option.timeToExpiry) * normcdf(-d1);
}
}

double VarianceCalculator::calculateOptionVariance(const Option& option) {
double delta = blackScholesDelta(option, underlyingValue, underlyingStdDev, annualReturn);
double optionVariance = pow(delta, 2) * pow(underlyingStdDev, 2);
return optionVariance;
}
double VarianceCalculator::calculatePortfolioVariance() {
double totalVariance = 0;
for (const auto& option : portfolio) {
double optionVariance = calculateOptionVariance(option);
totalVariance += pow(option.position, 2) * optionVariance; // Assuming independence
}
return totalVariance;
}

void VarianceCalculator::addOption(const Option& option) {
portfolio.push_back(option);
portfolioVariance = calculatePortfolioVariance();
}
void VarianceCalculator::resetPortfolio() {
portfolio.clear();
portfolioVariance = 0;
}

VarianceCalculator::VarianceInfo VarianceCalculator::getVarianceIfPurchased(const Option& option) {
VarianceInfo info;
info.currentVariance = portfolioVariance;
// Add option to portfolio for hypothetical calculation
portfolio.push_back(option);
info.newVariance = calculatePortfolioVariance();

// Remove the hypothetical option
portfolio.pop_back();

return info;

}

int main() {
    VarianceCalculator calculator;
    std::string typeStr;
    double strike, timeToExpiry, position;
    char continueInput = 'y';

    // User input loop to add options
    while (continueInput == 'y') {
        std::cout << "Enter option type (Call/Put), strike, time to expiry, position: ";
        std::cin >> typeStr >> strike >> timeToExpiry >> position;

        Option::Type type = (typeStr == "Call") ? Option::Type::Call : Option::Type::Put;
        calculator.addOption(Option(type, strike, timeToExpiry, position));

        std::cout << "Do you want to add more options? (y/n): ";
        std::cin >> continueInput;
    }

    // Get variance info for a new option
    std::cout << "Enter details of the new option to calculate variance (Type, Strike, Time to Expiry, Position): ";
    std::cin >> typeStr >> strike >> timeToExpiry >> position;
    Option::Type newOptionType = (typeStr == "Call") ? Option::Type::Call : Option::Type::Put;

    auto variance = calculator.getVarianceIfPurchased(Option(newOptionType, strike, timeToExpiry, position));
    std::cout << "Current Variance: " << variance.currentVariance << std::endl;
    std::cout << "New Variance if Purchased: " << variance.newVariance << std::endl;

    return 0;
}