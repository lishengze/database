#include <iostream>
#include <iomanip>
#include <QSqlDatabase>
#include <QStringList>
#include <QSqlQuery>
#include <QSqlError>
#include <QSqlDriver>
#include "windows.h"
#include <qDebug>
using namespace std;

#include "basicfunc.h"

#import "C:\Program Files\Common Files\System\ado\msado15.dll" \
        no_namespace rename("EOF","EndOfFile")

string getConnectStr(string userid, string pwd, string databaseName){
    string str = "Provider=SQLOLEDB.1;User ID=" + userid +";Password="
               + pwd +";Persist Security Info=True;Initial Catalog="+ databaseName;
	return str;
}

void testAdo() {
    ::CoInitialize(NULL); // 初始化COM组件;

	//类型在msado15.dll中已定义
    _ConnectionPtr m_pConnection("ADODB.Connection");
	_RecordsetPtr m_pRecordset("ADODB.Recordset");

	try
	{
        cout << "正在连接数据库..." << endl;
		m_pConnection.CreateInstance("ADODB.Connection");//创建Connection对象

		//设置连接字符串，必须是BSTR型或者_bstr_t类型
        string userid = "sa";
        string pwd = "sasa";
        string databaseName = "MarketDataTest";
        _bstr_t sqlstr_connDatabase = getConnectStr(userid, pwd, databaseName).c_str ();

        m_pConnection->Open(sqlstr_connDatabase, "", "", adModeUnknown);//服务器连接

		if (m_pConnection == NULL)
		{
			std::cerr << "Lind data ERROR!\n";
		}

		m_pRecordset.CreateInstance(__uuidof(Recordset));//创建记录集对象

        //取得表中的记录, 设置对应的sql语句
        string tableName = "SH600000";
        string sqlStr = "select * from [dbo].[" + tableName +"]";
        _bstr_t sqlstr_getData(sqlStr.c_str());
        m_pRecordset->Open(sqlstr_getData, m_pConnection.GetInterfacePtr(), adOpenDynamic, adLockOptimistic, adCmdText);

        _variant_t secode;//对应库中的字段
        string colname = "SECODE";

		cout << "--------------数据库数据--------------" << endl;
		while (!m_pRecordset->EndOfFile)
		{
            secode = m_pRecordset->GetCollect(colname.c_str ());
            cout << colname << setw(10) << (char*)(_bstr_t)secode << endl;
			m_pRecordset->MoveNext();//下一条记录
            break;
		}

		m_pRecordset->Close();//关闭记录集
	}

	catch (_com_error e)//捕捉异常
	{
		cerr << "\nERROR:" << (char*)e.Description();
	}

	if (m_pConnection->State)
	{
		m_pConnection->Close();
	}

	::CoUninitialize();
}

void testGetConnectStr(){
	string userid = "sa";
	string pwd = "sasa";
	string databaseName = "MarketDataTest";
    cout << getConnectStr(userid, pwd, databaseName)<< endl;
}

void outputDatabaseDrivers() {
    qDebug() << "Available drivers: ";
    QStringList driversList = QSqlDatabase::drivers();
    foreach (QString driver, driversList) {
        qDebug() << driver;
    }
}

QSqlDatabase createODBCConn() {
    QSqlDatabase db = QSqlDatabase::addDatabase ("QODBC");
    QString databaseName = QString::fromLocal8Bit ("SqlServer");
    db.setHostName("localhost");
    db.setDatabaseName (databaseName);
    db.setUserName ("sa");
    db.setPassword ("sasa");

    if (!db.open ()) {
        qDebug() << "Open database: " + databaseName  + " failed!";
        qDebug() << db.lastError ();
    } else {
        qDebug() << "Open database: " + databaseName  + " successfully!";
    }
    return db;
}

/*
测试qt ODBC连接数据库;
*/
void testODBC() {

    QSqlDatabase db = createODBCConn();

    QSqlQuery queryObj(db);
    QString sqlstr = "select top 10[TIME] from [MarketData].[dbo].[SH600000]";
    queryObj.exec(sqlstr);
    while(queryObj.next()) {
        qDebug() << queryObj.value(0).toInt ();
    }

    sqlstr = "select top 10[TOPEN] from [MarketData].[dbo].[SH600000]";
    queryObj.exec(sqlstr);
    while(queryObj.next()) {
        qDebug() << queryObj.value(0).toFloat ();
    }
}

void testSqlQueryModel () {
    QSqlDatabase db = createODBCConn();
    QSqlQuery queryObj(db);
    QString sqlstr = "select * from [MarketData].[dbo].[SH600000]";
    queryObj.exec(sqlstr);

    int numRows;
    if (db.driver()->hasFeature(QSqlDriver::QuerySize)) {
        qDebug() << "has feature: query Size";
        numRows = queryObj.size();
    } else {
        qDebug() << "no feature: query Size";
        queryObj.last();
        numRows = queryObj.at() + 1;
    }
    qDebug() << "numRows: " << numRows;
}

