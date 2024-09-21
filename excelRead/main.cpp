#include "widget.h"

#include <QApplication>
#include "excelread.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    ExcelRead excelread;
    QString agentxlsxfilePath = "C:/Users/WuCF/Desktop/78传输/A13-1.xlsx";
    excelread.readExcel(agentxlsxfilePath);

    QString agentxmlfilePath = "C:/Users/WuCF/Desktop/78传输/测78_Agent.xml";
    excelread.agentpathMatch(agentxmlfilePath);
    excelread.readXml();
    excelread.matchPath();
    excelread.writeXml();
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
