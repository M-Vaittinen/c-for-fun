<?php
/*
 * Site around Dominon card randomizer.
 * Uses MySQL database.
 *
 * AUTHOR: Matti Vaittinen <mazziesaccount@gmail.com>
 *
 * Written just for fun. No Warranty. Use at your own risk!
 *
 * Copyright 2026, Matti Vaittinen mazziesaccount@gmail.com>
 */

define("MVAROOTPATH", "");

require MVAROOTPATH.'include/db.php';
require MVAROOTPATH.'include/header.php';
require MVAROOTPATH.'include/nav_bar.php';

do_head("Tervetuloa - Welcome");
echo get_navlist(); 
?>
<div class="header nolink">
<img src="img/dominion-app-icon-4x4.jpg" alt="logo">
<h1>Suffle.fi / Dominionkortit.fi</h1>
</div>
<h2>Oikopolut (n&auml;it&auml; luultavasti etsit)</h2>

<ul>
  <div class="listhead"><li><a href="dominionarvonta.php">Dominion Korttiarvonta</a></li>
  <li><a href="aloittaja.php">Aloittajan arvonta</a></li></div>
</ul>

<br />

<hr style="height:10px;border-width:0;color:#d2691e;background-color:#d2691e">

<a href="dominionarvonta.php"><h3 id="dominfo">Dominion korttiarvonta</h3></a>
<p>
Korttiarvonnassa voit arpoa Dominion-peliss&auml; k&auml;ytett&auml;v&auml;t varastokortit sek&auml; joillekin lis&auml;osille my&ouml;s Tapahtuma, Maamerkki, Way ja Omen kortit. Hyv&auml;ksikoettu aloitussetuppi on sekoitus eri hintaisia kortteja siten, ett&auml; settiin sis&auml;ltyy halpoja ja keskihintaisia kortteja alkupeli&auml; varten sek&auml; kalliimpia kortteja. Arvontakone arpookin aina 3 halpaa (alle 4-rahaa), 3 keskihintaista (4 rahaa) ja 4 n&auml;it&auml; kalliimpaa korttia.
<p>
<?php
	$mobile = isMobileDevice();
	if ($mobile)
		$img = 'sufflemobi.png';
	else
		$img = 'suffleweb.png';

	echo '<img class="screenshot" src="img/'.$img.'" alt="arvontasivu">';
?>
</p>
<p>
Peruskorttien lis&auml;ksi koneessa on kortteja my&ouml;s useammasta lis&auml;osasta. Voit valita k&auml;ytett&auml;v&auml;t lis&auml;osat (sek&auml; halutessasi valittuihin lis&auml;osiin sis&auml;ltyv&auml;t "ei varasto" -kortit). Lis&auml;osien lis&auml;ksi voit s&auml;&auml;t&auml;&auml; kolmea eri painotusta k&auml;ytt&auml;en Tuhina'o-meter&copy;:i&auml;, Tupina'o-meter&copy;:i&auml; sek&auml; Kapita'o-meter&copy;:i&auml;. N&auml;m&auml; painotukset vaikuttavat arvottavien korttien todenn&auml;k&ouml;isyyteen, joko lis&auml;ten tai v&auml;hent&auml;en kyseisten korttien todenn&auml;k&ouml;isyytt&auml; arvonnassa.
<p>
<img class="selectscreen" src="img/selection.png" alt="arvontakoneen-valikko">
</p>

<h4>Tuhina'o-meter&copy;</h4>
<p>
Kasvattamalla arvoa positiiviseksi voit lis&auml;t&auml; toimintaketjuja mahdollistavien korttien todenn&auml;k&ouml;isyytt&auml;. Vastaavasti pienent&auml;m&auml;ll&auml; voit v&auml;hent&auml;&auml; todenn&auml;k&ouml;isyytt&auml;. On kuitenkin hyv&auml; huomata, ett&auml; taustalla oleva algoritmi perustuu yhden ihmisen subjektiiviseen n&auml;kemykseen korttien tuhina-arvosta :)
</p>
<h4> Tupina'o-meter&copy;</h4>
<p>
Nimens&auml; mukaisesti t&auml;m&auml;n tavoite on s&auml;&auml;t&auml;&auml; tupinaa ja mutinaa aiheuttavien korttien todenn&auml;k&ouml;isyytt&auml;. Ts, hy&ouml;kk&auml;ysten ja kirouksia jakavien korttien todenn&auml;k&ouml;isyys kasvaa/v&auml;henee. Nihilismi-optiolla koetetaan maksimoida pelin kurjuus, mutta Miksi?
</p>
<h4>Kapita'o-meter&copy;</h4>
<p>
Viimeisimp&auml;n&auml; s&auml;&auml;t&ouml;n&auml; s&auml;&auml;dell&auml;&auml;n maailmaa py&ouml;ritt&auml;v&auml;&auml; voimaa, eli rahaa. Kuten muutkin 'o-meter:it, t&auml;m&auml; lis&auml;&auml;/v&auml;hent&auml;&auml; todenn&auml;k&ouml;isyytt&auml; - t&auml;ll&auml;kertaa rahoja antaville toiminnoille.
</p>

<hr style="height:10px;border-width:0;color:#d2691e;background-color:#d2691e">

<a href="aloittaja.php"><h3 id="starterinfo">Aloittajan arvonta</h3></a>
<p>
Kolikonheitto on niin mennytt&auml; aikaa. Kuka edes k&auml;ytt&auml;&auml; en&auml;&auml; k&auml;teist&auml;? Pankkikorttia taas ei kannata heitell&auml;, ainakaan jos lautalattiassa on rakoja. Niinp&auml; tarvittiin uusi arvontamuoto. Ja koska kyse on peleist&auml;, on tietysti j&auml;rkev&auml;&auml; pelillist&auml;&auml; my&ouml;s arvonta ;)
</p>
<p>
<img class="selectscreen" src="img/aloittaja.png" alt="arvontakoneen-valikko">
</p>
<p>
Arvontamyllyn arvonnat sopii kahdenlaisiin tilanteisiin. Niihin, joissa jaksaa, ja niihin, joissa ei. Otetaan aluksi tilanteet joissa Ei Vaan Jaksa (koska t&auml;ll&auml; i&auml;ll&auml; n&auml;it&auml; on enemm&auml;n).
</p>
<h4>Ei Vaan Jaksa:</h4>
<p>
Aloittajan arvonnassa aloittaja p&auml;&auml;tet&auml;&auml;n kahden tekij&auml;n perusteella. Ensimm&auml;isen&auml; on jokin satunnainen ominaisuus, vaikkapa pelaajan pituus. Arvonta voi p&auml;&auml;ty&auml; vaikka pisimp&auml;&auml;n (tai lyhimp&auml;&auml;n) pelaajaan.
</p>
<p>
T&auml;m&auml; on kuitenkin napsun verran ep&auml;reilua heille, jotka ovat keskimittaisia, keskipainoisia, ... Siksi arvontamylly antaa viel&auml; toisen m&auml;&auml;reen - eli kertoo, onko alottaja (Pisin, Lyhin, ...) pelaaja itse, vai joku muu ymp&auml;rill&auml;oleva (esimerkiksi vasemmalla puolella oleva). T&auml;m&auml;n pit&auml;isi tasoittaa tilej&auml;. Ja tasapuolisuuden maksimoimiseksi arvonta ottaa viel&auml; huomioon sen, montako pelaajaa on mukana, jotta "itse"-vaihtoehdolle voidaan antaa yht&auml; suuri todenn&auml;k&ouml;isyys kuin muillekin vaihtoehdoille.
</p>
<p>
Tilanteissa, joissa Todellakin Jaksaa, voidaan k&auml;ytt&auml;&auml; napsun kilpailullisempaa versiota:
</p>
<h4>Todellakin Jaksaa:</h4>
<p>
T&auml;ss&auml; arvonnassa ratkaisee nopeus. Aseta siis arvontalaite niin, ett&auml; kaikki pelaajat n&auml;kev&auml;t sen, ja paina arvontanappia. Hetkisen odottelun j&auml;lkeen arvontakone n&auml;ytt&auml;&auml; aloittajan, joka on nyt yleens&auml; joku, joka tekee jotain ensimm&auml;isen&auml;. Esimerkiksi vaikka n&auml;ytt&auml;&auml; ensimm&auml;isen&auml; kielt&auml;&auml;n.
</p>
<p>
Arvontamyllyyn voit my&ouml;s l&auml;hett&auml;&auml; ehdotuksia uusista aloittajavalinnoista. Ehdotuslomake l&ouml;ytyy arvontakoneen alalaidasta. Katso kuitenkin sivun alaosan: "Palaute ja Yll&auml;pito" -osio :)
</p>
<hr style="height:10px;border-width:0;color:#d2691e;background-color:#d2691e">

<h2>Historia</h2>
<p>
T&auml;m&auml; sivusto syntyi Jouluna 2024, kun kokoonnuimme j&auml;lleen perheeni, &auml;itini ja siskojeni perheiden kanssa Joulun viettoon Pihtiputaalle. Meill&auml; n&auml;ihin tapaamisiin kuuluu kiinte&auml;n&auml; osana kortti- ja lautapelien pelaaminen.
</p>
<p>
Yhten&auml; suosikeista(ni) on jo jonkin aikaa ollut "pakanrakennuspeli" Dominion. Peliin ihastuneena tuli my&ouml;s nopeaan tahtiin hankittua muutama lis&auml;osa, jotka lis&auml;&auml;v&auml;t Dominionin uudelleenpelattavuutta merkitt&auml;v&auml;sti. Lis&auml;osien my&ouml;t&auml; korttien valinta kuitenkin vaikeutui. Pelej&auml; tuppaakin edelt&auml;m&auml;&auml;n jonkinmoinen tovi korttien valintaa ja valmisteluja.
</p>
<p>
Niinp&auml; p&auml;&auml;tin rakentaa arvontaohjelman korttien satunnaista valintaa varten... Monien vaiheiden j&auml;lkeen, lopputulos on n&auml;ill&auml; sivuilla. Ja koska toinen haastava p&auml;&auml;t&ouml;s on aina aloittajan valinta, p&auml;&auml;tin tehd&auml; my&ouml;s aloittajan arvonnan.
</p>
<p>
Teknisestä toteutuksesta kiinnostuneille on täällä myös <a href="story/index.php">Kuinka arvontakone tehtiin</a> -sivu.
</p>

<h2>Palaute ja yll&auml;pito</h2>
<p>
Palautetta ja kehitysehdotuksia voi laittaa s&auml;hk&ouml;postitse osoitteeseen: <i>mazziesaccount@gmail.com</i>. Kannattaa kuitenkin muistaa, ett&auml; t&auml;t&auml; tehd&auml;&auml;n harrastuspohjalta ja huvin vuoksi :) Kukaan ei maksa t&auml;st&auml; - p&auml;invastoin palvelintila ja domainit maksaa.
</p>
<p>Kurkkaa <a href="https://github.com/M-Vaittinen">Kuka Kumma T&auml;m&auml;n takana oikein on?</a></p>

<?php
include MVAROOTPATH.'include/footer.php';
echo generate_footer(false, false, true);
?>

