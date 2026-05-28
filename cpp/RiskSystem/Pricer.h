#include <map>
#include <memory>
#include <string>
#include "../Models/IPricingEngine.h"
#include "../RiskSystem/PricingConfigLoader.h"
#include "../Pricers/CorpBondPricingEngine.h"
#include "../Pricers/FxPricingEngine.h"
#include "../Pricers/GovBondPricingEngine.h"

class Pricer {

    protected:
        virtual void loadPricers()
        {
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

        std::map<std::string, std::unique_ptr<IPricingEngine>> pricers_;
};
