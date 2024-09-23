#include "widget.h"

#include <QApplication>
#include "excelread.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    ExcelRead excelread;
//    QString agentxlsxfilePath = "C:/Users/WuCF/Desktop/A13-1.xlsx";
//    excelread.readExcel(agentxlsxfilePath);

//    QString agentxmlfilePath = "C:/Users/WuCF/Desktop/78传输/测78_Agent.xml";
//    excelread.agentpathMatch(agentxmlfilePath);
//    excelread.readXml();
//    excelread.matchPath();
//    excelread.xmlRewrite_startTime();
//    excelread.xmlWrite_deflectors();
//    excelread.writeXml();

    QString agentxmlfilePath = "C:/Users/WuCF/Documents/WeChat Files/wxid_dt9l83nw8mvm22/FileStorage/File/2024-09/测78_Agent.jzj";
    excelread.agentpathMatch(agentxmlfilePath);
    excelread.readXml();
    excelread.xmlWrite_deflectors();
//    excelread.writeXml();

//    excelread.writeExcel();

//    QString agentxmlfilePath = "C:/Users/WuCF/Desktop/test001.xml";
//    excelread.agentpathMatch(agentxmlfilePath);
//    excelread.readXml();
//    excelread.xmlRewrite_tasknumber3();
//    excelread.writeXml();

    Widget w;
    w.show();
    return a.exec();
}
