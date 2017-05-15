#ifndef SERVANTBASE_DIAGNOSISUTILITIES_HPP
#define SERVANTBASE_DIAGNOSISUTILITIES_HPP

#include <string>

class DiagnosisUtilities
{
public:
    static DiagnosisUtilities *getInstance();
    ~DiagnosisUtilities();
    std::string getIpFromIpify();
    bool portIsUsing(unsigned int port);

private:
    DiagnosisUtilities();
    unsigned int timer = 0;
};


#endif //SERVANTBASE_DIAGNOSISUTILITIES_HPP
