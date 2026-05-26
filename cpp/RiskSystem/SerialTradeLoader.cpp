#include "SerialTradeLoader.h"
#include "../Loaders/BondTradeLoader.h"
#include "../Loaders/FxTradeLoader.h"
#include <memory>

std::vector<std::unique_ptr<ITradeLoader>> SerialTradeLoader::getTradeLoaders() {
    std::vector<std::unique_ptr<ITradeLoader>> loaders;
    
    std::unique_ptr<BondTradeLoader> bondLoader = std::make_unique<BondTradeLoader>();
    bondLoader->setDataFile("TradeData/BondTrades.dat");
    loaders.push_back(std::move(bondLoader));
    
    std::unique_ptr<FxTradeLoader> fxLoader = std::make_unique<FxTradeLoader>();
    fxLoader->setDataFile("TradeData/FxTrades.dat");
    loaders.push_back(std::move(fxLoader));
    
    return loaders;
}

TradeList SerialTradeLoader::loadTrades() {
    auto loaders = getTradeLoaders();
    TradeList result;
    
    for (auto& loader : loaders) {
        TradeList trades = loader->loadTrades();
        for (auto& trade : trades)
        {
            result.add(std::move(trade));
        }
    }
    return result;
}

