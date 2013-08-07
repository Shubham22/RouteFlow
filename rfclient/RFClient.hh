#include <net/if.h>
#include <map>
#include <vector>

#include "ipc/IPC.h"
#include "ipc/MongoIPC.h"
#include "ipc/RFProtocol.h"
#include "ipc/RFProtocolFactory.h"
#include "FlowTable.h"
#include "PortMapper.hh"

class RFClient : private RFProtocolFactory, private IPCMessageProcessor,
                 public InterfaceMap {
    public:
        RFClient(uint64_t id, const string &address);
        bool findInterface(const char *ifName, Interface *dst);

    private:
        FlowTable* flowTable;
        PortMapper* portMapper;
        IPCMessageService* ipc;
        uint64_t id;

        boost::mutex ifMutex; /* This guards both of the maps below. */
        map<string, Interface> ifacesMap;
        map<int, Interface*> interfaces;

        uint8_t hwaddress[IFHWADDRLEN];

        void startFlowTable();
        void startPortMapper(vector<Interface>);
        bool process(const string &from, const string &to, const string &channel, IPCMessage& msg);

        int set_hwaddr_byname(const char * ifname, uint8_t hwaddr[], int16_t flags);
        uint32_t get_port_number(string ifName);
        vector<Interface> load_interfaces();
};
