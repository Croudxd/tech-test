#ifndef SERIALPRICER_H
#define SERIALPRICER_H

#include "../Models/IPricingEngine.h"
#include "../Models/ITrade.h"
#include "../Models/TradeList.h"
#include "../Models/IScalarResultReceiver.h"
#include "PricingConfigLoader.h"
#include <map>
#include <memory>
#include <vector>
#include <string>

class SerialPricer {
private:
    std::map<std::string, std::unique_ptr<IPricingEngine>> pricers_;
    void loadPricers();
    
public:
    ~SerialPricer();
    void price(const TradeList& trades, 
               IScalarResultReceiver* resultReceiver);
};

#endif // SERIALPRICER_H
