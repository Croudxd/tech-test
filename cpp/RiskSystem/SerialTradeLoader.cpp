#include "SerialTradeLoader.h"
#include "../Loaders/BondTradeLoader.h"
#include "../Loaders/FxTradeLoader.h"

std::vector<ITradeLoader*> SerialTradeLoader::getTradeLoaders() {
    std::vector<ITradeLoader*> loaders;
    
    BondTradeLoader* bondLoader = new BondTradeLoader();
    bondLoader->setDataFile("TradeData/BondTrades.dat");
    loaders.push_back(bondLoader);
    
    FxTradeLoader* fxLoader = new FxTradeLoader();
    fxLoader->setDataFile("TradeData/FxTrades.dat");
    loaders.push_back(fxLoader);
    
    return loaders;
}

TradeList SerialTradeLoader::loadTrades() {
    auto loaders = getTradeLoaders();
    TradeList result;
    
    for (auto loader : loaders) {
        TradeList trades = loader->loadTrades();
        for (auto& trade : trades)
        {
            result.add(std::move(trade));
        }
    }
    return result;
}

