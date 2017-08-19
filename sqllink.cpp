// mysqllink.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include <winsock.h>
#include <mysql.h>
#include <iostream>
#include <string>
#include <vector>
using namespace std;
#define URL "localhost"
#define USERNAME "root"
#define PASSWORD "root"
vector<string> mPath,wmPath;
vector<int> mWeight, wmWeight;
vector<int> mCount, wmCount;
void connect_db(MYSQL& mysql)
{
	/*
	 * this function implements the connect action of the database.
	 */
	mysql_init(&mysql);
	if (!mysql_real_connect(&mysql, URL, USERNAME, PASSWORD, "adpath", 3306, NULL, 0)){
		exit(-1);
	}
	else{}
}
void query_db(MYSQL &mysql,int sex){
	/*
	 * this function implements the query action needed for this project
	 * Parameter meaning:
	       mysql : a connected database instance
	       sex : male or female of the person we get in the camera.
			     1 means male and 0 means female

	 */
	string query("select * from adpath where sex = ");
	if (sex == 1){
		query += " '1' ";
	}
	else{
		query += " '0' ";
	}
	MYSQL_RES *result_set;
	int res = mysql_query(&mysql, query.c_str());
	if (res == 0){
		result_set = mysql_store_result(&mysql);
		MYSQL_ROW row;
		while (row = mysql_fetch_row(result_set)){
			if (sex == 1) mPath.push_back(string(row[1]));
			else wmPath.push_back(string(row[1]));
		}
	}
}
void test(MYSQL& mysql,int sex)
{
	/*
	 * debug function to test the connection.
	 */
	if (sex==1) for (auto e : mPath)  cout << e << endl;
	else for (auto e : wmPath) cout << e << endl;
}
int _tmain(int argc, _TCHAR* argv[])
{
	MYSQL mysql; 
	connect_db(mysql);
	query_db(mysql, 1);
	test(mysql,1);
	cout << 1 << endl;
	query_db(mysql, 0);
	test(mysql, 0);
	mysql_close(&mysql);
	return 0;
}

