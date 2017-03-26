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

private:
    std::wstring uuid,
            name;

};


#endif //TESTEXE_VIRTUALMACHINE_H
