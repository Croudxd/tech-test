#include "FxTradeLoader.h"
#include <stdexcept>
#include <fstream>
#include <sstream>
#include <stdexcept>
#include <ctime>
#include <iomanip>
#include <chrono>

// NOTE: These methods are only here to allow the solution to compile prior to the test being completed.


FxTrade* FxTradeLoader::createTradeFromLine(std::string line) {
    std::vector<std::string> items;
    std::stringstream ss(line);
    std::string item;
    
    while (std::getline(ss, item, separator)) {
        items.push_back(item);
    }
    
    if (items.size() < 9) { 
        throw std::runtime_error("Invalid line format. Items found: " + std::to_string(items.size()) + " Line: " + line);
    }
    FxTrade* trade;

    if (items[0][2] == 'F')
    {
        trade = new FxTrade(items[8], FxTrade::FxForwardTradeType);
    }
    else 
    {
        trade = new FxTrade(items[8]);
    }

    std::tm tm = {};
    std::istringstream dateStream(items[1]);
    dateStream >> std::get_time(&tm, "%Y-%m-%d");
    auto timePoint = std::chrono::system_clock::from_time_t(std::mktime(&tm));

    std::istringstream dateStream2(items[6]);
    dateStream2 >> std::get_time(&tm, "%Y-%m-%d");
    auto valueDate = std::chrono::system_clock::from_time_t(std::mktime(&tm));

    trade->setTradeDate(timePoint);
    trade->setInstrument(trim(items[2]) + trim(items[3]));
    trade->setNotional(std::stod(items[4]));
    trade->setRate(std::stod(items[5]));
    trade->setValueDate(valueDate);
    trade->setCounterparty(trim(items[7]));
    return trade;
}

std::vector<ITrade*> FxTradeLoader::loadTrades() {
    auto filename = getDataFile();

    if (filename.empty()) {
        throw std::invalid_argument("Filename cannot be null");
    }
    
    std::ifstream stream(filename);
    if (!stream.is_open()) {
        throw std::runtime_error("Cannot open file: " + filename);
    }
    
    std::vector<ITrade*> result;
    int lineCount = 0;
    std::string line;
    while (std::getline(stream, line)) {
        if (lineCount < 2) { 
            lineCount++;
            continue;
        }
        if (line.find("END") == 0) break;
        else {
            result.push_back(createTradeFromLine(line));
        }
        lineCount++;
    }
    return result;
}

std::string FxTradeLoader::getDataFile() const {
    return dataFile_;
}

void FxTradeLoader::setDataFile(const std::string& file) {
    dataFile_ = file;
}

void FxTradeLoader::streamTrades(std::function<void(ITrade*)> onTradeLoaded) {
    std::ifstream stream(dataFile_);
    
    if (!stream.is_open()) {
        throw std::runtime_error("Cannot open file: " + dataFile_);
    }
    std::string line;
    bool isFirstLine = true;
    while (std::getline(stream, line)) {
        if (isFirstLine) { isFirstLine = false; continue; }
        
        ITrade* trade = createTradeFromLine(line); 
        onTradeLoaded(trade);
    }
}
