#ifndef SERIALPRICER_H
#define SERIALPRICER_H

#include "../Models/IPricingEngine.h"
#include "../Models/ITrade.h"
#include "../Models/TradeList.h"
#include "../Models/IScalarResultReceiver.h"
#include "Pricer.h"
#include "PricingConfigLoader.h"
#include <map>
#include <memory>
#include <vector>
#include <string>


class SerialPricer : Pricer {
private:
    std::map<std::string, std::unique_ptr<IPricingEngine>> pricers_;
    
public:
    void price(const TradeList& trades, 
               IScalarResultReceiver* resultReceiver);
};

#endif // SERIALPRICER_H
