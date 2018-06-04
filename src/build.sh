#!/bin/sh

g++ client.cpp -o client -lboost_system -lboost_thread
g++ main.cpp -o server -lboost_system -lboost_thread
