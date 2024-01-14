#include <vector>
#include <string>
#include <iostream>
#include <cassert>

// Define a structure to represent an option
struct Option {
    std::string type;  // "call" or "put"
    std::string position;  // "long" or "short"
    int strikePrice;
    double underlyingPrice;  // Price of the underlying asset
    int daysToExpiry;  // Time to expiry in days
};

class OptionsPortfolio {
public:
    // Constructor
    OptionsPortfolio() : portfolioVariance(0.0) {}

    // Method to add an option to the portfolio
    void addOption(const Option& option) {
    // Basic validation checks
        if (option.strikePrice <= 0 || option.underlyingPrice <= 0 || option.daysToExpiry <= 0) {
        std::cerr << "Invalid option parameters." << std::endl;
        return;
        }
        if (option.type != "call" && option.type != "put") {
        std::cerr << "Invalid option type. Must be 'call' or 'put'." << std::endl;
        return;
        }
        if (option.position != "long" && option.position != "short") {
        std::cerr << "Invalid option position. Must be 'long' or 'short'." << std::endl;
        return;
        }

    // Calculate individual variance for the option
    double individualVariance = calculateIndividualVariance(option);

    // Update the total variance incrementally
    portfolioVariance += individualVariance * individualVariance;  // Squaring as variance is always positive

    // Add the option to the portfolio list
    optionsList.push_back(option);
}


    // Method to calculate the variance of the portfolio
    double calculateVariance() {
        return portfolioVariance;
    }

    // Method to reset the portfolio
    void resetPortfolio() {
        optionsList.clear();
        portfolioVariance = 0.0;
    }

    // Method to handle a transaction and check variance
    void transactionVarianceCheck(const Option& option) {
    double individualVariance = calculateIndividualVariance(option);
    std::cout << "The variance for this transaction is: " << individualVariance * individualVariance << std::endl;
    }

    // Getter method for optionsList
    const std::vector<Option>& getOptionsList() const {
        return optionsList;
    }

private:
    std::vector<Option> optionsList;  // Container for options
    double portfolioVariance;         // Variance of the portfolio

    // Method to calculate individual variance of an option
    double calculateIndividualVariance(const Option& option) {
    double priceDifference = option.underlyingPrice - option.strikePrice;
    double individualVariance = 0.0;

    if (option.type == "call") {
        individualVariance = (option.position == "long" ? 1 : -1) * priceDifference;
    } else if (option.type == "put") {
        individualVariance = (option.position == "long" ? -1 : 1) * priceDifference;
    }

    return individualVariance;
    }
};

// Test functions follow here

void testAddingOptions() {
    OptionsPortfolio portfolio;
    Option callOption{"call", "long", 5000, 5100, 30};
    Option putOption{"put", "short", 5000, 4900, 30};

    portfolio.addOption(callOption);
    portfolio.addOption(putOption);

    assert(portfolio.getOptionsList().size() == 2); // Check if two options are added
    assert(portfolio.getOptionsList()[0].type == "call" && portfolio.getOptionsList()[0].position == "long"); // Details for the first option
    assert(portfolio.getOptionsList()[1].type == "put" && portfolio.getOptionsList()[1].position == "short"); // Details for the second option

    std::cout << "testAddingOptions passed." << std::endl;
}


void testVarianceCalculation() {
    OptionsPortfolio portfolio;
    // Add a set of options with known variance outcomes
    portfolio.addOption(Option{"call", "long", 5000, 5100, 30});
    portfolio.addOption(Option{"put", "short", 5000, 4900, 30});

    double calculatedVariance = portfolio.calculateVariance();

    // Check if calculated variance is as expected
    // The expected value here should be based on your variance calculation logic
    double expectedVariance = 20000; // Placeholder for expected variance
    assert(abs(calculatedVariance - expectedVariance) < 0.001); // Check if variance is close to expected

    std::cout << "testVarianceCalculation passed." << std::endl;
}


void testIncrementalVarianceUpdate() {
    OptionsPortfolio portfolio;
    portfolio.addOption(Option{"call", "long", 5000, 5100, 30});
    double varianceAfterFirstAddition = portfolio.calculateVariance();

    portfolio.addOption(Option{"put", "short", 5000, 4900, 30});
    double varianceAfterSecondAddition = portfolio.calculateVariance();

    // Check if variance is correctly updated
    assert(varianceAfterSecondAddition > varianceAfterFirstAddition);

    std::cout << "testIncrementalVarianceUpdate passed." << std::endl;
}


void testResetPortfolio() {
    OptionsPortfolio portfolio;
    portfolio.addOption(Option{"call", "long", 5000, 5100, 30});
    portfolio.resetPortfolio();
    assert(portfolio.getOptionsList().empty()); // Check if portfolio is empty
    assert(portfolio.calculateVariance() == 0.0); // Check if variance is reset to 0
    std::cout << "testResetPortfolio passed." << std::endl;
}

void testEdgeCases() {
    OptionsPortfolio portfolio;
    Option invalidOption{"call", "long", -100, 5100, 30}; // Invalid strike price

    portfolio.addOption(invalidOption);

    assert(portfolio.getOptionsList().empty()); // Check if valid option is added

    // Optionally, you can also test the output error messages if your method provides them

    std::cout << "testEdgeCases passed." << std::endl;
}

int main() {

    testAddingOptions();
    testVarianceCalculation();
    testIncrementalVarianceUpdate();
    testResetPortfolio();
    testEdgeCases();

    std::cout << "All tests completed successfully." << std::endl;
    OptionsPortfolio portfolio;

    // Creating a sample option
    Option sampleOption{"call", "long", -100, 5100, 30};

    // Adding an option and checking variance
    portfolio.transactionVarianceCheck(sampleOption);

    // Resetting the portfolio
    portfolio.resetPortfolio();

    return 0;
}

