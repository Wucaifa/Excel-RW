#ifndef DBTABLECLASS_H
#define DBTABLECLASS_H

#include <QObject>
#include <QSharedPointer>
#include <QSqlQuery>
#include <QVariant>
#include <QMetaProperty>
#include <QVector>


#define p_DataPtr(Class) \
    QSharedPointer<Class> p_DataPtr(new Class);

#define AGENTID "agentID"

enum emLoadstate
{
    emEmpty,    //空载
    empart,     //部分载重
    emfull,     //满载
};

enum emBoardstate
{
    emHangar = 0, // 在机库
    emBoard,      // 在甲板
    emSKY,        // 在空中

};

struct GDinfo
{
    int agentPlaneGDType;       //GD的类型是对应挂载点的，都挂2型
    int agentPlaneGDTime;       //每个位置挂对应D的时间   秒
    int agentPlaneGDNum;     //使的挂载点（需要判断是否为空点）
};


class AgentTransportSection
{
public:

    AgentTransportSection(){}
    AgentTransportSection(const AgentTransportSection& obj)
    {
       id                        = obj.id                      ;
       agentStartBoardstateType  = obj.agentStartBoardstateType;
       agentGoalBoardstateType   = obj.agentGoalBoardstateType ;
       agentStartX               = obj.agentStartX             ;
       agentStartY               = obj.agentStartY             ;
       agentGoalX                = obj.agentGoalX              ;
       agentGoalY                = obj.agentGoalY              ;
       agentStartHeading         = obj.agentStartHeading       ;
       agentGoalHeading          = obj.agentGoalHeading        ;
       agentgDuration            = obj.agentgDuration          ;
    }
    AgentTransportSection& operator=(const AgentTransportSection& obj)
    {
        if(this == &obj)
            return *this;
        id                        = obj.id                      ;
        agentStartBoardstateType  = obj.agentStartBoardstateType;
        agentGoalBoardstateType   = obj.agentGoalBoardstateType ;
        agentStartX               = obj.agentStartX             ;
        agentStartY               = obj.agentStartY             ;
        agentGoalX                = obj.agentGoalX              ;
        agentGoalY                = obj.agentGoalY              ;
        agentStartHeading         = obj.agentStartHeading       ;
        agentGoalHeading          = obj.agentGoalHeading        ;
        agentgDuration            = obj.agentgDuration          ;
        return *this;
    }
    int id;
    emBoardstate agentStartBoardstateType;   //初始甲板状态类型
    emBoardstate agentGoalBoardstateType;   //结束甲板状态类型
    float agentStartX;
    float agentStartY;
    float agentGoalX;
    float agentGoalY;
    float agentStartHeading;
    float agentGoalHeading;
    float agentgDuration;
};


class AgentSection
{
public:

    AgentSection(){}
    AgentSection(const AgentSection& obj)
    {
        id   = obj.id;
        tasknumber = obj.tasknumber;
        agentStartBoardstateType = obj.agentStartBoardstateType;
        agentGoalBoardstateType = obj.agentGoalBoardstateType;
        agentStartX = obj.agentStartX;
        agentStartY = obj.agentStartY;
        agentGoalX  = obj.agentGoalX ;
        agentGoalY  = obj.agentGoalY ;
        agentStartHeading = obj.agentStartHeading;
        agentGoalHeading  = obj.agentGoalHeading ;
        agentStartTime    = obj.agentStartTime   ;
        agentgDuration    = obj.agentgDuration   ;
        agentGDinfo       = obj.agentGDinfo      ;
        required_oil_volume = obj.required_oil_volume;
        connectstate     = obj.connectstate    ;
        workstate        = obj.workstate       ;
        pathfound        = obj.pathfound       ;
        runtime          = obj.runtime         ;
        transportSection = obj.transportSection;
        wingstate          = obj.wingstate         ;
    }
    AgentSection& operator=(const AgentSection& obj)
    {
        if(this == &obj)
            return *this;
        id   = obj.id;
        tasknumber = obj.tasknumber;
        agentStartBoardstateType = obj.agentStartBoardstateType;
        agentGoalBoardstateType = obj.agentGoalBoardstateType;
        agentStartX = obj.agentStartX;
        agentStartY = obj.agentStartY;
        agentGoalX  = obj.agentGoalX ;
        agentGoalY  = obj.agentGoalY ;
        agentStartHeading = obj.agentStartHeading;
        agentGoalHeading  = obj.agentGoalHeading ;
        agentStartTime    = obj.agentStartTime   ;
        agentgDuration    = obj.agentgDuration   ;
        agentGDinfo       = obj.agentGDinfo      ;
        required_oil_volume = obj.required_oil_volume;
        connectstate     = obj.connectstate    ;
        workstate        = obj.workstate       ;
        pathfound        = obj.pathfound       ;
        runtime          = obj.runtime         ;
        transportSection = obj.transportSection;
        wingstate          = obj.wingstate         ;
        return *this;
    }

//    bool operator>(const AgentSection& A)
//    {
//        return agentStartTime > A.agentStartTime;
//    }

    int id;
    int tasknumber;
    emBoardstate agentStartBoardstateType;   //初始甲板状态类型
    emBoardstate agentGoalBoardstateType;   //结束甲板状态类型
    float agentStartX;
    float agentStartY;
    float agentGoalX;
    float agentGoalY;
    float agentStartHeading;
    float agentGoalHeading;
    QString agentStartTime;//智能体执行某任务的开始时刻
    float agentgDuration;
    QVector<GDinfo> agentGDinfo;   // 飞机 GD信息`
    float required_oil_volume;      //飞机需要的加油量
    int connectstate;   //表示连接状态  0 断开  1 连接
    bool workstate;     //表示装载状态  0 空闲  1 在工作
    bool pathfound;
    double runtime;
    int wingstate;                  //表示机翼状态  0 展开  1 折起
    QVector<AgentTransportSection>  transportSection;

};

class AgentInitState
{
public:

    AgentInitState(){
        initX            = 0;
        initY            = 0;
        initHeading      = 0;
        initOilVolume    = 0;
        initWP           = false;
        initINS          = false;
        initINF          = false;
        initCHA          = false;
        initFreeGD_local = "";
        initWingstate    = 0;
        initConnectstate = false;
        initWorkstate    = false;
        initBoardstate   = emBoardstate::emBoard;//初始甲板
        initLoadstate    = 0;
    }
    AgentInitState(const AgentInitState& obj)
    {
        initX            = obj.initX           ;
        initY            = obj.initY           ;
        initHeading      = obj.initHeading     ;
        initOilVolume    = obj.initOilVolume   ;
        initWP           = obj.initWP          ;
        initINS          = obj.initINS         ;
        initINF          = obj.initINF         ;
        initCHA          = obj.initCHA         ;
        initFreeGD_local = obj.initFreeGD_local;
        initWingstate    = obj.initWingstate   ;
        initConnectstate = obj.initConnectstate;
        initWorkstate    = obj.initWorkstate   ;
        initBoardstate   = obj.initBoardstate  ;
        initLoadstate    = obj.initLoadstate   ;
    }
    AgentInitState& operator=(const AgentInitState& obj)
    {
        if(this == &obj)
            return *this;
        initX            = obj.initX           ;
        initY            = obj.initY           ;
        initHeading      = obj.initHeading     ;
        initOilVolume    = obj.initOilVolume   ;
        initWP           = obj.initWP          ;
        initINS          = obj.initINS         ;
        initINF          = obj.initINF         ;
        initCHA          = obj.initCHA         ;
        initFreeGD_local = obj.initFreeGD_local;
        initWingstate    = obj.initWingstate   ;
        initConnectstate = obj.initConnectstate;
        initWorkstate    = obj.initWorkstate   ;
        initBoardstate   = obj.initBoardstate  ;
        initLoadstate    = obj.initLoadstate   ;
        return *this;
    }
    float initX;
    float initY;
    float initHeading;
    float initOilVolume;
    bool  initWP;            //否完成暖机  false表示未完成，true表示完成
    bool  initINS;           //是否完成对准  false表示未完成，true表示完成
    bool  initINF;           //是否完成充气  f表示未完成，t表示完成
    bool  initCHA;       //是否完成充电  f表示未完成，t表示完成
    QString initFreeGD_local;
    int     initWingstate;      //  表示机翼状态  0 展开  1 折起
    bool  initConnectstate;      //QYCar 是否连接  f断开，t连接
    bool  initWorkstate;         //人员状态  f空闲，t工作
    emBoardstate initBoardstate;    //
    int initLoadstate;   //表示GD车装载状态  0 空载  1 部分载重   2 满载

};

/*
    数据库表基类
*/

class DBTableBaseTable : public QObject
{

public:
    DBTableBaseTable(){}
    virtual ~DBTableBaseTable(){};
    //virtual void parseQueryInfo(QList<QSharedPointer<DBTableBaseTable>>& infoList,QSqlQuery& query) = 0;
};


/*数据库操作人员类*/
class DBPersonTable : public DBTableBaseTable
{
    Q_OBJECT

    Q_CLASSINFO("TableName","person_table")

    Q_PROPERTY(int pID READ pID WRITE setpID)
    Q_CLASSINFO("pID","int(11) AUTO_INCREMENT PRIMARY KEY")

    Q_PROPERTY(QString pName READ pName WRITE setpName)
    Q_CLASSINFO("pName","VARCHAR(20) NOT NULL")

    Q_PROPERTY(QString pType READ pType WRITE setpType)
    Q_CLASSINFO("pType","VARCHAR(20) NOT NULL")

    Q_PROPERTY(int pPriority READ pPriority WRITE setpPriority)
    Q_CLASSINFO("pPriority","int(11)")

    Q_PROPERTY(double graphicsR READ graphicsR WRITE setgraphicsR)
    Q_CLASSINFO("graphicsR","double")

    Q_PROPERTY(float moveSpeed READ moveSpeed WRITE setmoveSpeed)
    Q_CLASSINFO("moveSpeed","float")

    Q_PROPERTY(float rotationSpeed READ rotationSpeed WRITE setrotationSpeed)
    Q_CLASSINFO("rotationSpeed","float")


public:
    DBPersonTable(){}
    DBPersonTable(const DBPersonTable& obj)
    {
        m_personID = obj.m_personID;
        m_personName = obj.m_personName;
        m_personType = obj.m_personType;
        m_priority = obj.m_priority;
        m_graphicsR= obj.m_graphicsR;
        m_moveSpeed = obj.m_moveSpeed;
        m_rotationSpeed = obj.m_rotationSpeed;
        agentInitState = obj.agentInitState;
        agentSection = obj.agentSection;
    }
    DBPersonTable& operator=(const DBPersonTable& obj)
    {
        if(this == &obj)
            return *this;
        m_personID = obj.m_personID;
        m_personName = obj.m_personName;
        m_personType = obj.m_personType;
        m_priority = obj.m_priority;
        m_graphicsR= obj.m_graphicsR;
        m_moveSpeed = obj.m_moveSpeed;
        m_rotationSpeed = obj.m_rotationSpeed;
        agentInitState = obj.agentInitState;
        agentSection = obj.agentSection;

        return *this;
    }

    int pID() const {return m_personID;}
    Q_INVOKABLE void setpID(int pID){m_personID = pID;}

    QString pName() const {return m_personName;}
    Q_INVOKABLE void setpName(QString pName){m_personName = pName;}

    QString pType() const {return m_personType;}
    Q_INVOKABLE void setpType(QString pType){m_personType = pType;}

    int pPriority() const {return m_priority;}
    Q_INVOKABLE void setpPriority(int pPriority){m_priority = pPriority;}

    double graphicsR() const {return m_graphicsR;}
    Q_INVOKABLE void setgraphicsR(double graphicsR){m_graphicsR = graphicsR;}

    float moveSpeed() const {return m_moveSpeed;}
    Q_INVOKABLE void setmoveSpeed(float moveSpeed){m_moveSpeed = moveSpeed;}

    float rotationSpeed() const {return m_rotationSpeed;}
    Q_INVOKABLE void setrotationSpeed(float rotationSpeed){m_rotationSpeed = rotationSpeed;}

    int m_personID = 0;//主键 人员ID
    QString m_personName = "aaaa";// 外键 人员编号
    QString m_personType = "0";  //人员类型
    int m_priority = 0;    //外键 人员优先级
    double m_graphicsR= 2;    //外键 图元半径
    float m_moveSpeed = 3; //外键 移动速度
    float m_rotationSpeed = 4;//外键 旋转速度

    AgentInitState agentInitState;
    QVector<AgentSection> agentSection;
};


/*数据库QY车类*/
class DBQYCarTable : public DBTableBaseTable
{
    Q_OBJECT

    ////以下属性名需要唯一 对应数据库表字段  其次classinfo的key需要和属性名保持一直 方便做映射  主键需要在第一个
    /// Q_PROPERTY 里的write方法也不能随意写 按下述格式写   否则会影响查询后的数据返回
    Q_CLASSINFO("TableName","QYcar_table")

    Q_PROPERTY(int pID READ pID WRITE setpID)
    Q_CLASSINFO("pID","int(11) AUTO_INCREMENT PRIMARY KEY")

    Q_PROPERTY(QString pName READ pName WRITE setpName)
    Q_CLASSINFO("pName","VARCHAR(20) NOT NULL")

    Q_PROPERTY(QString pType READ pType WRITE setpType)
    Q_CLASSINFO("pType","VARCHAR(20) NOT NULL")

    Q_PROPERTY(int pPriority READ pPriority WRITE setpPriority)
    Q_CLASSINFO("pPriority","int(11)")

    Q_PROPERTY(float pSize READ pSize WRITE setpSize)
    Q_CLASSINFO("pSize","float")

    Q_PROPERTY(QString graphicsScale READ graphicsScale WRITE setgraphicsScale)
    Q_CLASSINFO("graphicsScale","VARCHAR(20) NOT NULL")

    Q_PROPERTY(float carWidth READ carWidth WRITE setcarWidth)
    Q_CLASSINFO("carWidth","float")

    Q_PROPERTY(float carLength READ carLength WRITE setcarLength)
    Q_CLASSINFO("carLength","float")

    Q_PROPERTY(float moveSpeed READ moveSpeed WRITE setmoveSpeed)
    Q_CLASSINFO("moveSpeed","float")

    Q_PROPERTY(float rotationSpeed READ rotationSpeed WRITE setrotationSpeed)
    Q_CLASSINFO("rotationSpeed","float")

    Q_PROPERTY(double minTurnR READ minTurnR WRITE setminTurnR)
    Q_CLASSINFO("minTurnR","double")

    Q_PROPERTY(QString graphicsPath READ graphicsPath WRITE setgraphicsPath)
    Q_CLASSINFO("graphicsPath","TEXT")

    Q_PROPERTY(float wheelbase READ wheelbase WRITE setwheelbase)
    Q_CLASSINFO("wheelbase","float")

    Q_PROPERTY(float thread READ thread WRITE setthread)
    Q_CLASSINFO("thread","float")

    Q_PROPERTY(float wheelR READ wheelR WRITE setwheelR)
    Q_CLASSINFO("wheelR","float")

    Q_PROPERTY(float wheelWidth READ wheelWidth WRITE setwheelWidth)
    Q_CLASSINFO("wheelWidth","float")

    Q_PROPERTY(float agentFront READ agentFront WRITE setagentFront)
    Q_CLASSINFO("agentFront","float")

    Q_PROPERTY(float agentBack READ agentBack WRITE setagentBack)
    Q_CLASSINFO("agentBack","float")
    //alter table emp add constraint fk_emp_dept_id foreign key (dept_id) references dept(id);

public:

    int pID() const {return m_QYcarID;}
    Q_INVOKABLE void setpID(int pID){m_QYcarID = pID;}

    QString pName() const {return m_QYcarName;}
    Q_INVOKABLE void setpName(QString pName){m_QYcarName = pName;}

    QString pType() const {return m_QYcarType;}
    Q_INVOKABLE void setpType(QString pType){m_QYcarType = pType;}

    int pPriority() const {return m_priority;}
    Q_INVOKABLE void setpPriority(int pPriority){m_priority = pPriority;}

    float pSize() const {return m_size;}
    Q_INVOKABLE void setpSize(float pSize){m_size = pSize;}

    QString graphicsScale() const {return m_graphicsScale;}
    Q_INVOKABLE void setgraphicsScale(QString graphicsScale){m_graphicsScale = graphicsScale;}

    float carWidth() const {return m_carWidth;}
    Q_INVOKABLE void setcarWidth(float carWidth){m_carWidth = carWidth;}

    float carLength() const {return m_carLength;}
    Q_INVOKABLE void setcarLength(float carLength){m_carLength = carLength;}

    float moveSpeed() const {return m_moveSpeed;}
    Q_INVOKABLE void setmoveSpeed(float moveSpeed){m_moveSpeed = moveSpeed;}

    float rotationSpeed() const {return m_moveSpeed;}
    Q_INVOKABLE void setrotationSpeed(float rotationSpeed){m_rotationSpeed = rotationSpeed;}

    double minTurnR() const {return m_minTurnR;}
    Q_INVOKABLE void setminTurnR(double minTurnR){m_minTurnR = minTurnR;}

    QString graphicsPath() const {return m_graphicsPath;}
    Q_INVOKABLE void setgraphicsPath(QString graphicsPath){m_graphicsPath = graphicsPath;}

    float wheelbase() const {return m_wheelbase;}
    Q_INVOKABLE void setwheelbase(float wheelbase){m_wheelbase = wheelbase;}

    float thread() const {return m_thread;}
    Q_INVOKABLE void setthread(float thread){m_thread = thread;}

    float wheelR() const {return m_wheelR;}
    Q_INVOKABLE void setwheelR(float wheelR){m_wheelR = wheelR;}

    float wheelWidth() const {return m_wheelWidth;}
    Q_INVOKABLE void setwheelWidth(float wheelWidth){m_wheelWidth = wheelWidth;}

    float agentFront() const {return m_agentFront;}
    Q_INVOKABLE void setagentFront(float agentFront){m_agentFront = agentFront;}

    float agentBack() const {return m_agentBack;}
    Q_INVOKABLE void setagentBack(float agentBack){m_agentBack = agentBack;}



    DBQYCarTable(){}
    DBQYCarTable(const DBQYCarTable& obj)
    {
        m_QYcarID =         obj.m_QYcarID;
        m_QYcarName =       obj.m_QYcarName;
        m_QYcarType =       obj.m_QYcarType;
        m_priority =        obj.m_priority;
        m_size =            obj.m_size;
        m_graphicsScale =   obj.m_graphicsScale;
        m_carWidth =        obj.m_carWidth;
        m_carLength =       obj.m_carLength;
        m_moveSpeed =       obj.m_moveSpeed;
        m_rotationSpeed =   obj.m_rotationSpeed;
        m_minTurnR =        obj.m_minTurnR;
        m_graphicsPath =    obj.m_graphicsPath;
        m_wheelbase =       obj.m_wheelbase;
        m_thread =          obj.m_thread;
        m_wheelR =          obj.m_wheelR;
        m_wheelWidth =      obj.m_wheelWidth;
        m_agentFront =      obj.m_agentFront;
        m_agentBack =       obj.m_agentBack;
        agentInitState =    obj.agentInitState;
        agentSection =      obj.agentSection;
    }
    DBQYCarTable& operator=(const DBQYCarTable& obj)
    {
        if(this == &obj)
            return *this;
        m_QYcarID =         obj.m_QYcarID;
        m_QYcarName =       obj.m_QYcarName;
        m_QYcarType =       obj.m_QYcarType;
        m_priority =        obj.m_priority;
        m_size =            obj.m_size;
        m_graphicsScale =   obj.m_graphicsScale;
        m_carWidth =        obj.m_carWidth;
        m_carLength =       obj.m_carLength;
        m_moveSpeed =       obj.m_moveSpeed;
        m_rotationSpeed =   obj.m_rotationSpeed;
        m_minTurnR =        obj.m_minTurnR;
        m_graphicsPath =    obj.m_graphicsPath;
        m_wheelbase =       obj.m_wheelbase;
        m_thread =          obj.m_thread;
        m_wheelR =          obj.m_wheelR;
        m_wheelWidth =      obj.m_wheelWidth;
        m_agentFront =      obj.m_agentFront;
        m_agentBack =       obj.m_agentBack;
        agentInitState =    obj.agentInitState;
        agentSection =      obj.agentSection;

        return *this;
    }

    int m_QYcarID = 0;//主键 QY车ID
    QString m_QYcarName = "aaaa";//外键 QY车编号
    QString m_QYcarType = "0";  //外键 QY车类型
    int m_priority = 0;
    float m_size = 0;
    QString m_graphicsScale = "1:25";    //外键 图元比例尺
    float m_carWidth = 2;    //外键 车辆宽度
    float m_carLength = 3; //外键 人员长度
    float m_moveSpeed = 0;//外键 移动速度
    float m_rotationSpeed = 0;//外键 旋转速度
    double m_minTurnR = 0;//外键 最小转弯半径
    QString m_graphicsPath = "aaaa";//外键 图元路径
    float m_wheelbase = 1;
    float m_thread = 2;
    float m_wheelR = 3;
    float m_wheelWidth = 4;
    float m_agentFront = 5;
    float m_agentBack = 6;

    AgentInitState agentInitState;
    QVector<AgentSection> agentSection;
};


/*数据库GD车类*/
class DBGDCarTable : public DBTableBaseTable
{
    Q_OBJECT

    ////以下属性名需要唯一 对应数据库表字段  其次classinfo的key需要和属性名保持一直 方便做映射  主键需要在第一个
    /// Q_PROPERTY 里的write方法也不能随意写 按下述格式写   否则会影响查询后的数据返回
    Q_CLASSINFO("TableName","GDcar_table")

    Q_PROPERTY(int pID READ pID WRITE setpID)
    Q_CLASSINFO("pID","int(11) AUTO_INCREMENT PRIMARY KEY")

    Q_PROPERTY(QString pName READ pName WRITE setpName)
    Q_CLASSINFO("pName","VARCHAR(20) NOT NULL")

    Q_PROPERTY(QString pType READ pType WRITE setpType)
    Q_CLASSINFO("pType","VARCHAR(20) NOT NULL")

    Q_PROPERTY(int pPriority READ pPriority WRITE setpPriority)
    Q_CLASSINFO("pPriority","int(11)")

    Q_PROPERTY(float pSize READ pSize WRITE setpSize)
    Q_CLASSINFO("pSize","float")

    Q_PROPERTY(QString graphicsScale READ graphicsScale WRITE setgraphicsScale)
    Q_CLASSINFO("graphicsScale","VARCHAR(20) NOT NULL")

    Q_PROPERTY(float carWidth READ carWidth WRITE setcarWidth)
    Q_CLASSINFO("carWidth","float")

    Q_PROPERTY(float carLength READ carLength WRITE setcarLength)
    Q_CLASSINFO("carLength","float")

    Q_PROPERTY(float moveSpeed READ moveSpeed WRITE setmoveSpeed)
    Q_CLASSINFO("moveSpeed","float")

    Q_PROPERTY(float rotationSpeed READ rotationSpeed WRITE setrotationSpeed)
    Q_CLASSINFO("rotationSpeed","float")

    Q_PROPERTY(double minTurnR READ minTurnR WRITE setminTurnR)
    Q_CLASSINFO("minTurnR","double")

    Q_PROPERTY(QString graphicsPath READ graphicsPath WRITE setgraphicsPath)
    Q_CLASSINFO("graphicsPath","TEXT")//text比较长，NOT NULL不可以是空的

    Q_PROPERTY(QString canGDType READ canGDType WRITE setcanGDType)
    Q_CLASSINFO("canGDType","VARCHAR(255) NOT NULL")

    Q_PROPERTY(int canGDNum READ canGDNum WRITE setcanGDNum)
    Q_CLASSINFO("canGDNum","int(11)")

    Q_PROPERTY(float wheelbase READ wheelbase WRITE setwheelbase)
    Q_CLASSINFO("wheelbase","float")

    Q_PROPERTY(float thread READ thread WRITE setthread)
    Q_CLASSINFO("thread","float")

    Q_PROPERTY(float wheelR READ wheelR WRITE setwheelR)
    Q_CLASSINFO("wheelR","float")

    Q_PROPERTY(float wheelWidth READ wheelWidth WRITE setwheelWidth)
    Q_CLASSINFO("wheelWidth","float")

    Q_PROPERTY(float agentFront READ agentFront WRITE setagentFront)
    Q_CLASSINFO("agentFront","float")

    Q_PROPERTY(float agentBack READ agentBack WRITE setagentBack)
    Q_CLASSINFO("agentBack","float")

    //alter table emp add constraint fk_emp_dept_id foreign key (dept_id) references dept(id);

public:

    int pID() const {return m_GDcarID;}
    Q_INVOKABLE void setpID(int pID){m_GDcarID = pID;}

    QString pName() const {return m_GDcarName;}
    Q_INVOKABLE void setpName(QString pName){m_GDcarName = pName;}

    QString pType() const {return m_GDcarType;}
    Q_INVOKABLE void setpType(QString pType){m_GDcarType = pType;}

    int pPriority() const {return m_priority;}
    Q_INVOKABLE void setpPriority(int pPriority){m_priority = pPriority;}

    float pSize() const {return m_size;}
    Q_INVOKABLE void setpSize(float pSize){m_size = pSize;}

    QString graphicsScale() const {return m_graphicsScale;}
    Q_INVOKABLE void setgraphicsScale(QString graphicsScale){m_graphicsScale = graphicsScale;}

    float carWidth() const {return m_carWidth;}
    Q_INVOKABLE void setcarWidth(float carWidth){m_carWidth = carWidth;}

    float carLength() const {return m_carLength;}
    Q_INVOKABLE void setcarLength(float carLength){m_carLength = carLength;}

    float moveSpeed() const {return m_moveSpeed;}
    Q_INVOKABLE void setmoveSpeed(float moveSpeed){m_moveSpeed = moveSpeed;}

    float rotationSpeed() const {return m_moveSpeed;}
    Q_INVOKABLE void setrotationSpeed(float rotationSpeed){m_rotationSpeed = rotationSpeed;}

    double minTurnR() const {return m_minTurnR;}
    Q_INVOKABLE void setminTurnR(double minTurnR){m_minTurnR = minTurnR;}

    QString graphicsPath() const {return m_graphicsPath;}
    Q_INVOKABLE void setgraphicsPath(QString graphicsPath){m_graphicsPath = graphicsPath;}

    QString canGDType() const {return m_canGDType;}
    Q_INVOKABLE void setcanGDType(QString canGDType){m_canGDType = canGDType;}

    int canGDNum() const {return m_canGDNum;}
    Q_INVOKABLE void setcanGDNum(int canGDNum){m_canGDNum = canGDNum;}

    float wheelbase() const {return m_wheelbase;}
    Q_INVOKABLE void setwheelbase(float wheelbase){m_wheelbase = wheelbase;}

    float thread() const {return m_thread;}
    Q_INVOKABLE void setthread(float thread){m_thread = thread;}

    float wheelR() const {return m_wheelR;}
    Q_INVOKABLE void setwheelR(float wheelR){m_wheelR = wheelR;}

    float wheelWidth() const {return m_wheelWidth;}
    Q_INVOKABLE void setwheelWidth(float wheelWidth){m_wheelWidth = wheelWidth;}

    float agentFront() const {return m_agentFront;}
    Q_INVOKABLE void setagentFront(float agentFront){m_agentFront = agentFront;}

    float agentBack() const {return m_agentBack;}
    Q_INVOKABLE void setagentBack(float agentBack){m_agentBack = agentBack;}

    DBGDCarTable(){}
    DBGDCarTable(const DBGDCarTable& obj)
    {
        m_GDcarID =         obj.m_GDcarID;
        m_GDcarName =       obj.m_GDcarName;
        m_GDcarType =       obj.m_GDcarType;
        m_priority =        obj.m_priority;
        m_size =            obj.m_size;
        m_graphicsScale =   obj.m_graphicsScale;
        m_carWidth =        obj.m_carWidth;
        m_carLength =       obj.m_carLength;
        m_moveSpeed =       obj.m_moveSpeed;
        m_rotationSpeed =   obj.m_rotationSpeed;
        m_minTurnR =        obj.m_minTurnR;
        m_graphicsPath =    obj.m_graphicsPath;
        m_wheelbase =       obj.m_wheelbase;
        m_thread =          obj.m_thread;
        m_wheelR =          obj.m_wheelR;
        m_wheelWidth =      obj.m_wheelWidth;
        m_agentFront =      obj.m_agentFront;
        m_agentBack =       obj.m_agentBack;
        agentInitState =    obj.agentInitState;
        agentSection =      obj.agentSection;
    }
    DBGDCarTable& operator=(const DBGDCarTable& obj)
    {
        if(this == &obj)
            return *this;
        m_GDcarID =         obj.m_GDcarID;
        m_GDcarName =       obj.m_GDcarName;
        m_GDcarType =       obj.m_GDcarType;
        m_priority =        obj.m_priority;
        m_size =            obj.m_size;
        m_graphicsScale =   obj.m_graphicsScale;
        m_carWidth =        obj.m_carWidth;
        m_carLength =       obj.m_carLength;
        m_moveSpeed =       obj.m_moveSpeed;
        m_rotationSpeed =   obj.m_rotationSpeed;
        m_minTurnR =        obj.m_minTurnR;
        m_graphicsPath =    obj.m_graphicsPath;
        m_wheelbase =       obj.m_wheelbase;
        m_thread =          obj.m_thread;
        m_wheelR =          obj.m_wheelR;
        m_wheelWidth =      obj.m_wheelWidth;
        m_agentFront =      obj.m_agentFront;
        m_agentBack =       obj.m_agentBack;
        agentInitState =    obj.agentInitState;
        agentSection =      obj.agentSection;

        return *this;
    }

    int m_GDcarID = 0;//主键 GD车ID
    QString m_GDcarName = "aaaa";//外键 GD车编号
    QString m_GDcarType = "0";  //外键 GD车类型
    int m_priority = 0;
    float m_size = 0;
    QString m_graphicsScale = "1:25";    //外键 图元比例尺
    float m_carWidth = 2;    //外键 车辆宽度
    float m_carLength = 3; //外键 车辆长度
    float m_moveSpeed = 0;//外键 移动速度
    float m_rotationSpeed = 0;//外键 旋转速度
    double m_minTurnR = 0;//外键 最小转弯半径
    QString m_graphicsPath = "aaaa";//外键 图元路径
    QString m_canGDType = "bbbb";//外键 可GD类型
    int m_canGDNum = 0;//外键 可GD数量
    float m_wheelbase = 1;
    float m_thread = 2;
    float m_wheelR = 3;
    float m_wheelWidth = 4;
    float m_agentFront = 5;
    float m_agentBack = 6;


    AgentInitState agentInitState;
    QVector<AgentSection> agentSection;

};
/*数据库任务类*/
class DBCreateProjectTable : public DBTableBaseTable
{
    Q_OBJECT

    Q_CLASSINFO("TableName","JZJproject_table")

    Q_PROPERTY(int pID READ pID WRITE setpID)
    Q_CLASSINFO("pID","int(11) AUTO_INCREMENT PRIMARY KEY")

    Q_PROPERTY(QString pLabel READ pLabel WRITE setpLabel)
    Q_CLASSINFO("pLabel","VARCHAR(20) NOT NULL")

    Q_PROPERTY(QString pName READ pName WRITE setpName)
    Q_CLASSINFO("pName","VARCHAR(20) NOT NULL")

    Q_PROPERTY(QString startTime READ startTime WRITE setstartTime)
    Q_CLASSINFO("startTime","TEXT NOT NULL")

    Q_PROPERTY(QString describeTask READ describeTask WRITE setdescribeTask)
    Q_CLASSINFO("describeTask","LONGTEXT NOT NULL")

    Q_PROPERTY(QString solvePlan READ solvePlan WRITE setsolvePlan)
    Q_CLASSINFO("solvePlan","LONGTEXT NOT NULL")

    //alter table emp add constraint fk_emp_dept_id foreign key (dept_id) references dept(id);

public:

    int pID() const {return m_projectID;}
    Q_INVOKABLE void setpID(int pID){m_projectID = pID;}

    QString pLabel() const {return m_projectLabel;}
    Q_INVOKABLE void setpLabel(QString pLabel){m_projectLabel = pLabel;}

    QString pName() const {return m_projectName;}
    Q_INVOKABLE void setpName(QString pName){m_projectName = pName;}

    QString startTime() const {return m_startTime;}
    Q_INVOKABLE void setstartTime(QString startTime){m_startTime = startTime;}

    QString describeTask() const {return m_describeTask;}
    Q_INVOKABLE void setdescribeTask(QString describeTask){m_describeTask = describeTask;}

    QString solvePlan() const {return m_solvePlan;}
    Q_INVOKABLE void setsolvePlan(QString solvePlan){m_solvePlan = solvePlan;}


    DBCreateProjectTable(){}
    DBCreateProjectTable(const DBCreateProjectTable& obj)
    {
        m_projectID     = obj.m_projectID   ;
        m_projectLabel  = obj.m_projectLabel;
        m_projectName   = obj.m_projectName ;
        m_startTime     = obj.m_startTime   ;
        m_describeTask  = obj.m_describeTask;
        m_solvePlan     = obj.m_solvePlan   ;
    }
    DBCreateProjectTable& operator=(const DBCreateProjectTable& obj)
    {
        if(this == &obj)
            return *this;
        m_projectID     = obj.m_projectID   ;
        m_projectLabel  = obj.m_projectLabel;
        m_projectName   = obj.m_projectName ;
        m_startTime     = obj.m_startTime   ;
        m_describeTask  = obj.m_describeTask;
        m_solvePlan     = obj.m_solvePlan   ;

        return *this;
    }

    int m_projectID = 0;//主键 任务ID
    QString m_projectLabel = "aaaa";// 任务编号
    QString m_projectName = "Test";  // 任务名

    QString m_startTime = "12:00";    // 任务开始时间
    QString m_describeTask = "abcde";    // 任务描述
    QString m_solvePlan = "3"; // 解决方案

};


/*
数据库FJ类
*/
class DBPlaneTable : public DBTableBaseTable
{
    Q_OBJECT

    ////以下属性名需要唯一 对应数据库表字段  其次classinfo的key需要和属性名保持一直 方便做映射  主键需要在第一个
    /// Q_PROPERTY 里的write方法也不能随意写 按下述格式写   否则会影响查询后的数据返回
    Q_CLASSINFO("TableName","plane_table")

    Q_PROPERTY(int pID READ pID WRITE setpID)
    Q_CLASSINFO("pID","int(11) AUTO_INCREMENT PRIMARY KEY")

    Q_PROPERTY(QString pName READ pName WRITE setpName)
    Q_CLASSINFO("pName","VARCHAR(20) NOT NULL")

    Q_PROPERTY(int pPriority READ pPriority WRITE setpPriority)
    Q_CLASSINFO("pPriority","int(11)")

    Q_PROPERTY(float pSize READ pSize WRITE setpSize)
    Q_CLASSINFO("pSize","float")

    Q_PROPERTY(QString pType READ pType WRITE setpType)
    Q_CLASSINFO("pType","VARCHAR(20) NOT NULL")

    Q_PROPERTY(int pBoardState READ pBoardState WRITE setpBoardState)
    Q_CLASSINFO("pBoardState","int(11)")

    Q_PROPERTY(QString graphicsScale READ graphicsScale WRITE setgraphicsScale)
    Q_CLASSINFO("graphicsScale","VARCHAR(20) NOT NULL")

    Q_PROPERTY(float planeLength READ planeLength WRITE setplaneLength)
    Q_CLASSINFO("planeLength","float")

    Q_PROPERTY(double spreadWidth READ spreadWidth WRITE setspreadWidth)
    Q_CLASSINFO("spreadWidth","double")

    Q_PROPERTY(QString spreadGraphicsPath READ spreadGraphicsPath WRITE setspreadGraphicsPath)
    Q_CLASSINFO("spreadGraphicsPath","TEXT")

    Q_PROPERTY(double foldWidth READ foldWidth WRITE setfoldWidth)
    Q_CLASSINFO("foldWidth","double")

    Q_PROPERTY(QString foldGraphicsPath READ foldGraphicsPath WRITE setfoldGraphicsPath)
    Q_CLASSINFO("foldGraphicsPath","TEXT")

    Q_PROPERTY(double tankCapacity READ tankCapacity WRITE settankCapacity)
    Q_CLASSINFO("tankCapacity","double")

    Q_PROPERTY(double refuelRate READ refuelRate WRITE setrefuelRate)
    Q_CLASSINFO("refuelRate","double")

    Q_PROPERTY(double fuelCostRate READ fuelCostRate WRITE setfuelCostRate)
    Q_CLASSINFO("fuelCostRate","double")

    Q_PROPERTY(float minWarmUpTime READ minWarmUpTime WRITE setminWarmUpTime)
    Q_CLASSINFO("minWarmUpTime","float")

    Q_PROPERTY(QString canGDType READ canGDType WRITE setcanGDType)
    Q_CLASSINFO("canGDType","VARCHAR(255) NOT NULL")

    Q_PROPERTY(QString GDLocal READ GDLocal WRITE setGDLocal)
    Q_CLASSINFO("GDLocal","VARCHAR(255) NOT NULL")

    Q_PROPERTY(QString GDTime READ GDTime WRITE setGDTime)
    Q_CLASSINFO("GDTime","VARCHAR(255) NOT NULL")

    Q_PROPERTY(float alignTime READ alignTime WRITE setalignTime)
    Q_CLASSINFO("alignTime","float")

    Q_PROPERTY(float inflateTime READ inflateTime WRITE setinflateTime)
    Q_CLASSINFO("inflateTime","float")

    Q_PROPERTY(float chargeTime READ chargeTime WRITE setchargeTime)
    Q_CLASSINFO("chargeTime","float")

    Q_PROPERTY(double minTurnR READ minTurnR WRITE setminTurnR)
    Q_CLASSINFO("minTurnR","double")

    Q_PROPERTY(float moveSpeed READ moveSpeed WRITE setmoveSpeed)
    Q_CLASSINFO("moveSpeed","float")

    Q_PROPERTY(float rotationSpeed READ rotationSpeed WRITE setrotationSpeed)
    Q_CLASSINFO("rotationSpeed","float")

    Q_PROPERTY(int GDNum READ GDNum WRITE setGDNum)
    Q_CLASSINFO("GDNum","int(11)")

    Q_PROPERTY(float wheelbase READ wheelbase WRITE setwheelbase)
    Q_CLASSINFO("wheelbase","float")

    Q_PROPERTY(float thread READ thread WRITE setthread)
    Q_CLASSINFO("thread","float")

    Q_PROPERTY(float wheelR READ wheelR WRITE setwheelR)
    Q_CLASSINFO("wheelR","float")

    Q_PROPERTY(float wheelWidth READ wheelWidth WRITE setwheelWidth)
    Q_CLASSINFO("wheelWidth","float")

    Q_PROPERTY(float agentFront READ agentFront WRITE setagentFront)
    Q_CLASSINFO("agentFront","float")

    Q_PROPERTY(float agentBack READ agentBack WRITE setagentBack)
    Q_CLASSINFO("agentBack","float")
    //alter table emp add constraint fk_emp_dept_id foreign key (dept_id) references dept(id);

public:

    int pID() const {return m_planeID;}
    Q_INVOKABLE void setpID(int pID){m_planeID = pID;}

    QString pName() const {return m_planeName;}
    Q_INVOKABLE void setpName(QString planeName){m_planeName = planeName;}

    int pPriority() const {return m_priority;}
    Q_INVOKABLE void setpPriority(int pPriority){m_priority = pPriority;}

    float pSize() const {return m_size;}
    Q_INVOKABLE void setpSize(float pSize){m_size = pSize;}

    int pBoardState() const {return m_boardState;}
    Q_INVOKABLE void setpBoardState(int pBoardState){m_boardState = pBoardState;}

    QString pType() const {return m_planeType;}
    Q_INVOKABLE void setpType(QString pType){m_planeType = pType;}

    QString graphicsScale() const {return m_graphicsScale;}
    Q_INVOKABLE void setgraphicsScale(QString graphicsScale){m_graphicsScale = graphicsScale;}

    float planeLength() const {return m_planeLength;}
    Q_INVOKABLE void setplaneLength(float planeLength){m_planeLength = planeLength;}

    double spreadWidth() const {return m_spreadWidth;}
    Q_INVOKABLE void setspreadWidth(double spreadWidth){m_spreadWidth = spreadWidth;}

    QString spreadGraphicsPath() const {return m_spreadGraphicsPath;}
    Q_INVOKABLE void setspreadGraphicsPath(QString spreadGraphicsPath){m_spreadGraphicsPath = spreadGraphicsPath;}

    double foldWidth() const {return m_foldWidth;}
    Q_INVOKABLE void setfoldWidth(double foldWidth){m_foldWidth = foldWidth;}

    QString foldGraphicsPath() const {return m_foldGraphicsPath;}
    Q_INVOKABLE void setfoldGraphicsPath(QString foldGraphicsPath){m_foldGraphicsPath = foldGraphicsPath;}

    double tankCapacity() const {return m_tankCapacity;}
    Q_INVOKABLE void settankCapacity(double tankCapacity){m_tankCapacity = tankCapacity;}

    double refuelRate() const {return m_refuelRate;}
    Q_INVOKABLE void setrefuelRate(double refuelRate){m_refuelRate = refuelRate;}

    double fuelCostRate() const {return m_fuelCostRate;}
    Q_INVOKABLE void setfuelCostRate(double fuelCostRate){m_fuelCostRate = fuelCostRate;}

    float minWarmUpTime() const {return m_minWarmUpTime;}
    Q_INVOKABLE void setminWarmUpTime(float minWarmUpTime){m_minWarmUpTime = minWarmUpTime;}

    QString canGDType() const {return m_canGDType;}
    Q_INVOKABLE void setcanGDType(QString canGDType){m_canGDType = canGDType;}

    QString GDLocal() const {return m_GDLocal;}
    Q_INVOKABLE void setGDLocal(QString GDLocal){m_GDLocal = GDLocal;}

    float alignTime() const {return m_alignTime;}
    Q_INVOKABLE void setalignTime(float alignTime){m_alignTime = alignTime;}

    QString GDTime() const {return m_GDTime;}
    Q_INVOKABLE void setGDTime(QString GDTime){m_GDTime = GDTime;}

    float inflateTime() const {return m_inflateTime;}
    Q_INVOKABLE void setinflateTime(float inflateTime){m_inflateTime = inflateTime;}

    float chargeTime() const {return m_chargeTime;}
    Q_INVOKABLE void setchargeTime(float chargeTime){m_chargeTime = chargeTime;}

    double minTurnR() const {return m_minTurnR;}
    Q_INVOKABLE void setminTurnR(double minTurnR){m_minTurnR = minTurnR;}

    float moveSpeed() const {return m_moveSpeed;}
    Q_INVOKABLE void setmoveSpeed(float moveSpeed){m_moveSpeed = moveSpeed;}

    float rotationSpeed() const {return m_rotationSpeed;}
    Q_INVOKABLE void setrotationSpeed(float rotationSpeed){m_rotationSpeed = rotationSpeed;}

    int GDNum() const {return m_GDNum;}
    Q_INVOKABLE void setGDNum(int GDNum){m_GDNum = GDNum;}

    float wheelbase() const {return m_wheelbase;}
    Q_INVOKABLE void setwheelbase(float wheelbase){m_wheelbase = wheelbase;}

    float thread() const {return m_thread;}
    Q_INVOKABLE void setthread(float thread){m_thread = thread;}

    float wheelR() const {return m_wheelR;}
    Q_INVOKABLE void setwheelR(float wheelR){m_wheelR = wheelR;}

    float wheelWidth() const {return m_wheelWidth;}
    Q_INVOKABLE void setwheelWidth(float wheelWidth){m_wheelWidth = wheelWidth;}

    float agentFront() const {return m_agentFront;}
    Q_INVOKABLE void setagentFront(float agentFront){m_agentFront = agentFront;}

    float agentBack() const {return m_agentBack;}
    Q_INVOKABLE void setagentBack(float agentBack){m_agentBack = agentBack;}


    DBPlaneTable(){}
    DBPlaneTable(const DBPlaneTable& obj)
    {
        m_planeID =             obj.m_planeID;
        m_planeName =           obj.m_planeName;
        m_priority =            obj.m_priority;
        m_planeType =           obj.m_planeType;
        m_size =                obj.m_size;
        m_boardState    =       obj.m_boardState   ;
        m_graphicsScale =       obj.m_graphicsScale;
        m_planeLength =         obj.m_planeLength;
        m_spreadWidth =         obj.m_spreadWidth;
        m_spreadGraphicsPath =  obj.m_spreadGraphicsPath;
        m_foldWidth =           obj.m_foldWidth;
        m_foldGraphicsPath =    obj.m_foldGraphicsPath;
        m_tankCapacity =        obj.m_tankCapacity;
        m_refuelRate   =        obj.m_refuelRate  ;
        m_fuelCostRate  =       obj.m_fuelCostRate ;
        m_minWarmUpTime =       obj.m_minWarmUpTime;
        m_canGDType     =       obj.m_canGDType;
        m_GDLocal       =       obj.m_GDLocal  ;
        m_alignTime     =       obj.m_alignTime;
        m_GDTime        =       obj.m_GDTime   ;
        m_inflateTime =         obj.m_inflateTime;
        m_chargeTime  =         obj.m_chargeTime ;
        m_minTurnR =            obj.m_minTurnR;
        m_moveSpeed =           obj.m_moveSpeed;
        m_rotationSpeed =       obj.m_rotationSpeed;
        m_GDNum     =           obj.m_GDNum    ;
        m_wheelbase =           obj.m_wheelbase;
        m_thread    =           obj.m_thread   ;
        m_wheelR    =           obj.m_wheelR   ;
        m_wheelWidth =          obj.m_wheelWidth;
        m_agentFront =          obj.m_agentFront;
        m_agentBack  =          obj.m_agentBack ;
        agentInitState =        obj.agentInitState;
        agentSection  =         obj.agentSection;
    }
    DBPlaneTable& operator=(const DBPlaneTable& obj)
    {
        if(this == &obj)
            return *this;
        m_planeID =             obj.m_planeID;
        m_planeName =           obj.m_planeName;
        m_priority =            obj.m_priority;
        m_planeType =           obj.m_planeType;
        m_size =                obj.m_size;
        m_boardState    =       obj.m_boardState   ;
        m_graphicsScale =       obj.m_graphicsScale;
        m_planeLength =         obj.m_planeLength;
        m_spreadWidth =         obj.m_spreadWidth;
        m_spreadGraphicsPath =  obj.m_spreadGraphicsPath;
        m_foldWidth =           obj.m_foldWidth;
        m_foldGraphicsPath =    obj.m_foldGraphicsPath;
        m_tankCapacity =        obj.m_tankCapacity;
        m_refuelRate   =        obj.m_refuelRate  ;
        m_fuelCostRate  =       obj.m_fuelCostRate ;
        m_minWarmUpTime =       obj.m_minWarmUpTime;
        m_canGDType     =       obj.m_canGDType;
        m_GDLocal       =       obj.m_GDLocal  ;
        m_alignTime     =       obj.m_alignTime;
        m_GDTime        =       obj.m_GDTime   ;
        m_inflateTime =         obj.m_inflateTime;
        m_chargeTime  =         obj.m_chargeTime ;
        m_minTurnR =            obj.m_minTurnR;
        m_moveSpeed =           obj.m_moveSpeed;
        m_rotationSpeed =       obj.m_rotationSpeed;
        m_GDNum     =           obj.m_GDNum    ;
        m_wheelbase =           obj.m_wheelbase;
        m_thread    =           obj.m_thread   ;
        m_wheelR    =           obj.m_wheelR   ;
        m_wheelWidth =          obj.m_wheelWidth;
        m_agentFront =          obj.m_agentFront;
        m_agentBack  =          obj.m_agentBack ;
        agentInitState =        obj.agentInitState;
        agentSection  =         obj.agentSection;
        return *this;
    }

    int m_planeID = 0;//主键 飞机ID
    QString m_planeName = "03001";//外键 飞机编号 以下均是外键
    int m_priority = 0;//飞机优先级
    QString m_planeType = "0";  //飞机类型
    float m_size = 0;
    int m_boardState = 0;//是否在甲板上
    QString m_graphicsScale = "1:25";    //图元比例尺
    float m_planeLength = 2;    //飞机长度
    double m_spreadWidth = 3; //展翼宽度
    QString m_spreadGraphicsPath = "4";//展翼图元路径
    double m_foldWidth = 5;//折翼宽度
    QString m_foldGraphicsPath = "6";//折翼图元路径
    double m_tankCapacity = 7;//油箱容量
    double m_refuelRate = 8;//加油速率
    double m_fuelCostRate = 0.1;//耗油速率
    float m_minWarmUpTime = 9;//最小暖机时间
    QString m_canGDType = "aaaaa";//可GD类型
    QString m_GDLocal = "1;2;3;4;5";
    float m_alignTime = 10;//对准时间
    QString m_GDTime = "11";//GD时间
    float m_inflateTime = 12;//充气时间
    float m_chargeTime = 13;//充电时间
    double m_minTurnR = 14;//最小转弯半径
    float m_moveSpeed = 15;//移动速度
    float m_rotationSpeed = 16;//旋转速度
    int m_GDNum = 17;//GD数量
    float m_wheelbase = 1;
    float m_thread = 2;
    float m_wheelR = 3;
    float m_wheelWidth = 4;
    float m_agentFront = 5;
    float m_agentBack = 6;

    AgentInitState agentInitState;
    QVector<AgentSection> agentSection;
};

class DBPlaneTypeTable : public DBTableBaseTable{
    Q_OBJECT

    Q_CLASSINFO("TableName","PlaneType_table")

    Q_PROPERTY(int pID READ pID WRITE setpID)
    Q_CLASSINFO("pID","int(11) AUTO_INCREMENT PRIMARY KEY")

    Q_PROPERTY(QString pType READ pType WRITE setpType)
    Q_CLASSINFO("pType","VARCHAR(20) NOT NULL")

    Q_PROPERTY(float pSize READ pSize WRITE setpSize)
    Q_CLASSINFO("pSize","float")

    Q_PROPERTY(QString graphicsScale READ graphicsScale WRITE setgraphicsScale)
    Q_CLASSINFO("graphicsScale","VARCHAR(20) NOT NULL")

    Q_PROPERTY(float planeLength READ planeLength WRITE setplaneLength)
    Q_CLASSINFO("planeLength","float")

    Q_PROPERTY(double spreadWidth READ spreadWidth WRITE setspreadWidth)
    Q_CLASSINFO("spreadWidth","double")

    Q_PROPERTY(QString spreadGraphicsPath READ spreadGraphicsPath WRITE setspreadGraphicsPath)
    Q_CLASSINFO("spreadGraphicsPath","TEXT")//图元路径过长，修改成TEXT

    Q_PROPERTY(double foldWidth READ foldWidth WRITE setfoldWidth)
    Q_CLASSINFO("foldWidth","double")

    Q_PROPERTY(QString foldGraphicsPath READ foldGraphicsPath WRITE setfoldGraphicsPath)
    Q_CLASSINFO("foldGraphicsPath","TEXT")

    Q_PROPERTY(QString canGDType READ canGDType WRITE setcanGDType)
    Q_CLASSINFO("canGDType","VARCHAR(20) NOT NULL")

    Q_PROPERTY(QString GDLocal READ GDLocal WRITE setGDLocal)
    Q_CLASSINFO("GDLocal","VARCHAR(20) NOT NULL")

    Q_PROPERTY(QString GDTime READ GDTime WRITE setGDTime)
    Q_CLASSINFO("GDTime","VARCHAR(20) NOT NULL")

    Q_PROPERTY(int GDNum READ GDNum WRITE setGDNum)
    Q_CLASSINFO("GDNum","int(11)")

    Q_PROPERTY(float wheelbase READ wheelbase WRITE setwheelbase)
    Q_CLASSINFO("wheelbase","float")

    Q_PROPERTY(float thread READ thread WRITE setthread)
    Q_CLASSINFO("thread","float")

    Q_PROPERTY(float wheelR READ wheelR WRITE setwheelR)
    Q_CLASSINFO("wheelR","float")

    Q_PROPERTY(float wheelWidth READ wheelWidth WRITE setwheelWidth)
    Q_CLASSINFO("wheelWidth","float")

    Q_PROPERTY(float agentFront READ agentFront WRITE setagentFront)
    Q_CLASSINFO("agentFront","float")

    Q_PROPERTY(float agentBack READ agentBack WRITE setagentBack)
    Q_CLASSINFO("agentBack","float")

    Q_PROPERTY(int flag READ flag WRITE setflag)
    Q_CLASSINFO("flag","int(11) NOT NULL")

public:

    int pID() const {return m_PlaneTypeID;}
    Q_INVOKABLE void setpID(int pID){m_PlaneTypeID = pID;}

    QString pType() const {return m_planeType;}
    Q_INVOKABLE void setpType(QString pType){m_planeType = pType;}

    float pSize() const {return m_size;}
    Q_INVOKABLE void setpSize(float pSize){m_size = pSize;}

    QString graphicsScale() const {return m_graphicsScale;}
    Q_INVOKABLE void setgraphicsScale(QString graphicsScale){m_graphicsScale = graphicsScale;}

    float planeLength() const {return m_planeLength;}
    Q_INVOKABLE void setplaneLength(float planeLength){m_planeLength = planeLength;}

    double spreadWidth() const {return m_spreadWidth;}
    Q_INVOKABLE void setspreadWidth(double spreadWidth){m_spreadWidth = spreadWidth;}

    QString spreadGraphicsPath() const {return m_spreadGraphicsPath;}
    Q_INVOKABLE void setspreadGraphicsPath(QString spreadGraphicsPath){m_spreadGraphicsPath = spreadGraphicsPath;}

    double foldWidth() const {return m_foldWidth;}
    Q_INVOKABLE void setfoldWidth(double foldWidth){m_foldWidth = foldWidth;}

    QString foldGraphicsPath() const {return m_foldGraphicsPath;}
    Q_INVOKABLE void setfoldGraphicsPath(QString foldGraphicsPath){m_foldGraphicsPath = foldGraphicsPath;}

    QString canGDType() const {return m_canGDType;}
    Q_INVOKABLE void setcanGDType(QString canGDType){m_canGDType = canGDType;}

    QString GDLocal() const {return m_GDLocal;}
    Q_INVOKABLE void setGDLocal(QString GDLocal){m_GDLocal = GDLocal;}

    QString GDTime() const {return m_GDTime;}
    Q_INVOKABLE void setGDTime(QString GDTime){m_GDTime = GDTime;}

    int GDNum() const {return m_GDNum;}
    Q_INVOKABLE void setGDNum(int GDNum){m_GDNum = GDNum;}

    float wheelbase() const {return m_wheelbase;}
    Q_INVOKABLE void setwheelbase(float wheelbase){m_wheelbase = wheelbase;}

    float thread() const {return m_thread;}
    Q_INVOKABLE void setthread(float thread){m_thread = thread;}

    float wheelR() const {return m_wheelR;}
    Q_INVOKABLE void setwheelR(float wheelR){m_wheelR = wheelR;}

    float wheelWidth() const {return m_wheelWidth;}
    Q_INVOKABLE void setwheelWidth(float wheelWidth){m_wheelWidth = wheelWidth;}

    float agentFront() const {return m_agentFront;}
    Q_INVOKABLE void setagentFront(float agentFront){m_agentFront = agentFront;}

    float agentBack() const {return m_agentBack;}
    Q_INVOKABLE void setagentBack(float agentBack){m_agentBack = agentBack;}

    int flag() const {return m_flag;}
    Q_INVOKABLE void setflag(int flag){m_flag = flag;}

    int m_PlaneTypeID = 0;
    QString m_planeType = "0";
    float m_size = 0;
    QString m_graphicsScale = "1:25";
    float m_planeLength = 2;    //飞机长度
    double m_spreadWidth = 3; //展翼宽度
    QString m_spreadGraphicsPath = "4";//展翼图元路径
    double m_foldWidth = 5;//折翼宽度
    QString m_foldGraphicsPath = "6";//折翼图元路径
    QString m_canGDType = "aaaaa";//可GD类型
    QString m_GDLocal = "1;2;3;4;5";
    QString m_GDTime = "11";//GD时间
    int m_GDNum = 17;//GD数量
    float m_wheelbase = 1;
    float m_thread = 2;
    float m_wheelR = 3;
    float m_wheelWidth = 4;
    float m_agentFront = 5;
    float m_agentBack = 6;
    int m_flag = 0;

};

//数据库GD车类型表类
class DBGDCarTypeTable : public DBTableBaseTable{
    Q_OBJECT

    Q_CLASSINFO("TableName","GDCarType_table")

    Q_PROPERTY(int pID READ pID WRITE setpID)
    Q_CLASSINFO("pID","int(11) AUTO_INCREMENT PRIMARY KEY")

    Q_PROPERTY(QString pType READ pType WRITE setpType)
    Q_CLASSINFO("pType","VARCHAR(20) NOT NULL")

    Q_PROPERTY(float pSize READ pSize WRITE setpSize)
    Q_CLASSINFO("pSize","float")

    Q_PROPERTY(QString graphicsScale READ graphicsScale WRITE setgraphicsScale)
    Q_CLASSINFO("graphicsScale","VARCHAR(20) NOT NULL")

    Q_PROPERTY(float carWidth READ carWidth WRITE setcarWidth)
    Q_CLASSINFO("carWidth","float")

    Q_PROPERTY(float carLength READ carLength WRITE setcarLength)
    Q_CLASSINFO("carLength","float")

    Q_PROPERTY(QString graphicsPath READ graphicsPath WRITE setgraphicsPath)
    Q_CLASSINFO("graphicsPath","TEXT")

    Q_PROPERTY(QString canGDType READ canGDType WRITE setcanGDType)
    Q_CLASSINFO("canGDType","VARCHAR(20) NOT NULL")

    Q_PROPERTY(int canGDNum READ canGDNum WRITE setcanGDNum)
    Q_CLASSINFO("canGDNum","int(11)")

    Q_PROPERTY(float wheelbase READ wheelbase WRITE setwheelbase)
    Q_CLASSINFO("wheelbase","float")

    Q_PROPERTY(float thread READ thread WRITE setthread)
    Q_CLASSINFO("thread","float")

    Q_PROPERTY(float wheelR READ wheelR WRITE setwheelR)
    Q_CLASSINFO("wheelR","float")

    Q_PROPERTY(float wheelWidth READ wheelWidth WRITE setwheelWidth)
    Q_CLASSINFO("wheelWidth","float")

    Q_PROPERTY(float agentFront READ agentFront WRITE setagentFront)
    Q_CLASSINFO("agentFront","float")

    Q_PROPERTY(float agentBack READ agentBack WRITE setagentBack)
    Q_CLASSINFO("agentBack","float")

    Q_PROPERTY(int flag READ flag WRITE setflag)
    Q_CLASSINFO("flag","int(11) NOT NULL")

public:
    int pID() const {return m_GDCarTypeID;}
    Q_INVOKABLE void setpID(int pID){m_GDCarTypeID = pID;}

    QString pType() const {return m_GDcarType;}
    Q_INVOKABLE void setpType(QString pType){m_GDcarType = pType;}

    float pSize() const {return m_size;}
    Q_INVOKABLE void setpSize(float pSize){m_size = pSize;}

    QString graphicsScale() const {return m_graphicsScale;}
    Q_INVOKABLE void setgraphicsScale(QString graphicsScale){m_graphicsScale = graphicsScale;}

    float carWidth() const {return m_carWidth;}
    Q_INVOKABLE void setcarWidth(float carWidth){m_carWidth = carWidth;}

    float carLength() const {return m_carLength;}
    Q_INVOKABLE void setcarLength(float carLength){m_carLength = carLength;}

    QString graphicsPath() const {return m_graphicsPath;}
    Q_INVOKABLE void setgraphicsPath(QString graphicsPath){m_graphicsPath = graphicsPath;}

    QString canGDType() const {return m_canGDType;}
    Q_INVOKABLE void setcanGDType(QString canGDType){m_canGDType = canGDType;}

    int canGDNum() const {return m_canGDNum;}
    Q_INVOKABLE void setcanGDNum(int canGDNum){m_canGDNum = canGDNum;}

    float wheelbase() const {return m_wheelbase;}
    Q_INVOKABLE void setwheelbase(float wheelbase){m_wheelbase = wheelbase;}

    float thread() const {return m_thread;}
    Q_INVOKABLE void setthread(float thread){m_thread = thread;}

    float wheelR() const {return m_wheelR;}
    Q_INVOKABLE void setwheelR(float wheelR){m_wheelR = wheelR;}

    float wheelWidth() const {return m_wheelWidth;}
    Q_INVOKABLE void setwheelWidth(float wheelWidth){m_wheelWidth = wheelWidth;}

    float agentFront() const {return m_agentFront;}
    Q_INVOKABLE void setagentFront(float agentFront){m_agentFront = agentFront;}

    float agentBack() const {return m_agentBack;}
    Q_INVOKABLE void setagentBack(float agentBack){m_agentBack = agentBack;}

    int flag() const {return m_flag;}
    Q_INVOKABLE void setflag(int flag){m_flag = flag;}

    int m_GDCarTypeID = 0;
    QString m_GDcarType = "0";
    float m_size = 0;
    QString m_graphicsScale = "1:25";
    float m_carWidth = 2;    //外键 车辆宽度
    float m_carLength = 3; //外键 车辆长度
    QString m_graphicsPath = "aaaa";//外键 图元路径
    QString m_canGDType = "bbbb";//外键 可GD类型
    int m_canGDNum = 0;//外键 可GD数量
    float m_wheelbase = 1;
    float m_thread = 2;
    float m_wheelR = 3;
    float m_wheelWidth = 4;
    float m_agentFront = 5;
    float m_agentBack = 6;
    int m_flag = 0;
};

//数据库QY车类型表类
class DBQYCarTypeTable : public DBTableBaseTable{

    Q_OBJECT

    Q_CLASSINFO("TableName","QYCarType_table")

    Q_PROPERTY(int pID READ pID WRITE setpID)
    Q_CLASSINFO("pID","int(11) AUTO_INCREMENT PRIMARY KEY")

    Q_PROPERTY(QString pType READ pType WRITE setpType)
    Q_CLASSINFO("pType","VARCHAR(20) NOT NULL")

    Q_PROPERTY(float pSize READ pSize WRITE setpSize)
    Q_CLASSINFO("pSize","float")

    Q_PROPERTY(QString graphicsScale READ graphicsScale WRITE setgraphicsScale)
    Q_CLASSINFO("graphicsScale","VARCHAR(20) NOT NULL")

    Q_PROPERTY(float carWidth READ carWidth WRITE setcarWidth)
    Q_CLASSINFO("carWidth","float")

    Q_PROPERTY(float carLength READ carLength WRITE setcarLength)
    Q_CLASSINFO("carLength","float")

    Q_PROPERTY(QString graphicsPath READ graphicsPath WRITE setgraphicsPath)
    Q_CLASSINFO("graphicsPath","TEXT")

    Q_PROPERTY(float wheelbase READ wheelbase WRITE setwheelbase)
    Q_CLASSINFO("wheelbase","float")

    Q_PROPERTY(float thread READ thread WRITE setthread)
    Q_CLASSINFO("thread","float")

    Q_PROPERTY(float wheelR READ wheelR WRITE setwheelR)
    Q_CLASSINFO("wheelR","float")

    Q_PROPERTY(float wheelWidth READ wheelWidth WRITE setwheelWidth)
    Q_CLASSINFO("wheelWidth","float")

    Q_PROPERTY(float agentFront READ agentFront WRITE setagentFront)
    Q_CLASSINFO("agentFront","float")

    Q_PROPERTY(float agentBack READ agentBack WRITE setagentBack)
    Q_CLASSINFO("agentBack","float")

  //!!!新加，根据上一个复制
    Q_PROPERTY(int flag READ flag WRITE setflag)
    Q_CLASSINFO("flag","int(11) NOT NULL")
public:

    int pID() const {return m_QYCarTypeID;}
    Q_INVOKABLE void setpID(int pID){m_QYCarTypeID = pID;}

    QString pType() const {return m_QYcarType;}
    Q_INVOKABLE void setpType(QString pType){m_QYcarType = pType;}

    float pSize() const {return m_size;}
    Q_INVOKABLE void setpSize(float pSize){m_size = pSize;}

    QString graphicsScale() const {return m_graphicsScale;}
    Q_INVOKABLE void setgraphicsScale(QString graphicsScale){m_graphicsScale = graphicsScale;}

    float carWidth() const {return m_carWidth;}
    Q_INVOKABLE void setcarWidth(float carWidth){m_carWidth = carWidth;}

    float carLength() const {return m_carLength;}
    Q_INVOKABLE void setcarLength(float carLength){m_carLength = carLength;}

    QString graphicsPath() const {return m_graphicsPath;}
    Q_INVOKABLE void setgraphicsPath(QString graphicsPath){m_graphicsPath = graphicsPath;}

    float wheelbase() const {return m_wheelbase;}
    Q_INVOKABLE void setwheelbase(float wheelbase){m_wheelbase = wheelbase;}

    float thread() const {return m_thread;}
    Q_INVOKABLE void setthread(float thread){m_thread = thread;}

    float wheelR() const {return m_wheelR;}
    Q_INVOKABLE void setwheelR(float wheelR){m_wheelR = wheelR;}

    float wheelWidth() const {return m_wheelWidth;}
    Q_INVOKABLE void setwheelWidth(float wheelWidth){m_wheelWidth = wheelWidth;}

    float agentFront() const {return m_agentFront;}
    Q_INVOKABLE void setagentFront(float agentFront){m_agentFront = agentFront;}

    float agentBack() const {return m_agentBack;}
    Q_INVOKABLE void setagentBack(float agentBack){m_agentBack = agentBack;}

    int flag() const {return m_flag;}
    Q_INVOKABLE void setflag(int flag){m_flag = flag;}//新加


    int m_QYCarTypeID = 0;
    QString m_QYcarType = "0";
    float m_size = 0;
    QString m_graphicsScale = "1:25";    //外键 图元比例尺
    float m_carWidth = 2;    //外键 车辆宽度
    float m_carLength = 3; //外键 人员长度
    QString m_graphicsPath = "aaaa";//外键 图元路径
    float m_wheelbase = 1;
    float m_thread = 2;
    float m_wheelR = 3;
    float m_wheelWidth = 4;
    float m_agentFront = 5;
    float m_agentBack = 6;
    int m_flag = 0;//新加
};

//数据库人员类型表类
class DBPersonTypeTable: public DBTableBaseTable
{
    Q_OBJECT

    Q_CLASSINFO("TableName","PersonType_table")

    Q_PROPERTY(int pID READ pID WRITE setpID)
    Q_CLASSINFO("pID","int(11) AUTO_INCREMENT PRIMARY KEY")

    Q_PROPERTY(QString pType READ pType WRITE setpType)
    Q_CLASSINFO("pType","VARCHAR(20) NOT NULL")

    Q_PROPERTY(double graphicsR READ graphicsR WRITE setgraphicsR)
    Q_CLASSINFO("graphicsR","double")

    Q_PROPERTY(QString graphicScale READ graphicScale WRITE setgraphicScale)
    Q_CLASSINFO("graphicScale","VARCHAR(20)")

    Q_PROPERTY(int flag READ flag WRITE setflag)
    Q_CLASSINFO("flag","int(11) NOT NULL")

public:

    int pID() const {return m_PersonTypeID;}
    Q_INVOKABLE void setpID(int pID){m_PersonTypeID = pID;}

    QString pType() const {return m_personType;}
    Q_INVOKABLE void setpType(QString pType){m_personType = pType;}

    double graphicsR() const {return m_graphicsR;}
    Q_INVOKABLE void setgraphicsR(double graphicsR){m_graphicsR = graphicsR;}

    QString graphicScale() const{return m_graphicScale;}
    Q_INVOKABLE void setgraphicScale(QString graphicScale){m_graphicScale = graphicScale;}

    int flag() const {return m_flag;}
    Q_INVOKABLE void setflag(int flag){m_flag = flag;}

    int m_PersonTypeID = 0;
    QString m_personType = "0";
    double m_graphicsR = 2;
    QString m_graphicScale = "1:25";
    int m_flag = 0;
};

#endif // DBTABLECLASS_H
