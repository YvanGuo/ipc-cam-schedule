#include <stdio.h>
#include <iostream>

#include <time.h>
#include <stdlib.h>
#include <unistd.h>
//#include "sql_conn_pool.h"

#include <stdio.h>
//#include <mysql.h>

#include "server.h"

#include "SpeedDomeCam.h"
#include "HIKCam.h"
#include "mission_config.h"
#include "sql_conn.h"  
using namespace std;

void ServerMissionStart()
{

	server s(6061, 200, 0);
    s.run();

}

void ServerConfigStart()
{

	server s(6062, 20, 1);
    s.run();

}


int main(int argc, char **argv) {

#if 0
	int plateNumLen = 10;
	unsigned char plateNum[10] = {0xcb,0xd5,0x42,0x54,0x30,0x30,0x30,0x30,0x0,0x0};//{0xc3,0xf6,0x44,0x31,0x37,0x30,0x30,0x38,0,0};
	printf("plateNum :\r\n");
	for(int j = 0; j<plateNumLen; j++){
	
				
		printf("%x", plateNum[j]);
	}

	unsigned char cutf8[24] = {0};
	int size = gbk2utf8((char *)cutf8, (char *)plateNum, 24);
	cutf8[size] = 0;

	printf("\r\n");

	printf("cutf8 :\r\n");
	for(int k = 0; k<size; k++){
	
				
		printf("%x", cutf8[k]);
	}

	printf("\r\n");
	
	printf("cutf8 = %s\r\n", cutf8);

	return 0;
	#endif
	
	CMissionConfig msConfig;
	//msConfig.loadConfig("mission-config1.xml");
	msConfig.DBloadConfig("ms.db");
	

	boost::thread thrd1(ServerMissionStart);
	
	boost::thread thrd2(ServerConfigStart);

	while(1){

		sleep(100);
	}
	return 1;
}


#if 0
int SQL_init(int argc,char *argv[])
{
	MYSQL conn;
	int res;
	mysql_init(&conn);
	if(mysql_real_connect(&conn,"localhost","root","","hr",0,NULL,CLIENT_FOUND_ROWS)) //"root":数据库管理员 "":root密码 "test":数据库的名字
	{
		printf("connect success!\n");
		res=mysql_query(&conn,"insert into hr_info values(2,'jane')");
		if(res)
		{
			printf("error\n");
		}
		else
		{
			printf("OK\n");
		}
		mysql_close(&conn);
	}
	return 0;
}
#endif
                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                   
                                                                                                                                                                                                                                   
                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                          