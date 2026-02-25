#ifndef ITRADELOADER_H
#define ITRADELOADER_H

#include "../Models/ITrade.h"
#include <vector>
#include <string>
#include <functional>

class ITradeLoader {
public:
    virtual ~ITradeLoader() = default;
    virtual std::vector<ITrade*> loadTrades() = 0;
    virtual std::string getDataFile() const = 0;
    virtual void setDataFile(const std::string& file) = 0;
    std::string trim(const std::string& str) 
    {
        auto end = str.find_last_not_of(" \t\r\n\xC2");
        return (end == std::string::npos) ? "" : str.substr(0, end + 1);
    }
    virtual void streamTrades(std::function<void(ITrade*)> onTradeLoaded) = 0;
};

#endif // ITRADELOADER_H
