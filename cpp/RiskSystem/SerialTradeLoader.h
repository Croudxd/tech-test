#ifndef SERIALTRADELOADER_H
#define SERIALTRADELOADER_H

#include "../Loaders/ITradeLoader.h"
#include "../Models/ITrade.h"
#include <memory>
#include <vector>

class SerialTradeLoader {
private:
    std::vector<std::unique_ptr<ITradeLoader>> getTradeLoaders();
    
public:
    TradeList loadTrades();
};

#endif // SERIALTRADELOADER_H
