#include "BondTradeLoader.h"
#include <fstream>
#include <sstream>
#include <stdexcept>
#include <ctime>
#include <iomanip>
#include <chrono>
#include <functional>

//Small trim function to cut off any unseen special characters. 
//Okay so assuming that we are not sure if every data point has a trail, the below would be safest.
//perhaps we could do this faster by just cuttong off the end entity of the string however, we new definitvely
//but that speed i think would be very small.

BondTrade* BondTradeLoader::createTradeFromLine(std::string line) {
    std::vector<std::string> items;
    std::stringstream ss(line);
    std::string item;
    
    while (std::getline(ss, item, separator)) {
        items.push_back(item);
    }
    
    if (items.size() < 7) { 
        throw std::runtime_error("Invalid line format");
    }
    BondTrade* trade;
    //Assuming only gov/corp
    // Pretty crap check but again this assuming we have correct data and only gov/corp
    if (items[6][0] == 'C')
    {
        trade = new BondTrade(trim(items[6]), BondTrade::CorpBondTradeType);
    }
    else 
    {
        trade = new BondTrade(trim(items[6]));
    }

    // Always creating gov bonds.
    
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

void BondTradeLoader::loadTradesFromFile(std::string filename, BondTradeList& tradeList) {
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
            tradeList.add(createTradeFromLine(line));
        }
        lineCount++;
    }
}

std::vector<ITrade*> BondTradeLoader::loadTrades() {
    BondTradeList tradeList;
    loadTradesFromFile(dataFile_, tradeList);
    
    std::vector<ITrade*> result;
    for (size_t i = 0; i < tradeList.size(); ++i) {
        result.push_back(tradeList[i]); 
    }
    return result;
}

std::string BondTradeLoader::getDataFile() const {
    return dataFile_;
}

void BondTradeLoader::setDataFile(const std::string& file) {
    dataFile_ = file;
}

void BondTradeLoader::streamTrades(std::function<void(ITrade*)> onTradeLoaded) {
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
        
        ITrade* trade = createTradeFromLine(line);
        
        onTradeLoaded(trade);
    }
}
