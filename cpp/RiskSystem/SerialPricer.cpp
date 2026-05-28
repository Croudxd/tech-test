#include "SerialPricer.h"
#include <memory>

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
