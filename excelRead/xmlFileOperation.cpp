#include "xmlFileOperation.h"
#include <QGraphicsLineItem>
#include <QFileDialog>
#include <QDebug>
#include <QApplication>
#include <QMessageBox>
#include <QXmlStreamWriter>
#include <QDir>

class xmlFileOperationPrivate
{
public:
    void setCurrentFile(const QString &filepath){
        m_file = filepath;
    }

    void parseMapBaseInfo(const QDomElement& element,MapInfo& mapInfo);
    void parseAgentsLogPathInfo(const QDomElement& element, LogXmlInfo& agentLogInfo);
    void parseMapOutPolygnInfo(const QDomElement& element,MapOutPolygn& mapInfo,int xp,int yp);
    void parseMapNormalLineInfo(const QDomElement& element,MapBaseInfo& mapInfo,int xp,int yp);
    void parseMapInPolygnInfo(const QDomElement& element,MapBaseInfo& mapInfos,int xp,int yp);
    void parseMapCircleInfo(const QDomElement& element,MapBaseInfo& mapInfo,int xp,int yp);
    void parseMapShipislandInfo(const QDomElement& element,MapBaseInfo& mapInfo,int xp,int yp);
    void parseMapDeflectorsInfo(const QDomElement& element,MapBaseInfo& mapInfo,int xp,int yp);
    void parseMapElevatorInfo(const QDomElement& element,MapBaseInfo& mapInfo,int xp,int yp);
    void parseMapStationInfo(const QDomElement& element,QVector<MapPosData>& mapPosInfo);
    void parseMapTakeoffPos(const QDomElement& element,QVector<stationLocal>& takeoffInfo);
    void parseMapTakelandPos(const QDomElement& element,QVector<stationLocal>& takelandInfo);

    void parseAgentSectionInfo(const QDomElement& element,const QString& agentType,LogXmlInfo& planeInfo);

    void parseAgentPlaneInfo(const QDomElement& element,QVector<DBPlaneTable>& planeInfo);
    void parseAgentQYCarInfo(const QDomElement& element,QVector<DBQYCarTable>& QYcarInfo);
    void parseAgentGDCarInfo(const QDomElement& element,QVector<DBGDCarTable>& GDcarInfo);
    void parseAgentOperatorInfo(const QDomElement& element,QVector<DBPersonTable>& OperatorInfo);
    void parseAgentElevatorInfo(const QDomElement& element,QVector<ElevatorParameter>& ElevatorInfo);
    void parseAgentDeflectorInfo(const QDomElement& element,QVector<DeflectorParameter>& DeflectorInfo);

    void rewriteXmlNode4Plane(XMLDocument& doc,XMLElement *newnode,const DBPlaneTable& plane_agent);
    void rewriteXmlNode4QYCar(XMLDocument& doc,XMLElement *newnode,const DBQYCarTable& plane_agent);
    void rewriteXmlNode4GDCar(XMLDocument& doc,XMLElement *newnode,const DBGDCarTable& plane_agent);
    void rewriteXmlNode4Operator(XMLDocument& doc,XMLElement *newnode,const DBPersonTable& plane_agent);
    void rewriteXmlNode4Elevator(XMLDocument& doc,XMLElement *newnode,const ElevatorParameter& plane_agent);
    void rewriteXmlNode4Deflector(XMLDocument& doc,XMLElement *newnode,const DeflectorParameter& plane_agent);

    QString m_file;
private:
    LogXmlInfo m_logXmlInfo;
};




//生成xml基本框架,初始化
void xmlFileOperation:: writeLogXmlFile(const QString &fileName, const QString &timeValue){
    // 创建XML文件
    QFile file(fileName);
    if(file.exists())
        return;
    if (!file.open(QFile::WriteOnly | QFile::Text)) {
        return;
    }
    QXmlStreamWriter xmlWriter(&file);
    xmlWriter.setAutoFormatting(true); // 自动格式化XML

    // 写入XML头
    xmlWriter.writeStartDocument();

    // 写入根节点
    xmlWriter.writeStartElement("root");
    xmlWriter.writeStartElement("log");
    xmlWriter.writeAttribute("allstarttime",timeValue);

    // 飞机智能体
    xmlWriter.writeStartElement("plane_agents");
    xmlWriter.writeEndElement(); // plane_agents

    // QY智能车
    xmlWriter.writeStartElement("QYcar_agents");
    xmlWriter.writeEndElement(); // QYcar_agents

    // GD智能车
    xmlWriter.writeStartElement("GDcar_agents");
    xmlWriter.writeEndElement(); // GDcar_agents

    // 操作人员智能体
    xmlWriter.writeStartElement("operator_agents");
    xmlWriter.writeEndElement(); // operator_agents

    // 升降机智能体
    xmlWriter.writeStartElement("elevator_agents");
    xmlWriter.writeEndElement(); // operator_agents

    // 偏流板智能体
    xmlWriter.writeStartElement("deflector_agents");
    xmlWriter.writeEndElement(); // operator_agents

    // 结束根节点
    xmlWriter.writeEndElement(); // log

    // 结束根节点
    xmlWriter.writeEndElement(); // root

    // 结束XML文档
    xmlWriter.writeEndDocument();

    // 关闭文件并释放内存
    file.close();

    qDebug() << "XML文件已创建" << fileName;
}

//void xmlFileOperation:: writeXmlFile(const QString &fileName){
//    // 创建XML文件
//    QFile file(fileName);
//    if(file.exists())
//        return;
//    if (!file.open(QFile::WriteOnly | QFile::Text)) {
//        return;
//    }
//    QXmlStreamWriter xmlWriter(&file);
//    xmlWriter.setAutoFormatting(true); // 自动格式化XML

//    // 写入XML头
//    xmlWriter.writeStartDocument();

//    // 写入根节点
//    xmlWriter.writeStartElement("root");
//    xmlWriter.writeStartElement("log");

//    // 飞机智能体
//    xmlWriter.writeStartElement("plane_agents");
//    xmlWriter.writeEndElement(); // plane_agents

//    // QY智能车
//    xmlWriter.writeStartElement("QYcar_agents");
//    xmlWriter.writeEndElement(); // QYcar_agents

//    // GD智能车
//    xmlWriter.writeStartElement("GDcar_agents");
//    xmlWriter.writeEndElement(); // GDcar_agents

//    // 操作人员智能体
//    xmlWriter.writeStartElement("operator_agents");
//    xmlWriter.writeEndElement(); // operator_agents

//    // 升降机智能体
//    xmlWriter.writeStartElement("elevator_agents");
//    xmlWriter.writeEndElement(); // operator_agents

//    // 偏流板智能体
//    xmlWriter.writeStartElement("deflector_agents");
//    xmlWriter.writeEndElement(); // operator_agents

//    // 结束根节点
//    xmlWriter.writeEndElement(); // log

//    // 结束根节点
//    xmlWriter.writeEndElement(); // root

//    // 结束XML文档
//    xmlWriter.writeEndDocument();

//    // 关闭文件并释放内存
//    file.close();

//    qDebug() << "XML文件已创建" << fileName;
//}





void xmlFileOperation::writeMysqlConfig(QString dataBaseName, QString dataBaseIP, QString dataBaseUser, QString dataBasePassword, int dataBasePort)
{
    QString fileName = QApplication::applicationDirPath() + "/Config.xml";

    XMLDocument doc;
    XMLError errorId = doc.LoadFile(fileName.toLocal8Bit());
    if (errorId != XML_SUCCESS) {
        std::cout << "Error parsing XML file" << std::endl;
        return ;
    }
    // 获取根元素
    XMLElement *root = doc.RootElement();
    if (root == nullptr) {
        std::cout << "Unable to find root element" << std::endl;
        return ;
    }
    XMLElement *p_item = root->FirstChildElement();
    if(p_item == nullptr)
        return;

    for (; p_item != nullptr; p_item = p_item->NextSiblingElement()) {
        QString p_name = p_item->Name();
         if(p_name == "MysqlConfig")
         {
             p_item->SetAttribute("dataBaseName", dataBaseName.toStdString().c_str());
             p_item->SetAttribute("dataBaseIP", dataBaseIP.toStdString().c_str());
             p_item->SetAttribute("dataBaseUser", dataBaseUser.toStdString().c_str());
             p_item->SetAttribute("dataBasePassword", dataBasePassword.toStdString().c_str());
             p_item->SetAttribute("dataBasePort", dataBasePort);

             doc.SaveFile(fileName.toLocal8Bit());
             return;
         }
    }

    XMLElement *item = doc.NewElement("MysqlConfig");
    item->SetAttribute("dataBaseName", dataBaseName.toStdString().c_str());
    item->SetAttribute("dataBaseIP", dataBaseIP.toStdString().c_str());
    item->SetAttribute("dataBaseUser", dataBaseUser.toStdString().c_str());
    item->SetAttribute("dataBasePassword", dataBasePassword.toStdString().c_str());
    item->SetAttribute("dataBasePort", dataBasePort);

    root->InsertEndChild(item);

    doc.SaveFile(fileName.toLocal8Bit());
}

void xmlFileOperation::writeUdpConfig(QString ipAddress, quint16 port)
{
    QString fileName = QApplication::applicationDirPath() + "/Config.xml";

    XMLDocument doc;
    XMLError errorId = doc.LoadFile(fileName.toLocal8Bit());
    if (errorId != XML_SUCCESS) {
        std::cout << "Error parsing XML file" << std::endl;
        return ;
    }
    // 获取根元素
    XMLElement *root = doc.RootElement();
    if (root == nullptr) {
        std::cout << "Unable to find root element" << std::endl;
        return ;
    }
    XMLElement *p_item = root->FirstChildElement();
    if(p_item == nullptr)
        return;

    for (; p_item != nullptr; p_item = p_item->NextSiblingElement()) {
        QString p_name = p_item->Name();
         if(p_name == "UdpConfig")
         {
             p_item->SetAttribute("ipAddress", ipAddress.toStdString().c_str());
             p_item->SetAttribute("port", port);

             doc.SaveFile(fileName.toLocal8Bit());
             return;
         }
    }

    XMLElement *item = doc.NewElement("UdpConfig");
    item->SetAttribute("ipAddress", ipAddress.toStdString().c_str());
    item->SetAttribute("port", port);

    root->InsertEndChild(item);

    doc.SaveFile(fileName.toLocal8Bit());
}

void xmlFileOperation::readMysqlConfig(QString &dataBaseName, QString &dataBaseIP, QString &dataBaseUser, QString &dataBasePassword, int &dataBasePort)
{
    QString fileName = QApplication::applicationDirPath() + "/Config.xml";

    XMLDocument doc;
      XMLError errorId = doc.LoadFile(fileName.toLocal8Bit());
      if (errorId != XML_SUCCESS) {
          std::cout << "Error parsing XML file" << std::endl;
          return ;
      }

      // 获取根元素
      XMLElement *root = doc.RootElement();
      if (root == nullptr) {
          std::cout << "Unable to find root element" << std::endl;
          return;
      }
      XMLElement *p_item = root->FirstChildElement();
      if(p_item == nullptr)
          return;

      for (; p_item != nullptr; p_item = p_item->NextSiblingElement()) {
          QString p_name = p_item->Name();
           if(p_name == "MysqlConfig")
           {
               dataBaseName = p_item->Attribute("dataBaseName");
               dataBaseIP = p_item->Attribute("dataBaseIP");
               dataBasePort =  QString(p_item->Attribute("dataBasePort")).toInt();
               dataBaseUser = p_item->Attribute("dataBaseUser");
               dataBasePassword = p_item->Attribute("dataBasePassword");
           }
      }
}

void xmlFileOperation::readUdpConfig(QString &ipAddress, quint16 &port)
{
    QString fileName = QApplication::applicationDirPath() + "/Config.xml";

    XMLDocument doc;
      XMLError errorId = doc.LoadFile(fileName.toLocal8Bit());
      if (errorId != XML_SUCCESS) {
          std::cout << "Error parsing XML file" << std::endl;
          return ;
      }

      // 获取根元素
      XMLElement *root = doc.RootElement();
      if (root == nullptr) {
          std::cout << "Unable to find root element" << std::endl;
          return;
      }
      XMLElement *p_item = root->FirstChildElement();
      if(p_item == nullptr)
          return;

      for (; p_item != nullptr; p_item = p_item->NextSiblingElement()) {
          QString p_name = p_item->Name();
           if(p_name == "UdpConfig")
           {
               ipAddress = p_item->Attribute("ipAddress");
               port =  QString(p_item->Attribute("port")).toInt();
           }
      }
}

void xmlFileOperation::writeProjectFile(const projectConfig &config)
{
    QString filePath2 = QApplication::applicationDirPath() + g_projectPath;
    QDir dir2(filePath2);
    if(!dir2.exists())
    {
        dir2.mkdir(filePath2);
    }

    QString filePath = QApplication::applicationDirPath() + g_projectPath + config.projectName;
    QDir dir(filePath);
    if(!dir.exists())
    {
        dir.mkdir(filePath);
    }
    QString fileName = filePath +"/" + config.projectName + FILESUFFIX;

    XMLDocument xml;
    XMLDeclaration* declaration = xml.NewDeclaration();
    xml.InsertFirstChild(declaration);

    XMLElement* rootNode = xml.NewElement("myProject");
    xml.InsertEndChild(rootNode);

    rootNode->SetAttribute("projectName",config.projectName.toStdString().c_str());
    rootNode->SetAttribute("taskName",config.taskName.toStdString().c_str());
    rootNode->SetAttribute("taskId",config.taskId);
    rootNode->SetAttribute("JBMapPath",config.JBMapPath.toStdString().c_str());
    rootNode->SetAttribute("JKMapPath",config.JKMapPath.toStdString().c_str());
    rootNode->SetAttribute("allstarttime",config.allstarttime.toStdString().c_str());

    xml.SaveFile(fileName.toLocal8Bit());
    QFileInfo fileInfo1(config.JBMapPath);
    if(fileInfo1.isFile())
    {
        QString map1 = filePath + "/"+ fileInfo1.baseName() + FILESUFFIX;
//        QString map1 = filePath + "/"+ config.projectName + "_JBMap" + FILESUFFIX;
        QFile::copy(config.JBMapPath,map1);
    }

    QFileInfo fileInfo2(config.JKMapPath);
    if(fileInfo2.isFile())
    {
        QString map1 = filePath + "/"+ fileInfo2.baseName() + FILESUFFIX;
//        QString map1 = filePath + "/"+ config.projectName + "_JkMap" + FILESUFFIX;
        QFile::copy(config.JKMapPath,map1);
    }
    writeLogXmlFile(filePath +"/" + config.projectName + "_Agent"+FILESUFFIX,config.allstarttime);
}

bool xmlFileOperation::getProjectInfo(const QString &path, projectConfig &config)
{
    XMLDocument doc;
      XMLError errorId = doc.LoadFile(path.toLocal8Bit());
      if (errorId != XML_SUCCESS) {
          std::cout << "Error parsing XML file" << std::endl;
          return false;
      }

      // 获取根元素
      XMLElement *root = doc.RootElement();
      if (root == nullptr) {
          std::cout << "Unable to find root element" << std::endl;
          return false;
      }
      config.projectName = root->Attribute("projectName");
      config.taskName = root->Attribute("taskName");
      config.taskId =  QString(root->Attribute("taskId")).toInt();
      config.JBMapPath = root->Attribute("JBMapPath");
      config.JKMapPath = root->Attribute("JKMapPath");
      config.allstarttime = root->Attribute("allstarttime");

      return true;
}

bool xmlFileOperation::deleteProjectFile(const QString &projectName)
{
    QString filePath = QApplication::applicationDirPath() + g_projectPath + projectName;
    QDir qdir(filePath);
    return qdir.removeRecursively();
}

void xmlFileOperation::writePlayBackFile(const PlayBackRecord &info)
{
    QString filePath2 = QApplication::applicationDirPath() + g_projectPath;
    QDir dir2(filePath2);
    if(!dir2.exists())
    {
        dir2.mkdir(filePath2);
    }

    QString filePath1 = filePath2+ info.projectName;
    QString timeSec = QString::number(info.playTimeMs / 1000) + "秒";
    QString filePath = filePath1 + QString("/%1_[%2(%3)]").arg(info.playbackTitle,info.playFinishTimeStr,timeSec);

    QDir dir(filePath);
    if(!dir.exists())
    {
        dir.mkdir(filePath);
    }
    QString fileName = filePath +"/playbackInfo" + FILESUFFIX;

    XMLDocument xml;
    XMLDeclaration* declaration = xml.NewDeclaration();
    xml.InsertFirstChild(declaration);

    XMLElement* rootNode = xml.NewElement("playbackInfo");
    xml.InsertEndChild(rootNode);

    rootNode->SetAttribute("projectName",info.projectName.toStdString().c_str());
    rootNode->SetAttribute("playbackTitle",info.playbackTitle.toStdString().c_str());
    rootNode->SetAttribute("playStartTimeStr",info.playStartTimeStr.toStdString().c_str());
    rootNode->SetAttribute("playFinishTimeStr",info.playFinishTimeStr.toStdString().c_str());
    rootNode->SetAttribute("playTimeMs",info.playTimeMs);
    rootNode->SetAttribute("playFrame",info.playFrame);

    xml.SaveFile(fileName.toLocal8Bit());

    QDir dir3(filePath1);
    QFileInfoList list = dir3.entryInfoList(QDir::Files | QDir::AllDirs | QDir::NoSymLinks | QDir::NoDotAndDotDot);
    foreach(QFileInfo fileInfo, list)
    {
        if(!fileInfo.isDir())
        {
            QFile::copy(fileInfo.absoluteFilePath(),filePath + + "/"+ fileInfo.fileName());
        }
    }
}

bool xmlFileOperation::getPlayBackFile(const QString &prjName,const QString &name, PlayBackRecord &config)
{
    XMLDocument doc;
    QString filePath1 = QApplication::applicationDirPath() + g_projectPath + prjName+"/";
    QString filePath2 = filePath1+ name + "/playbackInfo" + FILESUFFIX;

      XMLError errorId = doc.LoadFile(filePath2.toLocal8Bit());
      if (errorId != XML_SUCCESS) {
          std::cout << "Error parsing XML file" << std::endl;
          return false;
      }

      // 获取根元素
      XMLElement *root = doc.RootElement();
      if (root == nullptr) {
          std::cout << "Unable to find root element" << std::endl;
          return false;
      }
      config.projectName = root->Attribute("projectName");
      config.playbackTitle = root->Attribute("playbackTitle");
      config.playTimeMs =  QString(root->Attribute("playTimeMs")).toLongLong();
      config.playStartTimeStr = root->Attribute("playStartTimeStr");
      config.playFinishTimeStr = root->Attribute("playFinishTimeStr");
      config.playFrame = QString(root->Attribute("playFrame")).toInt();

      projectConfig temp;
      getProjectInfo(filePath1+"/"+prjName+FILESUFFIX, temp);

      QFileInfo map1(temp.JBMapPath);
      if(map1.isFile())
      {
          auto filename1 = map1.fileName();
          config.JBMapFilePath = filePath1+ name + "/" + map1.baseName()+FILESUFFIX;
      }

      QFileInfo map2(temp.JKMapPath);
      if(map2.isFile())
      {
          auto filename2 = map2.fileName();
          config.JKMapFilePath = filePath1+ name + "/" + map2.baseName()+FILESUFFIX;
      }
      config.AgentFilePath = filePath1+name+"/"+prjName + QString("_Agent")+FILESUFFIX;
      return true;
}

bool xmlFileOperation::deletePlayBackFile(const QString &prjName,const QString &dirName)
{
    QString filePath = QApplication::applicationDirPath() + g_projectPath + prjName + "/" + dirName;
    QDir qdir(filePath);
    return qdir.removeRecursively();
}

void xmlFileOperation::setCurrentFile(const QString &filepath)
{
    d->setCurrentFile(filepath);
}

void GetXmlElement(QList<XMLElement *> &xml_list, XMLElement *node, const QString &tag_name)
{
    if(node == nullptr)
        return;
    XMLElement *p_item = node->FirstChildElement();
    if(p_item == nullptr)
        return;
    for (; p_item != nullptr; p_item = p_item->NextSiblingElement()) {
        QString p_name = p_item->Name();
         if(p_name == tag_name)
            xml_list.push_back(p_item);

        GetXmlElement(xml_list, p_item, tag_name);
    }
}

void xmlFileOperation::rewriteXmlFile4Plane(const QVector<DBPlaneTable> &agentInfos, int updateID)
{
//    // 打开 XML 文件
    XMLDocument doc;
    XMLError error = doc.LoadFile(d->m_file.toLocal8Bit());
    if(error != XML_SUCCESS)
    {
        qDebug() << "Error: Failed to open file for writing.";
        return;
    }
    XMLDeclaration* pDeclar = doc.FirstChild()->ToDeclaration();
    if (pDeclar != NULL) {
        printf("头部信息： version is  , encoding is \n");
    }
    XMLElement *root_element = doc.RootElement();
    QList<XMLElement *> nodeList;
    GetXmlElement(nodeList, root_element, "plane_agents");

    // 查找要重写的智能体节点
    if(nodeList.size() == 1)    //找到了智能体大节点
    {
        if(updateID < 0)
        {
            auto oldNode = nodeList.at(0);
            oldNode->DeleteAttribute("number");
            oldNode->DeleteChildren();
            XMLElement *element = nodeList.at(0)->ToElement();
            element->SetAttribute("number", agentInfos.size());
            for(auto &plane_agent : agentInfos)
            {
                XMLElement *newnode = doc.NewElement("plane_agent");

                d->rewriteXmlNode4Plane(doc,newnode,plane_agent);
                element->LinkEndChild(newnode);
             }
        }else   //找到指定飞机进行更新
        {
            auto Node = nodeList.at(0);            
            XMLElement *element = Node->FirstChildElement();
            for (; element != NULL; element = element->NextSiblingElement()) {

                if(QString(element->Attribute("id")).toInt() == updateID)
                {
                   XMLElement *newnode = doc.NewElement("plane_agent");
                   d->rewriteXmlNode4Plane(doc, newnode, agentInfos.at(0));
                   Node->InsertAfterChild(element, newnode);
                   Node->DeleteChild(element);
                   break;
                }
            }
        }
    }
    doc.SaveFile(d->m_file.toLocal8Bit());
}

void xmlFileOperationPrivate::rewriteXmlNode4Plane(XMLDocument& doc,XMLElement *newnode, const DBPlaneTable &plane_agent)
{
    newnode->SetAttribute("id", QString::number(plane_agent.m_planeID).toStdString().c_str());
    newnode->SetAttribute("agentname", plane_agent.m_planeName.toStdString().c_str());
    newnode->SetAttribute("priority", QString::number(plane_agent.m_priority).toStdString().c_str());
    newnode->SetAttribute("plane_type", plane_agent.m_planeType.toStdString().c_str());
    newnode->SetAttribute("size", QString::number(plane_agent.m_size).toStdString().c_str());
    newnode->SetAttribute("movespeed", QString::number(plane_agent.m_moveSpeed).toStdString().c_str());
    newnode->SetAttribute("rotationspeed", QString::number(plane_agent.m_rotationSpeed).toStdString().c_str());
    newnode->SetAttribute("Fuel_tank", QString::number(plane_agent.m_tankCapacity).toStdString().c_str());
    newnode->SetAttribute("Refueling_speed", QString::number(plane_agent.m_refuelRate).toStdString().c_str());
    newnode->SetAttribute("Refueling_costspeed", QString::number(plane_agent.m_fuelCostRate).toStdString().c_str());
    newnode->SetAttribute("Warm_up_time", QString::number(plane_agent.m_minWarmUpTime).toStdString().c_str());
    newnode->SetAttribute("Inertial_navigation_time", QString::number(plane_agent.m_alignTime).toStdString().c_str());
    newnode->SetAttribute("Inflation_time", QString::number(plane_agent.m_inflateTime).toStdString().c_str());
    newnode->SetAttribute("Charging_time", QString::number(plane_agent.m_chargeTime).toStdString().c_str());
    newnode->SetAttribute("Min_turning_radius", QString::number(plane_agent.m_minTurnR).toStdString().c_str());
    newnode->SetAttribute("GD_local", plane_agent.m_GDLocal.toStdString().c_str());
    newnode->SetAttribute("agentlength", QString::number(plane_agent.m_planeLength).toStdString().c_str());
    newnode->SetAttribute("agentwidth", QString::number(plane_agent.m_spreadWidth).toStdString().c_str());
    newnode->SetAttribute("wheelbase", QString::number(plane_agent.m_wheelbase).toStdString().c_str());
    newnode->SetAttribute("tread", QString::number(plane_agent.m_thread).toStdString().c_str());
    newnode->SetAttribute("Wheelradius", QString::number(plane_agent.m_wheelR).toStdString().c_str());
    newnode->SetAttribute("Wheelwidth", QString::number(plane_agent.m_wheelWidth).toStdString().c_str());
    newnode->SetAttribute("agentfront", QString::number(plane_agent.m_agentFront).toStdString().c_str());
    newnode->SetAttribute("agentback", QString::number(plane_agent.m_agentBack).toStdString().c_str());

    XMLElement * init_state_node = doc.NewElement("init_state");
    init_state_node->SetAttribute("init.boardstate", QString::number(plane_agent.agentInitState.initBoardstate).toStdString().c_str());
    init_state_node->SetAttribute("init.x", QString::number(plane_agent.agentInitState.initX).toStdString().c_str());
    init_state_node->SetAttribute("init.y", QString::number(plane_agent.agentInitState.initY).toStdString().c_str());
    init_state_node->SetAttribute("init.heading", QString::number(plane_agent.agentInitState.initHeading).toStdString().c_str());
    init_state_node->SetAttribute("init.oil_volume", QString::number(plane_agent.agentInitState.initOilVolume).toStdString().c_str());
    init_state_node->SetAttribute("init.WP", QString::number(plane_agent.agentInitState.initWP).toStdString().c_str());
    init_state_node->SetAttribute("init.INS", QString::number(plane_agent.agentInitState.initINS).toStdString().c_str());
    init_state_node->SetAttribute("init.INF", QString::number(plane_agent.agentInitState.initINF).toStdString().c_str());
    init_state_node->SetAttribute("init.CHA", QString::number(plane_agent.agentInitState.initCHA).toStdString().c_str());
    init_state_node->SetAttribute("freeGD_local", plane_agent.agentInitState.initFreeGD_local.toStdString().c_str());
    init_state_node->SetAttribute("wingstate", QString::number(plane_agent.agentInitState.initWingstate).toStdString().c_str());
    newnode->InsertEndChild(init_state_node);

    XMLElement * section_set = doc.NewElement("section_set");
    for (auto& planeSection : plane_agent.agentSection){
        XMLElement * childNode = doc.NewElement("section");
        childNode->SetAttribute("id", QString::number(planeSection.id).toStdString().c_str());
        childNode->SetAttribute("tasknumber", QString::number(planeSection.tasknumber).toStdString().c_str());
        childNode->SetAttribute("start.boardstate", QString::number(planeSection.agentStartBoardstateType).toStdString().c_str());
        childNode->SetAttribute("start.x", QString::number(planeSection.agentStartX).toStdString().c_str());
        childNode->SetAttribute("start.y", QString::number(planeSection.agentStartY).toStdString().c_str());
        childNode->SetAttribute("start.heading", QString::number(planeSection.agentStartHeading).toStdString().c_str());
        childNode->SetAttribute("goal.boardstate", QString::number(planeSection.agentGoalBoardstateType).toStdString().c_str());
        childNode->SetAttribute("goal.x", QString::number(planeSection.agentGoalX).toStdString().c_str());
        childNode->SetAttribute("goal.y", QString::number(planeSection.agentGoalY).toStdString().c_str());
        childNode->SetAttribute("goal.heading", QString::number(planeSection.agentGoalHeading).toStdString().c_str());
        childNode->SetAttribute("wingstate", QString::number(planeSection.wingstate).toStdString().c_str());
        childNode->SetAttribute("starttime", planeSection.agentStartTime.toStdString().c_str());
        childNode->SetAttribute("duration", QString::number(planeSection.agentgDuration).toStdString().c_str());

        if(planeSection.tasknumber == 2){
            QString GD_use_local,GD_type, GD_eachtime;
            for (auto& gdInfo : planeSection.agentGDinfo) {
                GD_use_local += QString::number(gdInfo.agentPlaneGDNum) + ";";
                GD_type += QString::number(gdInfo.agentPlaneGDType) + ";";
                GD_eachtime += QString::number(gdInfo.agentPlaneGDTime) + ";";
            }
            GD_use_local.chop(1);GD_type.chop(1);GD_eachtime.chop(1);
            childNode->SetAttribute("GD_use_local", GD_use_local.toStdString().c_str());
            childNode->SetAttribute("GD_type", GD_type.toStdString().c_str());
            childNode->SetAttribute("GD_eachtime", GD_eachtime.toStdString().c_str());
        }
        else if(planeSection.tasknumber == 1){
                childNode->SetAttribute("required_oil_volume", QString::number(planeSection.required_oil_volume).toStdString().c_str());
            }
            else if(planeSection.tasknumber == 3 || planeSection.tasknumber == 6 ||
                    planeSection.tasknumber == 7 || planeSection.tasknumber == 8){
                XMLElement * transport_path = doc.NewElement("transport_path");
                transport_path->SetAttribute("pathfound", planeSection.pathfound ? "true" : "false");
                transport_path->SetAttribute("runtime", QString::number(planeSection.runtime).toStdString().c_str());
                for (auto& transportsection : planeSection.transportSection){
                    XMLElement * transChildNode = doc.NewElement("transport_section");
                    transChildNode->SetAttribute("id", QString::number(transportsection.id).toStdString().c_str());
                    transChildNode->SetAttribute("start.boardstate", QString::number(transportsection.agentStartBoardstateType).toStdString().c_str());
                    transChildNode->SetAttribute("start.x", QString::number(transportsection.agentStartX).toStdString().c_str());
                    transChildNode->SetAttribute("start.y", QString::number(transportsection.agentStartY).toStdString().c_str());
                    transChildNode->SetAttribute("start.heading", QString::number(transportsection.agentStartHeading).toStdString().c_str());
                    transChildNode->SetAttribute("goal.boardstate", QString::number(transportsection.agentGoalBoardstateType).toStdString().c_str());
                    transChildNode->SetAttribute("goal.x", QString::number(transportsection.agentGoalX).toStdString().c_str());
                    transChildNode->SetAttribute("goal.y", QString::number(transportsection.agentGoalY).toStdString().c_str());
                    transChildNode->SetAttribute("goal.heading", QString::number(transportsection.agentGoalHeading).toStdString().c_str());
                    transChildNode->SetAttribute("duration", QString::number(transportsection.agentgDuration).toStdString().c_str());
                    transport_path->InsertEndChild(transChildNode);
                }
                childNode->InsertEndChild(transport_path);
            }
            section_set->InsertEndChild(childNode);
        }
    newnode->InsertEndChild(section_set);
}

void xmlFileOperation::rewriteXmlFile4QYCar(const QVector<DBQYCarTable> &agentInfos, int updateID)
{
    // 创建 QDomDocument 对象并加载 XML 内容
    XMLDocument doc;
    XMLError error = doc.LoadFile(d->m_file.toLocal8Bit());
    if(error != XML_SUCCESS)
    {
        qDebug() << "Error: Failed to open file for writing.";
        return;
    }
    XMLDeclaration* pDeclar = doc.FirstChild()->ToDeclaration();
    if (pDeclar != NULL) {
        printf("头部信息： version is %s , encoding is %s\n");
    }
    // 查找要重写的智能体节点
    XMLElement *root_element = doc.RootElement();
    QList<XMLElement *> nodeList;
    GetXmlElement(nodeList, root_element, "QYcar_agents");

    if(nodeList.size() == 1)    //找到了智能体大节点
    {
        XMLElement * newnode = doc.NewElement("QYcar_agent");
        if(updateID < 0)
        {
            auto oldNode = nodeList.at(0);
            oldNode->DeleteAttribute("number");
            oldNode->DeleteChildren();
            XMLElement *element = nodeList.at(0)->ToElement();
            element->SetAttribute("number", agentInfos.size());
            for(auto &plane_agent : agentInfos)
            {

                d->rewriteXmlNode4QYCar(doc,newnode,plane_agent);
                element->LinkEndChild(newnode);
             }
        }else   //找到指定飞机进行更新
        {
            auto Node = nodeList.at(0);
            XMLElement *element = Node->FirstChildElement();
            for (; element != NULL; element = element->NextSiblingElement()) {

                 if(QString(element->Attribute("id")).toInt() == updateID)
                 {
                    d->rewriteXmlNode4QYCar(doc,newnode,agentInfos.at(0));

                    Node->InsertAfterChild(element, newnode);
                    Node->DeleteChild(element);
                    break;
                 }
            }

        }
    }
    doc.SaveFile(d->m_file.toLocal8Bit());
}

void xmlFileOperationPrivate::rewriteXmlNode4QYCar(XMLDocument& doc,XMLElement *newnode, const DBQYCarTable &plane_agent)
{
    newnode->SetAttribute("id", QString::number(plane_agent.m_QYcarID).toStdString().c_str());
    newnode->SetAttribute("agentname", plane_agent.m_QYcarName.toStdString().c_str());
    newnode->SetAttribute("QY_priority", QString::number(plane_agent.m_priority).toStdString().c_str());
    newnode->SetAttribute("QY_type", plane_agent.m_QYcarType.toStdString().c_str());
    newnode->SetAttribute("size", QString::number(plane_agent.m_size).toStdString().c_str());
    newnode->SetAttribute("movespeed", QString::number(plane_agent.m_moveSpeed).toStdString().c_str());
    newnode->SetAttribute("rotationspeed", QString::number(plane_agent.m_rotationSpeed).toStdString().c_str());
    newnode->SetAttribute("Min_turning_radius", QString::number(plane_agent.m_minTurnR).toStdString().c_str());
    newnode->SetAttribute("agentlength", QString::number(plane_agent.m_carLength).toStdString().c_str());
    newnode->SetAttribute("agentwidth", QString::number(plane_agent.m_carWidth).toStdString().c_str());
    newnode->SetAttribute("wheelbase", QString::number(plane_agent.m_wheelbase).toStdString().c_str());
    newnode->SetAttribute("tread", QString::number(plane_agent.m_thread).toStdString().c_str());
    newnode->SetAttribute("Wheelradius", QString::number(plane_agent.m_wheelR).toStdString().c_str());
    newnode->SetAttribute("Wheelwidth", QString::number(plane_agent.m_wheelWidth).toStdString().c_str());
    newnode->SetAttribute("agentfront", QString::number(plane_agent.m_agentFront).toStdString().c_str());
    newnode->SetAttribute("agentback", QString::number(plane_agent.m_agentBack).toStdString().c_str());

    XMLElement * init_state_node = doc.NewElement("init_state");
    init_state_node->SetAttribute("init.boardstate", QString::number(plane_agent.agentInitState.initBoardstate).toStdString().c_str());
    init_state_node->SetAttribute("init.x", QString::number(plane_agent.agentInitState.initX).toStdString().c_str());
    init_state_node->SetAttribute("init.y", QString::number(plane_agent.agentInitState.initY).toStdString().c_str());
    init_state_node->SetAttribute("init.heading", QString::number(plane_agent.agentInitState.initHeading).toStdString().c_str());
    init_state_node->SetAttribute("connectstate", QString::number(plane_agent.agentInitState.initConnectstate).toStdString().c_str());
    newnode->LinkEndChild(init_state_node);

    XMLElement * section_set = doc.NewElement("section_set");
    for (auto& planeSection : plane_agent.agentSection){
        XMLElement * childNode = doc.NewElement("section");
        childNode->SetAttribute("id", QString::number(planeSection.id).toStdString().c_str());
        childNode->SetAttribute("tasknumber", QString::number(planeSection.tasknumber).toStdString().c_str());
        childNode->SetAttribute("start.boardstate", QString::number(planeSection.agentStartBoardstateType).toStdString().c_str());
        childNode->SetAttribute("start.x", QString::number(planeSection.agentStartX).toStdString().c_str());
        childNode->SetAttribute("start.y", QString::number(planeSection.agentStartY).toStdString().c_str());
        childNode->SetAttribute("start.heading", QString::number(planeSection.agentStartHeading).toStdString().c_str());
        childNode->SetAttribute("goal.boardstate", QString::number(planeSection.agentGoalBoardstateType).toStdString().c_str());
        childNode->SetAttribute("goal.x", QString::number(planeSection.agentGoalX).toStdString().c_str());
        childNode->SetAttribute("goal.y", QString::number(planeSection.agentGoalY).toStdString().c_str());
        childNode->SetAttribute("goal.heading", QString::number(planeSection.agentGoalHeading).toStdString().c_str());
        childNode->SetAttribute("starttime", planeSection.agentStartTime.toStdString().c_str());
        childNode->SetAttribute("duration", QString::number(planeSection.agentgDuration).toStdString().c_str());
        childNode->SetAttribute("connectstate", QString::number(planeSection.connectstate).toStdString().c_str());

        if(planeSection.tasknumber == 3 /*|| planeSection.tasknumber == 6 ||
                planeSection.tasknumber == 7 || planeSection.tasknumber == 8*/){
            XMLElement * transport_path = doc.NewElement("transport_path");
            transport_path->SetAttribute("pathfound", planeSection.pathfound ? "true" : "false");
            transport_path->SetAttribute("runtime", QString::number(planeSection.runtime).toStdString().c_str());
            for (auto& transportsection : planeSection.transportSection){
                XMLElement * transChildNode = doc.NewElement("transport_section");
                transChildNode->SetAttribute("id", QString::number(transportsection.id).toStdString().c_str());
                transChildNode->SetAttribute("start.boardstate", QString::number(transportsection.agentStartBoardstateType).toStdString().c_str());
                transChildNode->SetAttribute("start.x", QString::number(transportsection.agentStartX).toStdString().c_str());
                transChildNode->SetAttribute("start.y", QString::number(transportsection.agentStartY).toStdString().c_str());
                transChildNode->SetAttribute("start.heading", QString::number(transportsection.agentStartHeading).toStdString().c_str());
                transChildNode->SetAttribute("goal.boardstate", QString::number(transportsection.agentGoalBoardstateType).toStdString().c_str());
                transChildNode->SetAttribute("goal.x", QString::number(transportsection.agentGoalX).toStdString().c_str());
                transChildNode->SetAttribute("goal.y", QString::number(transportsection.agentGoalY).toStdString().c_str());
                transChildNode->SetAttribute("goal.heading", QString::number(transportsection.agentGoalHeading).toStdString().c_str());
                transChildNode->SetAttribute("duration", QString::number(transportsection.agentgDuration).toStdString().c_str());
                transport_path->LinkEndChild(transChildNode);
            }
            childNode->LinkEndChild(transport_path);
        }
        section_set->LinkEndChild(childNode);
    }
    newnode->LinkEndChild(section_set);
}

void xmlFileOperation::rewriteXmlFile4GDCar(const QVector<DBGDCarTable> &agentInfos, int updateID)
{
    // 创建 QDomDocument 对象并加载 XML 内容
    XMLDocument doc;
    XMLError error = doc.LoadFile(d->m_file.toLocal8Bit());
    if(error != XML_SUCCESS)
    {
        qDebug() << "Error: Failed to open file for writing.";
        return;
    }
    XMLDeclaration* pDeclar = doc.FirstChild()->ToDeclaration();
    if (pDeclar != NULL) {
        printf("头部信息： version is %s , encoding is %s\n");
    }
    XMLElement *root_element = doc.RootElement();
    QList<XMLElement *> nodeList;
    GetXmlElement(nodeList, root_element, "GDcar_agents");

    // 查找要重写的智能体节点

    if(nodeList.size() == 1)    //找到了智能体大节点
    {
        XMLElement * newnode = doc.NewElement("GDcar_agent");
        if(updateID < 0)
        {
            auto oldNode = nodeList.at(0);
            oldNode->DeleteAttribute("number");
            oldNode->DeleteChildren();
            XMLElement *element = nodeList.at(0)->ToElement();
            element->SetAttribute("number", agentInfos.size());
            for(auto &plane_agent : agentInfos)
            {
                d->rewriteXmlNode4GDCar(doc,newnode,plane_agent);
                element->LinkEndChild(newnode);
             }
        }else   //找到指定飞机进行更新
        {
            auto Node = nodeList.at(0);
            XMLElement *element = Node->FirstChildElement();
            for (; element != NULL; element = element->NextSiblingElement()) {

                if(QString(element->Attribute("id")).toInt() == updateID)
                {
                    d->rewriteXmlNode4GDCar(doc,newnode,agentInfos.at(0));

                    Node->InsertAfterChild(element, newnode);
                    Node->DeleteChild(element);
                    break;
                 }
            }

        }
    }
    doc.SaveFile(d->m_file.toLocal8Bit());
}

void xmlFileOperationPrivate::rewriteXmlNode4GDCar(XMLDocument& doc,XMLElement *newnode, const DBGDCarTable &plane_agent)
{
    newnode->SetAttribute("id", QString::number(plane_agent.m_GDcarID).toStdString().c_str());
    newnode->SetAttribute("agentname", plane_agent.m_GDcarName.toStdString().c_str());
    newnode->SetAttribute("GD_priority", QString::number(plane_agent.m_priority).toStdString().c_str());
    newnode->SetAttribute("GD_type", plane_agent.m_GDcarType.toStdString().c_str());
    newnode->SetAttribute("size", QString::number(plane_agent.m_size).toStdString().c_str());
    newnode->SetAttribute("movespeed", QString::number(plane_agent.m_moveSpeed).toStdString().c_str());
    newnode->SetAttribute("rotationspeed", QString::number(plane_agent.m_rotationSpeed).toStdString().c_str());
    newnode->SetAttribute("Min_turning_radius", QString::number(plane_agent.m_minTurnR).toStdString().c_str());
    newnode->SetAttribute("agentlength", QString::number(plane_agent.m_carLength).toStdString().c_str());
    newnode->SetAttribute("agentwidth", QString::number(plane_agent.m_carWidth).toStdString().c_str());
    newnode->SetAttribute("wheelbase", QString::number(plane_agent.m_wheelbase).toStdString().c_str());
    newnode->SetAttribute("tread", QString::number(plane_agent.m_thread).toStdString().c_str());
    newnode->SetAttribute("Wheelradius", QString::number(plane_agent.m_wheelR).toStdString().c_str());
    newnode->SetAttribute("Wheelwidth", QString::number(plane_agent.m_wheelWidth).toStdString().c_str());
    newnode->SetAttribute("agentfront", QString::number(plane_agent.m_agentFront).toStdString().c_str());
    newnode->SetAttribute("agentback", QString::number(plane_agent.m_agentBack).toStdString().c_str());

    XMLElement * init_state_node = doc.NewElement("init_state");
    init_state_node->SetAttribute("init.boardstate", QString::number(plane_agent.agentInitState.initBoardstate).toStdString().c_str());
    init_state_node->SetAttribute("init.x", QString::number(plane_agent.agentInitState.initX).toStdString().c_str());
    init_state_node->SetAttribute("init.y", QString::number(plane_agent.agentInitState.initY).toStdString().c_str());
    init_state_node->SetAttribute("init.heading", QString::number(plane_agent.agentInitState.initHeading).toStdString().c_str());
    init_state_node->SetAttribute("loadstate", QString::number(plane_agent.agentInitState.initLoadstate).toStdString().c_str());
    newnode->LinkEndChild(init_state_node);

    XMLElement * section_set = doc.NewElement("section_set");
    for (auto& planeSection : plane_agent.agentSection){
        XMLElement * childNode = doc.NewElement("section");
        childNode->SetAttribute("id", QString::number(planeSection.id).toStdString().c_str());
        childNode->SetAttribute("tasknumber", QString::number(planeSection.tasknumber).toStdString().c_str());
        childNode->SetAttribute("start.boardstate", QString::number(planeSection.agentStartBoardstateType).toStdString().c_str());
        childNode->SetAttribute("start.x", QString::number(planeSection.agentStartX).toStdString().c_str());
        childNode->SetAttribute("start.y", QString::number(planeSection.agentStartY).toStdString().c_str());
        childNode->SetAttribute("start.heading", QString::number(planeSection.agentStartHeading).toStdString().c_str());
        childNode->SetAttribute("goal.boardstate", QString::number(planeSection.agentGoalBoardstateType).toStdString().c_str());
        childNode->SetAttribute("goal.x", QString::number(planeSection.agentGoalX).toStdString().c_str());
        childNode->SetAttribute("goal.y", QString::number(planeSection.agentGoalY).toStdString().c_str());
        childNode->SetAttribute("goal.heading", QString::number(planeSection.agentGoalHeading).toStdString().c_str());
        childNode->SetAttribute("starttime", planeSection.agentStartTime.toStdString().c_str());
        childNode->SetAttribute("duration", QString::number(planeSection.agentgDuration).toStdString().c_str());
//        childNode.setAttribute("connectstate", QString::number(planeSection.connectstate));

        if(planeSection.tasknumber == 3 /*|| planeSection.tasknumber == 6 ||
                planeSection.tasknumber == 7 || planeSection.tasknumber == 8*/){
            XMLElement * transport_path = doc.NewElement("transport_path");
            transport_path->SetAttribute("pathfound", planeSection.pathfound ? "true" : "false");
            transport_path->SetAttribute("runtime", QString::number(planeSection.runtime).toStdString().c_str());
            for (auto& transportsection : planeSection.transportSection){
                XMLElement * transChildNode = doc.NewElement("transport_section");
                transChildNode->SetAttribute("id", QString::number(transportsection.id).toStdString().c_str());
                transChildNode->SetAttribute("start.boardstate", QString::number(transportsection.agentStartBoardstateType).toStdString().c_str());
                transChildNode->SetAttribute("start.x", QString::number(transportsection.agentStartX).toStdString().c_str());
                transChildNode->SetAttribute("start.y", QString::number(transportsection.agentStartY).toStdString().c_str());
                transChildNode->SetAttribute("start.heading", QString::number(transportsection.agentStartHeading).toStdString().c_str());
                transChildNode->SetAttribute("goal.boardstate", QString::number(transportsection.agentGoalBoardstateType).toStdString().c_str());
                transChildNode->SetAttribute("goal.x", QString::number(transportsection.agentGoalX).toStdString().c_str());
                transChildNode->SetAttribute("goal.y", QString::number(transportsection.agentGoalY).toStdString().c_str());
                transChildNode->SetAttribute("goal.heading", QString::number(transportsection.agentGoalHeading).toStdString().c_str());
                transChildNode->SetAttribute("duration", QString::number(transportsection.agentgDuration).toStdString().c_str());
                transport_path->LinkEndChild(transChildNode);
            }
            childNode->LinkEndChild(transport_path);
        }
        section_set->LinkEndChild(childNode);
    }
    newnode->LinkEndChild(section_set);
}

void xmlFileOperation::rewriteXmlFile4Operator(const QVector<DBPersonTable> &agentInfos, int updateID)
{
    XMLDocument doc;
    XMLError error = doc.LoadFile(d->m_file.toLocal8Bit());
    if(error != XML_SUCCESS)
    {
        qDebug() << "Error: Failed to open file for writing.";
        return;
    }
    XMLDeclaration* pDeclar = doc.FirstChild()->ToDeclaration();
    if (pDeclar != NULL) {
        printf("头部信息： version is %s , encoding is %s\n");
    }
    // 查找要重写的智能体节点
    XMLElement *root_element = doc.RootElement();
    QList<XMLElement *> nodeList;
    GetXmlElement(nodeList, root_element, "operator_agents");

    if(nodeList.size() == 1)    //找到了智能体大节点
    {
        XMLElement * newnode = doc.NewElement("operator_agent");
        if(updateID < 0)
        {
            auto oldNode = nodeList.at(0);
            oldNode->DeleteAttribute("number");
            oldNode->DeleteChildren();
            XMLElement *element = nodeList.at(0)->ToElement();
            element->SetAttribute("number", agentInfos.size());
            for(auto &plane_agent : agentInfos)
            {
                d->rewriteXmlNode4Operator(doc,newnode,plane_agent);
                element->LinkEndChild(newnode);
             }
        }else   //找到指定飞机进行更新
        {
            auto Node = nodeList.at(0);
            XMLElement *element = Node->FirstChildElement();
            for (; element != NULL; element = element->NextSiblingElement()) {

                if(QString(element->Attribute("id")).toInt() == updateID)
                {
                    d->rewriteXmlNode4Operator(doc,newnode,agentInfos.at(0));

                    Node->InsertAfterChild(element, newnode);
                    Node->DeleteChild(element);
                    break;
                 }
            }

        }
    }
    doc.SaveFile(d->m_file.toLocal8Bit());
}

void xmlFileOperationPrivate::rewriteXmlNode4Operator(XMLDocument& doc,XMLElement *newnode, const DBPersonTable &plane_agent)
{
    newnode->SetAttribute("id", QString::number(plane_agent.m_personID).toStdString().c_str());
    newnode->SetAttribute("agentname", plane_agent.m_personName.toStdString().c_str());
    newnode->SetAttribute("operator_priority", QString::number(plane_agent.m_priority).toStdString().c_str());
    newnode->SetAttribute("operator_type", plane_agent.m_personType.toStdString().c_str());
    newnode->SetAttribute("size", QString::number(plane_agent.m_graphicsR).toStdString().c_str());
    newnode->SetAttribute("movespeed", QString::number(plane_agent.m_moveSpeed).toStdString().c_str());
    newnode->SetAttribute("rotationspeed", QString::number(plane_agent.m_rotationSpeed).toStdString().c_str());

    XMLElement * init_state_node = doc.NewElement("init_state");
    init_state_node->SetAttribute("init.boardstate", QString::number(plane_agent.agentInitState.initBoardstate).toStdString().c_str());
    init_state_node->SetAttribute("init.x", QString::number(plane_agent.agentInitState.initX).toStdString().c_str());
    init_state_node->SetAttribute("init.y", QString::number(plane_agent.agentInitState.initY).toStdString().c_str());
    init_state_node->SetAttribute("init.heading", QString::number(plane_agent.agentInitState.initHeading).toStdString().c_str());
    init_state_node->SetAttribute("workstate", QString::number(plane_agent.agentInitState.initWorkstate).toStdString().c_str());
    newnode->LinkEndChild(init_state_node);

    XMLElement * section_set = doc.NewElement("section_set");
    for (auto& planeSection : plane_agent.agentSection){
        XMLElement * childNode = doc.NewElement("section");
        childNode->SetAttribute("id", QString::number(planeSection.id).toStdString().c_str());
        childNode->SetAttribute("tasknumber", QString::number(planeSection.tasknumber).toStdString().c_str());
        childNode->SetAttribute("start.boardstate", QString::number(planeSection.agentStartBoardstateType).toStdString().c_str());
        childNode->SetAttribute("start.x", QString::number(planeSection.agentStartX).toStdString().c_str());
        childNode->SetAttribute("start.y", QString::number(planeSection.agentStartY).toStdString().c_str());
        childNode->SetAttribute("start.heading", QString::number(planeSection.agentStartHeading).toStdString().c_str());
        childNode->SetAttribute("goal.boardstate", QString::number(planeSection.agentGoalBoardstateType).toStdString().c_str());
        childNode->SetAttribute("goal.x", QString::number(planeSection.agentGoalX).toStdString().c_str());
        childNode->SetAttribute("goal.y", QString::number(planeSection.agentGoalY).toStdString().c_str());
        childNode->SetAttribute("goal.heading", QString::number(planeSection.agentGoalHeading).toStdString().c_str());
        childNode->SetAttribute("starttime", planeSection.agentStartTime.toStdString().c_str());
        childNode->SetAttribute("duration", QString::number(planeSection.agentgDuration).toStdString().c_str());
        childNode->SetAttribute("workstate", QString::number(planeSection.workstate).toStdString().c_str());

        if(planeSection.tasknumber == 3 /*|| planeSection.tasknumber == 6 ||
                planeSection.tasknumber == 7 || planeSection.tasknumber == 8*/){
            XMLElement * transport_path = doc.NewElement("transport_path");
            transport_path->SetAttribute("pathfound", planeSection.pathfound ? "true" : "false");
            transport_path->SetAttribute("runtime", QString::number(planeSection.runtime).toStdString().c_str());
            for (auto& transportsection : planeSection.transportSection){
                XMLElement * transChildNode = doc.NewElement("transport_section");
                transChildNode->SetAttribute("id", QString::number(transportsection.id).toStdString().c_str());
                transChildNode->SetAttribute("start.boardstate", QString::number(transportsection.agentStartBoardstateType).toStdString().c_str());
                transChildNode->SetAttribute("start.x", QString::number(transportsection.agentStartX).toStdString().c_str());
                transChildNode->SetAttribute("start.y", QString::number(transportsection.agentStartY).toStdString().c_str());
                transChildNode->SetAttribute("start.heading", QString::number(transportsection.agentStartHeading).toStdString().c_str());
                transChildNode->SetAttribute("goal.boardstate", QString::number(transportsection.agentGoalBoardstateType).toStdString().c_str());
                transChildNode->SetAttribute("goal.x", QString::number(transportsection.agentGoalX).toStdString().c_str());
                transChildNode->SetAttribute("goal.y", QString::number(transportsection.agentGoalY).toStdString().c_str());
                transChildNode->SetAttribute("goal.heading", QString::number(transportsection.agentGoalHeading).toStdString().c_str());
                transChildNode->SetAttribute("duration", QString::number(transportsection.agentgDuration).toStdString().c_str());
                transport_path->LinkEndChild(transChildNode);
            }
            childNode->LinkEndChild(transport_path);
        }
        section_set->LinkEndChild(childNode);
    }
    newnode->LinkEndChild(section_set);
}

void xmlFileOperation::rewriteXmlFile4Elevator(const QVector<ElevatorParameter> &agentInfos, int updateID)
{
    XMLDocument doc;
    XMLError error = doc.LoadFile(d->m_file.toLocal8Bit());
    if(error != XML_SUCCESS)
    {
        qDebug() << "Error: Failed to open file for writing.";
        return;
    }
    XMLDeclaration* pDeclar = doc.FirstChild()->ToDeclaration();
    if (pDeclar != NULL) {
        printf("头部信息： version is %s , encoding is %s\n");
    }
    // 查找要重写的智能体节点
    XMLElement *root_element = doc.RootElement();
    QList<XMLElement *> nodeList;
    GetXmlElement(nodeList, root_element, "elevator_agents");

    if(nodeList.size() == 1)    //找到了智能体大节点
    {
        XMLElement * newnode = doc.NewElement("elevator_agent");
        if(updateID < 0)
        {
            auto oldNode = nodeList.at(0);
            oldNode->DeleteAttribute("number");
            oldNode->DeleteChildren();
            XMLElement *element = nodeList.at(0)->ToElement();
            element->SetAttribute("number", agentInfos.size());
            for(auto &plane_agent : agentInfos)
            {
                d->rewriteXmlNode4Elevator(doc,newnode,plane_agent);
                element->LinkEndChild(newnode);
             }
        }else   //找到指定飞机进行更新
        {
            auto Node = nodeList.at(0);
            XMLElement *element = Node->FirstChildElement();
            for (; element != NULL; element = element->NextSiblingElement()) {

                if(QString(element->Attribute("id")).toInt() == updateID)
                {
                    d->rewriteXmlNode4Elevator(doc,newnode,agentInfos.at(0));

                    Node->InsertAfterChild(element, newnode);
                    Node->DeleteChild(element);
                    break;
                 }
            }

        }
    }
    doc.SaveFile(d->m_file.toLocal8Bit());
}

void xmlFileOperationPrivate::rewriteXmlNode4Elevator(XMLDocument& doc,XMLElement *newnode, const ElevatorParameter &plane_agent)
{
    newnode->SetAttribute("id", QString::number(plane_agent.id).toStdString().c_str());
    newnode->SetAttribute("agentname", plane_agent.agentname.toStdString().c_str());
    newnode->SetAttribute("ele_priority", QString::number(plane_agent.ele_priority).toStdString().c_str());
    newnode->SetAttribute("ele_location", plane_agent.ele_location);
    newnode->SetAttribute("movespeed", QString::number(plane_agent.movespeed).toStdString().c_str());

    XMLElement * init_state_node = doc.NewElement("init_state");
    init_state_node->SetAttribute("init.boardstate", QString::number(plane_agent.initBoardstate).toStdString().c_str());
    newnode->LinkEndChild(init_state_node);

    XMLElement * section_set = doc.NewElement("section_set");
    for (auto& planeSection : plane_agent.elevatorSection){
        XMLElement * childNode = doc.NewElement("section");
        childNode->SetAttribute("id", QString::number(planeSection.id).toStdString().c_str());
        childNode->SetAttribute("tasknumber", QString::number(planeSection.taskNumber).toStdString().c_str());
        childNode->SetAttribute("start.boardstate", QString::number(planeSection.startBoardstate).toStdString().c_str());
        childNode->SetAttribute("goal.boardstate", QString::number(planeSection.GoalBoardstate).toStdString().c_str());
        childNode->SetAttribute("starttime", planeSection.startTime.toStdString().c_str());
        childNode->SetAttribute("duration", QString::number(planeSection.duration).toStdString().c_str());

        section_set->LinkEndChild(childNode);
    }
    newnode->LinkEndChild(section_set);
}

void xmlFileOperation::rewriteXmlFile4Deflectorr(const QVector<DeflectorParameter> &agentInfos, int updateID)
{
    XMLDocument doc;
    XMLError error = doc.LoadFile(d->m_file.toLocal8Bit());
    if(error != XML_SUCCESS)
    {
        qDebug() << "Error: Failed to open file for writing.";
        return;
    }
    XMLDeclaration* pDeclar = doc.FirstChild()->ToDeclaration();
    if (pDeclar != NULL) {
        printf("头部信息： version is %s , encoding is %s\n");
    }
    // 查找要重写的智能体节点
    XMLElement *root_element = doc.RootElement();
    QList<XMLElement *> nodeList;
    GetXmlElement(nodeList, root_element, "deflector_agents");

    if(nodeList.size() == 1)    //找到了智能体大节点
    {
        XMLElement * newnode = doc.NewElement("deflector_agent");
        if(updateID < 0)
        {
            auto oldNode = nodeList.at(0);
            oldNode->DeleteAttribute("number");
            oldNode->DeleteChildren();
            XMLElement *element = nodeList.at(0)->ToElement();
            element->SetAttribute("number", agentInfos.size());
            for(auto &plane_agent : agentInfos)
            {
                d->rewriteXmlNode4Deflector(doc,newnode,plane_agent);
                element->LinkEndChild(newnode);
             }
        }else   //找到指定飞机进行更新
        {
            auto Node = nodeList.at(0);
            XMLElement *element = Node->FirstChildElement();
            for (; element != NULL; element = element->NextSiblingElement()) {

                if(QString(element->Attribute("id")).toInt() == updateID)
                {
                    d->rewriteXmlNode4Deflector(doc,newnode,agentInfos.at(0));

                    Node->InsertAfterChild(element, newnode);
                    Node->DeleteChild(element);
                    break;
                 }
            }

        }
    }
    doc.SaveFile(d->m_file.toLocal8Bit());
}

void xmlFileOperationPrivate::rewriteXmlNode4Deflector(XMLDocument& doc,XMLElement *newnode, const DeflectorParameter &plane_agent)
{
    newnode->SetAttribute("id", QString::number(plane_agent.id).toStdString().c_str());
    newnode->SetAttribute("agentname", plane_agent.agentname.toStdString().c_str());
    newnode->SetAttribute("def_priority", QString::number(plane_agent.def_priority).toStdString().c_str());
    newnode->SetAttribute("def_location", plane_agent.def_location);
    newnode->SetAttribute("rotationspeed", QString::number(plane_agent.rotationspeed).toStdString().c_str());

    XMLElement * init_state_node = doc.NewElement("init_state");
    init_state_node->SetAttribute("init.locationstate", QString::number(plane_agent.initLocationstate).toStdString().c_str());
    newnode->LinkEndChild(init_state_node);

    XMLElement * section_set = doc.NewElement("section_set");
    for (auto& planeSection : plane_agent.deflectorSection){
        XMLElement * childNode = doc.NewElement("section");
        childNode->SetAttribute("id", QString::number(planeSection.id).toStdString().c_str());
        childNode->SetAttribute("tasknumber", QString::number(planeSection.taskNumber).toStdString().c_str());
        childNode->SetAttribute("start.locationstate", QString::number(planeSection.startLocationstate).toStdString().c_str());
        childNode->SetAttribute("goal.locationstate", QString::number(planeSection.GoalLocationstate).toStdString().c_str());
        childNode->SetAttribute("starttime", planeSection.startTime.toStdString().c_str());
        childNode->SetAttribute("duration", QString::number(planeSection.duration).toStdString().c_str());

        section_set->LinkEndChild(childNode);
    }
    newnode->LinkEndChild(section_set);
}


xmlFileOperation::xmlFileOperation(QWidget *parent)
    : QObject(parent),
      d(new xmlFileOperationPrivate)
{

}

xmlFileOperation::~xmlFileOperation()
{
    delete  d;
    d = nullptr;
}

void xmlFileOperation::readConfigXml(ConfigXmlInfo &config)
{
    QDomDocument doc;
    QString path = QApplication::applicationDirPath()+"/Config.xml";
    QFile file(path);
    QString error = "";
    int row = 0, column = 0;
    if (!file.open(QIODevice::ReadOnly)) return;

    if(!doc.setContent(&file, false, &error, &row, &column)){
        qDebug() << "parse file failed:" << row << "---" << column <<":" <<error;
        file.close();
        return;
    }
    file.close();
    QDomElement root = doc.documentElement();
    QDomNode node = root.firstChild();    //得到COMMAND标签
    while(!node.isNull()) {
        QDomElement element = node.toElement(); // try to convert the node to an element.
        if(!element.isNull()) {
            auto nn = element.tagName();
            if(element.tagName() == "taskIDwithNameString")   //
            {
                int count = element.childNodes().count();  //8
                for(int i = 0 ; i < count; ++i)
                {
                    const QDomNode& node = element.childNodes().at(i);
                    const auto &ele = node.toElement();

                    if(node.nodeName() == "taskInfo")
                    {
                        int num = ele.hasAttribute("taskID")?ele.attribute("taskID").toFloat():-1;
                        QString&& name = ele.hasAttribute("taskName")?ele.attribute("taskName"):"未知任务";
                        config.TaskID_Name[num] = name;
                    }
                }
            }
            else if(element.tagName() == "FileInfo")
            {
                int count = element.childNodes().count();  //8
                for(int i = 0 ; i < count; ++i)
                {
                    const QDomNode& node = element.childNodes().at(i);
                    const auto &ele = node.toElement();

                    if(node.nodeName() == "filepath")
                    {
                        config.filePath = ele.hasAttribute("path")?ele.attribute("path"):"";
                    }
                }
            }
        node = node.nextSibling();
    }

    }
}

void xmlFileOperation::readLogXmlFile(LogXmlInfo &logXmlInfo, const QString &filePath)
{
    //QDomDocument 用于解析xml
    QDomDocument doc;
    QFile file(filePath);
    QString error = "";
    int row = 0, column = 0;
    if (!file.open(QIODevice::ReadOnly)) return;

    if(!doc.setContent(&file, false, &error, &row, &column)){
        QMessageBox::critical(nullptr,"解析失败","xml文件格式有问题！");
        qDebug() << "parse file failed:" << row << "---" << column <<":" <<error;
        file.close();
        return;
    }
    file.close();

    QDomElement root = doc.documentElement();
    QDomNode node = root.firstChild();    //得到COMMAND标签

    while(!node.isNull()) {
        QDomElement element = node.toElement(); // try to convert the node to an element.
        if(!element.isNull()) {
            auto nn = element.tagName();
            if(element.tagName() == "map")     //地图信息
            {
                d->parseMapBaseInfo(element,logXmlInfo.mapStyle);
            }
            else if(element.tagName() == "log")     //智能体信息
            {
                logXmlInfo.allStartTime = element.hasAttribute("allstarttime")?element.attribute("allstarttime"):"";
                d->parseAgentsLogPathInfo(element,logXmlInfo);
            }
        }
        node = node.nextSibling();
    }
}


void xmlFileOperationPrivate::parseAgentsLogPathInfo(const QDomElement &element, LogXmlInfo &agentLogInfo)
{
    int count = element.childNodes().count();

    for(int i = 0 ; i < count; ++i)
    {
        const QDomNode& node = element.childNodes().at(i);
        const auto &ele = node.toElement();
        if(node.nodeName() == "plane_agents")
        {
            parseAgentPlaneInfo(ele,agentLogInfo.agentPlaneInfo);
        }
        else if(node.nodeName() == "QYcar_agents")
        {
            parseAgentQYCarInfo(ele,agentLogInfo.agentQYCarInfo);
        }
        else if(node.nodeName() == "GDcar_agents")
        {
            parseAgentGDCarInfo(ele,agentLogInfo.agentGDCarInfo);
        }
        else if(node.nodeName() == "operator_agents")
        {
            parseAgentOperatorInfo(ele,agentLogInfo.agentOperInfo);
        }
        else if(node.nodeName() == "elevator_agents")
        {
            parseAgentElevatorInfo(ele,agentLogInfo.agentElevatorInfo);
        }
        else if(node.nodeName() == "deflector_agents")
        {
            parseAgentDeflectorInfo(ele,agentLogInfo.agentDeflectorInfo);
        }
    }
}

void xmlFileOperationPrivate::parseMapNormalLineInfo(const QDomElement &element, MapBaseInfo &mapInfo,int xp,int yp)
{
    int count = element.childNodes().count();

    NormalLine temp;
    for(int i = 0 ; i < count; ++i)
    {
        const QDomNode& node = element.childNodes().at(i);
        const auto &sEle = node.toElement();

        if(node.nodeName() == "line")
        {
            int id = sEle.hasAttribute("id")?sEle.attribute("id").toInt():-1;
            temp.sx =xp + sEle.hasAttribute("start.x")?sEle.attribute("start.x").toInt():-1;
            temp.sy =yp + sEle.hasAttribute("start.y")?sEle.attribute("start.y").toInt():-1;
            temp.gx =xp + sEle.hasAttribute("goal.x")?sEle.attribute("goal.x").toInt():-1;
            temp.gy =yp + sEle.hasAttribute("goal.y")?sEle.attribute("goal.y").toInt():-1;

            temp.lineStyle.lineType = sEle.hasAttribute("linetype")?sEle.attribute("linetype").toStdString():"-";
            //QString lw = sEle.attribute("linewidth");   //不能为空
            temp.lineStyle.lineWidth = sEle.attribute("linewidth").toInt();
            temp.lineStyle.linecolor.r = sEle.hasAttribute("lineR")?sEle.attribute("lineR").toInt():-1;
            temp.lineStyle.linecolor.g = sEle.hasAttribute("lineG")?sEle.attribute("lineG").toInt():-1;
            temp.lineStyle.linecolor.b = sEle.hasAttribute("lineB")?sEle.attribute("lineB").toInt():-1;

            mapInfo.normalLineInfo.emplace_back(temp);
        }
    }
}

void xmlFileOperationPrivate::parseMapCircleInfo(const QDomElement &element, MapBaseInfo &mapInfo,int xp,int yp)
{
    int count = element.childNodes().count();

    CircleInfo temp;
    for(int i = 0 ; i < count; ++i)
    {
        const QDomNode& node = element.childNodes().at(i);
        const auto &sEle = node.toElement();

        if(node.nodeName() == "circleline")
        {
            int id = sEle.hasAttribute("id")?sEle.attribute("id").toInt():-1;
            temp.cx =xp + sEle.hasAttribute("center.x")?sEle.attribute("center.x").toInt():-1;
            temp.cy =yp + sEle.hasAttribute("center.y")?sEle.attribute("center.y").toInt():-1;
            temp.cr = sEle.hasAttribute("center.r")?sEle.attribute("center.r").toInt():-1;

            temp.lineStyle.lineType = sEle.hasAttribute("linetype")?sEle.attribute("linetype").toStdString():"-";
            //QString lw = sEle.attribute("linewidth");   //不能为空
            temp.lineStyle.lineWidth = sEle.attribute("linewidth").toInt();
            temp.lineStyle.linecolor.r = sEle.hasAttribute("lineR")?sEle.attribute("lineR").toInt():-1;
            temp.lineStyle.linecolor.g = sEle.hasAttribute("lineG")?sEle.attribute("lineG").toInt():-1;
            temp.lineStyle.linecolor.b = sEle.hasAttribute("lineB")?sEle.attribute("lineB").toInt():-1;

            mapInfo.circleInfo.emplace_back(temp);
        }
    }
}

void xmlFileOperationPrivate::parseMapShipislandInfo(const QDomElement &element, MapBaseInfo &mapInfo, int xp, int yp)
{
    int count = element.childNodes().count();

    for(int i = 0 ; i < count; ++i)
    {
        const QDomNode& node = element.childNodes().at(i);
        const auto &sEle = node.toElement();

        MapOutPolygn mInfo;
        if(node.nodeName() == "point")
        {
            int id = sEle.hasAttribute("id")?sEle.attribute("id").toInt():-1;
//            mapInfo.lineStyle.lineType = sEle.hasAttribute("linetype")?sEle.attribute("linetype").toStdString():"-";
//            //QString lw = sEle.attribute("linewidth");   //不能为空
//            mapInfo.lineStyle.lineWidth  = sEle.attribute("linewidth").toInt();
//            mapInfo.lineStyle.linecolor.r = sEle.hasAttribute("lineR")?sEle.attribute("lineR").toInt():-1;
//            mapInfo.lineStyle.linecolor.g = sEle.hasAttribute("lineG")?sEle.attribute("lineG").toInt():-1;
//            mapInfo.lineStyle.linecolor.b = sEle.hasAttribute("lineB")?sEle.attribute("lineB").toInt():-1;
//            mapInfo.fillColor.r = sEle.attribute("fillR").toInt();
//            mapInfo.fillColor.g = sEle.attribute("fillG").toInt();
//            mapInfo.fillColor.b = sEle.attribute("fillB").toInt();


//            mapInfos.inPolygnInfo.emplace_back(mapInfo);
        }
    }
}

void xmlFileOperationPrivate::parseMapDeflectorsInfo(const QDomElement &element, MapBaseInfo &mapInfo, int xp, int yp)
{
    int count = element.childNodes().count();

    for(int i = 0 ; i < count; ++i)
    {
        const QDomNode& node = element.childNodes().at(i);
        const auto &sEle = node.toElement();

        MapOutPolygn mInfo;
        if(node.nodeName() == "point")
        {
            int id = sEle.hasAttribute("id")?sEle.attribute("id").toInt():-1;
//            mapInfo.lineStyle.lineType = sEle.hasAttribute("linetype")?sEle.attribute("linetype").toStdString():"-";
//            //QString lw = sEle.attribute("linewidth");   //不能为空
//            mapInfo.lineStyle.lineWidth  = sEle.attribute("linewidth").toInt();
//            mapInfo.lineStyle.linecolor.r = sEle.hasAttribute("lineR")?sEle.attribute("lineR").toInt():-1;
//            mapInfo.lineStyle.linecolor.g = sEle.hasAttribute("lineG")?sEle.attribute("lineG").toInt():-1;
//            mapInfo.lineStyle.linecolor.b = sEle.hasAttribute("lineB")?sEle.attribute("lineB").toInt():-1;
//            mapInfo.fillColor.r = sEle.attribute("fillR").toInt();
//            mapInfo.fillColor.g = sEle.attribute("fillG").toInt();
//            mapInfo.fillColor.b = sEle.attribute("fillB").toInt();


//            mapInfos.inPolygnInfo.emplace_back(mapInfo);
        }
    }
}

void xmlFileOperationPrivate::parseMapStationInfo(const QDomElement &element, QVector<MapPosData> &mapPosInfo)
{
    int count = element.childNodes().count();

    for(int i = 0 ; i < count; ++i)
    {
        const QDomNode& node = element.childNodes().at(i);
        const auto &sEle = node.toElement();

        MapPosData temp;
        if(node.nodeName() == "station_local")
        {
            temp.id = sEle.attribute("id").toInt();
            temp.name = sEle.attribute("name");
            temp.boardstate = sEle.attribute("location");
            int x = sEle.attribute("station.x").toInt();
            int y = sEle.attribute("station.y").toInt();
            QPoint stationxy(x,y);
            temp.pos = stationxy;
            temp.heading = sEle.attribute("station.heading");
            temp.type = sEle.attribute("type");

            mapPosInfo.push_back(temp);
        }
    }

}

void xmlFileOperationPrivate::parseMapTakeoffPos(const QDomElement &element, QVector<stationLocal> &takeoffInfo)
{
    int count = element.childNodes().count();  //3
    for(int i = 0 ; i < count; ++i)
    {
        stationLocal temp;
        const QDomNode& stationLocalNode = element.childNodes().at(i);
        const auto &stationEle = stationLocalNode.toElement();
        if(stationLocalNode.nodeName() == "station_local")
        {
            temp.id = stationEle.hasAttribute("id")?stationEle.attribute("id").toInt():-1;
            temp.name = stationEle.hasAttribute("name")?stationEle.attribute("name"):"";
            temp.stationX = stationEle.hasAttribute("station.x")?stationEle.attribute("station.x").toFloat():-1;
            temp.stationY = stationEle.hasAttribute("station.y")?stationEle.attribute("station.y").toFloat():-1;
            temp.stationHeading = stationEle.hasAttribute("station.heading")?stationEle.attribute("station.heading").toFloat():-1;
            const QDomNode& sectionNode = stationLocalNode.childNodes().at(0);
            const auto &sectionEle = sectionNode.toElement();
            if(sectionEle.nodeName() == "section")
            {
                AgentSection temp1;
                temp1.tasknumber = sectionEle.hasAttribute("tasknumber")?sectionEle.attribute("tasknumber").toInt():-1;
                temp1.agentStartBoardstateType = static_cast<emBoardstate>(sectionEle.hasAttribute("start.boardstate")?sectionEle.attribute("start.boardstate").toInt():-1);
                temp1.agentStartX = sectionEle.hasAttribute("start.x")?sectionEle.attribute("start.x").toFloat():-1;
                temp1.agentStartY = sectionEle.hasAttribute("start.y")?sectionEle.attribute("start.y").toFloat():-1;
                temp1.agentStartHeading = sectionEle.hasAttribute("start.heading")?sectionEle.attribute("start.heading").toFloat():-1;
                temp1.agentGoalBoardstateType = static_cast<emBoardstate>(sectionEle.hasAttribute("goal.boardstate")?sectionEle.attribute("goal.boardstate").toInt():-1);
                temp1.agentGoalX = sectionEle.hasAttribute("goal.x")?sectionEle.attribute("goal.x").toFloat():-1;
                temp1.agentGoalY = sectionEle.hasAttribute("goal.y")?sectionEle.attribute("goal.y").toFloat():-1;
                temp1.agentGoalHeading = sectionEle.hasAttribute("goal.heading")?sectionEle.attribute("goal.heading").toFloat():-1;
                temp1.wingstate = sectionEle.hasAttribute("wingstate")?sectionEle.attribute("wingstate").toInt():-1;
                temp1.agentgDuration = sectionEle.hasAttribute("duration")?sectionEle.attribute("duration").toFloat():-1;
                const QDomNode& transportPathNode = sectionNode.childNodes().at(0);
                const auto &transportPathEle = transportPathNode.toElement();
                if(transportPathNode.nodeName() == "transport_path")
                {
                    temp1.pathfound = static_cast<bool>(transportPathEle.hasAttribute("pathfound")?transportPathEle.attribute("pathfound").toInt():false);
                    temp1.runtime = transportPathEle.hasAttribute("runtime")?transportPathEle.attribute("runtime").toFloat():-1;
                    int count1 = transportPathNode.childNodes().count();  //3
                    for(int j = 0 ; j < count1; ++j)
                    {
                        AgentTransportSection temp2;
                        const QDomNode& transportSectionNode = transportPathNode.childNodes().at(j);
                        const auto &transportSectionEle = transportSectionNode.toElement();
                        temp2.id = transportSectionEle.hasAttribute("id")?transportSectionEle.attribute("id").toInt():-1;
                        temp2.agentStartBoardstateType = static_cast<emBoardstate>(transportSectionEle.hasAttribute("start.boardstate")?transportSectionEle.attribute("start.boardstate").toInt():-1);
                        temp2.agentStartX = transportSectionEle.hasAttribute("start.x")?transportSectionEle.attribute("start.x").toFloat():-1;
                        temp2.agentStartY = transportSectionEle.hasAttribute("start.y")?transportSectionEle.attribute("start.y").toFloat():-1;
                        temp2.agentStartHeading = transportSectionEle.hasAttribute("start.heading")?transportSectionEle.attribute("start.heading").toFloat():-1;
                        temp2.agentGoalBoardstateType = static_cast<emBoardstate>(transportSectionEle.hasAttribute("goal.boardstate")?transportSectionEle.attribute("goal.boardstate").toInt():-1);
                        temp2.agentGoalX = transportSectionEle.hasAttribute("goal.x")?transportSectionEle.attribute("goal.x").toFloat():-1;
                        temp2.agentGoalY = transportSectionEle.hasAttribute("goal.y")?transportSectionEle.attribute("goal.y").toFloat():-1;
                        temp2.agentGoalHeading = transportSectionEle.hasAttribute("goal.heading")?transportSectionEle.attribute("goal.heading").toFloat():-1;
                        temp2.agentgDuration = transportSectionEle.hasAttribute("duration")?transportSectionEle.attribute("duration").toFloat():-1;
                        temp1.transportSection.append(temp2);
                    }
                }
                temp.section = temp1;
            }
        }
        takeoffInfo.append(temp);
    }
}

void xmlFileOperationPrivate::parseMapTakelandPos(const QDomElement &element, QVector<stationLocal> &takelandInfo)
{
    int count = element.childNodes().count();  //3
    for(int i = 0 ; i < count; ++i)
    {
        stationLocal temp;
        const QDomNode& stationLocalNode = element.childNodes().at(i);
        const auto &stationEle = stationLocalNode.toElement();
        if(stationLocalNode.nodeName() == "station_local")
        {
            temp.id = stationEle.hasAttribute("id")?stationEle.attribute("id").toInt():-1;
            temp.name = stationEle.hasAttribute("name")?stationEle.attribute("name"):"";
            temp.stationX = stationEle.hasAttribute("station.x")?stationEle.attribute("station.x").toFloat():-1;
            temp.stationY = stationEle.hasAttribute("station.y")?stationEle.attribute("station.y").toFloat():-1;
            temp.stationHeading = stationEle.hasAttribute("station.heading")?stationEle.attribute("station.heading").toFloat():-1;
            const QDomNode& sectionNode = stationLocalNode.childNodes().at(0);
            const auto &sectionEle = sectionNode.toElement();
            if(sectionEle.nodeName() == "section")
            {
                AgentSection temp1;
                temp1.tasknumber = sectionEle.hasAttribute("tasknumber")?sectionEle.attribute("tasknumber").toInt():-1;
                temp1.agentStartBoardstateType = static_cast<emBoardstate>(sectionEle.hasAttribute("start.boardstate")?sectionEle.attribute("start.boardstate").toInt():-1);
                temp1.agentStartX = sectionEle.hasAttribute("start.x")?sectionEle.attribute("start.x").toFloat():-1;
                temp1.agentStartY = sectionEle.hasAttribute("start.y")?sectionEle.attribute("start.y").toFloat():-1;
                temp1.agentStartHeading = sectionEle.hasAttribute("start.heading")?sectionEle.attribute("start.heading").toFloat():-1;
                temp1.agentGoalBoardstateType = static_cast<emBoardstate>(sectionEle.hasAttribute("goal.boardstate")?sectionEle.attribute("goal.boardstate").toInt():-1);
                temp1.agentGoalX = sectionEle.hasAttribute("goal.x")?sectionEle.attribute("goal.x").toFloat():-1;
                temp1.agentGoalY = sectionEle.hasAttribute("goal.y")?sectionEle.attribute("goal.y").toFloat():-1;
                temp1.agentGoalHeading = sectionEle.hasAttribute("goal.heading")?sectionEle.attribute("goal.heading").toFloat():-1;
                temp1.wingstate = sectionEle.hasAttribute("wingstate")?sectionEle.attribute("wingstate").toInt():-1;
                temp1.agentgDuration = sectionEle.hasAttribute("duration")?sectionEle.attribute("duration").toFloat():-1;
                const QDomNode& transportPathNode = sectionNode.childNodes().at(0);
                const auto &transportPathEle = transportPathNode.toElement();
                if(transportPathNode.nodeName() == "transport_path")
                {
                    temp1.pathfound = static_cast<bool>(transportPathEle.hasAttribute("pathfound")?transportPathEle.attribute("pathfound").toInt():false);
                    temp1.runtime = transportPathEle.hasAttribute("runtime")?transportPathEle.attribute("runtime").toFloat():-1;
                    int count1 = transportPathNode.childNodes().count();  //3
                    for(int j = 0 ; j < count1; ++j)
                    {
                        AgentTransportSection temp2;
                        const QDomNode& transportSectionNode = transportPathNode.childNodes().at(j);
                        const auto &transportSectionEle = transportSectionNode.toElement();
                        temp2.id = transportSectionEle.hasAttribute("id")?transportSectionEle.attribute("id").toInt():-1;
                        temp2.agentStartBoardstateType = static_cast<emBoardstate>(transportSectionEle.hasAttribute("start.boardstate")?transportSectionEle.attribute("start.boardstate").toInt():-1);
                        temp2.agentStartX = transportSectionEle.hasAttribute("start.x")?transportSectionEle.attribute("start.x").toFloat():-1;
                        temp2.agentStartY = transportSectionEle.hasAttribute("start.y")?transportSectionEle.attribute("start.y").toFloat():-1;
                        temp2.agentStartHeading = transportSectionEle.hasAttribute("start.heading")?transportSectionEle.attribute("start.heading").toFloat():-1;
                        temp2.agentGoalBoardstateType = static_cast<emBoardstate>(transportSectionEle.hasAttribute("goal.boardstate")?transportSectionEle.attribute("goal.boardstate").toInt():-1);
                        temp2.agentGoalX = transportSectionEle.hasAttribute("goal.x")?transportSectionEle.attribute("goal.x").toFloat():-1;
                        temp2.agentGoalY = transportSectionEle.hasAttribute("goal.y")?transportSectionEle.attribute("goal.y").toFloat():-1;
                        temp2.agentGoalHeading = transportSectionEle.hasAttribute("goal.heading")?transportSectionEle.attribute("goal.heading").toFloat():-1;
                        temp2.agentgDuration = transportSectionEle.hasAttribute("duration")?transportSectionEle.attribute("duration").toFloat():-1;
                        temp1.transportSection.append(temp2);
                    }
                }
                temp.section = temp1;
            }
        }
        takelandInfo.append(temp);
    }
}

void xmlFileOperationPrivate::parseMapElevatorInfo(const QDomElement &element, MapBaseInfo &mapInfo, int xp, int yp)
{

}

void xmlFileOperationPrivate::parseAgentPlaneInfo(const QDomElement &element, QVector<DBPlaneTable> &planeInfo)
{
    int count = element.childNodes().count();

    for(int i = 0 ; i < count; ++i)
    {
        const QDomNode& node = element.childNodes().at(i);
        const auto &ele = node.toElement();

        DBPlaneTable temp;
        temp.m_planeID = ele.attribute("id").toInt();
        temp.m_planeName = ele.attribute("agentname");
        temp.m_priority = ele.attribute("priority").toInt();
        temp.m_planeType = ele.attribute("plane_type");
        temp.m_size = ele.attribute("size").toFloat();
        temp.m_moveSpeed = ele.attribute("movespeed").toFloat();
        temp.m_rotationSpeed = ele.attribute("rotationspeed").toFloat();
        temp.m_tankCapacity = ele.attribute("Fuel_tank").toDouble();
        temp.m_refuelRate = ele.attribute("Refueling_speed").toDouble();
        temp.m_fuelCostRate = ele.attribute("Refueling_costspeed").toDouble();
        temp.m_minWarmUpTime = ele.attribute("Warm_up_time").toFloat();
        temp.m_alignTime = ele.attribute("Inertial_navigation_time").toFloat();
        temp.m_inflateTime = ele.attribute("Inflation_time").toFloat();
        temp.m_chargeTime = ele.attribute("Charging_time").toFloat();
        temp.m_minTurnR = ele.attribute("Min_turning_radius").toDouble();
        temp.m_GDLocal = ele.attribute("GD_local");
        temp.m_planeLength = ele.attribute("agentlength").toFloat();
        temp.m_spreadWidth = ele.attribute("agentwidth").toDouble();
        temp.m_wheelbase = ele.attribute("wheelbase").toFloat();
        temp.m_thread = ele.attribute("tread").toFloat();
        temp.m_wheelR = ele.attribute("Wheelradius").toFloat();
        temp.m_wheelWidth = ele.attribute("Wheelwidth").toFloat();
        temp.m_agentFront = ele.attribute("agentfront").toFloat();
        temp.m_agentBack = ele.attribute("agentback").toFloat();

        int pc = ele.childNodes().count();

        for(int j = 0 ; j < pc ; ++j)
        {
            const QDomNode& pnode = ele.childNodes().at(j);
            const auto &pele = pnode.toElement();
            if(pnode.nodeName() == "init_state")
            {
                temp.agentInitState.initBoardstate = static_cast<emBoardstate>(pele.attribute("init.boardstate").toInt());
                temp.agentInitState.initX = pele.attribute("init.x").toFloat();
                temp.agentInitState.initY = pele.attribute("init.y").toFloat();
                temp.agentInitState.initHeading = pele.attribute("init.heading").toFloat();
                temp.agentInitState.initOilVolume = pele.attribute("init.oil_volume").toFloat();
                temp.agentInitState.initWP = pele.attribute("init.WP").toInt() > 0;
                temp.agentInitState.initINS = pele.attribute("init.INS").toInt() > 0;
                temp.agentInitState.initINF = pele.attribute("init.INF").toInt() > 0;
                temp.agentInitState.initCHA = pele.attribute("init.CHA").toInt() > 0;
                temp.agentInitState.initFreeGD_local = pele.attribute("freeGD_local");
                temp.agentInitState.initWingstate = pele.attribute("wingstate").toInt();
            }
            else if(pnode.nodeName() == "section_set")
            {
                int pc2 = pele.childNodes().count();

                for(int k = 0 ; k < pc2 ; ++k)
                {
                    const QDomNode& snode = pele.childNodes().at(k);
                    const auto &sEle = snode.toElement();
                    if(snode.nodeName() == "section")
                    {
                        AgentSection temp2;
                        temp2.id = sEle.attribute("id").toInt();
                        temp2.tasknumber = sEle.attribute("tasknumber").toInt();
                        if(temp2.tasknumber == 2)   //GD任务
                        {
                              //这里需要下面三个类型信息格式及数据对应一致 否则会崩溃
                                QString &&GD_type = sEle.attribute("GD_type");
                                QString &&GD_eachtime = sEle.attribute("GD_eachtime");
                                QString &&GD_num = sEle.attribute("GD_use_local");

                                auto &&list1 = GD_type.split(";");
                                auto &&list2 = GD_eachtime.split(";");
                                auto &&list3 = GD_num.split(";");
                                //Q_ASSERT();
                                for(int k = 0; k < list1.size(); ++k)
                                {
                                    GDinfo t;
                                    t.agentPlaneGDType = list1.at(k).toInt();
                                    t.agentPlaneGDTime = list2.at(k).toInt();
                                    t.agentPlaneGDNum = list3.at(k).toInt();
                                    temp2.agentGDinfo.push_back(t);
                                }
                        }
                        else if(temp2.tasknumber == 1)  //加油
                        {
                            temp2.required_oil_volume = sEle.attribute("required_oil_volume").toFloat();
                        }
                        temp2.agentStartX = sEle.attribute("start.x").toFloat();
                        temp2.agentStartY = sEle.attribute("start.y").toFloat();
                        temp2.agentGoalX = sEle.attribute("goal.x").toFloat();
                        temp2.agentGoalY = sEle.attribute("goal.y").toFloat();
                        temp2.agentStartHeading = sEle.attribute("start.heading").toFloat();
                        temp2.agentGoalHeading = sEle.attribute("goal.heading").toFloat();
                        temp2.agentgDuration = sEle.attribute("duration").toFloat();
                        temp2.wingstate = sEle.attribute("wingstate").toInt();
                        temp2.agentStartTime = sEle.attribute("starttime");
                        temp2.agentStartBoardstateType = static_cast<emBoardstate>(sEle.attribute("start.boardstate").toInt());
                        temp2.agentGoalBoardstateType = static_cast<emBoardstate>(sEle.attribute("goal.boardstate").toInt());

                        //QDateTime starttime =  QDateTime::fromString(sEle.attribute("starttime"),TimeFORMAT);
                        int tsc = sEle.childNodes().count();
                        if(tsc > 0)
                        {
                            for(int m = 0; m < tsc; ++m)
                            {
                                const QDomNode& tnode = sEle.childNodes().at(m);
                                const auto &tEle = tnode.toElement();
                                if(tnode.nodeName() == "transport_path")
                                {
                                    temp2.pathfound = tEle.attribute("pathfound") == "true";
                                    temp2.runtime = tEle.attribute("runtime").toDouble();
                                    int t_sc = tEle.childNodes().count();
                                    for(int n = 0; n < t_sc; ++n)
                                    {
                                        const QDomNode& t_node = tEle.childNodes().at(n);
                                        const auto &t_Ele = t_node.toElement();
                                        if(t_node.nodeName() == "transport_section")
                                        {
                                            AgentTransportSection temp3;
                                            temp3.id = t_Ele.attribute("id").toInt();
                                            temp3.agentStartX = t_Ele.attribute("start.x").toFloat();
                                            temp3.agentStartY = t_Ele.attribute("start.y").toFloat();
                                            temp3.agentGoalX = t_Ele.attribute("goal.x").toFloat();
                                            temp3.agentGoalY = t_Ele.attribute("goal.y").toFloat();
                                            temp3.agentStartHeading = t_Ele.attribute("start.heading").toFloat();
                                            temp3.agentGoalHeading = t_Ele.attribute("goal.heading").toFloat();

                                            temp3.agentStartBoardstateType = static_cast<emBoardstate>(t_Ele.attribute("start.boardstate").toInt());
                                            temp3.agentGoalBoardstateType = static_cast<emBoardstate>(t_Ele.attribute("goal.boardstate").toInt());
                                            temp3.agentgDuration = t_Ele.attribute("duration").toFloat();
                                            temp2.transportSection.append(temp3);
                                            //temp3.agentStartTime = starttime.toString(TimeFORMAT);
                                            //starttime = starttime.addSecs(temp3.agentgDuration);
                                        }
                                    }

                                }
                            }
                        }
                        temp.agentSection.append(temp2);
                    }
                }
            }
        }
        planeInfo.push_back(temp);
    }
}

void xmlFileOperationPrivate::parseAgentQYCarInfo(const QDomElement &element, QVector<DBQYCarTable> &QYcarInfo)
{
    int count = element.childNodes().count();

    for(int i = 0 ; i < count; ++i)
    {
        const QDomNode& node = element.childNodes().at(i);
        const auto &ele = node.toElement();

        DBQYCarTable temp;
        temp.m_QYcarID = ele.attribute("id").toInt();
        temp.m_QYcarName = ele.attribute("agentname");
        temp.m_priority = ele.attribute("QY_priority").toInt();
        temp.m_QYcarType = ele.attribute("QY_type");
        temp.m_size = ele.attribute("size").toFloat();
        temp.m_moveSpeed = ele.attribute("movespeed").toFloat();
        temp.m_rotationSpeed = ele.attribute("rotationspeed").toFloat();
        temp.m_minTurnR = ele.attribute("Min_turning_radius").toDouble();
        temp.m_carLength = ele.attribute("agentlength").toFloat();
        temp.m_carWidth = ele.attribute("agentwidth").toFloat();
        temp.m_wheelbase = ele.attribute("wheelbase").toFloat();
        temp.m_thread = ele.attribute("tread").toFloat();
        temp.m_wheelR = ele.attribute("Wheelradius").toFloat();
        temp.m_wheelWidth = ele.attribute("Wheelwidth").toFloat();
        temp.m_agentFront = ele.attribute("agentfront").toFloat();
        temp.m_agentBack = ele.attribute("agentback").toFloat();

        int pc = ele.childNodes().count();

        for(int j = 0 ; j < pc ; ++j)
        {
            const QDomNode& pnode = ele.childNodes().at(j);
            const auto &pele = pnode.toElement();
            if(pnode.nodeName() == "init_state")
            {
                temp.agentInitState.initBoardstate = static_cast<emBoardstate>(pele.attribute("init.boardstate").toInt());
                temp.agentInitState.initX = pele.attribute("init.x").toFloat();
                temp.agentInitState.initY = pele.attribute("init.y").toFloat();
                temp.agentInitState.initHeading = pele.attribute("init.heading").toFloat();
                temp.agentInitState.initConnectstate = pele.attribute("connectstate").toInt() > 0;
            }
            else if(pnode.nodeName() == "section_set")
            {
                int pc2 = pele.childNodes().count();

                for(int k = 0 ; k < pc2 ; ++k)
                {
                    const QDomNode& snode = pele.childNodes().at(k);
                    const auto &sEle = snode.toElement();
                    if(snode.nodeName() == "section")
                    {
                        AgentSection temp2;
                        temp2.id = sEle.attribute("id").toInt();
                        temp2.tasknumber = sEle.attribute("tasknumber").toInt();
                        temp2.agentStartX = sEle.attribute("start.x").toFloat();
                        temp2.agentStartY = sEle.attribute("start.y").toFloat();
                        temp2.agentGoalX = sEle.attribute("goal.x").toFloat();
                        temp2.agentGoalY = sEle.attribute("goal.y").toFloat();
                        temp2.agentStartHeading = sEle.attribute("start.heading").toFloat();
                        temp2.agentGoalHeading = sEle.attribute("goal.heading").toFloat();
                        temp2.agentgDuration = sEle.attribute("duration").toFloat();
                        temp2.agentStartTime = sEle.attribute("starttime");
                        temp2.agentStartBoardstateType = static_cast<emBoardstate>(sEle.attribute("start.boardstate").toInt());
                        temp2.agentGoalBoardstateType = static_cast<emBoardstate>(sEle.attribute("goal.boardstate").toInt());
                        temp2.connectstate = sEle.attribute("connectstate").toInt();

                        //QDateTime starttime =  QDateTime::fromString(sEle.attribute("starttime"),TimeFORMAT);
                        int tsc = sEle.childNodes().count();
                        if(tsc > 0)
                        {
                            for(int m = 0; m < tsc; ++m)
                            {
                                const QDomNode& tnode = sEle.childNodes().at(m);
                                const auto &tEle = tnode.toElement();
                                if(tnode.nodeName() == "transport_path")
                                {
                                    temp2.pathfound = tEle.attribute("pathfound") == "true";
                                    temp2.runtime = tEle.attribute("runtime").toDouble();
                                    int t_sc = tEle.childNodes().count();
                                    for(int n = 0; n < t_sc; ++n)
                                    {
                                        const QDomNode& t_node = tEle.childNodes().at(n);
                                        const auto &t_Ele = t_node.toElement();
                                        if(t_node.nodeName() == "transport_section")
                                        {
                                            AgentTransportSection temp3;
                                            temp3.id = t_Ele.attribute("id").toInt();
                                            temp3.agentStartX = t_Ele.attribute("start.x").toFloat();
                                            temp3.agentStartY = t_Ele.attribute("start.y").toFloat();
                                            temp3.agentGoalX = t_Ele.attribute("goal.x").toFloat();
                                            temp3.agentGoalY = t_Ele.attribute("goal.y").toFloat();
                                            temp3.agentStartHeading = t_Ele.attribute("start.heading").toFloat();
                                            temp3.agentGoalHeading = t_Ele.attribute("goal.heading").toFloat();

                                            temp3.agentStartBoardstateType = static_cast<emBoardstate>(t_Ele.attribute("start.boardstate").toInt());
                                            temp3.agentGoalBoardstateType = static_cast<emBoardstate>(t_Ele.attribute("goal.boardstate").toInt());
                                            temp3.agentgDuration = t_Ele.attribute("duration").toFloat();
                                            temp2.transportSection.append(temp3);
                                            //temp3.agentStartTime = starttime.toString(TimeFORMAT);
                                            //starttime = starttime.addSecs(temp3.agentgDuration);
                                        }
                                    }

                                }
                            }
                        }
                        temp.agentSection.append(temp2);
                    }
                }
            }
        }
        QYcarInfo.append(temp);
    }
}

void xmlFileOperationPrivate::parseAgentGDCarInfo(const QDomElement &element, QVector<DBGDCarTable> &GDcarInfo)
{
    int count = element.childNodes().count();

    for(int i = 0 ; i < count; ++i)
    {
        const QDomNode& node = element.childNodes().at(i);
        const auto &ele = node.toElement();

        DBGDCarTable temp;
        temp.m_GDcarID = ele.attribute("id").toInt();
        temp.m_GDcarName = ele.attribute("agentname");
        temp.m_priority = ele.attribute("GD_priority").toInt();
        temp.m_GDcarType = ele.attribute("GD_type");
        temp.m_size = ele.attribute("size").toFloat();
        temp.m_moveSpeed = ele.attribute("movespeed").toFloat();
        temp.m_rotationSpeed = ele.attribute("rotationspeed").toFloat();
        temp.m_minTurnR = ele.attribute("Min_turning_radius").toDouble();
        temp.m_carLength = ele.attribute("agentlength").toFloat();
        temp.m_carWidth = ele.attribute("agentwidth").toFloat();
        temp.m_wheelbase = ele.attribute("wheelbase").toFloat();
        temp.m_thread = ele.attribute("tread").toFloat();
        temp.m_wheelR = ele.attribute("Wheelradius").toFloat();
        temp.m_wheelWidth = ele.attribute("Wheelwidth").toFloat();
        temp.m_agentFront = ele.attribute("agentfront").toFloat();
        temp.m_agentBack = ele.attribute("agentback").toFloat();

        int pc = ele.childNodes().count();

        for(int j = 0 ; j < pc ; ++j)
        {
            const QDomNode& pnode = ele.childNodes().at(j);
            const auto &pele = pnode.toElement();
            if(pnode.nodeName() == "init_state")
            {
                temp.agentInitState.initBoardstate = static_cast<emBoardstate>(pele.attribute("init.boardstate").toInt());
                temp.agentInitState.initX = pele.attribute("init.x").toFloat();
                temp.agentInitState.initY = pele.attribute("init.y").toFloat();
                temp.agentInitState.initHeading = pele.attribute("init.heading").toFloat();
                temp.agentInitState.initLoadstate = pele.attribute("loadstate").toInt();
            }
            else if(pnode.nodeName() == "section_set")
            {
                int pc2 = pele.childNodes().count();

                for(int k = 0 ; k < pc2 ; ++k)
                {
                    const QDomNode& snode = pele.childNodes().at(k);
                    const auto &sEle = snode.toElement();
                    if(snode.nodeName() == "section")
                    {
                        AgentSection temp2;
                        temp2.id = sEle.attribute("id").toInt();
                        temp2.tasknumber = sEle.attribute("tasknumber").toInt();
                        temp2.agentStartX = sEle.attribute("start.x").toFloat();
                        temp2.agentStartY = sEle.attribute("start.y").toFloat();
                        temp2.agentGoalX = sEle.attribute("goal.x").toFloat();
                        temp2.agentGoalY = sEle.attribute("goal.y").toFloat();
                        temp2.agentStartHeading = sEle.attribute("start.heading").toFloat();
                        temp2.agentGoalHeading = sEle.attribute("goal.heading").toFloat();
                        temp2.agentgDuration = sEle.attribute("duration").toFloat();
                        temp2.agentStartTime = sEle.attribute("starttime");
                        temp2.agentStartBoardstateType = static_cast<emBoardstate>(sEle.attribute("start.boardstate").toInt());
                        temp2.agentGoalBoardstateType = static_cast<emBoardstate>(sEle.attribute("goal.boardstate").toInt());
                        //temp2.connectstate = sEle.attribute("connectstate").toInt();

                        //QDateTime starttime =  QDateTime::fromString(sEle.attribute("starttime"),TimeFORMAT);
                        int tsc = sEle.childNodes().count();
                        if(tsc > 0)
                        {
                            for(int m = 0; m < tsc; ++m)
                            {
                                const QDomNode& tnode = sEle.childNodes().at(m);
                                const auto &tEle = tnode.toElement();
                                if(tnode.nodeName() == "transport_path")
                                {
                                    temp2.pathfound = tEle.attribute("pathfound") == "true";
                                    temp2.runtime = tEle.attribute("runtime").toDouble();
                                    int t_sc = tEle.childNodes().count();
                                    for(int n = 0; n < t_sc; ++n)
                                    {
                                        const QDomNode& t_node = tEle.childNodes().at(n);
                                        const auto &t_Ele = t_node.toElement();
                                        if(t_node.nodeName() == "transport_section")
                                        {
                                            AgentTransportSection temp3;
                                            temp3.id = t_Ele.attribute("id").toInt();
                                            temp3.agentStartX = t_Ele.attribute("start.x").toFloat();
                                            temp3.agentStartY = t_Ele.attribute("start.y").toFloat();
                                            temp3.agentGoalX = t_Ele.attribute("goal.x").toFloat();
                                            temp3.agentGoalY = t_Ele.attribute("goal.y").toFloat();
                                            temp3.agentStartHeading = t_Ele.attribute("start.heading").toFloat();
                                            temp3.agentGoalHeading = t_Ele.attribute("goal.heading").toFloat();

                                            temp3.agentStartBoardstateType = static_cast<emBoardstate>(t_Ele.attribute("start.boardstate").toInt());
                                            temp3.agentGoalBoardstateType = static_cast<emBoardstate>(t_Ele.attribute("goal.boardstate").toInt());
                                            temp3.agentgDuration = t_Ele.attribute("duration").toFloat();
                                            temp2.transportSection.append(temp3);
                                            //temp3.agentStartTime = starttime.toString(TimeFORMAT);
                                            //starttime = starttime.addSecs(temp3.agentgDuration);
                                        }
                                    }

                                }
                            }
                        }
                        temp.agentSection.append(temp2);
                    }
                }
            }
        }
        GDcarInfo.append(temp);
    }
}

void xmlFileOperationPrivate::parseAgentOperatorInfo(const QDomElement &element, QVector<DBPersonTable> &OperatorInfo)
{
    int count = element.childNodes().count();

    for(int i = 0 ; i < count; ++i)
    {
        const QDomNode& node = element.childNodes().at(i);
        const auto &ele = node.toElement();

        DBPersonTable temp;
        temp.m_personID = ele.attribute("id").toInt();
        temp.m_personName = ele.attribute("agentname");
        temp.m_priority = ele.attribute("GD_priority").toInt();
        temp.m_personType = ele.attribute("operator_type");
        temp.m_graphicsR = ele.attribute("size").toDouble();
        temp.m_moveSpeed = ele.attribute("movespeed").toFloat();
        temp.m_rotationSpeed = ele.attribute("rotationspeed").toFloat();

        int pc = ele.childNodes().count();

        for(int j = 0 ; j < pc ; ++j)
        {
            const QDomNode& pnode = ele.childNodes().at(j);
            const auto &pele = pnode.toElement();
            if(pnode.nodeName() == "init_state")
            {
                temp.agentInitState.initBoardstate = static_cast<emBoardstate>(pele.attribute("init.boardstate").toInt());
                temp.agentInitState.initX = pele.attribute("init.x").toFloat();
                temp.agentInitState.initY = pele.attribute("init.y").toFloat();
                temp.agentInitState.initHeading = pele.attribute("init.heading").toFloat();
                temp.agentInitState.initWorkstate = pele.attribute("workstate").toInt() > 0;
            }
            else if(pnode.nodeName() == "section_set")
            {
                int pc2 = pele.childNodes().count();

                for(int k = 0 ; k < pc2 ; ++k)
                {
                    const QDomNode& snode = pele.childNodes().at(k);
                    const auto &sEle = snode.toElement();
                    if(snode.nodeName() == "section")
                    {
                        AgentSection temp2;
                        temp2.id = sEle.attribute("id").toInt();
                        temp2.tasknumber = sEle.attribute("tasknumber").toInt();
                        temp2.agentStartX = sEle.attribute("start.x").toFloat();
                        temp2.agentStartY = sEle.attribute("start.y").toFloat();
                        temp2.agentGoalX = sEle.attribute("goal.x").toFloat();
                        temp2.agentGoalY = sEle.attribute("goal.y").toFloat();
                        temp2.agentStartHeading = sEle.attribute("start.heading").toFloat();
                        temp2.agentGoalHeading = sEle.attribute("goal.heading").toFloat();
                        temp2.agentgDuration = sEle.attribute("duration").toFloat();
                        temp2.agentStartTime = sEle.attribute("starttime");
                        temp2.agentStartBoardstateType = static_cast<emBoardstate>(sEle.attribute("start.boardstate").toInt());
                        temp2.agentGoalBoardstateType = static_cast<emBoardstate>(sEle.attribute("goal.boardstate").toInt());
                        temp2.workstate = sEle.attribute("workstate").toInt() > 0;

                        //QDateTime starttime =  QDateTime::fromString(sEle.attribute("starttime"),TimeFORMAT);
                        int tsc = sEle.childNodes().count();
                        if(tsc > 0)
                        {
                            for(int m = 0; m < tsc; ++m)
                            {
                                const QDomNode& tnode = sEle.childNodes().at(m);
                                const auto &tEle = tnode.toElement();
                                if(tnode.nodeName() == "transport_path")
                                {
                                    temp2.pathfound = tEle.attribute("pathfound") == "true";
                                    temp2.runtime = tEle.attribute("runtime").toDouble();
                                    int t_sc = tEle.childNodes().count();
                                    for(int n = 0; n < t_sc; ++n)
                                    {
                                        const QDomNode& t_node = tEle.childNodes().at(n);
                                        const auto &t_Ele = t_node.toElement();
                                        if(t_node.nodeName() == "transport_section")
                                        {
                                            AgentTransportSection temp3;
                                            temp3.id = t_Ele.attribute("id").toInt();
                                            temp3.agentStartX = t_Ele.attribute("start.x").toFloat();
                                            temp3.agentStartY = t_Ele.attribute("start.y").toFloat();
                                            temp3.agentGoalX = t_Ele.attribute("goal.x").toFloat();
                                            temp3.agentGoalY = t_Ele.attribute("goal.y").toFloat();
                                            temp3.agentStartHeading = t_Ele.attribute("start.heading").toFloat();
                                            temp3.agentGoalHeading = t_Ele.attribute("goal.heading").toFloat();

                                            temp3.agentStartBoardstateType = static_cast<emBoardstate>(t_Ele.attribute("start.boardstate").toInt());
                                            temp3.agentGoalBoardstateType = static_cast<emBoardstate>(t_Ele.attribute("goal.boardstate").toInt());
                                            temp3.agentgDuration = t_Ele.attribute("duration").toFloat();
                                            temp2.transportSection.append(temp3);
                                            //temp3.agentStartTime = starttime.toString(TimeFORMAT);
                                            //starttime = starttime.addSecs(temp3.agentgDuration);
                                        }
                                    }

                                }
                            }
                        }
                        temp.agentSection.append(temp2);
                    }
                }
            }
        }
        OperatorInfo.append(temp);
    }
}

void xmlFileOperationPrivate::parseAgentElevatorInfo(const QDomElement &element, QVector<ElevatorParameter> &ElevatorInfo)
{
    int count = element.childNodes().count();

    for(int i = 0 ; i < count; ++i)
    {
        const QDomNode& node = element.childNodes().at(i);
        const auto &ele = node.toElement();

        ElevatorParameter temp;
        temp.id = ele.attribute("id").toInt();
        temp.agentname = ele.attribute("agentname");
        temp.ele_priority = ele.attribute("ele_priority").toInt();
        temp.ele_location = ele.attribute("ele_location").toInt();
        temp.movespeed = ele.attribute("movespeed").toFloat();

        int pc = ele.childNodes().count();

        for(int j = 0 ; j < pc ; ++j)
        {
            const QDomNode& pnode = ele.childNodes().at(j);
            const auto &pele = pnode.toElement();
            if(pnode.nodeName() == "init_state")
            {
                temp.initBoardstate = static_cast<emBoardstate>(pele.attribute("init.boardstate").toInt());
            }
            else if(pnode.nodeName() == "section_set")
            {
                int pc2 = pele.childNodes().count();

                for(int k = 0 ; k < pc2 ; ++k)
                {
                    const QDomNode& snode = pele.childNodes().at(k);
                    const auto &sEle = snode.toElement();
                    if(snode.nodeName() == "section")
                    {
                        ElevatorSection temp2;
                        temp2.id = sEle.attribute("id").toInt();
                        temp2.taskNumber = sEle.attribute("tasknumber").toInt();
                        temp2.duration = sEle.attribute("duration").toFloat();
                        temp2.startTime = sEle.attribute("starttime");
                        temp2.startBoardstate = static_cast<emBoardstate>(sEle.attribute("start.boardstate").toInt());
                        temp2.GoalBoardstate = static_cast<emBoardstate>(sEle.attribute("goal.boardstate").toInt());
                        temp.elevatorSection.append(temp2);
                    }
                }
            }
        }
        ElevatorInfo.append(temp);
    }
}

void xmlFileOperationPrivate::parseAgentDeflectorInfo(const QDomElement &element, QVector<DeflectorParameter> &DeflectorInfo)
{
    int count = element.childNodes().count();

    for(int i = 0 ; i < count; ++i)
    {
        const QDomNode& node = element.childNodes().at(i);
        const auto &ele = node.toElement();

        DeflectorParameter temp;
        temp.id = ele.attribute("id").toInt();
        temp.agentname = ele.attribute("agentname");
        temp.def_priority = ele.attribute("def_priority").toInt();
        temp.def_location = ele.attribute("def_location").toInt();
        temp.rotationspeed = ele.attribute("rotationspeed").toFloat();

        int pc = ele.childNodes().count();

        for(int j = 0 ; j < pc ; ++j)
        {
            const QDomNode& pnode = ele.childNodes().at(j);
            const auto &pele = pnode.toElement();
            if(pnode.nodeName() == "init_state")
            {
                temp.initLocationstate = static_cast<emBoardstate>(pele.attribute("init.locationstate").toInt());
            }
            else if(pnode.nodeName() == "section_set")
            {
                int pc2 = pele.childNodes().count();

                for(int k = 0 ; k < pc2 ; ++k)
                {
                    const QDomNode& snode = pele.childNodes().at(k);
                    const auto &sEle = snode.toElement();
                    if(snode.nodeName() == "section")
                    {
                        DeflectorSection temp2;
                        temp2.id = sEle.attribute("id").toInt();
                        temp2.taskNumber = sEle.attribute("tasknumber").toInt();
                        temp2.duration = sEle.attribute("duration").toFloat();
                        temp2.startTime = sEle.attribute("starttime");
                        temp2.startLocationstate = static_cast<emBoardstate>(sEle.attribute("start.locationstate").toInt());
                        temp2.GoalLocationstate = static_cast<emBoardstate>(sEle.attribute("goal.locationstate").toInt());
                        temp.deflectorSection.append(temp2);
                    }
                }
            }
        }
        DeflectorInfo.append(temp);
    }
}

void xmlFileOperationPrivate::parseMapInPolygnInfo(const QDomElement &element, MapBaseInfo &mapInfos,int xp,int yp)
{
    int count = element.childNodes().count();

    for(int i = 0 ; i < count; ++i)
    {
        const QDomNode& node = element.childNodes().at(i);
        const auto &sEle = node.toElement();

        MapOutPolygn mapInfo;
        if(node.nodeName() == "rec")
        {
            int id = sEle.hasAttribute("id")?sEle.attribute("id").toInt():-1;
            mapInfo.lineStyle.lineType = sEle.hasAttribute("linetype")?sEle.attribute("linetype").toStdString():"-";
            //QString lw = sEle.attribute("linewidth");   //不能为空
            mapInfo.lineStyle.lineWidth  = sEle.attribute("linewidth").toInt();
            mapInfo.lineStyle.linecolor.r = sEle.hasAttribute("lineR")?sEle.attribute("lineR").toInt():-1;
            mapInfo.lineStyle.linecolor.g = sEle.hasAttribute("lineG")?sEle.attribute("lineG").toInt():-1;
            mapInfo.lineStyle.linecolor.b = sEle.hasAttribute("lineB")?sEle.attribute("lineB").toInt():-1;
            mapInfo.fillColor.r = sEle.attribute("fillR").toInt();
            mapInfo.fillColor.g = sEle.attribute("fillG").toInt();
            mapInfo.fillColor.b = sEle.attribute("fillB").toInt();
            mapInfo.polygnName = sEle.hasAttribute("name")?sEle.attribute("name") : "";
            mapInfo.polygonType = sEle.hasAttribute("type")?sEle.attribute("type") : "";

            int count2 = sEle.childNodes().count();
            std::pair<int,int> temp;
            for(int i = 0 ; i < count2; ++i)
            {
                const QDomNode& node2 = sEle.childNodes().at(i);
                const auto &sEle2 = node2.toElement();

                if(node2.nodeName() == "point")
                {
                    int id = sEle2.hasAttribute("id")?sEle2.attribute("id").toInt():-1;
                    temp.first =xp + sEle2.hasAttribute("x")?sEle2.attribute("x").toInt():-1;
                    temp.second =yp + sEle2.hasAttribute("y")?sEle2.attribute("y").toInt():-1;
                    mapInfo.outPolygnMap[id] = temp;
                }
            }
            mapInfos.inPolygnInfo.emplace_back(mapInfo);
        }
    }
}

void xmlFileOperationPrivate::parseMapOutPolygnInfo(const QDomElement &element, MapOutPolygn &mapInfo,int xp,int yp)
{
    mapInfo.fillColor.r = element.attribute("fillR").toInt();
    mapInfo.fillColor.g = element.attribute("fillG").toInt();
    mapInfo.fillColor.b = element.attribute("fillB").toInt();
    mapInfo.lineStyle.lineType = element.attribute("linetype").toStdString();

    //QString lw = element.attribute("linewidth");   //不能为空
    mapInfo.lineStyle.lineWidth  = element.attribute("linewidth").toInt();
    mapInfo.lineStyle.linecolor.r = element.attribute("lineR").toInt();
    mapInfo.lineStyle.linecolor.g = element.attribute("lineG").toInt();
    mapInfo.lineStyle.linecolor.b = element.attribute("lineB").toInt();

    int count = element.childNodes().count();

    std::pair<int,int> temp;
    for(int i = 0 ; i < count; ++i)
    {
        const QDomNode& node = element.childNodes().at(i);
        const auto &sEle = node.toElement();

        if(node.nodeName() == "point")
        {
            int id = sEle.hasAttribute("id")?sEle.attribute("id").toInt():-1;
            temp.first =xp + sEle.attribute("x").toInt();
            temp.second =yp + sEle.attribute("y").toInt();
            mapInfo.outPolygnMap[id] = temp;
        }
    }
}

void xmlFileOperationPrivate::parseMapBaseInfo(const QDomElement &element, MapInfo &mapInfo)
{

    QString mapName = element.attribute("name");
    int mapX_bias = element.attribute("X_bias").toInt();
    int mapY_bias = element.attribute("Y_bias").toInt();
    int mapX_table = element.attribute("table_O_pointX").toInt();
    int mapY_table = element.attribute("table_O_pointY").toInt();
    int mapRealsize = element.attribute("realsize").toInt();

    if(mapName == "JB")   //甲板地图
    {
        mapInfo.hasBoardMap = true;
        mapInfo.BoardMap.mapName = mapName;
        mapInfo.BoardMap.X_bias = mapX_bias;
        mapInfo.BoardMap.Y_bias = mapY_bias;
        mapInfo.BoardMap.tableX = mapX_table;
        mapInfo.BoardMap.tableY = mapY_table;
        mapInfo.BoardMap.realsize = mapRealsize;

        int count = element.childNodes().count();
        for(int i = 0 ; i < count; ++i)
        {
            const QDomNode& node = element.childNodes().at(i);
            const auto &ele = node.toElement();
            if(node.nodeName() == "grid")
            {
                mapInfo.BoardMap.columnCount = ele.hasAttribute("width")?ele.attribute("width").toInt():0;
                mapInfo.BoardMap.rowCount = ele.hasAttribute("height")?ele.attribute("height").toInt():0;
                mapInfo.BoardMap.gridSize = ele.hasAttribute("gridSize")?ele.attribute("gridSize").toInt():1;
            }
            else if(node.nodeName() == "bottomXY")     //地图轮廓信息
            {
                parseMapOutPolygnInfo(ele,mapInfo.BoardMap.outPolygnInfo,mapX_bias,mapY_bias);
            }
            else if(node.nodeName() == "linespace")     //地图里线段信息
            {
                parseMapNormalLineInfo(ele,mapInfo.BoardMap,mapX_bias,mapY_bias);
            }
            else if(node.nodeName() == "rectangle")     //地图里矩形信息
            {
                parseMapInPolygnInfo(ele,mapInfo.BoardMap,mapX_bias,mapY_bias);
            }
            else if(node.nodeName() == "circlelinespace")     //地图里圆信息
            {
                parseMapCircleInfo(ele,mapInfo.BoardMap,mapX_bias,mapY_bias);
            }
            else if(node.nodeName() == "island")     //地图里岛屿信息
            {
                parseMapShipislandInfo(ele,mapInfo.BoardMap,mapX_bias,mapY_bias);
            }
            else if(node.nodeName() == "deflectors_set")     //地图里偏流板信息
            {
                parseMapDeflectorsInfo(ele,mapInfo.BoardMap,mapX_bias,mapY_bias);
            }
            else if(node.nodeName() == "elevator_set")     //地图里升降机信息
            {
                parseMapElevatorInfo(ele,mapInfo.BoardMap,mapX_bias,mapY_bias);
            }
            else if(node.nodeName() == "station")     //地图里站位信息
            {
                parseMapStationInfo(ele,mapInfo.BoardMapStation);
            }
            else if(node.nodeName() == "takeoffPos")
            {
                parseMapTakeoffPos(ele,mapInfo.TakeoffPos);
            }
            else if(node.nodeName() == "takelandPos")
            {
                parseMapTakeoffPos(ele,mapInfo.TakelandPos);
            }
        }

    }
    else if(mapName == "JK")  //机库地图
    {
        mapInfo.hasHangarMap = true;

        mapInfo.HangarMap.mapName = mapName;
        mapInfo.HangarMap.X_bias = mapX_bias;
        mapInfo.HangarMap.Y_bias = mapY_bias;
        mapInfo.HangarMap.tableX = mapX_table;
        mapInfo.HangarMap.tableY = mapY_table;
        mapInfo.HangarMap.realsize = mapRealsize;

        int count = element.childNodes().count();
        for(int i = 0 ; i < count; ++i)
        {
            const QDomNode& node = element.childNodes().at(i);
            const auto &ele = node.toElement();
            if(node.nodeName() == "grid")
            {
                mapInfo.HangarMap.columnCount = ele.hasAttribute("width")?ele.attribute("width").toInt():0;
                mapInfo.HangarMap.rowCount = ele.hasAttribute("height")?ele.attribute("height").toInt():0;
                mapInfo.HangarMap.gridSize = ele.hasAttribute("gridSize")?ele.attribute("gridSize").toInt():1;
            }
            else if(node.nodeName() == "bottomXY")     //地图轮廓信息
            {
                parseMapOutPolygnInfo(ele,mapInfo.HangarMap.outPolygnInfo,mapX_bias,mapY_bias);
            }
            else if(node.nodeName() == "linespace")     //地图里线段信息
            {
                parseMapNormalLineInfo(ele,mapInfo.HangarMap,mapX_bias,mapY_bias);
            }
            else if(node.nodeName() == "rectangle")     //地图里矩形信息
            {
                parseMapInPolygnInfo(ele,mapInfo.HangarMap,mapX_bias,mapY_bias);
            }
            else if(node.nodeName() == "circlelinespace")     //地图里圆信息
            {
                parseMapCircleInfo(ele,mapInfo.HangarMap,mapX_bias,mapY_bias);
            }
        }
    }

#if 0   //  暂时不需要读取地图详细障碍物信息
        int _count = ele.childNodes().count();
        assert(mapInfo.rowCount == _count);
        for(int i = 0 ; i < _count ; ++i)
        {
            const QDomNode& rowNode = ele.childNodes().at(i);
            const auto& rEle = rowNode.toElement();
            if(rEle.tagName() == "row")
            {
                const auto& list = rEle.text().split(" ",QString::SplitBehavior::SkipEmptyParts);
                if(list.size() == mapInfo.columnCount)
                {
                    for(int c = 0 ; c < list.size() ; ++c)
                    {
                        std::pair<int,int> pos{c,i};
                        if(list.at(c) == "1")
                        {
                            mapInfo.barrierVec.emplace_back(pos);
                        }
                        else
                        {
                            mapInfo.nonbarrierVec.emplace_back(pos);
                        }
                    }
                }
            }
        }
#endif
}
