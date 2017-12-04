 g++ -std=c++11 -pthread -lwiringPi `mysql_config --cflags` `mysql_config --libs`  main.cpp serialcmdinterface.cpp mysqlcon.cpp -o test
