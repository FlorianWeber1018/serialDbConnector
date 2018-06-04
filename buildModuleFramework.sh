git pull
cd moduleFramework
g++ -g -std=c++17  `mysql_config --cflags` `mysql_config --libs` mainModuleFramework.cpp signalRouter.cpp paramRouter.cpp module.cpp modConMan.cpp util.cpp clock.cpp control.cpp timer.cpp ../IoD/mysqlcon.cpp -o ModuleD
cd ..
