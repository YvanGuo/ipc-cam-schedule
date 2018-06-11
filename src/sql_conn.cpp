#include "sql_conn.h"  
#include <boost/typeof/typeof.hpp>  
#include <boost/lexical_cast.hpp>
#include "mission.h"
#include "CaptureDevice.h"

#include "SpeedDomeCam.h"


static CMission G_mission;
static CCaptureDevice G_capdev;
static CSpeedDomeCam G_SDC;
static Preset G_preset;

static int callback(void *NotUsed, int argc, char **argv, char **azColName){
   int i;


   string *tableName = (string *)NotUsed;
   

   for(i=0; i<argc; i++){
     	printf("%s = %s\n", azColName[i], argv[i] ? argv[i] : "NULL");
		string sColName = azColName[i]; 
  		string sColVal = argv[i];

		
		if("SDCs" == *tableName){

			//printf("[sql callback]: SDCs\n");
			if("missionID" == sColName){

				G_SDC.missionNum = boost::lexical_cast<int32_t>(sColVal);
			}else if("camNum" == sColName){

				G_SDC.camNum = sColVal;
			 }else if("IP" == sColName){

				G_SDC.m_IP = sColVal;
			 }else if("port" == sColName){

				G_SDC.m_port = boost::lexical_cast<int32_t>(sColVal);
			 }else if("usrName" == sColName){

				G_SDC.m_usrName = sColVal;
			 }else if("pwd" == sColName){

				G_SDC.m_pwd = sColVal;
			 }else if("RTSPaddr" == sColName){

				G_SDC.RTSPaddr = sColVal;
			 }else if("brandName" == sColName){

				G_SDC.brandName = sColVal;
			 }else if("productType" == sColName){

				G_SDC.productType = sColVal;
			 }else if("presetNum" == sColName){

				G_SDC.presetTableID = boost::lexical_cast<int32_t>(sColVal);

				printf("presetNum add G_mission.addSDC(&G_SDC) \r\n");
				G_mission.addSDC(&G_SDC);
			 }

			 
			
		}else if("missions" == *tableName){

			 printf("[sql callback]: missions\n");
			
			 if("ID" == sColName){

				G_mission.missionNum = boost::lexical_cast<int32_t>(sColVal);
				MissionAdd(&G_mission);
			 }

			 
			 
		}else if("capDevs" == *tableName){

			// printf("[sql callback]: capDevs\n");
			 if("missionID" == sColName){

				G_capdev.missionNum = boost::lexical_cast<int32_t>(sColVal);
					
			 }else if("IP" == sColName){

				G_capdev.IP = sColVal;
			 }
			 else if("devType" == sColName){

				G_capdev.devType = sColVal;
			 }
			 else if("MAC" == sColName){

				G_capdev.MAC = sColVal;
			 }else if("devNum" == sColName){

				G_capdev.devNum = sColVal;
			 }
			 else if("devName" == sColName){

				G_capdev.devName = sColVal;

			 	printf("devName add G_mission.addCapDev(&G_capdev) \r\n");
				G_mission.addCapDev(&G_capdev);
			 }

			
		}
   		else if("preset" == *tableName){
	
			// printf("[sql callback]: capDevs\n");
			 if("ID" == sColName){

					
			 }else if("num" == sColName){

				printf(" num  == sColName\r\n");
				//G_mission.SpdDomeCam[G_mission.totalSpdDomeCam-1]->missionPreset[G_mission.SpdDomeCam[G_mission.totalSpdDomeCam-1]->totalPTZpreset].presetNum = boost::lexical_cast<int32_t>(sColVal);
				G_preset.presetNum = boost::lexical_cast<uint32_t>(sColVal);
			 }
			 else if("SDC_ID" == sColName){

			 printf(" SDC_ID  == sColName\r\n");

				G_preset.camNum = sColVal;
			 }
			 else if("holdSeconds" == sColName){

			 printf(" holdSeconds  == sColName\r\n");

				//G_mission.SpdDomeCam[G_mission.totalSpdDomeCam-1]->missionPreset[G_mission.SpdDomeCam[G_mission.totalSpdDomeCam-1]->totalPTZpreset].holdSeconds = boost::lexical_cast<int32_t>(sColVal);
				G_preset.holdSeconds = 3;//boost::lexical_cast<uint32_t>(sColVal);

			 }else if("mode" == sColName){

			 printf(" mode  == sColName\r\n");

				//G_mission.SpdDomeCam[G_mission.totalSpdDomeCam-1]->missionPreset[G_mission.SpdDomeCam[G_mission.totalSpdDomeCam-1]->totalPTZpreset].mode = boost::lexical_cast<int32_t>(sColVal);
				G_preset.mode = boost::lexical_cast<uint32_t>(sColVal);
			 }
			 else if("priority" == sColName){

				//G_mission.SpdDomeCam[G_mission.totalSpdDomeCam-1]->missionPreset[G_mission.SpdDomeCam[G_mission.totalSpdDomeCam-1]->totalPTZpreset].priority = boost::lexical_cast<int32_t>(sColVal);
				
				//G_mission.SpdDomeCam[G_mission.totalSpdDomeCam-1]->totalPTZpreset++;
				printf(" priority  == sColName\r\n");

				G_preset.priority = boost::lexical_cast<uint32_t>(sColVal);

				G_mission.addPreset(&G_preset);
			 	
			 }

			
		}
   }
   printf("\n");
   return 0;
}


CSQL_conn::~CSQL_conn()  
{  
  
}  
  
CSQL_conn& CSQL_conn::getInstance()  
{  
    static CSQL_conn connection;  
	
    return connection;  
}  
  
void CSQL_conn::setParam(const string& conn_str)  
{  
	dbName = conn_str;
}  
  
int8_t CSQL_conn::initConnection()  
{  

	char *zErrMsg = 0;

	int rc;

	//打开指定的数据库文件,如果不存在将创建一个同名的数据库文件

	rc = sqlite3_open(dbName.c_str(), &handle);

	if( rc ) 
	{

		fprintf(stderr, "Can't open database: %s\n", sqlite3_errmsg(handle));

		sqlite3_close(handle);

		return -1;

	}
	
	return 0;
	
}  
   

void CSQL_conn::releaseConnection()  
{  
	if(handle){

		sqlite3_close(handle);
	}
	
}  

int8_t CSQL_conn::createTable()
{
   
   char *zErrMsg = 0;
   int  rc;
   string sql;

   /* Create SQL statement */

	sql = "CREATE TABLE SDCs("  \
     "camNum INT PRIMARY KEY     NOT NULL," \
     "missionID  INT NOT NULL," \
     "IP           TEXT    NOT NULL," \
     "port            INT     NOT NULL," \
     "usrName        TEXT," \
     "pwd         TEXT," \
     "RTSPaddr        TEXT," \
     "brandName        TEXT," \
     "productType        TEXT," \
     "presetNum        INT );" ;

	/* Execute SQL statement */
	rc = sqlite3_exec(handle, sql.c_str(), callback, 0, &zErrMsg);
	if( rc != SQLITE_OK ){
	fprintf(stderr, "SQL error: %s\n", zErrMsg);
	  sqlite3_free(zErrMsg);
	}else{
	  fprintf(stdout, "Table created TABLE SDCs successfully\n");
	}

	sql = "CREATE TABLE missions("  \
     "ID INT PRIMARY KEY     NOT NULL," \
     "SDC_ID           TEXT    NOT NULL," \
     "capDevID            INT     NOT NULL);" ;

	/* Execute SQL statement */
	rc = sqlite3_exec(handle, sql.c_str(), callback, 0, &zErrMsg);
	if( rc != SQLITE_OK ){
	fprintf(stderr, "SQL error: %s\n", zErrMsg);
	  sqlite3_free(zErrMsg);
	}else{
	  fprintf(stdout, "Table created TABLE missions successfully\n");
	}

	sql = "CREATE TABLE capDevs("  \
     "devNum TEXT PRIMARY KEY     NOT NULL," \
     "missionID INT  NOT NULL," \
     "IP           TEXT    NOT NULL," \
     "devType        TEXT," \
     "MAC        TEXT," \
     "devName        TEXT );";

	/* Execute SQL statement */
	rc = sqlite3_exec(handle, sql.c_str(), callback, 0, &zErrMsg);
	if( rc != SQLITE_OK ){
	fprintf(stderr, "SQL error: %s\n", zErrMsg);
	  sqlite3_free(zErrMsg);
	}else{
	  fprintf(stdout, "Table created TABLE capDevs successfully\n");
	}

	sql = "CREATE TABLE preset("  \
     "ID INT PRIMARY KEY     NOT NULL," \
     "num           INT    NOT NULL," \
     "SDC_ID           INT    NOT NULL," \
     "holdSeconds           INT    NOT NULL," \
     "mode           INT    NOT NULL," \
     "priority            INT     NOT NULL);" ;
	
	rc = sqlite3_exec(handle, sql.c_str(), callback, 0, &zErrMsg);
	if( rc != SQLITE_OK ){
	fprintf(stderr, "SQL error: %s\n", zErrMsg);
	  sqlite3_free(zErrMsg);
	}else{
	  fprintf(stdout, "Table created TABLE capDevs successfully\n");
	}
	//sqlite3_close(handle);
	return 0;
}


int8_t CSQL_conn::insert()
{
 
   char *zErrMsg = 0;
   int rc;
   char sql[1024];


	for(int i = 0;i<10; i++){

	   /* Create SQL statement */
	   //sql = ;

		sprintf(sql, "INSERT INTO capDevs (devNum,missionID,IP,devType,MAC,devName) "  \
	         "VALUES ('LBTCZDZPXXXXXXX0%d', %d, '192.168.9.200', 'XMYHL-LBTCZDZP', '38.162.1.132.84.114', '嵌入式路边停车自动抓拍仪'); ", i , i);
	   /* Execute SQL statement */
	   rc = sqlite3_exec(handle, sql, callback, 0, &zErrMsg);
	   if( rc != SQLITE_OK ){
	      fprintf(stderr, "[insert]:SQL error: %s\n", zErrMsg);
	      sqlite3_free(zErrMsg);
	   }else{
	      fprintf(stdout, "INSERT SDCs successfully\n");
	   }

	      /* Create SQL statement */
	   //sql = "INSERT INTO SDCs (camNum,missionID,IP,port,usrName,pwd,RTSPaddr,brandName,productType,presetNum) "  \
	   //      "VALUES (1, 1, '192.168.9.229', 8000, 'admin', 'admin12345', 'rtsp://admin:admin12345@192.168.9.229/h264/ch1/main/av_stream', 'HIK', 'DS-2DF7286-A', 4); ";
	      sprintf(sql, "INSERT INTO SDCs (camNum,missionID,IP,port,usrName,pwd,RTSPaddr,brandName,productType,presetNum) "  \
	         "VALUES (%d, %d, '192.168.9.229', 8000, 'admin', 'admin12345', 'rtsp://admin:admin12345@192.168.9.229/h264/ch1/main/av_stream', 'HIK', 'DS-2DF7286-A', 4); " , i, i);
	   /* Execute SQL statement */
	   rc = sqlite3_exec(handle, sql, callback, 0, &zErrMsg);
	   if( rc != SQLITE_OK ){
	      fprintf(stderr, "[insert]:SQL error: %s\n", zErrMsg);
	      sqlite3_free(zErrMsg);
	   }else{
	      fprintf(stdout, "INSERT SDCs successfully\n");
	   }

   }

	int k = 0;

	for(int i=0; i<4; i++){

		for(int j=1; j<5; j++){

			
			sprintf(sql, "INSERT INTO preset (ID,num,SDC_ID,holdSeconds,mode,priority) "  \
	         "VALUES (%d, %d, %d, 15, 1, 0); ", k++ , j, i);
		   /* Execute SQL statement */
		   rc = sqlite3_exec(handle, sql, callback, 0, &zErrMsg);
		   if( rc != SQLITE_OK ){
		      fprintf(stderr, "[insert]:SQL preset error: %s\n", zErrMsg);
		      sqlite3_free(zErrMsg);
		   }else{
		      fprintf(stdout, "INSERT SDCs preset successfully\n");
		   }
		}

	}
//	sprintf(sql, "INSERT INTO missions (ID,SDC_ID,capDevID) VALUES (0, 0, 0)";
		   
	

   return 0;
}


int8_t CSQL_conn::select(string tableName)
{

   char *zErrMsg = 0;
   int rc;
   string sql;
   const char* data = "Callback function called";

   /* Create SQL statement */
   sql = "SELECT * from " + tableName;

   /* Execute SQL statement */
   rc = sqlite3_exec(handle, sql.c_str(), callback, (void*)&tableName, &zErrMsg);
   if( rc != SQLITE_OK ){
      fprintf(stderr, "SQL error: %s\n", zErrMsg);
      sqlite3_free(zErrMsg);
   }else{
      fprintf(stdout, "Operation done successfully\n");
   }
		
  // sqlite3_close(handle);
   return 0;
}


int8_t CSQL_conn::select(string tableName, int32_t missionID)
{

   char *zErrMsg = 0;
   int rc;
   string sql;
   const char* data = "Callback function called";

   /* Create SQL statement */
   sql = "SELECT * FROM " + tableName + " WHERE missionID=" + boost::lexical_cast<string>(missionID);

   /* Execute SQL statement */
   rc = sqlite3_exec(handle, sql.c_str(), callback, (void*)&tableName, &zErrMsg);
   if( rc != SQLITE_OK ){
      fprintf(stderr, "SQL error: %s\n", zErrMsg);
      sqlite3_free(zErrMsg);
   }else{
      fprintf(stdout, "Operation done successfully\n");
   }
  // sqlite3_close(handle);
   return 0;
}
int8_t CSQL_conn::update(int argc, char* argv[])
{
   char *zErrMsg = 0;
   int rc;
   char *sql;
   const char* data = "Callback function called";

   /* Create merged SQL statement */
   sql = "UPDATE COMPANY set SALARY = 25000.00 where ID=1; " \
         "SELECT * from COMPANY";

   /* Execute SQL statement */
   rc = sqlite3_exec(handle, sql, callback, (void*)data, &zErrMsg);
   if( rc != SQLITE_OK ){
      fprintf(stderr, "SQL error: %s\n", zErrMsg);
      sqlite3_free(zErrMsg);
   }else{
      fprintf(stdout, "Operation done successfully\n");
   }

   return 0;
}

int8_t CSQL_conn::sql_delete(int argc, char* argv[])
{
   char *zErrMsg = 0;
   int rc;
   char *sql;
   const char* data = "Callback function called";

   /* Create merged SQL statement */
   sql = "DELETE from COMPANY where ID=2; " \
         "SELECT * from COMPANY";

   /* Execute SQL statement */
   rc = sqlite3_exec(handle, sql, callback, (void*)data, &zErrMsg);
   if( rc != SQLITE_OK ){
      fprintf(stderr, "SQL error: %s\n", zErrMsg);
      sqlite3_free(zErrMsg);
   }else{
      fprintf(stdout, "Operation done successfully\n");
   }

   return 0;
}                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                
