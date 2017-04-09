#ifndef SERVANTBASE_MALLITEM_HPP
#define SERVANTBASE_MALLITEM_HPP

#include <string>

class MallItem
{
public:
    void setName(const std::string &name);

    void setDescription(const std::string &description);

    void setProducer(const std::string &producer);

    void setIconPath(const std::string &iconPath);

    void setUpdateDate(const std::string &updateDate);

    void setOperatingSystem(const std::string &operatingSystem);

    void setManagementUI(const std::string &managementUI);

    void setIntroduction(const std::string &introduction);

    const std::string &getName() const;

    const std::string &getDescription() const;

    const std::string &getProducer() const;

    const std::string &getIconPath() const;

    const std::string &getUpdateDate() const;

    const std::string &getOperatingSystem() const;

    const std::string &getManagementUI() const;

    const std::string &getIntroduction() const;

    const std::string &getVboxImageFile() const;

    void setVboxImageFile(const std::string &vboxImageFile);

    int getCpuLevel() const;

    void setCpuLevel(int cpuLevel);

    int getRamLevel() const;

    void setRamLevel(int ramLevel);

    int getDiskSpeedLevel() const;

    void setDiskSpeedLevel(int diskSpeedLevel);

    bool isRequireVMX() const;

    void setRequireVMX(bool requireVMX);

private:
    //All These Strings Use UTF-8 in std::string.
    std::string name,
            description,
            producer,
            iconPath,
            updateDate,
            operatingSystem,
            managementUI,
            introduction,
            vboxImageFile;
    int cpuLevel,
            ramLevel,
            diskSpeedLevel;
    bool requireVMX;
};


#endif //SERVANTBASE_MALLITEM_HPP
