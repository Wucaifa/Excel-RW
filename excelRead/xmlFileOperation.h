#ifndef xmlFileOperation_H
#define xmlFileOperation_H

#include <QDomDocument>
#include <QObject>
#include "DBTableClass.h"
#include "CommonStructDefine.h"
#include "tinyxml2.h"

using namespace tinyxml2;

class xmlFileOperationPrivate;

class xmlFileOperation : public QObject
{
    Q_OBJECT

public:
    static xmlFileOperation* getInstance() {
        static xmlFileOperation instance;      //此处线程安全
        return &instance;
    }

    void setCurrentFile(const QString &filepath);
    void readConfigXml(ConfigXmlInfo& config);
    void readLogXmlFile(LogXmlInfo& logXmlInfo,const QString& filePath);
//    void writeXmlFile(const QString &fileName);
    void writeLogXmlFile(const QString &fileName, const QString &timeValue);

    void writeMysqlConfig(QString dataBaseName,QString dataBaseIP,QString dataBaseUser,QString dataBasePassword,int dataBasePort);
    void writeUdpConfig(QString ipAddress,quint16 port);

    void readMysqlConfig(QString &dataBaseName,QString &dataBaseIP,QString& dataBaseUser,QString &dataBasePassword,int& dataBasePort);
    void readUdpConfig(QString& ipAddress,quint16& port);

    void writeProjectFile(const projectConfig &config);
    bool getProjectInfo(const QString &path,projectConfig & config);
    bool deleteProjectFile(const QString &projectName);

    void writePlayBackFile(const PlayBackRecord& info);
    bool getPlayBackFile(const QString &prjName,const QString &name,PlayBackRecord & config);
    bool deletePlayBackFile(const QString &prjName,const QString &dirName);

    //void rewriteXmlFile(const QString &fileName, LogXmlInfo &logXmlInfo, const QString &agentNodeName, const int &id);

    /*
    *   brief   更新xml  当updateID为-1时，该函数为替换更新全部智能体信息，大于0时，为更新指定智能体信息
    *   param[in]    agentInfos-需要更新写入的信息，updateID大于0时，只有一个
    *   param[in]    updateID-智能体id
    *   return       void
    */
    void rewriteXmlFile4Plane(const QVector<DBPlaneTable> &agentInfos,int updateID = -1);
    void rewriteXmlFile4QYCar(const QVector<DBQYCarTable> &agentInfos,int updateID = -1);
    void rewriteXmlFile4GDCar(const QVector<DBGDCarTable> &agentInfos,int updateID = -1);
    void rewriteXmlFile4Operator(const QVector<DBPersonTable> &agentInfos,int updateID = -1);
    void rewriteXmlFile4Elevator(const QVector<ElevatorParameter> &agentInfos,int updateID = -1);
    void rewriteXmlFile4Deflectorr(const QVector<DeflectorParameter> &agentInfos,int updateID = -1);
private:
    xmlFileOperation(QWidget *parent = nullptr);
    ~xmlFileOperation();
    xmlFileOperation(const xmlFileOperation& it) = delete;  //删除拷贝构造
    xmlFileOperation& operator=(const xmlFileOperation& s) = delete;//删除赋值

    xmlFileOperationPrivate *d;
};
#endif // xmlFileOperation_H
