cd IoD
g++ -std=c++11 -pthread -lwiringPi `mysql_config --cflags` `mysql_config --libs`  mainIoInterface.cpp serialcmdinterface.cpp mysqlcon.cpp ArduIoInterface.cpp -o IoD
