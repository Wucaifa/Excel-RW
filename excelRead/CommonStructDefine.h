#ifndef COMMONSTRUCTDEFINE_H
#define COMMONSTRUCTDEFINE_H

#include <iostream>
#include <vector>
#include <map>
#include <QStringList>
#include <QVector>
#include "DBTableClass.h"
#include <QApplication>

#define TimeFORMAT "yyyy/MM/dd-hh:mm:ss.zzz"
#define TIMERATIO 1000
const QString g_projectPath = "/projectFile/";

#define FILESUFFIX ".jzj"

struct PlayBackRecord
{
    QString projectName;    //所属项目
    QString playbackTitle;  //回放文件标题
    QString playStartTimeStr;  //播放开始时间
    QString playFinishTimeStr;  //播放结束时间
    long long playTimeMs;     //播放持续时长 ms
    int playFrame;          //播放结束帧
    QString JBMapFilePath;
    QString JKMapFilePath;
    QString AgentFilePath;

};

struct projectConfig
{
    QString projectName;
    QString taskName;
    int taskId;
    QString JBMapPath;
    QString JKMapPath;
    QString allstarttime;
};


struct ElevatorSection
{
    int id;
    int taskNumber;
    emBoardstate startBoardstate;
    emBoardstate GoalBoardstate;
    QString startTime;
    float duration;
};

//升降机 结构
struct ElevatorParameter
{
    int id;
    QString agentname;
    int ele_priority;
    int ele_location;
    float movespeed;

    emBoardstate initBoardstate;
    QVector<ElevatorSection> elevatorSection;
};

struct DeflectorSection
{
    int id;
    int taskNumber;
    int startLocationstate; //偏流板位置状态  0表示放平，1表示竖起
    int GoalLocationstate;  //偏流板位置状态  0表示放平，1表示竖起
    QString startTime;
    float duration;
};

//偏流板 结构
struct DeflectorParameter
{
    int id;
    QString agentname;
    int def_priority;
    int def_location;   //位置状态， 0表示放平，1表示竖起
    float rotationspeed;

    int initLocationstate;
    QVector<DeflectorSection> deflectorSection;
};


struct ColorStyle
{
    int r;
    int g;
    int b;
    int a = 250;
};

struct LineStyle
{
    ColorStyle linecolor;
    std::string lineType;
    int lineWidth;
};


struct MapOutPolygn
{
    ColorStyle fillColor;
    LineStyle lineStyle;
    QString polygnName;     //升降机还是偏流板的名称
    QString polygonType;    //是升降机还是偏流板 elevator    shipisland  deflector
    std::map<int,std::pair<int,int>> outPolygnMap; //地图轮廓
};

struct NormalLine
{
    LineStyle lineStyle;
    int sx;
    int sy;
    int gx;
    int gy;
};

struct CircleInfo
{
    int cx;
    int cy;
    int cr;
    LineStyle lineStyle;
};

struct MapBaseInfo
{
    QString mapName;
    int rowCount;
    int columnCount;
    int gridSize;
    int X_bias;
    int Y_bias;
    int realsize;
    int tableX;
    int tableY;

    MapOutPolygn outPolygnInfo;

    std::vector<MapOutPolygn> inPolygnInfo;
    std::vector<NormalLine> normalLineInfo;
    std::vector<CircleInfo> circleInfo;
//    std::vector<std::pair<int,int>> barrierVec; //障碍墙
//    std::vector<std::pair<int,int>> nonbarrierVec; //非障碍墙
};

struct MapPosData
{
    int id;
    QString name;
    QString boardstate;
    QPoint pos;
    QString heading;
    QString type;
};

struct stationLocal
{
    int id;
    QString name;
    float stationX;
    float stationY;
    float stationHeading;
    AgentSection section;
};

struct MapInfo
{
    bool hasBoardMap;
    bool hasHangarMap;
    MapBaseInfo BoardMap;       //甲板地图样式
    MapBaseInfo HangarMap;       //机库地图样式
    QVector<MapPosData> BoardMapStation;  //甲板地图站位
    QVector<MapPosData> HangarMapStation; //机库地图站位
    QVector<stationLocal> TakeoffPos;    //起飞信息
    QVector<stationLocal> TakelandPos;   //降落信息
};

struct LogXmlInfo
{
    QString allStartTime;
    MapInfo mapStyle;
    QVector<DBPlaneTable> agentPlaneInfo;
    QVector<DBGDCarTable> agentGDCarInfo;
    QVector<DBQYCarTable> agentQYCarInfo;
    QVector<DBPersonTable> agentOperInfo;
    QVector<ElevatorParameter> agentElevatorInfo;
    QVector<DeflectorParameter> agentDeflectorInfo;

};

enum class LogDisplayStatus
{
    emNone = 0,     //无状态
    emRunning,        //已开始演示
    emPause,        //暂停演示中
};



enum class ItemType
{
    emBarrierWall = 0,      //障碍墙
    emNonBarrierWall,
    emAgentPlane,        //智能体飞机
    emDynamicObstacle,     //动态障碍物
    emAgentGDCar,        //智能体挂弹车
    emAgentQYCar,        //智能体牵引车
    emAgentOper,     //地面操作人员
    emAgentElevator,    //升降机
    emAgentDeflector    //偏流板
};


#include <QPointF>

//动画使用参数
struct ItemAnimationParam
{
    int agentID;
    double step;
    QPointF pos;
    float logicX;
    float logicY;
    int wingState;
    int tasknumber;
    double rotationAngle;
};


enum class ItemWingState
{
   emUnfold = 0,    //展翼
   emFold           //折翼
};

enum class MenuType
{
   emStandWait = 0,    //原地等待
   emRefuel ,          //加油
   emLoadRounds, //挂弹
   emTransport, //转运
   emGD ,   //惯导
   emNJ ,   //暖机
   emQF,    //起飞
   EMJL,     //降落
   emSky,   //  在空中
   emAerate,//  充气
   emCharge,    //充电
   emSport, //运动设置
   emWork,  //工作设置
   emStillness, //静止
   emEdit,  //编辑
   emAdd,   //新增
   emDelete,    //删除

   emSetBaseInfo,   //设置基本参数
   emSetInitInfo,   //设置初始状态
};

struct ConfigXmlInfo
{
    std::map<int,QString> TaskID_Name;
    QString filePath;

    //数据库
    QString dataBaseName;
    QString dataBaseIP;
    QString dataBaseUser;
    QString dataBasePassword;
    int dataBasePort;

    //udp
    QString ipAddress;
    quint16 port;
};

struct rotationForAnima
{
    qreal currrotation;
    qreal step;
    qreal rotationerror;
};



#endif // COMMONSTRUCTDEFINE_H
