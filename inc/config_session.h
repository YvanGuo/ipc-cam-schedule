#pragma once  

#include <sys/socket.h>
#include <netinet/in.h>     //socket
#include <unistd.h>            //gethostname
#include <arpa/inet.h>
#include <string.h>            //memset

#include <cstdlib>
#include <iostream>
#include <stdexcept>
#include <boost/bind.hpp>
#include <boost/asio.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/thread.hpp>

#include "protocol.h"

using namespace std;
using boost::asio::ip::tcp;


boost::mutex cfg_cout_mtx;

class CConfigSession
{
public:
    CConfigSession(boost::asio::io_service& io_service)
        : socket_(io_service)
        , recv_times(0)
    {
    	memset(data_, 0 ,MAX_PACKET_LEN);
    }

    virtual ~CConfigSession()
    {
        boost::mutex::scoped_lock lock(cfg_cout_mtx);
    }

    tcp::socket& socket()
    {
        return socket_;
    }

    inline void start()
    {

        socket_.async_read_some(boost::asio::buffer(data_, MAX_PACKET_LEN),
            boost::bind(&CConfigSession::handle_read, this,
            boost::asio::placeholders::error,
            boost::asio::placeholders::bytes_transferred));
    }

    void handle_read(const boost::system::error_code& error, size_t bytes_transferred)
    {
        if (!error)
        {
            ++ recv_times;

			for(int i=0; i<bytes_transferred; i++){

				printf("%x,", data_[i]);
			}

			printf("\r\n");

		
		   #if 1
		   socket_.async_read_some(boost::asio::buffer(data_, 4096),
					   boost::bind(&CConfigSession::handle_read, this,
					   boost::asio::placeholders::error,
					   boost::asio::placeholders::bytes_transferred));
			#endif



        }
        else
        {
            delete this;
        }
    }

    void handle_write(const boost::system::error_code& error)
    {
        if (!error)
        {
            start();
        }
        else
        {
            delete this;
        }
    }

private:
    tcp::socket socket_;
    unsigned char data_[MAX_PACKET_LEN];
	char data_write[MAX_PACKET_LEN];
    int recv_times;
};


