#include "DataBaseOperation.h"
#include "DBTableClass.h"
DataBaseOperation::DataBaseOperation():
    d(new DataBaseOperationPrivate)
{

//    //先连接工作数据库
//    d->connectDataBase();

//    //创建数据库人员列表
//     d->createDataTable(DBPersonTable());
//    //创建数据库QY车列表
//     d->createDataTable(DBQYCarTable());
//    //创建数据库GD车列表
//     d->createDataTable(DBGDCarTable());
//    //创建数据库项目表
//     d->createDataTable(DBCreateProjectTable());
//    //创建数据库FJ表
//     d->createDataTable(DBPlaneTable());
//    //创建数据库FJ类型表
//     d->createDataTable(DBPlaneTypeTable());
//    //创建数据库QY类型表
//     d->createDataTable(DBQYCarTypeTable());
//    //创建数据库GD类型表
//     d->createDataTable(DBGDCarTypeTable());
//    //创建数据库人员类型表
//     d->createDataTable(DBPersonTypeTable());
}
bool DataBaseOperation::connectWithDatabase(QString dataBaseName, QString dataBaseIP, QString dataBasePassword, QString dataBaseUser, int dataBasePort,QString& errorMsg)
{
    QString privateConnect= "jzjConnect";
    connectNum++;
    d->m_DBName = dataBaseName;
    d->m_dbIP = dataBaseIP;
    d->m_dbPassword = dataBasePassword;
    d->m_dbUser = dataBaseUser;
    d->m_dbPort = dataBasePort;
    d->connectName = privateConnect + QString::number(connectNum,10);
    if(d->connectDataBase(errorMsg))
    {
        //创建数据库人员列表
        d->createDataTable(DBPersonTable());
        //创建数据库QY车列表
        d->createDataTable(DBQYCarTable());
        //创建数据库GD车列表
        d->createDataTable(DBGDCarTable());
        //创建数据库项目表
        d->createDataTable(DBCreateProjectTable());
        //创建数据库FJ表
        d->createDataTable(DBPlaneTable());
        //创建数据库FJ类型表
        d->createDataTable(DBPlaneTypeTable());
        //创建数据库QY类型表
        d->createDataTable(DBQYCarTypeTable());
        //创建数据库GD类型表
        d->createDataTable(DBGDCarTypeTable());
        //创建数据库人员类型表
        d->createDataTable(DBPersonTypeTable());

        return true;
    }

    return false;
}

DataBaseOperation::~DataBaseOperation()
{
    d->m_db.close();//关闭数据库链接
    qDebug() <<"数据库连接已关闭";
    delete d;
}
