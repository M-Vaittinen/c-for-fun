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
-- Table structure for table `prophecies`
--

DROP TABLE IF EXISTS `prophecies`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!50503 SET character_set_client = utf8mb4 */;
CREATE TABLE `prophecies` (
  `id` int unsigned NOT NULL AUTO_INCREMENT,
  `expansion_id` int unsigned NOT NULL,
  `type_id` int unsigned NOT NULL DEFAULT '6',
  `name` varchar(255) DEFAULT NULL,
  `description` varchar(1024) DEFAULT NULL,
  `setup_id` int unsigned DEFAULT '0',
  `imagename` varchar(255) DEFAULT NULL,
  PRIMARY KEY (`id`)
) ENGINE=InnoDB AUTO_INCREMENT=17 DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_0900_ai_ci;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `prophecies`
--

LOCK TABLES `prophecies` WRITE;
/*!40000 ALTER TABLE `prophecies` DISABLE KEYS */;
INSERT INTO `prophecies` VALUES (1,18,6,'Kind Emperor','At the start of your turn, and when you remove the last omen: Gain an Action to your hand',0,'Kind_Emperor.jpg'),(2,18,6,'Approaching Army','After you play an Attack card, +1 Raha',43,'Approaching_Army.jpg'),(3,18,6,'Biding Time','At the start of the Clean-up, set aside your hand face down. At the start of your next turn, put those cards in your hand',0,'Biding_Time.jpg'),(4,18,6,'Bureaucracy','When you gain a card that doesn\'t cost 0, gain a copper',0,'Bureaucracy.jpg'),(5,18,6,'Divine Wind','When you remove the last omen, remove all Kingdom card piles from the Supply, and set-up 10 new random piles',0,'Divine_Wind.jpg'),(6,18,6,'Enlightenment','Treasures are also Actions. When you play a Treasure in Action phase, instead of following it\'s instructions, +1 Card and +1 Action',0,'Enlightenment.jpg'),(7,18,6,'Flourishing Trade','Cards cost 1 Raha less. You may use Action plays as Buys',0,'Flourishing_Trade.jpg'),(8,18,6,'Good Harvest','The first time you play each differently named Treasure each turn, first, +1 Buy and +1 Raha',0,'Good_Harvest.jpg'),(9,18,6,'Great Leader','After each Action card you play, +1 Action',0,'Great_Leader.jpg'),(10,18,6,'Growth','When you gain a Treasure, gain a cheaper card',0,'Growth.jpg'),(11,18,6,'Harsh Winter','When you gain a card on your turn, if there\'s velkaa on it\'s pile, take it; othervice put 2 velkaa on its pile',33,'Harsh_Winter.jpg'),(12,18,6,'Kind Emperor','At the start of your turn, and when you remove the last omen: Gain an Action to your hand',0,'Kind_Emperor.jpg'),(13,18,6,'Panic','When you play a Treasure, +2 Buys, and when you discard one from play, return it to its pile',0,'Panic.jpg'),(14,18,6,'Progress','When you gain a card, put it onto your deck',0,'Progress.jpg'),(15,18,6,'Rapid Expansion','When you gain an Action or Treasure, set it aside and play it at the start of your next turn',0,'Rapid_Expansion.jpg'),(16,18,6,'Sickness','At the start of your turn, choose one: Gain a curse onto your deck; or discard 3 cards',44,'Sickness.jpg');
/*!40000 ALTER TABLE `prophecies` ENABLE KEYS */;
UNLOCK TABLES;
/*!40103 SET TIME_ZONE=@OLD_TIME_ZONE */;

/*!40101 SET SQL_MODE=@OLD_SQL_MODE */;
/*!40014 SET FOREIGN_KEY_CHECKS=@OLD_FOREIGN_KEY_CHECKS */;
/*!40014 SET UNIQUE_CHECKS=@OLD_UNIQUE_CHECKS */;
/*!40101 SET CHARACTER_SET_CLIENT=@OLD_CHARACTER_SET_CLIENT */;
/*!40101 SET CHARACTER_SET_RESULTS=@OLD_CHARACTER_SET_RESULTS */;
/*!40101 SET COLLATION_CONNECTION=@OLD_COLLATION_CONNECTION */;
/*!40111 SET SQL_NOTES=@OLD_SQL_NOTES */;

-- Dump completed on 2026-01-10 10:09:23
