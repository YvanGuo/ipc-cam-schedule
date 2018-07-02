#ifndef __SERVER_H__
#define __SERVER_H__


#include <sys/socket.h>
#include <netinet/in.h>     //socket
#include <unistd.h>            //gethostname
#include <arpa/inet.h>
#include <string.h>            //memset

typedef int SOCKET;
typedef struct sockaddr_in SOCKADDR_IN;
typedef struct sockaddr SOCKADDR;

#ifdef M_I386
typedef int socklen_t;
#endif

#define BOOL             int
#define INVALID_SOCKET    -1
#define SOCKET_ERROR     -1
#define TRUE             1
#define FALSE             0

#include <cstdlib>
#include <iostream>
#include <stdexcept>
#include <boost/bind.hpp>
#include <boost/asio.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/thread.hpp>



#include "session.h"
#include "service_pool.h"
#include "config_session.h"

using namespace std;
using boost::asio::ip::tcp;

class server
{
public:
    server(short port, int thread_cnt, int type)
        : io_service_pool_(thread_cnt)
        , acceptor_(io_service_pool_.get_io_service(), tcp::endpoint(tcp::v4(), port))
    {

		if(0 == type){

			CCaptureDevSession* new_session = new CCaptureDevSession(io_service_pool_.get_io_service());
        acceptor_.async_accept(new_session->socket(),
            boost::bind(&server::handle_accept, this, new_session, boost::asio::placeholders::error));
		
		}else{

			CConfigSession* new_session = new CConfigSession(io_service_pool_.get_io_service());
        acceptor_.async_accept(new_session->socket(),
            boost::bind(&server::handle_accept_cfg, this, new_session, boost::asio::placeholders::error));
		}
        
    }

    void handle_accept(CCaptureDevSession* new_session, const boost::system::error_code& error)
    {
    
    	printf("[handle_accept]:new session connect\r\n");
		
        if (!error)
        {
            new_session->start();
			
			new_session = new CCaptureDevSession(io_service_pool_.get_io_service());
			acceptor_.async_accept(new_session->socket(),
			boost::bind(&server::handle_accept, this, new_session, boost::asio::placeholders::error));
        }
        else
        {
        	printf("[handle_accept]: invalid new session connect\r\n");
            delete new_session;
        }

		
       
	
	}

	void handle_accept_cfg(CConfigSession* new_session, const boost::system::error_code& error)
    {
        if (!error)
        {
            new_session->start();
        }
        else
        {
            delete new_session;
        }

		printf("[handle_accept_cfg]:new session connect\r\n");
		
        new_session = new CConfigSession(io_service_pool_.get_io_service());
        acceptor_.async_accept(new_session->socket(),
            boost::bind(&server::handle_accept_cfg, this, new_session, boost::asio::placeholders::error));

	
	}

    void run()
    {
        io_service_pool_.start();
        io_service_pool_.join();
    }

private:

    io_service_pool io_service_pool_;
    tcp::acceptor acceptor_;
	
};


#endif

