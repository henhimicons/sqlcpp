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
		query += " 'male' ";
	}
	else{
		query += " 'female' ";
	}
	
}
void test(MYSQL& mysql)
{
	/*
	 * debug function to test the connection.
	 */
	MYSQL_RES *result_set;
	MYSQL_ROW row;
	int res = mysql_query(&mysql, "select * from adpath");
	if (res == 0){
		result_set = mysql_store_result(&mysql);
		while (row = mysql_fetch_row(result_set)){
			cout << row[0] << " " << row[1] << " " << row[2] << " " << row[3] << " " << row[4] << " " << row[5] << endl;
		}
	}
	mysql_free_result(result_set); 
}
int _tmain(int argc, _TCHAR* argv[])
{
	MYSQL mysql; 
	connect_db(mysql);
	//test(mysql);
	mysql_close(&mysql);
	return 0;
}

