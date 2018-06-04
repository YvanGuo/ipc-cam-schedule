//
// Tcp_message.hpp
// ~~~~~~~~~~~~~~~~
//
// Copyright (c) 2003-2015 Christopher M. Kohlhoff (chris at kohlhoff dot com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//

#ifndef TCP_MESSAGE_HPP
#define TCP_MESSAGE_HPP

#include <cstdio>
#include <cstdlib>
#include <cstring>

class Tcp_message
{
  public:

  Tcp_message(char *framebuff, size_t len)
  {

	using namespace std;
	data_ = (char *)malloc(len);
  	memcpy(data_, framebuff, len);
	data_length_ = len;
	
  }
  ~Tcp_message()
  {

	//free(data_);
  }

  const char* data() const
  {
    return data_;
  }

  char* data()
  {
    return data_;
  }

  size_t length() const
  {
    return data_length_;
  }

 

public:
  char *data_;
  size_t data_length_;
};

#endif // Tcp_MESSAGE_HPP  



