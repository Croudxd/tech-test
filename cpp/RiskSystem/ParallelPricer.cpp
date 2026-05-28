#include "ParallelPricer.h"
#include "../Pricers/CorpBondPricingEngine.h"
#include "../Pricers/FxPricingEngine.h"
#include "../Pricers/GovBondPricingEngine.h"
#include <memory>


void ParallelPricer::loadPricers() {
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

void ParallelPricer::price(const TradeList& tradeContainers, 
                           IScalarResultReceiver* resultReceiver) {
    if (pricers_.empty()) loadPricers(); 
    
    std::vector<std::future<void>> futures;
    
    for (const auto& trade : tradeContainers) {
        ITrade* rTrade = trade.get(); 
        futures.push_back(std::async(std::launch::async, [this, rTrade, resultReceiver]() {
            
            std::string tradeType = rTrade->getTradeType();
            
            auto it = pricers_.find(tradeType);
            if (it == pricers_.end()) {
                resultReceiver->addError(rTrade->getTradeId(), "No Pricing Engines available");
                return;
            }
            
            IPricingEngine* pricer = it->second.get();
            pricer->price(rTrade, resultReceiver);
            
        }));
        }
    
    for (auto& f : futures) {
        f.get();
    }
}
