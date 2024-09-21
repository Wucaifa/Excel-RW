#ifndef DATABASEOPERATION_P_H
#define DATABASEOPERATION_P_H

#include <QDebug>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <QMetaProperty>
#include "DBTableClass.h"

class DataBaseOperationPrivate
{
public:
    QSqlDatabase m_db;      //数据库对象
    const QString m_dbType = "QMYSQL";
    QString m_DBName /*= "jzjdb"*/;     //数据库名 暂定为jzjdb 可改
    QString m_dbIP /*= "localhost"*/;
    QString m_dbUser /*= "root"*/;
    QString m_dbPassword /*= "123456"*/;
    int m_dbPort /*= 3308*/;
    QString connectName = "jzjConnect";

    //创建库
    void connectWithMyDatabase(){

    }
    bool connectDataBase(QString& error)
    {
        //初始化数据库连接
        m_db = QSqlDatabase::addDatabase(m_dbType,connectName);     //设置数据库类型
        m_db.setHostName(m_dbIP);     //IP地址
        m_db.setUserName(m_dbUser);   //账号
        m_db.setPassword(m_dbPassword);   //密码
        m_db.setPort(m_dbPort);         //端口
        //m_db.setDatabaseName(m_DBName);     //设置工作数据库名 需要提前创建好这个数据库   utf8_general_ci
        if (m_db.open())       //链接数据库
        {
            QString &&command = QString("CREATE DATABASE IF NOT EXISTS %1").arg(m_DBName);
            QSqlQuery query(m_db);
            if(query.exec(command))
            {
                m_db.setDatabaseName(m_DBName);
                if(m_db.open())
                {
                    qDebug() << "数据库["+m_DBName+"]打开成功";
                    return true;
                }
            }
        }else
        {
            error = "数据库打开失败：" + m_db.lastError().text();
            qDebug() << error;
            return false;
        }
    }

    template<typename T>
    void createDataTable(T &&table)
    {
        const QMetaObject *metaobject = table.metaObject();
        int pcount = metaobject->propertyCount();
        int ccount = metaobject->classInfoCount();

        QMap<QString,QString> classInfoMap;
        for (int i = 0; i < ccount; ++i) {
            auto &&metaClass = metaobject->classInfo(i);
            const char *name = metaClass.name();
            QVariant value = metaClass.value();
            classInfoMap[name] = value.toString();
        }
        //开始组合创建表的sql语句
        QString &&command = QString("CREATE TABLE If Not Exists %1(").arg(classInfoMap["TableName"]);
        QString emptyStr(" ");
        for (int i = 1; i < pcount; ++i) {
            QMetaProperty &&metaproperty = metaobject->property(i);
            const char *name = metaproperty.name();
            //QVariant value = table->property(name);

            command += name + emptyStr + classInfoMap[name]+",";
        }
        command.chop(1);    //删除末尾 ,
        command +=")ENGINE=InnoDB  DEFAULT CHARSET=utf8 AUTO_INCREMENT=1";
        //qDebug() << command;
        QSqlQuery query(m_db);
        if(!query.exec(command))
        {
            qDebug() << query.lastError().text();
        }
    }
    template<typename T>
    void queryTableInfo(QList<QSharedPointer<T>>& infoList)
    {
        QString &&tableName = T::staticMetaObject.classInfo(0).value();
        QString &&command = QString("select * from %1").arg(tableName);
        QSqlQuery query(m_db);

        if(query.exec(command))
        {
            const QMetaObject &metaobject = T::staticMetaObject;
            int pcount = metaobject.propertyCount();
            int mcount = metaobject.methodCount();
            int offset = metaobject.methodOffset();

            while (query.next()) {
                QSharedPointer<T> tempData(new T);
                for (int i = 1; i < pcount; ++i) {
                    auto &&propery = metaobject.property(i);
                    QString pName(propery.name());
                    auto type = QString(propery.typeName());

                    for (int i = offset; i < mcount; ++i) {
                        QMetaMethod &&metaMethod = metaobject.method(i);
                        QString mName(metaMethod.name());
                        if(mName.endsWith(pName))
                        {
                            if(type == "int")
                            {
                                int data = query.value(pName).toInt();
                                QMetaObject::invokeMethod(tempData.get(),metaMethod.name(),Qt::DirectConnection,Q_ARG(int,data));
                            }
                            else if(type == "QString")
                            {
                                QString&& data = query.value(pName).toString();
                                QMetaObject::invokeMethod(tempData.get(),metaMethod.name(),Qt::DirectConnection,Q_ARG(QString,data));
                            }
                            else if(type == "float")
                            {
                                float data = query.value(pName).toFloat();
                                QMetaObject::invokeMethod(tempData.get(),metaMethod.name(),Qt::DirectConnection,Q_ARG(float,data));
                            }
                            else if(type == "double")
                            {
                                double data = query.value(pName).toDouble();
                                QMetaObject::invokeMethod(tempData.get(),metaMethod.name(),Qt::DirectConnection,Q_ARG(double,data));
                            }
                            break;
                        }
                    }
                }

                infoList.append(tempData);
            }

//            if(tableName == DBProjectTable::tableName)
//            {
//                DBProjectTable::parseQueryInfo(info,query);
//            }
//            else if(tableName == DBProjectTable::tableName)
//            {
//                DBProjectTable::parseQueryInfo(info,query);
//            }
        }
    }

    template<typename T>
    void insertTableInfo(const QList<QSharedPointer<T>>& infoList)
    {
        if(infoList.isEmpty())  return;     //空直接返回
        QSqlQuery query(m_db);
        const QMetaObject &meta = T::staticMetaObject;
        int pcount = meta.propertyCount();

        QString &&tableName = meta.classInfo(0).value();
        QString &&command = QString("INSERT INTO %1(").arg(tableName);
        //先遍历一遍属性 组合出要新增的字段

        for (int i = 1; i < pcount; ++i) {
            QMetaProperty &&metaproperty = meta.property(i);
            const char *name = metaproperty.name();
            command += name+QString(",");
       }
        command.chop(1);
        command +=") VALUES";  //INSERT INTO abc(name,birthday,id) VALUES

        for(auto& it : infoList)
        {
            QString valueStr("(");

            //再遍历一遍属性 组合出要新增的数据
            for (int i = 1; i < pcount; ++i) {
                QMetaProperty &&metaproperty = meta.property(i);
                const char *name = metaproperty.name();
                QVariant value = it->property(name);
                auto type = QString(metaproperty.typeName());
                if(type == "int")
                {
                    if(QString(name) == "pID")       //主键赋值为0 触发自动递增
                        valueStr += "0,";
                    else
                        valueStr += value.toString()+",";
                }
                else if(type == "QString")
                {
                    valueStr += "'" + value.toString() +"',";
                }
                else if(type == "float" || type == "double")
                {
                    valueStr += value.toString()+",";
                }
            }
            valueStr.chop(1);
            valueStr +="),";    //('老铁3','1999-1-3','3'),
            command +=  valueStr;
        }
        command.chop(1);

        qDebug() <<"insert--  " <<command;
        if(!query.exec(command))
        {
            qDebug() << query.lastError().text();
        }
    }

    template<typename T>
    void updateTableInfo(const QList<QSharedPointer<T>>& infoList)
    {
        if(infoList.isEmpty())  return;     //空直接返回
        QSqlQuery query(m_db);
        const QMetaObject &meta = T::staticMetaObject;

        for(auto& it : infoList)
        {
            QString &&tableName = meta.classInfo(0).value();
            QString command = QString("UPDATE %1 SET ").arg(tableName);

            int pcount = meta.propertyCount();

            //遍历属性 组合出要更新的数据
            for (int i = 2; i < pcount; ++i) {      //i = 2  直接跳过主键属性
                QMetaProperty &&metaproperty = meta.property(i);
                const char *name = metaproperty.name();
                QVariant value = it->property(name);
                auto type = QString(metaproperty.typeName());
                if(type == "int" || type == "float" || type == "double")
                {
                    command += QString(name) + "=" + value.toString()+",";
                }
                else if(type == "QString")
                {
                    command +=   QString(name) + "=" + "'" + value.toString() +"',";
                }

            }
            command.chop(1);
            const char *priKeyName = meta.property(1).name();  //位置1 为主键
            QString whereStr = QString(" WHERE %1=%2").arg(priKeyName,it->property(priKeyName).toString());
            command += whereStr;
            qDebug() <<"update--  " <<command;
            if(!query.exec(command))        //由于数据量不大 这里简单点 直接进行循环更新
            {
                qDebug() << query.lastError().text();
            }
        }
    }

    template<typename T>
    void deleteTableInfo(const QList<int> &infoID, T& Class)
    {
        if(infoID.isEmpty())    return;

        auto meta = Class.metaObject();
        QString &&tableName = meta->classInfo(0).value();
        QString &&prikeyName = meta->property(1).name();

        QSqlQuery query(m_db);
        for(auto& id : infoID)
        {
            QString &&command = QString("DELETE FROM %1 WHERE %2 = %3").arg(tableName,prikeyName,QString::number(id));
            query.exec(command);
        }
    }

};



#endif // DATABASEOPERATION_P_H
