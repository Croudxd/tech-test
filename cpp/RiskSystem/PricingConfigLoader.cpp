#include "PricingConfigLoader.h"
#include <stdexcept>
#include <fstream>
#include "../Models/BondTrade.h"
#include "../Models/FxTrade.h"

const std::string& PricingConfigLoader::getConfigFile() const noexcept {
    return configFile_;
}

void PricingConfigLoader::setConfigFile(const std::string& file) noexcept {
    configFile_ = file;
}

std::string PricingConfigLoader::extractAttribute(const std::string& line, const std::string& attributeName) {
    std::string searchStr = attributeName + "=\"";
    size_t startPos = line.find(searchStr);
    
    if (startPos == std::string::npos) {
        return "";
    }
    
    startPos += searchStr.length();
    
    size_t endPos = line.find("\"", startPos);
    
    if (endPos == std::string::npos) {
        return ""; 
    }
    
    return line.substr(startPos, endPos - startPos);
}

PricingEngineConfig PricingConfigLoader::loadConfig() {
    std::ifstream stream(getConfigFile());
    if (!stream.is_open()) {
        throw std::runtime_error("Cannot open config file: " + getConfigFile());
    }
    
    PricingEngineConfig config; 
    std::string line;
    
    while (std::getline(stream, line)) {
        if (line.find("<Engine ") == std::string::npos) {
            continue;
        }
        
        std::string tradeType = extractAttribute(line, "tradeType");
        std::string assembly = extractAttribute(line, "assembly");
        std::string pricingEngine = extractAttribute(line, "pricingEngine");
        

        PricingEngineConfigItem item;
        item.setTypeName(pricingEngine);
        item.setAssembly(assembly);

        if (tradeType.empty())
        {
            throw std::runtime_error("Trade type is empty check config file.");
        }
        else if (tradeType[0] == 'G')
        {
            item.setTradeType(BondTrade::GovBondTradeType);
        }
        else if (tradeType[0] == 'C')
        {
            item.setTradeType(BondTrade::CorpBondTradeType);
        }
        else if (tradeType[0] == 'F')
        {
            if (tradeType[2] == 'S')
            {
                item.setTradeType(FxTrade::FxSpotTradeType);
            }
            else
            {
                item.setTradeType(FxTrade::FxForwardTradeType);
            }
        }

        config.push_back(item);
    }
    
    return config;
}

