bin_PROGRAMS  = ipc-server

ipc_server_SOURCES  = ipc-server.cpp HIKCam.cpp SpeedDomeCam.cpp protocol.cpp mission.cpp mission_config.cpp CaptureDevice.cpp sql_conn.cpp 

ipc_server_CPPFLAGS = -I"$(top_srcdir)/inc" \
				-I"$(top_srcdir)/inc/incCn" \
				-I"$(BUILD_ROOT_PATH)/usr/include/glib-2.0" \
				-I"$(BUILD_ROOT_PATH)/usr/include/gio-unix-2.0" \
				-I/usr/include/mysql -g  -m64 -fPIC  -g -fabi-version=2 -fno-omit-frame-pointer -fno-strict-aliasing

ipc_server_LDFLAGS  = -L$(BUILD_ROOT_PATH)/usr/lib \
		-L/usr/local/lib64/HCNetSDKCom \
		-L/usr/lib64 \
		-L/usr/lib \
		-L/usr/lib64/mysql \
		-L"$(top_srcdir)/lib" \
				-Wl,-rpath=/lib \
				-Wl,-rpath=/usr/lib \
				-Wl,-rpath=/usr/local/lib \
				-Wl,-rpath=/usr/local/lib64 \
				-Wl,-rpath=/usr/local/lib64/HCNetSDKCom

ipc_server_LDADD    = -lhcnetsdk \
			-lhpr -lHCCore -lHCCoreDevCfg \
			-lStreamTransClient \
			-lSystemTransform -lHCPreview \
			-lHCAlarm -lHCGeneralCfgMgr \
			-lHCIndustry -lHCPlayBack \
			-lHCVoiceTalk -lanalyzedata -lHCDisplay \
			-lboost_system -lboost_thread \
			-lmyodbc8w -lmysqlclient \
			-lodbc -lodbcinst -lm -ldl  -lsqlite3 -lpthread
