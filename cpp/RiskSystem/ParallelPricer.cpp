#include "ParallelPricer.h"
#include "../Pricers/CorpBondPricingEngine.h"
#include "../Pricers/FxPricingEngine.h"
#include "../Pricers/GovBondPricingEngine.h"
#include <stdexcept>

ParallelPricer::~ParallelPricer() {
    for (auto& pair : pricers_) {
        delete pair.second;
    }
    pricers_.clear();
}

void ParallelPricer::loadPricers() {
    PricingConfigLoader pricingConfigLoader;
    pricingConfigLoader.setConfigFile("./PricingConfig/PricingEngines.xml");
    PricingEngineConfig pricerConfig = pricingConfigLoader.loadConfig();
    
    for (const auto& configItem : pricerConfig) {
        std::string typeName = configItem.getTypeName();
        std::string tradeType = configItem.getTradeType();

        if (typeName == "HmxLabs.TechTest.Pricers.GovBondPricingEngine") {
            pricers_[tradeType] = new GovBondPricingEngine();
        } 
        else if (typeName == "HmxLabs.TechTest.Pricers.CorpBondPricingEngine") {
            pricers_[tradeType] = new CorpBondPricingEngine();
        } 
        else if (typeName == "HmxLabs.TechTest.Pricers.FxPricingEngine") {
            pricers_[tradeType] = new FxPricingEngine();
        }
    }
}

void ParallelPricer::price(const std::vector<std::vector<ITrade*>>& tradeContainers, 
                           IScalarResultReceiver* resultReceiver) {
    loadPricers(); 
    
    std::vector<std::future<void>> futures;
    std::mutex receiverMutex; 
    
    for (const auto& tradeContainer : tradeContainers) {
        for (ITrade* trade : tradeContainer) {
            
            futures.push_back(std::async(std::launch::async, [this, trade, resultReceiver, &receiverMutex]() {
                
                std::string tradeType = trade->getTradeType();
                
                if (pricers_.find(tradeType) == pricers_.end()) {
                    std::lock_guard<std::mutex> lock(receiverMutex);
                    resultReceiver->addError(trade->getTradeId(), "No Pricing Engines available");
                    return;
                }
                
                IPricingEngine* pricer = pricers_[tradeType];
                
                
                std::lock_guard<std::mutex> lock(receiverMutex);
                pricer->price(trade, resultReceiver);
                
            }));
        }
    }
    
    for (auto& f : futures) {
        f.get();
    }
}
