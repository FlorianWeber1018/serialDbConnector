git pull
cd moduleFramework
g++ -g -std=c++11  `mysql_config --cflags` `mysql_config --libs` mainModuleFramework.cpp signalRouter.cpp module.cpp  util.cpp -o ModuleD
cd ..
