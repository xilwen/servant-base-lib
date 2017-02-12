#ifndef SERVANTBASE_VIRTUALMACHINE_H
#define SERVANTBASE_VIRTUALMACHINE_H

#include <string>

class VirtualMachine
{
public:
    VirtualMachine();

    void setUuid(std::wstring uuid);

    void setName(std::wstring name);

    std::wstring getUuid();
    std::wstring getName();
    std::wstring launch();
    std::wstring sendPowerOffSignal();
    void rename(std::wstring newName);

private:
    std::wstring uuid,
            name;

};


#endif //TESTEXE_VIRTUALMACHINE_H
