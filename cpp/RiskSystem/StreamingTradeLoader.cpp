#include "StreamingTradeLoader.h"
#include "../Loaders/FxTradeLoader.h"
#include "../Loaders/BondTradeLoader.h"
#include <memory>
#include <utility>

std::vector<std::unique_ptr<ITradeLoader>> StreamingTradeLoader::getTradeLoaders() {
    std::vector<std::unique_ptr<ITradeLoader>> loaders;
    
    std::unique_ptr<BondTradeLoader> bondLoader = std::make_unique<BondTradeLoader>();
    bondLoader->setDataFile("TradeData/BondTrades.dat");
    loaders.push_back(std::move(bondLoader));
    
    std::unique_ptr<FxTradeLoader> fxLoader = std::make_unique<FxTradeLoader>();
    fxLoader->setDataFile("TradeData/FxTrades.dat");
    loaders.push_back(std::move(fxLoader));
    
    return loaders;
}

void StreamingTradeLoader::loadAndPrice(IScalarResultReceiver* resultReceiver) {
    if (pricers_.empty()) loadPricers();
    auto loaders = getTradeLoaders();
    
    for (const auto& loader : loaders) {
        
        loader->streamTrades([&](std::unique_ptr<ITrade> trade) {
            
            std::string tradeType = trade->getTradeType();
            
            const auto it = pricers_.find(tradeType);
            if (it == pricers_.end()) {
                resultReceiver->addError(trade->getTradeId(), "No Pricing Engines available for this trade type");
            } else {
                it->second->price(trade.get(), resultReceiver);
            }
            
        });
    }
}
