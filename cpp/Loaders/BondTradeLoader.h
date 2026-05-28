#ifndef BONDTRADELOADER_H
#define BONDTRADELOADER_H

#include "ITradeLoader.h"
#include "../Models/BondTrade.h"
#include "../Models/TradeList.h"
#include <string>
#include <vector>
#include <memory>

class BondTradeLoader : public ITradeLoader {
private:
    static constexpr char separator = ',';
    std::string dataFile_;
    static std::unique_ptr<ITrade> createTradeFromLine(const std::string& line);
    void loadTradesFromFile(const std::string& filename, TradeList& tradeList);
    
public:
    TradeList loadTrades() override;
    const std::string& getDataFile() const noexcept override;
    void setDataFile(const std::string& file) noexcept override;
    void streamTrades(const std::function<void(std::unique_ptr<ITrade>)>& onTradeLoaded) override;
};

#endif // BONDTRADELOADER_H
