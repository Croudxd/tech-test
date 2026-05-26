#include "StreamingTradeLoader.h"
#include "../Loaders/BondTradeLoader.h"
#include "../Loaders/FxTradeLoader.h"
#include "PricingConfigLoader.h"
#include "StreamingTradeLoader.h"
#include "../Loaders/BondTradeLoader.h"
#include "../Loaders/FxTradeLoader.h"
#include "../Pricers/CorpBondPricingEngine.h"
#include "../Pricers/FxPricingEngine.h"
#include "../Pricers/GovBondPricingEngine.h"
#include "../RiskSystem/PricingConfigLoader.h"
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

void StreamingTradeLoader::loadPricers() {
    PricingConfigLoader pricingConfigLoader;
    pricingConfigLoader.setConfigFile("./PricingConfig/PricingEngines.xml");
    PricingEngineConfig pricerConfig = pricingConfigLoader.loadConfig();
    
    for (const auto& configItem : pricerConfig) {
        std::string typeName = configItem.getTypeName();
        std::string tradeType = configItem.getTradeType();

        if (typeName == "HmxLabs.TechTest.Pricers.GovBondPricingEngine") {
            pricers_[tradeType] = std::make_unique<GovBondPricingEngine>();
        } 
        else if (typeName == "HmxLabs.TechTest.Pricers.CorpBondPricingEngine") {
            pricers_[tradeType] = std::make_unique<CorpBondPricingEngine>();
        } 
        else if (typeName == "HmxLabs.TechTest.Pricers.FxPricingEngine") {
            pricers_[tradeType] = std::make_unique<FxPricingEngine>();
        }
    }
}

void StreamingTradeLoader::loadAndPrice(IScalarResultReceiver* resultReceiver) {
    loadPricers();
    auto loaders = getTradeLoaders();
    
    for (auto& loader : loaders) {
        
        loader->streamTrades([&](std::unique_ptr<ITrade> trade) {
            
            std::string tradeType = trade->getTradeType();
            
            if (pricers_.find(tradeType) == pricers_.end()) {
                resultReceiver->addError(trade->getTradeId(), "No Pricing Engines available for this trade type");
            } else {
                IPricingEngine* pricer = pricers_[tradeType].get();
                
                pricer->price(trade.get(), resultReceiver);
            }
            
        });
    }
}
