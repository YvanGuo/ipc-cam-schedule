//
// Tcp_server.cpp
// ~~~~~~~~~~~~~~~
//

#include <algorithm>
#include <cstdlib>
#include <deque>
#include <iostream>
#include <list>
#include <set>
/*
#include <boost/bind.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/enable_shared_from_this.hpp>
#include <boost/asio.hpp>
*/
#include "tcp_message.hpp"
#include <signal.h>
#include <time.h>
#include <sys/time.h>
#include "boost/bind.hpp"
#include "boost/shared_ptr.hpp"
#include "boost/enable_shared_from_this.hpp"
#include "boost/asio.hpp"
#include "boost/thread/thread.hpp"

using boost::asio::ip::tcp;


//----------------------------------------------------------------------

typedef std::deque<Tcp_message *> Tcp_message_queue;

class Tcp_session
    :public boost::enable_shared_from_this<Tcp_session>
{
public:
  Tcp_session(boost::asio::io_service& io_service)
    : socket_(io_service)
      
  {
  		strcpy(heartbeat, "heart beat\n");
  }

  tcp::socket& socket()
  {
    return socket_;
  }

  ~Tcp_session()
  {

		printf("delete a session\n");
  }

  void start()
  {

	printf("start \n");
    boost::asio::async_write(socket_,
        boost::asio::buffer(heartbeat, 128),
        boost::bind(
          &Tcp_session::handle_write, shared_from_this(),
          boost::asio::placeholders::error));
	
  }

  void Post(char *buff, int size){

		printf("Post \n");
		bool empty;
		
		Tcp_message *c = new Tcp_message(buff, size);
		empty = write_msgs_.empty();
		write_msgs_.push_front(c);

		if(empty){

			boost::asio::async_write(socket_,
					boost::asio::buffer(write_msgs_.front()->data(), write_msgs_.front()->length()),
					boost::bind(&Tcp_session::handle_write, shared_from_this(),
					  boost::asio::placeholders::error));

		}
		
  }

  void CloseSocket()
  {

	printf("------------CloseSocket close a sock\n");

	closed = true;

	socket_.close();
  }
  void handle_read(const boost::system::error_code& error )
  {
    if (!error)
    {
    printf("handle_read \n");
      boost::asio::async_read(socket_,
          boost::asio::buffer(heartbeat, 128),
          boost::bind(&Tcp_session::handle_read, shared_from_this(),
            boost::asio::placeholders::error));
    }
    else
    {
     // room_.leave(shared_from_this());
    }
  }


  void handle_write(const boost::system::error_code& error)
  {
    if (!error)
    {


      write_msgs_.pop_front();

      if (!write_msgs_.empty())
      {
      	printf("handle_write1 \n");
        boost::asio::async_write(socket_,
            boost::asio::buffer(write_msgs_.front()->data(), write_msgs_.front()->length()),
            boost::bind(&Tcp_session::handle_write, shared_from_this(),
              boost::asio::placeholders::error));
		printf("handle_write2 \n");
      }
    }
    else
    {
      CloseSocket();
    }
  }

public:
  tcp::socket socket_;
  Tcp_message_queue write_msgs_;

public:
  char *readbuff;
  char heartbeat[128];
  int framSize;
  int data_ready;
  bool closed;
};

typedef boost::shared_ptr<Tcp_session> Tcp_session_ptr;
typedef std::list<Tcp_session_ptr> Tcp_session_list;


//----------------------------------------------------------------------

class Tcp_server
{
public:
  Tcp_server(boost::asio::io_service& io_service,
      const tcp::endpoint& endpoint)
    : io_service_(io_service),
      acceptor_(io_service, endpoint)
  {
    start_accept();
  }

  void Post(char *buff, int size){

		if(!list.empty()){
			for(Tcp_session_it=list.begin();Tcp_session_it!=list.end();Tcp_session_it++){

				Tcp_session_ptr it = *Tcp_session_it;

				if(it->closed){

					printf("Post ------------ close a sock\n");
					Tcp_session_it = list.erase(Tcp_session_it);
				
				}else{

					it->Post(buff, size);
				}
				

			} 

			printf("list has %d sessions\n", list.size());
		}

		

  }
  void start_accept()
  {
    
	Tcp_session_ptr new_session(new Tcp_session(io_service_));
	
	new_session->closed = false;
    acceptor_.async_accept(new_session->socket(),
        boost::bind(&Tcp_server::handle_accept, this, new_session,
          boost::asio::placeholders::error));
  }

  void handle_accept(Tcp_session_ptr session,
      const boost::system::error_code& error)
  {

  	printf("new handle\n");
    if (!error)
    {

      list.push_back(session);
	  
    }

    start_accept();
  }

public:
  boost::asio::io_service& io_service_;
  tcp::acceptor acceptor_;
  Tcp_session_list list;
  std::list<Tcp_session_ptr>::iterator Tcp_session_it;
  bool empty;

};

typedef boost::shared_ptr<Tcp_server> Tcp_server_ptr;
typedef std::list<Tcp_server_ptr> Tcp_server_list;

//----------------------------------------------------------------------
Tcp_server_list servers;
boost::asio::io_service io_service;

char heartbeat[128] = "hello\n";


void sgn_sys(int a)
{
	static int initted = 0;
	int port = 9001;

	if(0 == initted){
		//Tcp_server_init((void *)&port);
		//if(0 != pthread_create(&th_tcp_srv, NULL, Tcp_server_init, (void *)&port)){

	 	 //   printf("pthread_create SDCard_detection erro!\n");

	   // }
	    
		initted = 1;
	}

	servers.front()->Post(heartbeat, 8);
}
Tcp_server_list Tcp_server_init(int vport)
{
	int port = vport;
	char heartbeat[128] = "hello world!\n";
	struct itimerval value, ovalue, value2;

  try
  {

    

    using namespace std; // For atoi.
    tcp::endpoint endpoint(tcp::v4(), port);
    Tcp_server_ptr server(new Tcp_server(io_service, endpoint));

    servers.push_back(server);

 	boost::thread t(boost::bind(&boost::asio::io_service::run, &io_service));
 	/*
 	 signal(SIGALRM, sgn_sys);
	value.it_value.tv_sec = 1;
	value.it_value.tv_usec = 0;
	value.it_interval.tv_sec = 1;
	value.it_interval.tv_usec = 0;
	setitimer(ITIMER_REAL, &value, &ovalue); 
	
	io_service.run();
   
*/
   

  }
  catch (std::exception& e)
  {
    std::cerr << "Exception: " << e.what() << "\n";
  }

  return servers;
}
