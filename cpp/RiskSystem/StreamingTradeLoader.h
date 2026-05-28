#ifndef STREAMINGTRADELOADER_H
#define STREAMINGTRADELOADER_H

#include "../Loaders/ITradeLoader.h"
#include "../Models/ITrade.h"
#include "../Models/IScalarResultReceiver.h"
#include "../Models/IPricingEngine.h"
#include "SerialPricer.h"
#include <memory>
#include <vector>
#include <map>
#include <string>

class StreamingTradeLoader : Pricer {
private:
    std::vector<std::unique_ptr<ITradeLoader>> getTradeLoaders();
    
public:
    ~StreamingTradeLoader();
    void loadAndPrice(IScalarResultReceiver* resultReceiver);
};

#endif // STREAMINGTRADELOADER_H
