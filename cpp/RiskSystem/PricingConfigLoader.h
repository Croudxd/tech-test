#ifndef PRICINGCONFIGLOADER_H
#define PRICINGCONFIGLOADER_H

#include "PricingEngineConfig.h"
#include <string>

class PricingConfigLoader {
private:
    std::string configFile_;
    PricingEngineConfig parseXml(const std::string& content);
    static std::string extractAttribute(const std::string& line, const std::string& attributeName); 
public:
    const std::string& getConfigFile() const noexcept;
    void setConfigFile(const std::string& file) noexcept;
    PricingEngineConfig loadConfig();
};

#endif // PRICINGCONFIGLOADER_H
