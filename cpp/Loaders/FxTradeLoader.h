#ifndef FXTRADELOADER_H
#define FXTRADELOADER_H

#include "ITradeLoader.h"
#include "../Models/FxTrade.h"
#include <string>
#include <vector>

class FxTradeLoader : public ITradeLoader {
private:
    static constexpr char separator = '\xAC';
    std::string dataFile_;
    FxTrade* createTradeFromLine(std::string line); 
    void loadTradesFromFile(std::string filename, TradeList& tradeList); 

public:
    FxTradeLoader() = default;
    TradeList loadTrades() override;
    std::string getDataFile() const override;
    void setDataFile(const std::string& file) override;
    void streamTrades(std::function<void(ITrade*)> onTradeLoaded) override;
};

#endif // FXTRADELOADER_H
