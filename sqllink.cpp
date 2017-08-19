// DataBaseConnection2.cpp : 定义控制台应用程序的入口点。
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
#define PASSWORD "Escort10086"
vector<string> mPath, wmPath;
vector<int> mWeight, wmWeight;
vector<int> mCount, wmCount;
void connect_db(MYSQL& mysql)
{
	/*
	* this function implements the connect action of the database.
	*/
	mysql_init(&mysql);
	if (!mysql_real_connect(&mysql, URL, USERNAME, PASSWORD, "videopath", 3306, NULL, 0)){
		exit(-1);
	}
	else{}
}
void query_db(MYSQL &mysql, int sex){
	/*
	* this function implements the query action needed for this project
	* Parameter meaning:
	mysql : a connected database instance
	sex : male or female of the person we get in the camera.
	1 means male and 0 means female
	* at the end , the mPath and the wmPath will be filled by the corresponding ad path.
	mPath for the male , and wmPath for the female.
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
			if (sex == 1) 
			{ 
				mPath.push_back(string(row[1]));
				string num = row[3];
				int length = num.length();
				int sum = 0;
				for (int i = 0; i < length; i++){
					sum = sum * 10 + num[i]-'0';
				}
				mWeight.push_back(sum);
			}
			else 
			{ 
				wmPath.push_back(string(row[1]));
				string num = row[3];
				int length = num.length();
				int sum = 0;
				for (int i = 0; i < length; i++){
					sum = sum * 10 + num[i] - '0';
				}
				wmWeight.push_back(sum);
			}
		}
		mysql_free_result(result_set);
	}
}
void update_ad_data(MYSQL &mysql, string url, int count)
{
	/*
	* this function implements update count in the database
	* parameter meaning:
	mysql : a connected database instance
	url   : video path
	count : the count of play time of the url
	* at the end of this function , the count of the item which
	has the same path with the url will be changed to count.
	*/
	string num;
	while (count){
		num += ((count % 10)+'0');
		count /= 10;
	}
	num.reserve();
	string query("update videopath.adpath ");
	query += " set Count =  ";
	query += num;
	query += "  ";
	query += " where Path = '";
	query += url;
	query += "'";
	cout << query << endl;
	mysql_query(&mysql, query.c_str());
}
void test(MYSQL& mysql, int sex)
{
	/*
	* debug function to test some function whether worked properly.
	*/
	if (sex == 1) { for (auto e : mPath)  cout << e << endl; for (auto e : mWeight) cout << e << endl; }
	else { for (auto e : wmPath) cout << e << endl; for (auto e : wmWeight) cout << e << endl; }
}

int _tmain(int argc, _TCHAR* argv[])
{
	MYSQL mysql;
	connect_db(mysql);
	query_db(mysql, 1);
	test(mysql, 1);
	query_db(mysql, 0);
	test(mysql, 0);
	++wmWeight[0];
	update_ad_data(mysql, wmPath[0], wmWeight[0]);
	mysql_close(&mysql);
	return 0;
}
