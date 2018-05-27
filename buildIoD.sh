git pull
cd IoD
g++ -g -std=c++17 -pthread -lwiringPi `mysql_config --cflags` `mysql_config --libs`  mainIoInterface.cpp serialcmdinterface.cpp mysqlcon.cpp ArduIoInterface.cpp -o IoD
