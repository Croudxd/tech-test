#include "FxTradeLoader.h"
#include <memory>
#include <stdexcept>
#include <fstream>
#include <sstream>
#include <stdexcept>
#include <ctime>
#include <iomanip>
#include <chrono>
#include <utility>


std::unique_ptr<ITrade> FxTradeLoader::createTradeFromLine(const std::string& line) {
    std::vector<std::string> items;
    
    std::string remaining = line;
    size_t start = 0, pos;
    while ((pos = line.find(separator, start)) != std::string::npos) {
        items.push_back(remaining.substr(start, pos - start));
        start = pos + separator.size();
    }
    items.push_back(line.substr(start));

    if (!items.empty() && items.back().back() == '\r') {
        items.back().pop_back();
    }
    
    if (items.size() < 9) { 
        throw std::runtime_error("Invalid line format. Items found: " + std::to_string(items.size()) + " Line: " + line);
    }
    std::unique_ptr<FxTrade> trade;

    if (items[0][2] == 'F')
    {
        trade = std::make_unique<FxTrade>(items[8], FxTrade::FxForwardTradeType);
    }
    else 
    {
        trade = std::make_unique<FxTrade>(items[8]); 
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

void FxTradeLoader::loadTradesFromFile(const std::string& filename, TradeList& tradeList) {
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
        if (lineCount < 2) {
            lineCount++;
            continue;
        } 
        if (line.find("END") == 0) break;
        else {
            tradeList.add(createTradeFromLine(line));
        }
        lineCount++;
    }
}

TradeList FxTradeLoader::loadTrades() {
    TradeList tradelist;
    loadTradesFromFile(dataFile_, tradelist);
    return tradelist;
}

const std::string& FxTradeLoader::getDataFile() const noexcept {
    return dataFile_;
}

void FxTradeLoader::setDataFile(const std::string& file) noexcept {
    dataFile_ = file;
}

void FxTradeLoader::streamTrades(const std::function<void(std::unique_ptr<ITrade>)>& onTradeLoaded) {
    std::ifstream stream(dataFile_);
    
    if (!stream.is_open()) {
        throw std::runtime_error("Cannot open file: " + dataFile_);
    }
    std::string line;
    bool isFirstLine = true;
    while (std::getline(stream, line)) {
        if (isFirstLine) { isFirstLine = false; continue; }
        
        std::unique_ptr<ITrade> trade = createTradeFromLine(line); 
        onTradeLoaded(std::move(trade));
    }
}
