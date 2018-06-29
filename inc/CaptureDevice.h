#pragma once  

#include "protocol.h"
#include "SpeedDomeCam.h"
#include "CaptureDevice.h"

#include <boost/bind.hpp>
#include <boost/asio.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/thread.hpp>
#include <list> 
#include <map> 

#include <boost/thread/mutex.hpp>  
using namespace std;  
using namespace boost;  
using boost::asio::ip::tcp;


typedef enum STATUS{

	MISSION_FAILD = 0,
	MISSION_COMPLETED,
	MISSION_PROCESSING,
	MISSION_READY

}STATUS;

class CCaptureDevice{

public:
	CCaptureDevice(){

		isOnline = 0;
		missionStatus = MISSION_READY;
		memset(&loginInfo, 0, sizeof(LoginInfo));
		//memset(&m_sdcCfg, 0, sizeof(SDCcfg));

	}
	
	int8_t start();
	int8_t stop();
	int8_t reload_Preset();
	int8_t threeD_Request(ThreeDirReq &tdr);

	
	int8_t init(tcp::socket *socket_, LoginInfo *para_loginInfo);
private:
	
	int8_t _start();

	void MissoinLoop();

	void handle_write(const boost::system::error_code& error);
	
public:
	int8_t isOnline;
	LoginInfo loginInfo;
	int32_t missionNum;
	int8_t missionStatus;
	string devType;
	string IP;
	string MAC;
	string devNum;
	string devName;
	string curCamNum;
//	SDCcfg m_sdcCfg;
	
	boost::shared_ptr<CSpeedDomeCam> SpdDomeCam;
		
	boost::shared_ptr<boost::asio::deadline_timer> timer;

	tcp::socket *m_socket;

	int8_t run;
	CProtocol protocol;

//	char data_write[4096];
	
};

