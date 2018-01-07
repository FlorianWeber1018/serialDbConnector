-- MySQL dump 10.16  Distrib 10.1.23-MariaDB, for debian-linux-gnueabihf (armv7l)
--
-- Host: localhost    Database: 
-- ------------------------------------------------------
-- Server version	10.1.23-MariaDB-9+deb9u1

/*!40101 SET @OLD_CHARACTER_SET_CLIENT=@@CHARACTER_SET_CLIENT */;
/*!40101 SET @OLD_CHARACTER_SET_RESULTS=@@CHARACTER_SET_RESULTS */;
/*!40101 SET @OLD_COLLATION_CONNECTION=@@COLLATION_CONNECTION */;
/*!40101 SET NAMES utf8mb4 */;
/*!40103 SET @OLD_TIME_ZONE=@@TIME_ZONE */;
/*!40103 SET TIME_ZONE='+00:00' */;
/*!40014 SET @OLD_UNIQUE_CHECKS=@@UNIQUE_CHECKS, UNIQUE_CHECKS=0 */;
/*!40014 SET @OLD_FOREIGN_KEY_CHECKS=@@FOREIGN_KEY_CHECKS, FOREIGN_KEY_CHECKS=0 */;
/*!40101 SET @OLD_SQL_MODE=@@SQL_MODE, SQL_MODE='NO_AUTO_VALUE_ON_ZERO' */;
/*!40111 SET @OLD_SQL_NOTES=@@SQL_NOTES, SQL_NOTES=0 */;

--
-- Current Database: `heizung`
--

CREATE DATABASE /*!32312 IF NOT EXISTS*/ `heizung` /*!40100 DEFAULT CHARACTER SET utf8mb4 */;

USE `heizung`;

--
-- Table structure for table `IoConfig`
--

DROP TABLE IF EXISTS `IoConfig`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `IoConfig` (
  `Config` int(10) unsigned NOT NULL,
  `PortType` varchar(100) NOT NULL,
  `TpEn` tinyint(1) DEFAULT NULL,
  `SensorEn` tinyint(1) DEFAULT NULL,
  `Sensortype` enum('PT1000','PT2000','2=n/a','3=n/a') DEFAULT NULL,
  `PinConfig` enum('ninvertInput','invertInput','ninvertOutput','invertOutput') DEFAULT NULL,
  PRIMARY KEY (`Config`,`PortType`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `IoConfig`
--

LOCK TABLES `IoConfig` WRITE;
/*!40000 ALTER TABLE `IoConfig` DISABLE KEYS */;
INSERT INTO `IoConfig` VALUES (0,'A',0,0,'PT1000',NULL),(0,'I',NULL,NULL,NULL,'ninvertInput'),(1,'A',1,0,'PT1000',NULL),(1,'I',NULL,NULL,NULL,'invertInput'),(2,'A',0,1,'PT1000',NULL),(2,'I',NULL,NULL,NULL,'ninvertOutput'),(3,'A',1,1,'PT1000',NULL),(3,'I',NULL,NULL,NULL,'invertOutput'),(4,'A',0,0,'PT2000',NULL),(5,'A',1,0,'PT2000',NULL),(6,'A',0,1,'PT2000',NULL),(7,'A',1,1,'PT2000',NULL),(8,'A',0,0,'2=n/a',NULL),(9,'A',1,0,'2=n/a',NULL),(10,'A',0,1,'2=n/a',NULL),(11,'A',1,1,'2=n/a',NULL),(12,'A',0,0,'3=n/a',NULL),(13,'A',1,0,'3=n/a',NULL),(14,'A',0,1,'3=n/a',NULL),(15,'A',1,1,'3=n/a',NULL);
/*!40000 ALTER TABLE `IoConfig` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `IoConfigValue`
--

DROP TABLE IF EXISTS `IoConfigValue`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `IoConfigValue` (
  `DeviceID` varchar(100) NOT NULL,
  `PortType` varchar(100) NOT NULL,
  `Port` varchar(100) NOT NULL,
  `Pin` varchar(100) NOT NULL,
  `targetConfig` int(10) unsigned DEFAULT NULL,
  `actualConfig` int(10) unsigned DEFAULT NULL,
  PRIMARY KEY (`DeviceID`,`PortType`,`Port`,`Pin`),
  KEY `IoConfigValue_IoConfig_FK` (`targetConfig`,`PortType`),
  KEY `IoConfigValueactual_IoConfig_FK` (`actualConfig`,`PortType`),
  CONSTRAINT `IoConfigValue_IoConfig_FK` FOREIGN KEY (`targetConfig`, `PortType`) REFERENCES `IoConfig` (`Config`, `PortType`),
  CONSTRAINT `IoConfigValue_IoValue_FK` FOREIGN KEY (`DeviceID`, `PortType`, `Port`, `Pin`) REFERENCES `IoValue` (`DeviceID`, `PortType`, `Port`, `Pin`),
  CONSTRAINT `IoConfigValueactual_IoConfig_FK` FOREIGN KEY (`actualConfig`, `PortType`) REFERENCES `IoConfig` (`Config`, `PortType`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `IoConfigValue`
--

LOCK TABLES `IoConfigValue` WRITE;
/*!40000 ALTER TABLE `IoConfigValue` DISABLE KEYS */;
INSERT INTO `IoConfigValue` VALUES ('/dev/ttyACM0','A','0','0',7,7),('/dev/ttyACM0','A','0','1',7,7),('/dev/ttyACM0','A','0','2',7,7),('/dev/ttyACM0','A','0','3',7,7),('/dev/ttyACM0','A','0','4',7,7),('/dev/ttyACM0','A','0','5',7,7),('/dev/ttyACM0','A','0','6',7,7),('/dev/ttyACM0','A','0','7',7,7),('/dev/ttyACM0','A','1','0',7,7),('/dev/ttyACM0','A','1','1',7,7),('/dev/ttyACM0','A','1','2',7,7),('/dev/ttyACM0','A','1','3',7,7),('/dev/ttyACM0','A','1','4',7,7),('/dev/ttyACM0','A','1','5',7,7),('/dev/ttyACM0','A','1','6',7,7),('/dev/ttyACM0','A','1','7',7,7),('/dev/ttyACM0','I','0','0',3,3),('/dev/ttyACM0','I','0','1',3,3),('/dev/ttyACM0','I','0','2',3,3),('/dev/ttyACM0','I','0','3',3,3),('/dev/ttyACM0','I','0','4',3,3),('/dev/ttyACM0','I','0','5',3,3),('/dev/ttyACM0','I','0','6',3,3),('/dev/ttyACM0','I','0','7',3,3),('/dev/ttyACM0','I','1','0',3,3),('/dev/ttyACM0','I','1','1',3,3),('/dev/ttyACM0','I','1','2',3,3),('/dev/ttyACM0','I','1','3',3,3),('/dev/ttyACM0','I','1','4',3,3),('/dev/ttyACM0','I','1','5',3,3),('/dev/ttyACM0','I','1','6',3,3),('/dev/ttyACM0','I','1','7',3,3),('/dev/ttyACM0','I','2','0',3,3),('/dev/ttyACM0','I','2','1',3,3),('/dev/ttyACM0','I','2','2',3,3),('/dev/ttyACM0','I','2','3',3,3),('/dev/ttyACM0','I','2','4',3,3),('/dev/ttyACM0','I','2','5',3,3),('/dev/ttyACM0','I','2','6',3,3),('/dev/ttyACM0','I','2','7',3,3),('/dev/ttyACM0','I','3','0',0,0),('/dev/ttyACM0','I','3','1',0,0),('/dev/ttyACM0','I','3','2',0,0),('/dev/ttyACM0','I','3','3',0,0),('/dev/ttyACM0','I','3','4',0,0),('/dev/ttyACM0','I','3','5',0,0),('/dev/ttyACM0','I','3','6',0,0),('/dev/ttyACM0','I','3','7',0,0),('/dev/ttyACM0','I','4','0',3,3),('/dev/ttyACM0','I','4','1',3,3),('/dev/ttyACM0','I','4','2',3,3),('/dev/ttyACM0','I','4','3',3,3),('/dev/ttyACM0','I','4','4',3,3),('/dev/ttyACM0','I','4','5',3,3),('/dev/ttyACM0','I','4','6',3,3),('/dev/ttyACM0','I','4','7',3,3);
/*!40000 ALTER TABLE `IoConfigValue` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `IoValue`
--

DROP TABLE IF EXISTS `IoValue`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `IoValue` (
  `DeviceID` varchar(100) NOT NULL,
  `PortType` varchar(100) NOT NULL,
  `Port` varchar(100) NOT NULL,
  `Pin` varchar(100) NOT NULL,
  `targetState` int(11) DEFAULT NULL,
  `actualState` int(11) NOT NULL,
  `UserDescription` varchar(100) DEFAULT NULL,
  PRIMARY KEY (`DeviceID`,`PortType`,`Port`,`Pin`),
  CONSTRAINT `IoValue_IoConfigValue_FK` FOREIGN KEY (`DeviceID`, `PortType`, `Port`, `Pin`) REFERENCES `IoConfigValue` (`DeviceID`, `PortType`, `Port`, `Pin`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `IoValue`
--

LOCK TABLES `IoValue` WRITE;
/*!40000 ALTER TABLE `IoValue` DISABLE KEYS */;
INSERT INTO `IoValue` VALUES ('/dev/ttyACM0','A','0','0',NULL,38,'Radiatoren Vorlauf'),('/dev/ttyACM0','A','0','1',NULL,30,'Puffer Unten'),('/dev/ttyACM0','A','0','2',NULL,57,'Holzkessel'),('/dev/ttyACM0','A','0','3',NULL,109,'DUMMY rauchgas'),('/dev/ttyACM0','A','0','4',NULL,6,'Außenfühler'),('/dev/ttyACM0','A','0','5',NULL,111,'LEER'),('/dev/ttyACM0','A','0','6',NULL,111,'LEER'),('/dev/ttyACM0','A','0','7',NULL,111,'LEER'),('/dev/ttyACM0','A','1','0',NULL,111,'LEER'),('/dev/ttyACM0','A','1','1',NULL,111,'LEER'),('/dev/ttyACM0','A','1','2',NULL,111,'LEER'),('/dev/ttyACM0','A','1','3',NULL,111,'LEER'),('/dev/ttyACM0','A','1','4',NULL,111,'LEER'),('/dev/ttyACM0','A','1','5',NULL,111,'LEER'),('/dev/ttyACM0','A','1','6',NULL,111,'LEER'),('/dev/ttyACM0','A','1','7',NULL,111,'LEER'),('/dev/ttyACM0','I','0','0',1,1,NULL),('/dev/ttyACM0','I','0','1',0,0,NULL),('/dev/ttyACM0','I','0','2',0,0,NULL),('/dev/ttyACM0','I','0','3',0,0,NULL),('/dev/ttyACM0','I','0','4',0,0,NULL),('/dev/ttyACM0','I','0','5',0,0,NULL),('/dev/ttyACM0','I','0','6',0,0,NULL),('/dev/ttyACM0','I','0','7',0,0,NULL),('/dev/ttyACM0','I','1','0',0,0,NULL),('/dev/ttyACM0','I','1','1',0,0,NULL),('/dev/ttyACM0','I','1','2',0,0,NULL),('/dev/ttyACM0','I','1','3',0,0,NULL),('/dev/ttyACM0','I','1','4',0,0,NULL),('/dev/ttyACM0','I','1','5',0,0,NULL),('/dev/ttyACM0','I','1','6',0,0,NULL),('/dev/ttyACM0','I','1','7',0,0,NULL),('/dev/ttyACM0','I','2','0',0,0,NULL),('/dev/ttyACM0','I','2','1',0,0,NULL),('/dev/ttyACM0','I','2','2',0,0,NULL),('/dev/ttyACM0','I','2','3',0,0,NULL),('/dev/ttyACM0','I','2','4',0,0,NULL),('/dev/ttyACM0','I','2','5',0,0,NULL),('/dev/ttyACM0','I','2','6',0,0,NULL),('/dev/ttyACM0','I','2','7',1,1,'Heizkreispumpe'),('/dev/ttyACM0','I','3','0',0,1,NULL),('/dev/ttyACM0','I','3','1',0,1,NULL),('/dev/ttyACM0','I','3','2',0,1,NULL),('/dev/ttyACM0','I','3','3',0,1,NULL),('/dev/ttyACM0','I','3','4',0,1,NULL),('/dev/ttyACM0','I','3','5',0,1,NULL),('/dev/ttyACM0','I','3','6',0,1,NULL),('/dev/ttyACM0','I','3','7',0,1,NULL),('/dev/ttyACM0','I','4','0',0,0,NULL),('/dev/ttyACM0','I','4','1',0,0,NULL),('/dev/ttyACM0','I','4','2',0,0,NULL),('/dev/ttyACM0','I','4','3',0,0,NULL),('/dev/ttyACM0','I','4','4',0,0,NULL),('/dev/ttyACM0','I','4','5',0,0,NULL),('/dev/ttyACM0','I','4','6',0,0,NULL),('/dev/ttyACM0','I','4','7',0,0,NULL);
/*!40000 ALTER TABLE `IoValue` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `infodata`
--

DROP TABLE IF EXISTS `infodata`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `infodata` (
  `id` bigint(20) unsigned NOT NULL AUTO_INCREMENT,
  `key` varchar(100) DEFAULT NULL,
  `value` varchar(100) DEFAULT NULL,
  PRIMARY KEY (`id`)
) ENGINE=InnoDB AUTO_INCREMENT=2 DEFAULT CHARSET=utf8mb4;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `infodata`
--

LOCK TABLES `infodata` WRITE;
/*!40000 ALTER TABLE `infodata` DISABLE KEYS */;
INSERT INTO `infodata` VALUES (1,'temp1','22');
/*!40000 ALTER TABLE `infodata` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `module_tempDiff`
--

DROP TABLE IF EXISTS `module_tempDiff`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `module_tempDiff` (
  `ID` int(10) unsigned NOT NULL AUTO_INCREMENT,
  `I0_DeviceID` varchar(100) DEFAULT NULL,
  `I0_PortType` varchar(100) DEFAULT NULL,
  `I0_Port` varchar(100) DEFAULT NULL,
  `I0_Pin` varchar(100) DEFAULT NULL,
  `I1_DeviceID` varchar(100) DEFAULT NULL,
  `I1_PortType` varchar(100) DEFAULT NULL,
  `I1_Port` varchar(100) DEFAULT NULL,
  `I1_Pin` varchar(100) DEFAULT NULL,
  `sigOut` int(11) DEFAULT '0',
  `I0_Min` int(11) DEFAULT '50',
  `I0_Max` int(11) DEFAULT '110',
  `I1_Min` int(11) DEFAULT '20',
  `I1_Max` int(11) DEFAULT '110',
  `dTheta_ON` int(11) DEFAULT '8',
  `dTheta_OFF` int(11) DEFAULT '4',
  `InvertOutput` tinyint(1) DEFAULT '0',
  `Enable` tinyint(1) DEFAULT '1',
  `UpdateRequired` tinyint(1) NOT NULL DEFAULT '1',
  PRIMARY KEY (`ID`)
) ENGINE=InnoDB AUTO_INCREMENT=2 DEFAULT CHARSET=utf8mb4;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `module_tempDiff`
--

LOCK TABLES `module_tempDiff` WRITE;
/*!40000 ALTER TABLE `module_tempDiff` DISABLE KEYS */;
INSERT INTO `module_tempDiff` VALUES (1,'/dev/ttyACM0','A','0','2','/dev/ttyACM0','A','0','1',NULL,50,110,-150,110,8,4,0,1,1);
/*!40000 ALTER TABLE `module_tempDiff` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `sessions`
--

DROP TABLE IF EXISTS `sessions`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `sessions` (
  `id` bigint(20) unsigned NOT NULL AUTO_INCREMENT,
  `token` varchar(100) DEFAULT NULL,
  `userid` bigint(20) unsigned DEFAULT NULL,
  `info` varchar(100) DEFAULT NULL,
  `creationDate` datetime DEFAULT NULL,
  `deletionDate` datetime DEFAULT NULL,
  PRIMARY KEY (`id`),
  KEY `sessions_users_FK` (`userid`),
  CONSTRAINT `sessions_users_FK` FOREIGN KEY (`userid`) REFERENCES `users` (`id`)
) ENGINE=InnoDB AUTO_INCREMENT=248 DEFAULT CHARSET=utf8mb4;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `sessions`
--

LOCK TABLES `sessions` WRITE;
/*!40000 ALTER TABLE `sessions` DISABLE KEYS */;
INSERT INTO `sessions` VALUES (234,'rNViwz0cmMaabeLn',1,'','0000-00-00 00:00:00','0000-00-00 00:00:00'),(235,'ueNa6uiT8HiBcrvI',1,'','0000-00-00 00:00:00','0000-00-00 00:00:00'),(236,'rYYKVM0Qnoa0Db1P',1,'','0000-00-00 00:00:00','0000-00-00 00:00:00'),(237,'R2FRmWwtCrVC7QrY',1,'','0000-00-00 00:00:00','0000-00-00 00:00:00'),(238,'wWsl8VsbFfJusVt8',1,'','0000-00-00 00:00:00','0000-00-00 00:00:00'),(239,'FDvwqun3WJKo1RS8',1,'','0000-00-00 00:00:00','0000-00-00 00:00:00'),(240,'dOZtCnzAHfOBUZxO',1,'','0000-00-00 00:00:00','0000-00-00 00:00:00'),(241,'ZGBgr2VW0PG57Vet',1,'','0000-00-00 00:00:00','0000-00-00 00:00:00'),(242,'kzI1SABbC7FWC07K',1,'','0000-00-00 00:00:00','0000-00-00 00:00:00'),(243,'PpqZdzeZwI7848B8',1,'','0000-00-00 00:00:00','0000-00-00 00:00:00'),(244,'PybtT3tXGWbVa4Zt',1,'','0000-00-00 00:00:00','0000-00-00 00:00:00'),(245,'Gdkb6IP7mNNq7rHr',1,'','0000-00-00 00:00:00','0000-00-00 00:00:00'),(246,'4Toy4WyYFO2f0ETk',1,'','0000-00-00 00:00:00','0000-00-00 00:00:00'),(247,'viHoyDPUC8oD13qt',1,'','0000-00-00 00:00:00','0000-00-00 00:00:00');
/*!40000 ALTER TABLE `sessions` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `users`
--

DROP TABLE IF EXISTS `users`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `users` (
  `id` bigint(20) unsigned NOT NULL AUTO_INCREMENT,
  `username` varchar(100) DEFAULT NULL,
  `password` varchar(100) DEFAULT NULL,
  `rights` varchar(100) DEFAULT NULL,
  `lastlogin` varchar(100) DEFAULT NULL,
  PRIMARY KEY (`id`)
) ENGINE=InnoDB AUTO_INCREMENT=4 DEFAULT CHARSET=utf8mb4;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `users`
--

LOCK TABLES `users` WRITE;
/*!40000 ALTER TABLE `users` DISABLE KEYS */;
INSERT INTO `users` VALUES (1,'knapp','heizung','all','d'),(2,'view','test','view',''),(3,'flo','mango','all',NULL);
/*!40000 ALTER TABLE `users` ENABLE KEYS */;
UNLOCK TABLES;
/*!40103 SET TIME_ZONE=@OLD_TIME_ZONE */;

/*!40101 SET SQL_MODE=@OLD_SQL_MODE */;
/*!40014 SET FOREIGN_KEY_CHECKS=@OLD_FOREIGN_KEY_CHECKS */;
/*!40014 SET UNIQUE_CHECKS=@OLD_UNIQUE_CHECKS */;
/*!40101 SET CHARACTER_SET_CLIENT=@OLD_CHARACTER_SET_CLIENT */;
/*!40101 SET CHARACTER_SET_RESULTS=@OLD_CHARACTER_SET_RESULTS */;
/*!40101 SET COLLATION_CONNECTION=@OLD_COLLATION_CONNECTION */;
/*!40111 SET SQL_NOTES=@OLD_SQL_NOTES */;

-- Dump completed on 2017-12-28  9:55:26
