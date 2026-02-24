#include "ScreenResultPrinter.h"
#include <iostream>

void ScreenResultPrinter::printResults(ScalarResults& results) {
    for (const auto& result : results) {
        // Write code here to print out the results such that we have:
        // TradeID : Result : Error
        bool hasResult = result.getResult().has_value();
        bool hasError = result.getError().has_value();
        
        if (hasResult && hasError) {
            std::cout << result.getTradeId() << " : " 
                      << result.getResult().value() << " : " 
                      << result.getError().value() << std::endl;
        } 
        else if (hasResult && !hasError) {
            std::cout << result.getTradeId() << " : " 
                      << result.getResult().value() << std::endl;
        } 
        else if (!hasResult && hasError) {
            std::cout << result.getTradeId() << " : " 
                      << result.getError().value() << std::endl;
        }
    }
}
