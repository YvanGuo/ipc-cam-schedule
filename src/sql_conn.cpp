#include "sql_conn.h"  
#include <boost/typeof/typeof.hpp>  
#include <boost/lexical_cast.hpp>
#include "mission.h"

static CMission G_mission;
CMission G_missionUnstart ;

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

				G_SDC.m_sdcCfg.missionNum = boost::lexical_cast<int32_t>(sColVal);
				//printf("");
			}else if("camNum" == sColName){

				G_SDC.m_sdcCfg.camNum = sColVal;
			 }else if("IP" == sColName){

				G_SDC.m_sdcCfg.IP = sColVal;
			 }else if("port" == sColName){

				G_SDC.m_sdcCfg.port = boost::lexical_cast<int32_t>(sColVal);
			 }else if("usrName" == sColName){

				G_SDC.m_sdcCfg.m_usrName = sColVal;
			 }else if("pwd" == sColName){

				G_SDC.m_sdcCfg.m_pwd = sColVal;
			 }else if("RTSPaddr" == sColName){

				G_SDC.m_sdcCfg.RTSPaddr = sColVal;
			 }else if("brandName" == sColName){

				G_SDC.m_sdcCfg.brandName = sColVal;
			 }else if("productType" == sColName){

				G_SDC.m_sdcCfg.productType = sColVal;
			 }else if("presetNum" == sColName){

				G_SDC.presetTableID = boost::lexical_cast<int32_t>(sColVal);

				//printf("presetNum add G_mission.addSDC(&G_SDC) \r\n");
				//G_mission.addSDC(&G_SDC);
				
				if(-1 == MissionAddSDC(&G_SDC)){
					
					G_SDC.m_sdcCfg.missionNum = -1;
					G_missionUnstart.addSDC(&G_SDC);
				}
			 }

			 
			
		}else if("missions" == *tableName){

			 //printf("[sql callback]: missions\n");
			
			 if("ID" == sColName){

				G_mission.missionNum = boost::lexical_cast<int32_t>(sColVal);
				//printf("[sql callback]: missions OK1\n");
				MissionAdd(&G_mission);
				//G_MissionMap.insert(pair<int, CMission>(G_mission.missionNum, G_mission)); 
			 }

			// printf("[sql callback]: missions OK2\n");
			 
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

			 	//printf("devName add G_mission.addCapDev(&G_capdev) \r\n");
				//G_mission.addCapDev(&G_capdev);
				
				if(-1 == MissionAddCapDev(&G_capdev)){

					G_capdev.missionNum = -1;
					G_missionUnstart.addCapDev(&G_capdev);
				}
			 }

			
		}
   		else if("preset" == *tableName){
		
			 if("num" == sColName){

				G_preset.presetNum = boost::lexical_cast<uint32_t>(sColVal);
			 }
			 else if("SDC_ID" == sColName){

				G_preset.camNum = sColVal;
				
			 printf("gggggggggggggggggggggggggggggG_preset.camNum  = %s\r\n", G_preset.camNum.c_str());
			 }
			 else if("holdSeconds" == sColName){

				G_preset.holdSeconds = boost::lexical_cast<uint32_t>(sColVal);

			 }else if("mode" == sColName){
			 
				G_preset.mode = boost::lexical_cast<uint32_t>(sColVal);
			 }
			 else if("priority" == sColName){

				G_preset.priority = boost::lexical_cast<uint32_t>(sColVal);

			 }
			 else if("plateMinWidth" == sColName){

				G_preset.plateMinWidth = boost::lexical_cast<uint32_t>(sColVal);

			 }
			 else if("plateMaxWidth" == sColName){

				G_preset.plateMaxWidth = boost::lexical_cast<uint32_t>(sColVal);

			 }
			 else if("nNum1" == sColName){

				G_preset.illparkpara.nNum1 = boost::lexical_cast<uint32_t>(sColVal);
			 	
			 }
			 else if("nNum2" == sColName){
 
				 G_preset.illparkpara.nNum2 = boost::lexical_cast<uint32_t>(sColVal);
 
			  }
			  else if("Area1x0" == sColName){
 
				 G_preset.illparkpara.Area1[0].x = boost::lexical_cast<uint32_t>(sColVal);
  
			  }
			   else if("Area1y0" == sColName){
 
				 G_preset.illparkpara.Area1[0].y = boost::lexical_cast<uint32_t>(sColVal);
 
			  }
			  else if("Area2x0" == sColName){
			  
				 G_preset.illparkpara.Area2[0].x = boost::lexical_cast<uint32_t>(sColVal);
 
			  }
			  else if("Area2y0" == sColName){
 
				 G_preset.illparkpara.Area2[0].y = boost::lexical_cast<uint32_t>(sColVal);
 
			  }// Areax[0]
			  else if("Area1x1" == sColName){
 
				 G_preset.illparkpara.Area1[1].x = boost::lexical_cast<uint32_t>(sColVal);
  
			  }
			   else if("Area1y1" == sColName){
 
				 G_preset.illparkpara.Area1[1].y = boost::lexical_cast<uint32_t>(sColVal);
 
			  }
			  else if("Area2x1" == sColName){
			  
				 G_preset.illparkpara.Area2[1].x = boost::lexical_cast<uint32_t>(sColVal);
 
			  }
			  else if("Area2y1" == sColName){
 
				 G_preset.illparkpara.Area2[1].y = boost::lexical_cast<uint32_t>(sColVal);
 
			  }// Areax[1]
			  else if("Area1x2" == sColName){
 
				 G_preset.illparkpara.Area1[2].x = boost::lexical_cast<uint32_t>(sColVal);
  
			  }
			   else if("Area1y2" == sColName){
 
				 G_preset.illparkpara.Area1[2].y = boost::lexical_cast<uint32_t>(sColVal);
 
			  }
			  else if("Area2x2" == sColName){
			  
				 G_preset.illparkpara.Area2[2].x = boost::lexical_cast<uint32_t>(sColVal);
 
			  }
			  else if("Area2y2" == sColName){
 
				 G_preset.illparkpara.Area2[2].y = boost::lexical_cast<uint32_t>(sColVal);
 
			  }// Areax[2]
			  else if("Area1x3" == sColName){
 
				 G_preset.illparkpara.Area1[3].x = boost::lexical_cast<uint32_t>(sColVal);
  
			  }
			   else if("Area1y3" == sColName){
 
				 G_preset.illparkpara.Area1[3].y = boost::lexical_cast<uint32_t>(sColVal);
 
			  }
			  else if("Area2x3" == sColName){
			  
				 G_preset.illparkpara.Area2[3].x = boost::lexical_cast<uint32_t>(sColVal);
 
			  }
			  else if("Area2y3" == sColName){
 
				 G_preset.illparkpara.Area2[3].y = boost::lexical_cast<uint32_t>(sColVal);
 
			  }// Areax[3]
			  else if("Area1x4" == sColName){
 
				 G_preset.illparkpara.Area1[4].x = boost::lexical_cast<uint32_t>(sColVal);
  
			  }
			   else if("Area1y4" == sColName){
 
				 G_preset.illparkpara.Area1[4].y = boost::lexical_cast<uint32_t>(sColVal);
 
			  }
			  else if("Area2x4" == sColName){
			  
				 G_preset.illparkpara.Area2[4].x = boost::lexical_cast<uint32_t>(sColVal);
 
			  }
			  else if("Area2y4" == sColName){
 
				 G_preset.illparkpara.Area2[4].y = boost::lexical_cast<uint32_t>(sColVal);
 
			  }// Areax[4]
			  else if("Area1x5" == sColName){
 
				 G_preset.illparkpara.Area1[5].x = boost::lexical_cast<uint32_t>(sColVal);
  
			  }
			   else if("Area1y5" == sColName){
 
				 G_preset.illparkpara.Area1[5].y = boost::lexical_cast<uint32_t>(sColVal);
 
			  }
			  else if("Area2x5" == sColName){
			  
				 G_preset.illparkpara.Area2[5].x = boost::lexical_cast<uint32_t>(sColVal);
 
			  }
			  else if("Area2y5" == sColName){
 
				 G_preset.illparkpara.Area2[5].y = boost::lexical_cast<uint32_t>(sColVal);
 
			  }// Areax[5]
			  else if("Area1x6" == sColName){
 
				 G_preset.illparkpara.Area1[6].x = boost::lexical_cast<uint32_t>(sColVal);
  
			  }
			   else if("Area1y6" == sColName){
 
				 G_preset.illparkpara.Area1[6].y = boost::lexical_cast<uint32_t>(sColVal);
 
			  }
			  else if("Area2x6" == sColName){
			  
				 G_preset.illparkpara.Area2[6].x = boost::lexical_cast<uint32_t>(sColVal);
 
			  }
			  else if("Area2y6" == sColName){
 
				 G_preset.illparkpara.Area2[6].y = boost::lexical_cast<uint32_t>(sColVal);
 
			  }// Areax[6]
			  else if("Area1x7" == sColName){
 
				 G_preset.illparkpara.Area1[7].x = boost::lexical_cast<uint32_t>(sColVal);
  
			  }
			   else if("Area1y7" == sColName){
 
				 G_preset.illparkpara.Area1[7].y = boost::lexical_cast<uint32_t>(sColVal);
 
			  }
			  else if("Area2x7" == sColName){
			  
				 G_preset.illparkpara.Area2[7].x = boost::lexical_cast<uint32_t>(sColVal);
 
			  }
			  else if("Area2y7" == sColName){
 
				 G_preset.illparkpara.Area2[7].y = boost::lexical_cast<uint32_t>(sColVal);
 
			  }// Areax[7]
			  else if("Area1x8" == sColName){
 
				 G_preset.illparkpara.Area1[8].x = boost::lexical_cast<uint32_t>(sColVal);
  
			  }
			   else if("Area1y8" == sColName){
 
				 G_preset.illparkpara.Area1[8].y = boost::lexical_cast<uint32_t>(sColVal);
 
			  }
			  else if("Area2x8" == sColName){
			  
				 G_preset.illparkpara.Area2[8].x = boost::lexical_cast<uint32_t>(sColVal);
 
			  }
			  else if("Area2y8" == sColName){
 
				 G_preset.illparkpara.Area2[8].y = boost::lexical_cast<uint32_t>(sColVal);
 
			  }// Areax[8]
			  else if("Area1x9" == sColName){
 
				 G_preset.illparkpara.Area1[9].x = boost::lexical_cast<uint32_t>(sColVal);
  
			  }
			   else if("Area1y9" == sColName){
 
				 G_preset.illparkpara.Area1[9].y = boost::lexical_cast<uint32_t>(sColVal);
 
			  }
			  else if("Area2x9" == sColName){
			  
				 G_preset.illparkpara.Area2[9].x = boost::lexical_cast<uint32_t>(sColVal);
 
			  }
			  else if("Area2y9" == sColName){
 
				 G_preset.illparkpara.Area2[9].y = boost::lexical_cast<uint32_t>(sColVal);
 
			  }// Areax[9]
			  else if("rectLeft" == sColName){
 
				 G_preset.rect.left = boost::lexical_cast<uint32_t>(sColVal);
 
			  }
			  else if("rectRight" == sColName){
 
				 G_preset.rect.right = boost::lexical_cast<uint32_t>(sColVal);
 
			  }
			  else if("rectBottom" == sColName){
 
				 G_preset.rect.bottom = boost::lexical_cast<uint32_t>(sColVal);
 
			  }
			  else if("rectTop" == sColName){
 
				 G_preset.rect.top = boost::lexical_cast<uint32_t>(sColVal);

			  	 printf("rrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrectTop = %d, G_SDC.m_sdcCfg.missionNum = %d\r\n", G_preset.rect.top,G_SDC.m_sdcCfg.missionNum );
				  
				 if(-1 == MissionAddPreset(&G_preset)){

					printf("-1 == G_SDC.m_sdcCfg.missionNum\r\n");
					G_missionUnstart.addPreset(&G_preset);
				 }
				 
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
     "camNum TEXT PRIMARY KEY     NOT NULL," \
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
	//CREATE TABLE preset(num INT NOT NULL,SDC_ID INT NOT NULL,holdSeconds INT NOT NULL,mode INT NOT NULL,priority INT NOT NULL);

	sql = "CREATE TABLE preset("  \
     "num           INT    NOT NULL," \
     "SDC_ID           TEXT    NOT NULL," \
     "holdSeconds           INT    NOT NULL," \
     "mode           INT    NOT NULL," \
     "priority            INT     NOT NULL,"\
     "plateMinWidth           INT    NOT NULL," \
     "plateMaxWidth           INT    NOT NULL," \
     "nNum1           INT    NOT NULL," \
     "nNum2           INT    NOT NULL," \
     "Area1x0           INT    NOT NULL," \
     "Area1y0           INT    NOT NULL," \
     "Area1x1           INT    NOT NULL," \
     "Area1y1           INT    NOT NULL," \
     "Area1x2           INT    NOT NULL," \
     "Area1y2           INT    NOT NULL," \
     "Area1x3           INT    NOT NULL," \
     "Area1y3           INT    NOT NULL," \
     "Area1x4           INT    NOT NULL," \
     "Area1y4           INT    NOT NULL," \
     "Area1x5           INT    NOT NULL," \
     "Area1y5           INT    NOT NULL," \
     "Area1x6           INT    NOT NULL," \
     "Area1y6           INT    NOT NULL," \
     "Area1x7           INT    NOT NULL," \
     "Area1y7           INT    NOT NULL," \
     "Area1x8           INT    NOT NULL," \
     "Area1y8           INT    NOT NULL," \
     "Area1x9           INT    NOT NULL," \
     "Area1y9           INT    NOT NULL," \
     "Area2x0           INT    NOT NULL," \
     "Area2y0           INT    NOT NULL," \
     "Area2x1           INT    NOT NULL," \
     "Area2y1           INT    NOT NULL," \
     "Area2x2           INT    NOT NULL," \
     "Area2y2           INT    NOT NULL," \
     "Area2x3           INT    NOT NULL," \
     "Area2y3           INT    NOT NULL," \
     "Area2x4           INT    NOT NULL," \
     "Area2y4           INT    NOT NULL," \
     "Area2x5           INT    NOT NULL," \
     "Area2y5           INT    NOT NULL," \
     "Area2x6           INT    NOT NULL," \
     "Area2y6           INT    NOT NULL," \
     "Area2x7           INT    NOT NULL," \
     "Area2y7           INT    NOT NULL," \
     "Area2x8           INT    NOT NULL," \
     "Area2y8           INT    NOT NULL," \
     "Area2x9           INT    NOT NULL," \
     "Area2y9           INT    NOT NULL," \
     "fScaleScreen           REAL    NOT NULL," \
     "rectLeft           INT    NOT NULL," \
	 "rectRight           INT    NOT NULL," \
	 "rectBottom           INT    NOT NULL," \
	 "rectTop           INT    NOT NULL," \
	 "photo           TEXT    NOT NULL);" ;

	
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

int8_t CSQL_conn::insert(SDCcfg *sdc)
{
 
   char *zErrMsg = 0;
   int rc;
   char sql[1024];


	sprintf(sql, "INSERT INTO SDCs (camNum,missionID,IP,port,usrName,pwd,RTSPaddr,brandName,productType,presetNum) "  \
			"VALUES ('%s', %d, '%s', %d, '%s', '%s', '%s', '%s', '%s', %d); " ,\
			sdc->camNum.c_str(), sdc->missionNum, sdc->IP.c_str(),\
			sdc->port, sdc->m_usrName.c_str(), sdc->m_pwd.c_str(), sdc->RTSPaddr.c_str(),\
			sdc->brandName.c_str(), sdc->productType.c_str(), sdc->presetTotal);
	printf("[SQL_conn::insertSDC]: %s\r\n", sql);
	  /* Execute SQL statement */
	rc = sqlite3_exec(handle, sql, callback, 0, &zErrMsg);
	if( rc != SQLITE_OK ){
	 fprintf(stderr, "[insert]:SQL insertSDC error: %s\n", zErrMsg);
	 sqlite3_free(zErrMsg);
	}else{
	 fprintf(stdout, "INSERT SDCs successfully\n");
	}


	for(int i=0; i<sdc->presetTotal; i++){

		sdc->missionPreset[i].rect.left = 178;
		sdc->missionPreset[i].rect.right = 1787;
		sdc->missionPreset[i].rect.bottom = 876;
		sdc->missionPreset[i].rect.top = 164;
		sdc->missionPreset[i].illparkpara.fScaleScreen = 2;

		sprintf(sql, "INSERT INTO preset (num,SDC_ID,holdSeconds,mode,priority,plateMinWidth,"\
		 "plateMaxWidth,nNum1,nNum2,Area1x0,Area1y0,Area1x1,Area1y1,Area1x2,Area1y2,"\
		 "Area1x3,Area1y3,Area1x4,Area1y4,Area1x5,Area1y5,Area1x6,Area1y6,Area1x7,Area1y7,"\
		 "Area1x8,Area1y8,Area1x9,Area1y9,"\
		 "Area2x0,Area2y0,Area2x1,Area2y1,Area2x2,Area2y2,"\
		 "Area2x3,Area2y3,Area2x4,Area2y4,Area2x5,Area2y5,Area2x6,Area2y6,Area2x7,Area2y7,"\
		 "Area2x8,Area2y8,Area2x9,Area2y9,"\
		 "fScaleScreen,rectLeft,rectRight,rectBottom,rectTop,photo) "  \
         "VALUES (%d,'%s',%d,%d,%d,%d,%d,%d,%d,"\
         "%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,"\
         "%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,"\
         "%0.2f,%d,%d,%d,%d,'NULL'); ",\
         sdc->missionPreset[i].presetNum , sdc->camNum.c_str(),\
         sdc->missionPreset[i].holdSeconds,sdc->missionPreset[i].mode,\
         sdc->missionPreset[i].priority,sdc->missionPreset[i].plateMinWidth,\
         sdc->missionPreset[i].plateMaxWidth,sdc->missionPreset[i].illparkpara.nNum1,\
         sdc->missionPreset[i].illparkpara.nNum2,\
         sdc->missionPreset[i].illparkpara.Area1[0].x,sdc->missionPreset[i].illparkpara.Area1[0].y,\
         sdc->missionPreset[i].illparkpara.Area1[1].x,sdc->missionPreset[i].illparkpara.Area1[1].y,\
         sdc->missionPreset[i].illparkpara.Area1[2].x,sdc->missionPreset[i].illparkpara.Area1[2].y,\
         sdc->missionPreset[i].illparkpara.Area1[3].x,sdc->missionPreset[i].illparkpara.Area1[3].y,\
         sdc->missionPreset[i].illparkpara.Area1[4].x,sdc->missionPreset[i].illparkpara.Area1[4].y,\
         sdc->missionPreset[i].illparkpara.Area1[5].x,sdc->missionPreset[i].illparkpara.Area1[5].y,\
         sdc->missionPreset[i].illparkpara.Area1[6].x,sdc->missionPreset[i].illparkpara.Area1[6].y,\
         sdc->missionPreset[i].illparkpara.Area1[7].x,sdc->missionPreset[i].illparkpara.Area1[7].y,\
         sdc->missionPreset[i].illparkpara.Area1[8].x,sdc->missionPreset[i].illparkpara.Area1[8].y,\
         sdc->missionPreset[i].illparkpara.Area1[9].x,sdc->missionPreset[i].illparkpara.Area1[9].y,\
         sdc->missionPreset[i].illparkpara.Area2[0].x,sdc->missionPreset[i].illparkpara.Area2[0].y,\
         sdc->missionPreset[i].illparkpara.Area2[1].x,sdc->missionPreset[i].illparkpara.Area2[1].y,\
         sdc->missionPreset[i].illparkpara.Area2[2].x,sdc->missionPreset[i].illparkpara.Area2[2].y,\
         sdc->missionPreset[i].illparkpara.Area2[3].x,sdc->missionPreset[i].illparkpara.Area2[3].y,\
         sdc->missionPreset[i].illparkpara.Area2[4].x,sdc->missionPreset[i].illparkpara.Area2[4].y,\
         sdc->missionPreset[i].illparkpara.Area2[5].x,sdc->missionPreset[i].illparkpara.Area2[5].y,\
         sdc->missionPreset[i].illparkpara.Area2[6].x,sdc->missionPreset[i].illparkpara.Area2[6].y,\
         sdc->missionPreset[i].illparkpara.Area2[7].x,sdc->missionPreset[i].illparkpara.Area2[7].y,\
         sdc->missionPreset[i].illparkpara.Area2[8].x,sdc->missionPreset[i].illparkpara.Area2[8].y,\
         sdc->missionPreset[i].illparkpara.Area2[9].x,sdc->missionPreset[i].illparkpara.Area2[9].y,\
         sdc->missionPreset[i].illparkpara.fScaleScreen,\
         sdc->missionPreset[i].rect.left,sdc->missionPreset[i].rect.right,\
         sdc->missionPreset[i].rect.bottom, sdc->missionPreset[i].rect.top);
		printf("[SQL_conn::insertSDC]: %s\r\n", sql);
	   /* Execute SQL statement */
	   rc = sqlite3_exec(handle, sql, callback, 0, &zErrMsg);
	   if( rc != SQLITE_OK ){
	      fprintf(stderr, "[insert]:SQL preset error: %s\n", zErrMsg);
	      sqlite3_free(zErrMsg);
	   }else{
	      fprintf(stdout, "INSERT SDCs preset successfully\n");
	   }

	}

   return 0;
}


int8_t CSQL_conn::insert(CCaptureDevice *capdev)
{
 
	char *zErrMsg = 0;
	int rc;
	char sql[1024];

	sprintf(sql, "INSERT INTO capDevs (devNum,missionID,IP,devType,MAC,devName) "  \
		"VALUES ('%s', %d, '%s', '%s', '%s', '%s'); ",\
		capdev->devNum.c_str() , capdev->missionNum, capdev->IP.c_str(), capdev->devType.c_str(),\
		capdev->MAC.c_str(), capdev->devName.c_str());
	/* Execute SQL statement */
	rc = sqlite3_exec(handle, sql, callback, 0, &zErrMsg);
	if( rc != SQLITE_OK ){
		
	 	 fprintf(stderr, "[insert]:SQL CaptureDevice error: %s\n", zErrMsg);
		 sqlite3_free(zErrMsg);
	
	}else{
		
	 	 fprintf(stdout, "INSERT CaptureDevice successfully\n");
	}


   return 0;
}

int8_t CSQL_conn::insert(CMission *mis)
{
 
	char *zErrMsg = 0;
	int rc;
	char sql[1024];

	sprintf(sql, "INSERT INTO missions (ID,SDC_ID,capDevID) "  \
		"VALUES (%d, 0, 0); ",\
		mis->missionNum );
	/* Execute SQL statement */
	rc = sqlite3_exec(handle, sql, callback, 0, &zErrMsg);
	if( rc != SQLITE_OK ){
		
	 	 fprintf(stderr, "[insert]:SQL mission error: %s\n", zErrMsg);
		 sqlite3_free(zErrMsg);
	
	}else{
		
	 	 fprintf(stdout, "INSERT mission successfully\n");
	}


   return 0;
}

int8_t CSQL_conn::update(CCaptureDevice *capdev)
{
 
	char *zErrMsg = 0;
	int rc;
	char sql[1024];

	sprintf(sql, "UPDATE capDevs SET missionID=%d,IP='%s',devType='%s',MAC='%s',devName='%s' "  \
		"where devNum='%s';",\
		capdev->missionNum, capdev->IP.c_str(), capdev->devType.c_str(),\
		capdev->MAC.c_str(), capdev->devName.c_str(), capdev->devNum.c_str());
	/* Execute SQL statement */
	rc = sqlite3_exec(handle, sql, callback, 0, &zErrMsg);
	if( rc != SQLITE_OK ){
		
	 	 fprintf(stderr, "[update]:SQL update CaptureDevice error: %s\n", zErrMsg);
		 sqlite3_free(zErrMsg);
	
	}else{
		
	 	 fprintf(stdout, "update CaptureDevice successfully\n");
	}
	


   return 0;
}

int8_t CSQL_conn::update(SDCcfg *sdc)
{

	char *zErrMsg = 0;
	int rc;
	char sql[1024];


	 sprintf(sql, "UPDATE SDCs SET missionID=%d,IP='%s',port=%d,usrName='%s',pwd='%s',RTSPaddr='%s',brandName='%s',productType='%s',presetNum=%d "  \
			 "WHERE camNum='%s'; " ,\
			 sdc->missionNum, sdc->IP.c_str(),\
			 sdc->port, sdc->m_usrName.c_str(), sdc->m_pwd.c_str(), sdc->RTSPaddr.c_str(),\
			 sdc->brandName.c_str(), sdc->productType.c_str(), sdc->presetTotal,sdc->camNum.c_str());
	 printf("[SQL_conn::UPDATE]: %s\r\n", sql);
	   /* Execute SQL statement */
	 rc = sqlite3_exec(handle, sql, callback, 0, &zErrMsg);
	 if( rc != SQLITE_OK ){
		fprintf(stderr, "[UPDATE]:SQL insertSDC error: %s\n", zErrMsg);
		sqlite3_free(zErrMsg);
	 }else{
		fprintf(stdout, "UPDATE SDCs successfully\n");
	 }
#if 0

	 for(int i=0; i<sdc->presetTotal; i++){
	 
	 
		 sprintf(sql, "UPDATE preset holdSeconds=%d,mode=%d,priority=%d "	\
		  "WHERE SDC_ID=%s and num=%d;", \
		  sdc->missionPreset[i].holdSeconds,\
		  sdc->missionPreset[i].mode, sdc->missionPreset[i].priority,\
		  sdc->camNum.c_str(), sdc->missionPreset[i].presetNum);
		 printf("[SQL_conn::UPDATE]: %s\r\n", sql);
		/* Execute SQL statement */
		rc = sqlite3_exec(handle, sql, callback, 0, &zErrMsg);
		if( rc != SQLITE_OK ){
		   fprintf(stderr, "[UPDATE]:SQL preset error: %s\n", zErrMsg);
		   sqlite3_free(zErrMsg);
		}else{
		   fprintf(stdout, "UPDATE SDCs preset successfully\n");
		}
 
	 }

#endif
}

int8_t CSQL_conn::update(missionGroupChgReq &req)
{

	char *zErrMsg = 0;
	int rc;
	char sql[1024];


	 sprintf(sql, "UPDATE SDCs SET missionID=%d "  \
			 "WHERE camNum='%s'; " ,\
			 req.dts_num, req.num.c_str());
	 printf("[SQL_conn::UPDATE]: %s\r\n", sql);
	   /* Execute SQL statement */
	 rc = sqlite3_exec(handle, sql, callback, 0, &zErrMsg);
	 if( rc != SQLITE_OK ){
		fprintf(stderr, "[UPDATE]:SQL insertSDC error: %s\n", zErrMsg);
		sqlite3_free(zErrMsg);
	 }else{
		fprintf(stdout, "UPDATE SDCs successfully\n");
	 }



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
