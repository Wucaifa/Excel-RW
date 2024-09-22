#include "excelread.h"

ExcelRead::ExcelRead()
{

}

void ExcelRead::readExcel(const QString &filePath) {
    // 检查文件是否存在
    if (!QFile::exists(filePath)) {
        qWarning() << "File does not exist:" << filePath;
        return;
    }
    QXlsx::Document xlsx(filePath);  // 直接在构造函数中加载文件
    if (xlsx.dimension().isValid()) {  // 检查文档是否成功加载
        int rowCount = xlsx.dimension().rowCount();
        int colCount = xlsx.dimension().columnCount();

        //taskInfo,第一行
        for (int col = 1; col <= colCount; ++col) {
            QXlsx::Cell *cell = xlsx.cellAt(1, col);
            if (cell && !cell->value().isNull()) {
                TaskInfo temp;
                temp.col = col;
                temp.taskID = cell->value().toString();
                temp.taskStartTime = {};
                taskInfo.append(temp);
            }
        }

        //starttime，第二行
        for (int col = 1; col <= colCount; ++col) {
            QXlsx::Cell *cell = xlsx.cellAt(2, col);
            if (cell && !cell->value().isNull()) {
                StartTime temp;
                temp.col = col;
                temp.starttime = cell->value().toString();
                starttime.append(temp);
            }
        }

        //补全taskInfo信息
        int taskInfoSize = taskInfo.size();
        for(int i = 0; i < (taskInfoSize - 1); ++i){
            int left = i, right = i + 1;
            for(auto time:starttime){
                if(time.col > taskInfo[left].col && time.col < taskInfo[right].col){
                    taskInfo[i].taskStartTime.push_back(time);
                }
            }
        }
        for(auto time:starttime){
            if(time.col > taskInfo[taskInfoSize - 1].col){
                taskInfo[taskInfoSize - 1].taskStartTime.push_back(time);
            }
        }

        //position
        for (int row = 2; row <= rowCount; ++row) {
            QVector<Position> temp;
            for (int col = 1; col <= colCount; ++col) {
                QXlsx::Cell *cell = xlsx.cellAt(row, col);
                if (cell && !cell->value().isNull()) {
                    Position temp1;
                    temp1.col = col;
                    temp1.position = cell->value().toString();
                    temp.append(temp1);
                    qDebug() << "Cell (" << row << "," << col << "):" << cell->value();
                }
            }
            position.append(temp);
        }

        //planeInfo
        for(auto& pos:position){
            PlaneExcel temp;
            temp.id = pos[0].position;
            int taskIndex = 0;
//            int startTimeIndex = 0;
            int posIndex = 1;
            while(taskIndex < taskInfo.size() && posIndex + 1 < pos.size()){
                if (taskInfo[taskIndex].taskStartTime.size() > 1){
                    for(auto time:taskInfo[taskIndex].taskStartTime){
                        if(time.col == pos[posIndex + 1].col){
                            QDateTime startDateTime = QDateTime::fromString(time.starttime, TimeFORMAT);
                            path path;
                            path.startTime = startDateTime.toString(TimeFORMAT);
                            path.startPosition = pos[posIndex].position;
                            path.goalPosition = pos[posIndex + 1].position;
                            temp.transSections.append(path);
                            break;
                        }
                    }
                }else if(taskInfo[taskIndex].taskStartTime.size() == 1){
                    qint64 timeInterval = static_cast<qint64>(20 * 1000);//20s间隔
                    QDateTime startDateTime = QDateTime::fromString(taskInfo[taskIndex].taskStartTime[0].starttime, TimeFORMAT);
                    timeInterval = timeInterval * (pos[posIndex + 1].col - taskInfo[taskIndex].taskStartTime[0].col);
                    startDateTime = startDateTime.addMSecs(timeInterval);
                    path path;
                    path.startTime = startDateTime.toString(TimeFORMAT);
                    path.startPosition = pos[posIndex].position;
                    path.goalPosition = pos[posIndex + 1].position;
                    temp.transSections.append(path);
                }
                taskIndex++;
                posIndex += 2;
            }
//            while(startTimeIndex < starttime.size() && posIndex + 1 < pos.size()){
//                qint64 timeInterval = static_cast<qint64>(20 * 1000);//20s间隔
//                QDateTime startDateTime = QDateTime::fromString(starttime[startTimeIndex].starttime, TimeFORMAT);
//                timeInterval = timeInterval * (pos[posIndex + 1].col - starttime[startTimeIndex].col);
//                startDateTime = startDateTime.addMSecs(timeInterval);
//                path path;
//                path.startTime = startDateTime.toString(TimeFORMAT);
//                path.startPosition = pos[posIndex].position;
//                path.goalPosition = pos[posIndex + 1].position;
//                temp.transSections.append(path);
//                startTimeIndex++;
//                posIndex += 2;
//            }
            planeInfo.append(temp);
        }

        qDebug()<< "信息："<<planeInfo.size();
    } else {
        qWarning() << "Failed to load Excel file!";
    }
}

void ExcelRead::writeExcel(){
    QXlsx::Document xlsx;
    xlsx.write("A1", "1");
    xlsx.save();
}

void ExcelRead::matchPath(){

    for(auto& plane:planeInfo){
        PlaneTransection planetran;
        planetran.id = plane.id;
        for(auto& tranPath:plane.transSections){
            AgentSection section;
            m_currTranPath = QApplication::applicationDirPath() + "/path/" +
                    tranPath.startPosition + "-" +tranPath.goalPosition + ".xml";
            //QDomDocument 用于解析xml
            qDebug()<< m_currTranPath;
            QDomDocument doc;
            QFile file(m_currTranPath);
            QString error = "";
            int row = 0, column = 0;
            if (!file.open(QIODevice::ReadOnly)) {
                qDebug()<<"转运xml未打开";
                return;
            }

            if(!doc.setContent(&file, false, &error, &row, &column)){
                qDebug() << "parse file failed:" << row << "---" << column <<":" <<error;
                file.close();
                return;
            }

            QDomElement root = doc.documentElement();
            QDomNode node = root.firstChild();    //得到COMMAND标签

            while(!node.isNull()) {
                QDomElement sEle = node.toElement(); // try to convert the node to an element.
                if(sEle.nodeName() == "section"){
                    section.id = sEle.attribute("id").toInt();
                    section.tasknumber = sEle.attribute("tasknumber").toInt();
                    section.agentStartX = sEle.attribute("start.x").toFloat();
                    section.agentStartY = sEle.attribute("start.y").toFloat();
                    section.agentGoalX = sEle.attribute("goal.x").toFloat();
                    section.agentGoalY = sEle.attribute("goal.y").toFloat();
                    section.agentStartHeading = sEle.attribute("start.heading").toFloat();
                    section.agentGoalHeading = sEle.attribute("goal.heading").toFloat();
                    section.agentgDuration = sEle.attribute("duration").toFloat();
                    section.wingstate = sEle.attribute("wingstate").toInt();
                    section.agentStartTime = tranPath.startTime;
                    section.agentStartBoardstateType = static_cast<emBoardstate>(sEle.attribute("start.boardstate").toInt());
                    section.agentGoalBoardstateType = static_cast<emBoardstate>(sEle.attribute("goal.boardstate").toInt());
                }

                int tsc = sEle.childNodes().count();
                if(tsc > 0)
                {
                    for(int m = 0; m < tsc; ++m)
                    {
                        const QDomNode& tnode = sEle.childNodes().at(m);
                        const auto &tEle = tnode.toElement();
                        if(tnode.nodeName() == "transport_path")
                        {
                            section.pathfound = tEle.attribute("pathfound") == "true";
                            section.runtime = tEle.attribute("runtime").toDouble();
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
                                    section.transportSection.append(temp3);
                                    //temp3.agentStartTime = starttime.toString(TimeFORMAT);
                                    //starttime = starttime.addSecs(temp3.agentgDuration);
                                }
                            }

                        }
                    }
                }
                node = node.nextSibling();
                planetran.tranSection.append(section);
            }
        }
        planeTranInfo.append(planetran);
    }

    //整合进m_logInfo里
    for(auto& plane:planeTranInfo){
        for(auto& planeset:m_logInfo.agentPlaneInfo){
            if(planeset.m_planeName == plane.id){
                for(auto& section:plane.tranSection){
                    planeset.agentSection.append(section);
                }
            }
        }
    }
}

void ExcelRead::readXml(){
    xmlFileOperation::getInstance()->readLogXmlFile(m_logInfo,m_currAgentPath);   //  读智能体数据
}

void ExcelRead::writeXml(){
    xmlFileOperation::getInstance()->setCurrentFile(m_currAgentPath);
    xmlFileOperation::getInstance()->rewriteXmlFile4Plane(m_logInfo.agentPlaneInfo);
}

void ExcelRead::agentpathMatch(const QString &filePath){
    m_currAgentPath = filePath;
}

void ExcelRead::xmlRewrite_startTime(){
    for(auto& plane:m_logInfo.agentPlaneInfo){
        int planesSize = m_logInfo.agentOperInfo.size();
        int sectionIndex = 0;
        for(auto& section:plane.agentSection){
            //sectionID排序
            section.id = sectionIndex;
            //6:起飞，修改前第一条等待的starttime,修改前第二条转运的starttime
            if((section.tasknumber == 6 && sectionIndex > 1) &&
                    (plane.agentSection[sectionIndex - 1].tasknumber == 12 &&
                     plane.agentSection[sectionIndex - 2].tasknumber == 3)){
                QDateTime startDateTime = QDateTime::fromString(section.agentStartTime, TimeFORMAT);
                //1
                qint64 timeInterval = static_cast<qint64>(plane.agentSection[sectionIndex - 1].agentgDuration * 1000);
                startDateTime = startDateTime.addMSecs(-timeInterval);
                plane.agentSection[sectionIndex - 1].agentStartTime = startDateTime.toString(TimeFORMAT);
                //2
                timeInterval = static_cast<qint64>(plane.agentSection[sectionIndex - 2].agentgDuration * 1000);
                startDateTime = startDateTime.addMSecs(-timeInterval);
                plane.agentSection[sectionIndex - 2].agentStartTime = startDateTime.toString(TimeFORMAT);
            }
            //7:降落，后一段
            else if((section.tasknumber == 7 && (sectionIndex < (planesSize - 1))) &&
                     plane.agentSection[sectionIndex + 1].tasknumber == 3){
                QDateTime startDateTime = QDateTime::fromString(section.agentStartTime, TimeFORMAT);
                //1
                qint64 timeInterval = static_cast<qint64>(plane.agentSection[sectionIndex].agentgDuration * 1000);
                startDateTime = startDateTime.addMSecs(timeInterval);
                plane.agentSection[sectionIndex + 1].agentStartTime = startDateTime.toString(TimeFORMAT);
            }
            sectionIndex++;
        }
    }
}

