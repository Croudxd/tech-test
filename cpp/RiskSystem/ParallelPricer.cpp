#include "ParallelPricer.h"
#include "../Pricers/CorpBondPricingEngine.h"
#include "../Pricers/FxPricingEngine.h"
#include "../Pricers/GovBondPricingEngine.h"
#include <memory>
#include <stdexcept>

ParallelPricer::~ParallelPricer() {
}

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

void ParallelPricer::price(TradeList& tradeContainers, 
                           IScalarResultReceiver* resultReceiver) {
    loadPricers(); 
    
    std::vector<std::future<void>> futures;
    std::mutex receiverMutex; 
    
    for (const auto& trade : tradeContainers) {
        ITrade* rTrade = trade.get(); 
        futures.push_back(std::async(std::launch::async, [this, rTrade, resultReceiver, &receiverMutex]() {
            
            std::string tradeType = rTrade->getTradeType();
            
            if (pricers_.find(tradeType) == pricers_.end()) {
                std::lock_guard<std::mutex> lock(receiverMutex);
                resultReceiver->addError(rTrade->getTradeId(), "No Pricing Engines available");
                return;
            }
            
            IPricingEngine* pricer = pricers_[tradeType].get();
            pricer->price(rTrade, resultReceiver);
            
        }));
        }
    
    for (auto& f : futures) {
        f.get();
    }
}
