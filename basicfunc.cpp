#include <iostream>
#include <iomanip>
#include "windows.h"
#include <qDebug>
using namespace std;

#include "basicfunc.h"

#import "C:\Program Files\Common Files\System\ado\msado15.dll" no_namespace rename("EOF","EndOfFile")

string getConnectStr(string userid, string pwd, string databaseName){
	string str = "Provider=SQLOLEDB.1;User ID=" + userid +";Password=" + pwd +";Persist Security Info=True;Initial Catalog="+ databaseName;
	return str;
}

void testConnect() {
	::CoInitialize(NULL);

	//类型在msado15.dll中已定义
	_RecordsetPtr m_pRecordset("ADODB.Recordset");
	_ConnectionPtr m_pConnection("ADODB.Connection");
	//string originDataTable = "[dbo].[AExchangeData]";
	//string queryString = string("select * from ") + originDataTable;
    string tableName = "SH600000";
    string sqlStr = "select * from [dbo].[" + tableName +"]";
	_bstr_t bstrSQL(sqlStr.c_str()); //对应的sql语句

	try
	{
        cout << "正在连接数据库..." << endl;
		m_pConnection.CreateInstance("ADODB.Connection");//创建Connection对象
		//设置连接字符串，必须是BSTR型或者_bstr_t类型
        string userid = "sa";
        string pwd = "sasa";
        string databaseName = "MarketDataTest";
        _bstr_t strConnect = getConnectStr(userid, pwd, databaseName).c_str ();

		m_pConnection->Open(strConnect, "", "", adModeUnknown);//服务器连接

		if (m_pConnection == NULL)
		{
			std::cerr << "Lind data ERROR!\n";
		}

		m_pRecordset.CreateInstance(__uuidof(Recordset));//创建记录集对象

		//取得表中的记录
		m_pRecordset->Open(bstrSQL, m_pConnection.GetInterfacePtr(), adOpenDynamic, adLockOptimistic, adCmdText);

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

void testFunc(){
	string userid = "sa";
	string pwd = "sasa";
	string databaseName = "MarketDataTest";
    cout << getConnectStr(userid, pwd, databaseName)<< endl;
}
