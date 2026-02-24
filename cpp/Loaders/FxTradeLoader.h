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
    
public:
    // NOTE: These methods are only here to allow the solution to compile prior to the test being completed.
    FxTradeLoader() = default;
    std::vector<ITrade*> loadTrades() override;
    std::string getDataFile() const override;
    void setDataFile(const std::string& file) override;
};

#endif // FXTRADELOADER_H
