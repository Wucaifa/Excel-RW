#ifndef EXCELREAD_H
#define EXCELREAD_H

#include<CommonStructDefine.h>
#include<xmlFileOperation.h>
#include <QCoreApplication>
#include <QDebug>
#include "xlsxdocument.h" // 包含 QXlsx
#include <QFile>
#include<QVector>
#include<QDateTime>
struct StartTime{
    int col;
    QString starttime;
};

struct Position{
    int col;
    QString position;
};

struct path{
    QString startTime;
    QString startPosition;
    QString goalPosition;
};

struct PlaneExcel{
    QString id;
    QVector<path> transSections;
};

struct PlaneTransection{
    QString id;
    QVector<AgentSection> tranSection;
};

class ExcelRead
{
public:
    ExcelRead();
    void readExcel(const QString &filePath);
    void writeExcel();
    void matchPath();//解析转运xml到planeTranInfo，planeTranInfo信息整合到m_logInfo
    void readXml();
    void writeXml();
    void agentpathMatch(const QString &filePath);
    void xmlRewrite_tasknumber3();//重写起飞section前一段转运路径时间,针对m_logInfo,排序sectionID
private:
    QVector<StartTime> starttime;
    QVector<QVector<Position>> position;
    QVector<PlaneExcel> planeInfo;
    QVector<PlaneTransection> planeTranInfo;
    LogXmlInfo m_logInfo;
    QString m_currTranPath;//当前转运xml路径
    QString m_currAgentPath;
};


#endif // EXCELREAD_H
