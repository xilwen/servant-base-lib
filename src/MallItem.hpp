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

    void downloadAdditionalFiles();

    bool filesAreReady();

    void installAdditionalInfoToLastInstalledMachine();

    const std::string &getFileSize() const;

    void setFileSize(const std::string &fileSize);

    const std::string &getManagementURL() const;

    void setManagementURL(const std::string &managementURL);

    unsigned int getPort() const;

    void setPort(unsigned int port);

    const std::string &getShareAdditionURL() const;

    void setShareAdditionURL(const std::string &shareAdditionURL);

    unsigned int getManagementPort() const;

    void setManagementPort(unsigned int managementPort);

    unsigned int getProfile0CPU() const;

    void setProfile0CPU(unsigned int profile0CPU);

    unsigned int getProfile0RAM() const;

    void setProfile0RAM(unsigned int profile0RAM);

    unsigned int getProfile1CPU() const;

    void setProfile1CPU(unsigned int profile1CPU);

    unsigned int getProfile1RAM() const;

    void setProfile1RAM(unsigned int profile1RAM);

    unsigned int getProfile2CPU() const;

    void setProfile2CPU(unsigned int profile2CPU);

    unsigned int getProfile2RAM() const;

    void setProfile2RAM(unsigned int profile2RAM);

    const std::string &getIntroductionURL() const;

    void setIntroductionURL(const std::string &introductionURL);

    const std::string &getProtocol() const;

    void setProtocol(const std::string &protocol);

    const std::string &getTipURL() const;

    void setTipURL(const std::string &tipURL);

    bool isOnlyOneServerAllowed() const;

    void setOnlyOneServerAllowed(bool onlyOneServerAllowed);

    unsigned int getSingletonPort() const;

    void setSingletonPort(unsigned int singletonPort);

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
            vboxImageFile,
            fileSize,
            managementURL,
            shareAdditionURL,
            introductionURL,
            protocol,
            tipURL;

    int cpuLevel = 0,
            ramLevel = 0,
            diskSpeedLevel = 0;
    unsigned int port = 0,
            managementPort = 0,
            singletonPort = 0,
            profile0CPU = 0,
            profile0RAM = 0,
            profile1CPU = 0,
            profile1RAM = 0,
            profile2CPU = 0,
            profile2RAM = 0;
    bool requireVMX = false,
            onlyOneServerAllowed = false;
};

#endif //SERVANTBASE_MALLITEM_HPP
