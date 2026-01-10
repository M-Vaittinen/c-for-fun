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
-- Table structure for table `landmarks`
--

DROP TABLE IF EXISTS `landmarks`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!50503 SET character_set_client = utf8mb4 */;
CREATE TABLE `landmarks` (
  `id` int unsigned NOT NULL AUTO_INCREMENT,
  `expansion_id` int unsigned NOT NULL,
  `type_id` int unsigned NOT NULL DEFAULT '0',
  `name` varchar(255) NOT NULL,
  `name_en` varchar(255) DEFAULT NULL,
  `description` varchar(1024) DEFAULT NULL,
  `setup_id` int unsigned DEFAULT '0',
  `imagename` varchar(255) DEFAULT NULL,
  PRIMARY KEY (`id`)
) ENGINE=InnoDB AUTO_INCREMENT=42 DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_0900_ai_ci;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `landmarks`
--

LOCK TABLES `landmarks` WRITE;
/*!40000 ALTER TABLE `landmarks` DISABLE KEYS */;
INSERT INTO `landmarks` VALUES (1,11,2,'Muuri','Wall','Pistelasku: -1 pts jokaisesta yli 15 kortista jotka sinulla on pakassasi.',0,'Wall.jpg'),(2,11,2,'Viinitila','Orchard','Pistelasku: +4 pts Jokaista eri nimistä toimintakorttia kohti, joita sinulla on vähintään 3 kappaletta',0,'Orchard.jpg'),(3,11,2,'Palatsi','Palace','Pistelasku: +3 pts jokaista kulta-hopea-kupari -settiä kohti jotka sinulla on',0,'Palace.jpg'),(4,11,2,'Toivomuslähde','Fountain','Pistelasku: +15 pts jos sinulla on vähintään 10 kuparia',0,'Fountain.jpg'),(5,11,2,'Sudenpesä','Wolf Den','Pistelasku: -3 pts jokaista korttia kohden joita sinulla on 1 kappale',0,'Wolf_Den.jpg'),(6,11,2,'Rosvojen Linnake','Bandit Fort','Pistelasku: -2 pts jokaista pakassasi olevaa kultaa ja hopeaa kohden',0,'Bandit_Fort.jpg'),(7,11,2,'Linnake','Keep','Pistelasku: +5 pts jokaista eri nimistä Rahakorttia kohden, joita sinulla on eniten tai yhtä monta kuin seuraavaksi eniten omaavalla on',0,'Keep.jpg'),(8,11,2,'Museo','Museum','Pistelasku: +2 pts jokaista eri nimistä korttiasi kohden',0,'Museum.jpg'),(9,11,2,'Torni','Tower','Pistelasku: +1 pts per kortti(si), joka ei ole Pistekortti ja jonka varastopino on tyhjentynyt',0,'Tower.jpg'),(10,11,2,'Riemukaari','Triumphal Arch','Pistelasku: +3 pts jokaista toiseksi yleisintä toimintakorttiasi kohden. Jos tasamäärä, valitse kumpi vain',0,'Triumphal_Arch.jpg'),(11,11,2,'Obeliski','Obelisk','Pistelasku: +2 pts jokaista korttia kohden, joka sinulla (alkutoimien aikana, sattumanvaraisesti) valitusta toimintakorttien varastopinosta',28,'Obelisk.jpg'),(12,11,2,'Taistelutanner','Battlefield','Kun otat Pistekortin, ota maamerkiltä 2 pts, (Joita alkutoimien aikana laitettu 6 per pelaaja)',29,'Battlefield.jpg'),(13,11,2,'Pylväskäytävä','Colonnade','Kun otat Toimintakortin ja sinulla on sellainen myös pelissä, ota maamerkiltä 2 pts. (Joita alkutoimien aikana laitettu 6 per pelaaja)',29,'Colonnade.jpg'),(14,11,2,'Häväisty Pyhättö','Defiled Shrine','Kun otat toimintakortin, siirrä sen kasan päältä 1 pts tälle maamerkille. Kun otat kirouksen, ota pisteet tältä maamerkiltä.',31,'Defiled_Shrine.jpg'),(15,11,2,'Akvedukti','Aqueduct','Kun ostat Rahakortin, siirrä sen varastopinon päältä 1 pts tämän kortin päälle. Kun otat Pistekortin, ota pisteet tältä maamerkiltä.',32,'Aqueduct.jpg'),(16,11,2,'Basilika','Basilica','Kun otat kortin ja sinulla jää 2 rahaa tai enemmän, ota 2 pts tältä maamerkiltä.',29,'Basilica.jpg'),(17,11,2,'Labyrintti','Labyrinth','Kun omalla vuorollasin otat vuorosi toisen kortin, ota 2 pts tältä maamerkiltä.',29,'Labyrinth.jpg'),(18,11,2,'Vuoristoreitti','Mountain Pass','Kun ensimmäisen pelaajan joka ottaa läänin vuoro loppuu, käydään huutokauppa. Aktiiviseen pelaajaan loppuen, (muut???) pelaajat tarjoavat enintään 40-velkaa. Korkeimman tarjouksen tehnyt saa 8 pts ja tarjoamansa velan.',33,'Mountain_Pass.jpg'),(19,11,2,'Hauta','Tomb','Kun tuhoat kortin, +1 pts.',0,'Tomb.jpg'),(20,11,2,'Kylpylä','Baths','Ota 2 pistettä tältä maamerkiltä, jos lopetat vuorosi ottamatta korttiakaan.',29,'Baths.jpg'),(21,11,2,'Areena','Arena','Ostovuorosi alussa voit poistaa toimintakortin kädestäsi. Jos teet niin, ota 2 pts tältä maamerkiltä.',29,'Arena.jpg'),(22,14,3,'Way of the Pig','Way of the Pig','Voit korvata Toimintakortin toiminnon (muista jakoviivasääntö!): +1 kortti, +1 Action',0,'Way_of_the_Pig.jpg'),(23,14,3,'Way of the Rat','Way of the Rat','Voit korvata Toimintakortin toiminnon (muista jakoviivasääntö!): Discard a Treasure to gain a copy of this (this = toimintakortti)',0,'Way_of_the_Rat.jpg'),(24,14,3,'Way of the Seal','Way of the Seal','Voit korvata Toimintakortin toiminnon (muista jakoviivasääntö!): +1 Raha, This turn, when you gain a card, you may put it onto your deck',0,'Way_of_the_Seal.jpg'),(25,14,3,'Way of the Sheep','Way of the Sheep','Voit korvata Toimintakortin toiminnon (muista jakoviivasääntö!): +2 Rahaa',0,'Way_of_the_Sheep.jpg'),(26,14,3,'Way of the Squirrel','Way of the Squirrel','Voit korvata Toimintakortin toiminnon (muista jakoviivasääntö!): +2 Cards at the end of this turn',0,'Way_of_the_Squirrel.jpg'),(27,14,3,'Way of the Turtle','Way of the Turtle','Voit korvata Toimintakortin toiminnon (muista jakoviivasääntö!): Set this (this = toimintakortti) aside. If you did, play it at the start of your next turn.',0,'Way_of_the_Turtle.jpg'),(28,14,3,'Way of the Worm','Way of the Worm','Voit korvata Toimintakortin toiminnon (muista jakoviivasääntö!): Exile an estate from the Supply',25,'Way_of_the_Worm.jpg'),(29,14,3,'Way of the Mole','Way of the Mole','Voit korvata Toimintakortin toiminnon (muista jakoviivasääntö!): +1 Action, Discard your hand. +3 Cards',0,'Way_of_the_Mole.jpg'),(30,14,3,'Way of the Monkey','Way of the Monkey','Voit korvata Toimintakortin toiminnon (muista jakoviivasääntö!): +1 Buy, +1 Raha',0,'Way_of_the_Monkey.jpg'),(31,14,3,'Way of the Mouse','Way of the Mouse','Voit korvata Toimintakortin toiminnon (muista jakoviivasääntö!): Play the set-aside card (set-aside during setup), leaving it there',42,'Way_of_the_Mouse.jpg'),(32,14,3,'Way of the Mule','Way of the Mule','Voit korvata Toimintakortin toiminnon (muista jakoviivasääntö!): +1 Action, +1 Raha',0,'Way_of_the_Mule.jpg'),(33,14,3,'Way of the Otter','Way of the Otter','Voit korvata Toimintakortin toiminnon (muista jakoviivasääntö!): +2 Cards',0,'Way_of_the_Otter.jpg'),(34,14,3,'Way of the Owl','Way of the Owl','Voit korvata Toimintakortin toiminnon (muista jakoviivasääntö!): Draw until you have 6 cards in hand',0,'Way_of_the_Owl.jpg'),(35,14,3,'Way of the Ox','Way of the Ox','Voit korvata Toimintakortin toiminnon (muista jakoviivasääntö!): +2 Actions',0,'Way_of_the_Ox.jpg'),(36,14,3,'Way of the Butterfly','Way of the Butterfly','Voit korvata Toimintakortin toiminnon (muista jakoviivasääntö!): You may return this (this = Tomintakortti) to it\'s pile to gain a card costing exactly 1 more than it',0,'Way_of_the_Butterfly.jpg'),(37,14,3,'Way of the Camel','Way of the Camel','Voit korvata Toimintakortin toiminnon (muista jakoviivasääntö!): Exile a Gold from the Supply',25,'Way_of_the_Camel.jpg'),(38,14,3,'Way of the Chameleon','Way of the Chameleon','Voit korvata Toimintakortin toiminnon (muista jakoviivasääntö!): Follow this card\'s (this card = Toimintokortti) instructions; each time that would give you +Cards, take +Rahaa instead, and vice-versa',0,'Way_of_the_Chameleon.jpg'),(39,14,3,'Way of the Frog','Way of the Frog','Voit korvata Toimintakortin toiminnon (muista jakoviivasääntö!): +1 Action, When you discard this (this = Toimintakortti) from play this turn, put it onto your deck',0,'Way_of_the_Frog.jpg'),(40,14,3,'Way of the Goat','Way of the Goat','Voit korvata Toimintakortin toiminnon (muista jakoviivasääntö!): Trash a card from your hand',0,'Way_of_the_Goat.jpg'),(41,14,3,'Way of the Horse','Way of the Horse','Voit korvata Toimintakortin toiminnon (muista jakoviivasääntö!): +2 Cards, +1 Action, Return this (this = Toimintakortti) to its pile',0,'Way_of_the_Horse.jpg');
/*!40000 ALTER TABLE `landmarks` ENABLE KEYS */;
UNLOCK TABLES;
/*!40103 SET TIME_ZONE=@OLD_TIME_ZONE */;

/*!40101 SET SQL_MODE=@OLD_SQL_MODE */;
/*!40014 SET FOREIGN_KEY_CHECKS=@OLD_FOREIGN_KEY_CHECKS */;
/*!40014 SET UNIQUE_CHECKS=@OLD_UNIQUE_CHECKS */;
/*!40101 SET CHARACTER_SET_CLIENT=@OLD_CHARACTER_SET_CLIENT */;
/*!40101 SET CHARACTER_SET_RESULTS=@OLD_CHARACTER_SET_RESULTS */;
/*!40101 SET COLLATION_CONNECTION=@OLD_COLLATION_CONNECTION */;
/*!40111 SET SQL_NOTES=@OLD_SQL_NOTES */;

-- Dump completed on 2026-01-10 10:08:51
