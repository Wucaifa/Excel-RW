#ifndef DATABASEOPERATION_H
#define DATABASEOPERATION_H

#include "DBTableClass.h"
#include "DataBaseOperation_p.h"


//class DataBaseOperationPrivate;
/*
    2024.4.4
    数据库业务类 单例实现
    由于此项目无高并发场景需求，此处就简单将CRUD做在此单例类里
*/
class DataBaseOperation {

public:
    static DataBaseOperation* getInstance() {
        static DataBaseOperation instance;      //此处线程安全
        return &instance;
    }
    int connectNum = 0;
    //查询表所有信息
    template<typename T>
    void queryTableInfo(QList<QSharedPointer<T>> & infoList){ d->queryTableInfo(infoList);}

    //新增表信息
    template<typename T>
    void insertTableInfo(const QList<QSharedPointer<T>> & infoList){d->insertTableInfo(infoList); }

    //更新表信息
    template<typename T>
    void updateTableInfo(const QList<QSharedPointer<T>> & infoList){d->updateTableInfo(infoList);}

    //删除表信息
    template<typename T>
    void deleteTableInfo(const QList<int> & infoID,T&& Class){d->deleteTableInfo(infoID,Class);}

    //登录数据库
    bool connectWithDatabase(QString dataBaseName, QString dataBaseIP, QString dataBasePassword
                             , QString dataBaseUser, int dataBasePort,QString& errorMsg);
private:
    DataBaseOperation();
    ~DataBaseOperation();
    DataBaseOperation(const DataBaseOperation& it) = delete;  //删除拷贝构造
    DataBaseOperation& operator=(const DataBaseOperation& s) = delete;//删除赋值
    //数据库业务类
    DataBaseOperationPrivate *d;
};

#endif // DATABASEOPERATION_H
