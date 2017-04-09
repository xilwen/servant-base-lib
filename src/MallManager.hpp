#ifndef SERVANTBASE_MALLMANAGER_HPP
#define SERVANTBASE_MALLMANAGER_HPP

#include <vector>
#include <json.hpp>
#include "MallItem.hpp"

using json = nlohmann::json;


class MallManager
{
public:
    MallManager();

    ~MallManager();

    std::vector<MallItem> *getItemList();

    bool remoteServerIsReady();

    void updateItemList();

private:
    std::vector<MallItem> items;

    void getListFromRemoteServer();

    void loadLocalItemList();
};


#endif //SERVANTBASE_MALLMANAGER_HPP
