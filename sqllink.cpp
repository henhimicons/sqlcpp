// mysqllink.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include <winsock.h>
#include <mysql.h>
#include <iostream>
#include <string>
#include <vector>
#include <ctime>
using namespace std;
#define URL "localhost"
#define USERNAME "root"
#define PASSWORD "root"
vector<string> mPath,wmPath;
vector<int> mWeight, wmWeight;
vector<int> mCount, wmCount;
vector<int> mid, wmid;
string get_year(int x)
{ 
	/*
	* this function implements turn x into year represents in string.
	*/
	int year = 1900 + x;
	string ans;
	while (year){
		ans += (year % 10 + '0');
		year /= 10;
	}
	reverse(ans.begin(), ans.end());
	return ans; 
}
string get_month(int x) 
{ 
	/*
	* this function implements turn x into month represents in string.
	*/
	int  mon = x + 1;
	string ans;
	if (mon / 10 == 0){
		ans += '0';
		ans += (char)('0' + mon);
		return ans;
	}
	else{
		ans += (char)(mon / 10 + '0');
		ans += (char)(mon % 10 + '0');
		return ans;
	} 
}
string get_day(int x) 
{ 
	/*
	 * this function implements turn x into day represents in string.
	 */
	string ans;
	if (x / 10 == 0)
	{
		ans += '0';
		ans += (char)('0' + x);
		return ans;
	}
	else
	{
		ans += (char)('0' + x / 10);
		ans += (char)('0' + x % 10);
		return ans;
	}
}
string get_time(int h,int m,int s) 
{ 
	/*
	* this function implements turning hour and minute and second into format we want.
	  return a string.
	* mind the time zone.we need plus 8 in hour.
	*/
	h = h + 8;
	string hour, min, sec;
	hour += (char)('0' + h / 10);
	hour += (char)('0' + h % 10);
	min += (char)('0' + m / 10);
	min += (char)('0' + m % 10);
	sec += (char)('0' + s / 10);
	sec += (char)('0' + s % 10);
	return string (hour+":"+min+":"+sec); 
}
string get_date_and_time()
{
	/*
	 * this function implements get local time.
	 * it returns a string represents time in format below:
	   year-month-day hh:mm:ss
	   like 2017-08-17 12:34:56
	        2017-11-12 12:34:56
	 */
	time_t raw_time;
	time(&raw_time);
	struct tm *p;
	p = gmtime(&raw_time);
	string year, month, day,time;
	year = get_year(p->tm_year);
	month = get_month(p->tm_mon);
	day = get_day(p->tm_mday);
	time = get_time(p->tm_hour, p->tm_min, p->tm_sec);
	return string(year + "-" + month + "-" + day + " " + time);
}
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
	string query("select * from adpath.adpath where sex = ");
	if (sex == 1){
		query += " 1 ";
	}
	else{
		query += " 0 ";
	}
	query += " ORDER BY weight DESC";
	MYSQL_RES *result_set;
	int res = mysql_query(&mysql, query.c_str());
	if (res == 0){
		result_set = mysql_store_result(&mysql);
		MYSQL_ROW row;
		while (row = mysql_fetch_row(result_set)){
			if (sex == 1)
			{
				mPath.push_back(string(row[1]));
				string num = row[2];
				int length = num.length();
				int sum = 0;
				for (int i = 0; i < length; i++){
					sum = sum * 10 + num[i] - '0';
				}
				mWeight.push_back(sum);
				num = row[3];
				length = num.length();
				sum = 0;
				for (int i = 0; i < length; i++)
				{
					sum = sum * 10 + (num[i] - '0');
				}
				mCount.push_back(sum);
				num = row[0];
                                length = num.length();
				sum = 0;
				for (int i = 0; i < length; i++)
				{
					sum = sum * 10 + (num[i] - '0');
				}
				mid.push_back(sum);
			}
			else
			{
				wmPath.push_back(string(row[1]));
				string num = row[2];
				int length = num.length();
				int sum = 0;
				for (int i = 0; i < length; i++){
					sum = sum * 10 + num[i] - '0';
				}
				wmWeight.push_back(sum);
				num = row[3];
				length = num.length();
				sum = 0;
				for (int i = 0; i < length; i++)
				{
					sum = sum * 10 + (num[i] - '0');
				}
				wmCount.push_back(sum);
				num = row[0];
				length = num.length();
				sum = 0;
				for (int i = 0; i < length; i++)
				{
					sum = sum * 10 + (num[i] - '0');
				}
				mid.push_back(sum);
			}
		}
		mysql_free_result(result_set);
	}
}
void update_ad_data(MYSQL &mysql, int id, int count,string datetime)
{
	/*
	* this function implements update count and datetime in the database
	* parameter meaning:
	mysql : a connected database instance
	url   : video path
	count : the count of play time of the url
	datetime : the time of ad played
	* at the end of this function , the count of the item which
	has the same path with the url will be changed to count.
	*/
	string num;
	while (count){
		num += ((count % 10) + '0');
		count /= 10;
	}
	num.reserve();
	string query("update adpath.adpath ");
	query += " set count =  ";
	query += num;
	query += "  , ";
	query += "time = ";
	query += "'";
	query += datetime;
	query += "' ";
	query += " where id = ";
	query += (char)('0'+id);
	cout << query << endl;
	mysql_query(&mysql, query.c_str());
}
void test(int sex)
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
	query_db(mysql, 0);
	test(1);
	test(0);
	string ans = get_date_and_time();
	mCount[0]++;
	update_ad_data(mysql, mid[0], mCount[0], ans);
	cout << ans << endl;
	return 0;
}
