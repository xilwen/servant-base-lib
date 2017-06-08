#ifndef SERVANTBASE_VIRTUALMACHINE_H
#define SERVANTBASE_VIRTUALMACHINE_H

#include <string>

enum VirtualMachineState
{
    PoweredOff, Running, Aborted, Starting, Stopping, Unknown
};

class VirtualMachine
{
public:
    VirtualMachine();

    ~VirtualMachine();

    void setUuid(std::wstring uuid);

    void setName(std::wstring name);

    std::wstring getUuid();

    std::wstring getName();

    std::wstring launch();

    std::wstring sendPowerOffSignal();

    VirtualMachineState getStatus();

    VirtualMachine *getInstance();

    static VirtualMachine *getVirtualMachine(const std::wstring &nameOrUuid);

    void rename(std::wstring newName);

    std::string &getIconPath();

    void setIconPath(const std::string &iconPath);

    std::string &getType();

    void setType(const std::string &type);

    const std::string &getManagementURL() const;

    void setManagementURL(const std::string &managementURL);

    unsigned int getPortNumber() const;

    void setPortNumber(unsigned int portNumber);

    unsigned int getCustomPortNumber() const;

    void setCustomPortNumber(unsigned int customPortNumber);

    const std::string &getShareURL() const;

    void setShareURL(const std::string &shareURL);

    const std::string &getShareAdditionURL() const;

    void setShareAdditionURL(const std::string &shareAdditionURL);

    unsigned int getManagementPort() const;

    void setManagementPort(unsigned int managementPort);

    const std::string &getProtocol() const;

    void setProtocol(const std::string &protocol);

    const std::string &getTipURL() const;

    void setTipURL(const std::string &tipURL);

    bool isUseLocalIP() const;

    void setUseLocalIP(bool useLocalIP);

    void setCPUAmount(int cpuAmount);

    void setRAMAmount(int ramAmount);

    void addPortForwardingRule(unsigned int guestPort, unsigned int hostPort);

    void removePortForwardingRule(unsigned int guestPort);

    void saveUseLocalIP(bool useLocalIP);

    const std::string &getUuidString() const;

    void setUuidString(const std::string &uuidString);

    void saveCustomPort(unsigned int customPort);

    void enableWebmin();

    void disableWebmin();

    bool webminEnabled();

    unsigned int getSingletonPort() const;

    void setSingletonPort(unsigned int singletonPort);

    void exportMachine(std::wstring path);

private:
    std::wstring uuid,
            name;
    std::string uuidString,
            iconPath,
            type,
            managementURL,
            shareURL,
            shareAdditionURL,
            protocol,
            tipURL;
    unsigned int portNumber,
            customPortNumber,
            managementPort,
            singletonPort;
    bool useLocalIP = false;


};

#endif //TESTEXE_VIRTUALMACHINE_H
