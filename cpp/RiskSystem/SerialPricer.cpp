#include "SerialPricer.h"
#include "../Pricers/CorpBondPricingEngine.h"
#include "../Pricers/FxPricingEngine.h"
#include "../Pricers/GovBondPricingEngine.h"
#include <memory>

void SerialPricer::loadPricers() {
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

void SerialPricer::price(const TradeList& trades, 
                         IScalarResultReceiver* resultReceiver) {
    if (pricers_.empty()) loadPricers();
    
    for (const auto& trade : trades) {

        std::string tradeType = trade->getTradeType();
        auto it = pricers_.find(tradeType);
        if (it == pricers_.end()) {
            resultReceiver->addError(trade->getTradeId(), "No Pricing Engines available for this trade type");
            continue;
        }
        it->second->price(trade.get(), resultReceiver);
    }
}
