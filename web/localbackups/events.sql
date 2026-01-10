-- MySQL dump 10.13  Distrib 8.4.6, for Linux (x86_64)
--
-- Host: localhost    Database: dominion
-- ------------------------------------------------------
-- Server version	8.4.6

/*!40101 SET @OLD_CHARACTER_SET_CLIENT=@@CHARACTER_SET_CLIENT */;
/*!40101 SET @OLD_CHARACTER_SET_RESULTS=@@CHARACTER_SET_RESULTS */;
/*!40101 SET @OLD_COLLATION_CONNECTION=@@COLLATION_CONNECTION */;
/*!50503 SET NAMES utf8mb4 */;
/*!40103 SET @OLD_TIME_ZONE=@@TIME_ZONE */;
/*!40103 SET TIME_ZONE='+00:00' */;
/*!40014 SET @OLD_UNIQUE_CHECKS=@@UNIQUE_CHECKS, UNIQUE_CHECKS=0 */;
/*!40014 SET @OLD_FOREIGN_KEY_CHECKS=@@FOREIGN_KEY_CHECKS, FOREIGN_KEY_CHECKS=0 */;
/*!40101 SET @OLD_SQL_MODE=@@SQL_MODE, SQL_MODE='NO_AUTO_VALUE_ON_ZERO' */;
/*!40111 SET @OLD_SQL_NOTES=@@SQL_NOTES, SQL_NOTES=0 */;

--
-- Table structure for table `events`
--

DROP TABLE IF EXISTS `events`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!50503 SET character_set_client = utf8mb4 */;
CREATE TABLE `events` (
  `id` int unsigned NOT NULL AUTO_INCREMENT,
  `event_type_id` int unsigned NOT NULL,
  `expansion_id` int unsigned NOT NULL,
  `setup_id` int unsigned DEFAULT NULL,
  `name` varchar(255) NOT NULL,
  `name_en` varchar(255) DEFAULT NULL,
  `prize` int unsigned DEFAULT '0',
  `debt` tinyint(1) DEFAULT '0',
  `curses` tinyint(1) DEFAULT '0',
  `imagename` varchar(255) DEFAULT NULL,
  PRIMARY KEY (`id`)
) ENGINE=InnoDB AUTO_INCREMENT=64 DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_0900_ai_ci;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `events`
--

LOCK TABLES `events` WRITE;
/*!40000 ALTER TABLE `events` DISABLE KEYS */;
INSERT INTO `events` VALUES (1,1,11,NULL,'Valloitus','Conquest',6,0,0,'Conquest.jpg'),(2,1,11,NULL,'Juomingit','Banquet',3,0,0,'Banquet.jpg'),(3,1,11,NULL,'Nöyryytys','Dominate',14,0,0,'Dominate.jpg'),(4,1,11,NULL,'Rituaali','Ritual',4,0,1,'Ritual.jpg'),(5,1,11,27,'Verotus','Tax',2,0,0,'Tax.jpg'),(6,1,11,NULL,'Lahjoitus','Donate',8,1,0,'Donate.jpg'),(7,1,11,NULL,'Häät','Wedding',7,1,0,'Wedding.jpg'),(8,1,11,NULL,'Voitonjuhla','Triumph',5,1,0,'Triumph.jpg'),(9,1,11,NULL,'Pakkoliitos','Annex',8,1,0,'Annex.jpg'),(10,1,11,NULL,'Onnenkantamoinen','Windfall',5,0,0,'Windfall.jpg'),(11,1,11,NULL,'Kaivanto','Delve',2,0,0,'Delve.jpg'),(12,1,11,NULL,'Ylennys','Advance',0,0,0,'Advance.jpg'),(13,1,11,NULL,'Myrkytetty Maa','Salt the Earth',4,0,0,'Salt_the_Earth.jpg'),(14,1,18,NULL,'Continue','Continue',8,1,0,'Continue.jpg'),(15,1,18,NULL,'Amass','Amass',2,0,0,'Amass.jpg'),(16,1,18,NULL,'Asceticism','Asceticism',2,0,0,'Asceticism.jpg'),(17,1,18,33,'Credit','Credit',2,0,0,'Credit.jpg'),(18,1,18,NULL,'Foresight','Foresight',2,0,0,'Foresight.jpg'),(19,1,18,NULL,'Kintsugi','Kintsugi',3,0,0,'Kintsugi.jpg'),(20,1,18,NULL,'Practice','Practice',3,0,0,'Practice.jpg'),(21,1,18,NULL,'Sea Trade','Sea Trade',4,0,0,'Sea_Trade.jpg'),(22,1,18,NULL,'Receive Tribute','Receive Tribute',5,0,0,'Receive_Tribute.jpg'),(23,1,18,NULL,'Gather','Gather',7,0,0,'Gather.jpg'),(24,1,14,NULL,'Delay','Delay',0,0,0,'Delay.jpg'),(25,1,14,NULL,'Desperation','Desperation',0,0,1,'Desperation.jpg'),(26,1,14,NULL,'Gamble','Gamble',2,0,0,'Gamble.jpg'),(27,1,14,NULL,'Pursue','Pursue',2,0,0,'Pursue.jpg'),(28,1,14,NULL,'Toil','Toil',2,0,0,'Toil.jpg'),(29,1,14,26,'Ride','Ride',2,0,0,'Ride.jpg'),(30,1,14,NULL,'Enhance','Enhance',3,0,0,'Enhance.jpg'),(31,1,14,NULL,'March','March',3,0,0,'March.jpg'),(32,1,14,25,'Transport','Transport',3,0,0,'Transport.jpg'),(33,1,14,25,'Banish','Banish',4,0,0,'Banish.jpg'),(34,1,14,26,'Bargain','Bargain',4,0,0,'Bargain.jpg'),(35,1,14,25,'Invest','Invest',4,0,0,'Invest.jpg'),(36,1,14,NULL,'Seize The Day','Seize The Day',4,0,0,'Seize_the_Day.jpg'),(37,1,14,NULL,'Commerce','Commerce',5,0,0,'Commerce.jpg'),(38,1,14,26,'Demand','Demand',5,0,0,'Demand.jpg'),(39,1,14,26,'Stampede','Stampede',5,0,0,'Stampede.jpg'),(40,1,14,NULL,'Reap','Reap',7,0,0,'Reap.jpg'),(41,1,14,25,'Enclave','Enclave',8,0,0,'Enclave.jpg'),(42,1,14,NULL,'Alliance','Alliance',10,0,0,'Alliance.jpg'),(43,1,14,NULL,'Populate','Populate',10,0,0,'Populate.jpg'),(44,1,10,21,'Pyhiinvaellus','Pilgrimage',4,0,0,'Pilgrimage.jpg'),(45,1,10,NULL,'Almut','Alms',0,0,0,'Alms.jpg'),(46,1,10,24,'Tanssiaiset','Ball',5,0,0,'Ball.jpg'),(47,1,10,NULL,'Kokko','Bonfire',3,0,0,'Bonfire.jpg'),(48,1,10,24,'Vippi','Borrow',0,0,0,'Borrow.jpg'),(49,1,10,NULL,'Tutkimusmatka','Expedition',3,0,0,'Expedition.jpg'),(50,1,10,34,'Lautta','Ferry',3,0,0,'Ferry.jpg'),(51,1,10,35,'Perintö','Inheritance',7,0,0,'Inheritance.jpg'),(52,1,10,39,'Merireitti','Seaway',5,0,0,'Seaway.jpg'),(53,1,10,36,'Unohdetut Kyvyt','Lost Arts',6,0,0,'Lost_Arts.jpg'),(54,1,10,37,'Reitin Etsintä','Pathfinding',8,0,0,'Pathfinding.jpg'),(55,1,10,38,'Suunnitelma','Plan',3,0,0,'Plan.jpg'),(56,1,10,NULL,'Tehtävä','Mission',4,0,0,'Mission.jpg'),(57,1,10,NULL,'Kiertelevät Markkinat','Travelling Fair',2,0,0,'Travelling_Fair.jpg'),(58,1,10,40,'Koulutus','Training',6,0,0,'Training.jpg'),(59,1,10,NULL,'Vaihtokauppa','Trade',5,0,0,'Trade.jpg'),(60,1,10,NULL,'Tiedustelupartio','Scouting Party',2,0,0,'Scouting_Party.jpg'),(61,1,10,NULL,'Säästöt','Save',1,0,0,'Save.jpg'),(62,1,10,41,'Ryöstöretki','Raid',5,0,0,'Raid.jpg'),(63,1,10,NULL,'Seikkailu','Quest',0,0,0,'Quest.jpg');
/*!40000 ALTER TABLE `events` ENABLE KEYS */;
UNLOCK TABLES;
/*!40103 SET TIME_ZONE=@OLD_TIME_ZONE */;

/*!40101 SET SQL_MODE=@OLD_SQL_MODE */;
/*!40014 SET FOREIGN_KEY_CHECKS=@OLD_FOREIGN_KEY_CHECKS */;
/*!40014 SET UNIQUE_CHECKS=@OLD_UNIQUE_CHECKS */;
/*!40101 SET CHARACTER_SET_CLIENT=@OLD_CHARACTER_SET_CLIENT */;
/*!40101 SET CHARACTER_SET_RESULTS=@OLD_CHARACTER_SET_RESULTS */;
/*!40101 SET COLLATION_CONNECTION=@OLD_COLLATION_CONNECTION */;
/*!40111 SET SQL_NOTES=@OLD_SQL_NOTES */;

-- Dump completed on 2026-01-10 10:08:28
