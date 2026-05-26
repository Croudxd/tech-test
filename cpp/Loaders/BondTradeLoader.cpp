#include "BondTradeLoader.h"
#include <fstream>
#include <memory>
#include <sstream>
#include <stdexcept>
#include <ctime>
#include <iomanip>
#include <chrono>
#include <functional>


std::unique_ptr<ITrade> BondTradeLoader::createTradeFromLine(std::string line) {
    std::vector<std::string> items;
    std::stringstream ss(line);
    std::string item;
    
    while (std::getline(ss, item, separator)) {
        items.push_back(item);
    }
    
    if (items.size() < 7) { 
        throw std::runtime_error("Invalid line format");
    }
    std::unique_ptr<ITrade> trade;
    if (items[6][0] == 'C')
    {
        trade = std::make_unique<BondTrade>(BondTrade(trim(items[6]), BondTrade::CorpBondTradeType));
    }
    else 
    {
        trade = std::make_unique<BondTrade>(BondTrade(trim(items[6])));
    }
    
    std::tm tm = {};
    std::istringstream dateStream(items[1]);
    dateStream >> std::get_time(&tm, "%Y-%m-%d");
    auto timePoint = std::chrono::system_clock::from_time_t(std::mktime(&tm));
    trade->setTradeDate(timePoint);
    
    trade->setInstrument(items[2]);
    trade->setCounterparty(items[3]);
    trade->setNotional(std::stod(items[4]));
    trade->setRate(std::stod(items[5]));
    
    return trade;
}

void BondTradeLoader::loadTradesFromFile(std::string filename, TradeList& tradeList) {
    if (filename.empty()) {
        throw std::invalid_argument("Filename cannot be null");
    }
    
    std::ifstream stream(filename);
    if (!stream.is_open()) {
        throw std::runtime_error("Cannot open file: " + filename);
    }
    
    int lineCount = 0;
    std::string line;
    while (std::getline(stream, line)) {
        if (lineCount == 0) {
        } else {
            tradeList.add(std::move(createTradeFromLine(line)));
        }
        lineCount++;
    }
}

TradeList BondTradeLoader::loadTrades() {
    TradeList tradeList;
    loadTradesFromFile(dataFile_, tradeList);
    return tradeList;
}

std::string BondTradeLoader::getDataFile() const {
    return dataFile_;
}

void BondTradeLoader::setDataFile(const std::string& file) {
    dataFile_ = file;
}

void BondTradeLoader::streamTrades(std::function<void(std::unique_ptr<ITrade>)> onTradeLoaded) {
    std::ifstream stream(dataFile_);
    if (!stream.is_open()) {
        throw std::runtime_error("Cannot open file: " + dataFile_);
    }
    
    std::string line;
    bool isFirstLine = true;
    
    while (std::getline(stream, line)) {
        if (isFirstLine) {
            isFirstLine = false;
            continue; 
        }
        
        std::unique_ptr<ITrade> trade = createTradeFromLine(line);
        
        onTradeLoaded(std::move(trade));
    }
}
