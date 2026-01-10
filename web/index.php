<?php

require 'include/header.php';
do_head("Tervetuloa - Welcome");
?>
<div class="header">
<img src="img/dominion-app-icon-4x4.jpg" alt="logo">
<h1>Suffle.fi / Dominionkortit.fi</h1>
</div>
<div><br /></div>
<h2>Oikopolut (näitä luultavasti etsit)</h2>
<ul>
  <li>
    <a href="dominionarvonta.php">Dominion Korttiarvonta</a>
  </li>
  <li>
    <a href="aloittaja.php">Aloittajan arvonta</a>
  </li>
</ul>

<h3>Dominion korttiarvonta</h3>
<p>Korttiarvonnassa voit arpoa Dominion pelissä käytettävät varastokortit, sekä joillekin lisäosille myös Tapahtuma, Maamerkki, Way ja Omen kortit. Hyväksikoettu aloitussetuppi on sekoitus eri hintaisia kortteja siten, että settiin sisältyy halpoja ja keskihintaisia kortteja alkupeliä varten, sekä kalliimpia kortteja. Arvontakone arpookin aina 3 halpaa (alle 4-rahaa), 3 keskihintaista (4 rahaa) ja 4 näitä kalliimpaa korttia.
<p>Peruskorttien lisäksi koneessa on kortteja myös useammasta lisäosasta. Voit valita käytettävät lisäosat (sekä halutessasi valittuihin lisäosiin sisältyvät "ei varasto" -kortit). Lisäosien lisäksi voit säätää kolmea eri painotusta käyttäen Tuhina'o-meter&copy;:iä, Tupina'o-meter&copy;:iä sekä Kapita'o-meter&copy;:iä. Nämä painotukset vaikuttavat arvottavien korttien todennäköisyyteen, joko lisäten tai vähentäen kysesiten korttien todennäköisyyttä arvonnassa.

<p><b>Tuhina'o-meter&copy;</b>
<p>
Kasvattamalla arvoa positiiviseksi voit lisätä toimintaketjuja mahdollistavien korttien todennäköisyyttä. Vastaavasti pienentämällä voit vähentää todennäköisyyttä. On kuitenkin hyvä huomata että taustalla oleva algoritmi perustuu yhden ihmisen subjektiiviseen näkemykseen korttien tuhina-arvosta :)
<p><b> Tupina'o-meter&copy;</b>
<p>Nimensä mukaisesti tämän tavoite on säätää tupinaa ja mutinaa aiheuttavien korttien todennäköisyyttä. Ts, hyökkäysten ja kirouksia jakavien korttien todennäköisyys kasvaa/vähenee. Nihilismi-optiolla koetetaan maksimoida pelin kurjuus, mutta Miksi?
<p><b>Kapita'o-meter&copy;</b>
<p>Viimeisimpänä säätönä säädellään maailmaa pyörittävää voimaa, eli rahaa. Kuten muutkin 'o-meter:it, tämä lisää/vähentää todennäköisyyttä - tälläkertaa rahoja antaville toiminnoille.

<h3>Aloittajan arvonta</h3>
<p>Kolikonheitto on niin mennyttä aikaa. Kuka edes käyttää enää käteistä? Pankkikorttia taas ei kanata heitellä, ainakaan jos lautalattiassa on rakoja. Niinpä tarvittiin uusi arvontamuoto. Ja koska kyse on peleistä, on tietysti järkevää pelillistää myös arvonta ;)
<p>Arvontamyllyn arvonnat sopii kahdenlaisiin tilanteisiin. Niihin joissa jaksaa, ja niihin joissa ei. Otetaan aluksi tilanteet jossa Ei Vaan Jaksa (koska tällä iällä näitä on enemmän).
<p><b>Ei Vaan Jaksa:</b>
<p>Aloittajan arvonnassa aloittaja päätetään kahden tekijän perusteella. Ensimmäisenä on jokin satunnainen ominaisuus, vaikkapa pelaajan pituus. Arvonta voi päätyä vaikka pisimpään (tai lyhimpään) pelaajaan.
<p>Tämä on kuitenkin napsun verran epäreilua heille, jotka ovat keskimittaisia, keskipainoisia, ... Siksi arvontamylly antaa vielä toisen määreen - eli kertoo onko alottaja (Pisin, Lyhin, ...) pelaaja itse, vai joku muu ympärilläoleva (esimerkiksi vasemmalla puolella oleva). Tämän pitäisi tasoittaa tilejä. Ja tasapuolisuuden maksimoimiseksi arvonta ottaa vielä huomioon sen, montako pelaajaa on mukana jotta "itse"-vaihtoehdolle voidaan antaa yhtä suuri todennäköisyys kuin muillekin vaihtoehdoille.
<p>Tilanteissa joissa Todellakin Jaksaa voidaan käyttää napsun kilpailullisempaa versiota:
<p><b>Todellakin Jaksaa:</b>
<p>Tässä arvonnassa ratkaisee nopeus. Aseta siis arvontalaite niin että kaikki pelaajat näkevät sen, ja paina arvontanappia. Hetkisen odottelun jälkeen arvontakone näyttää aloittajan, joka on nyt yleensä joku joka tekee jotain ensimmäisenä. Esimerkiksi vaikka näyttää ensimmäisenä kieltään.
<p>Arvontamyllyyn voit myös lähettää ehdotuksia uusista aloittajavalinnoista. Ehdotuslomake löytyy arvontakoneen alalaidasta. Katso kuitenkin sivun alaosan: "Palaute ja Ylläpito" -osio :)

<h2>Historia</h2>
<p>Tämä sivusto syntyi Jouluna 2024 kun kokoonnuimme jälleen perheeni, äitini ja siskojeni perheiden kanssa Joulun viettoon Pihtiputaalle. Meillä näihin tapaamisiin kuuluu kiinteänä osana kortti- ja lautapelien pelaaminen.
<p>Yhtenä suosikeista(ni) on jo jonkin aikaa ollut "pakanrakennuspeli" Dominion. Peliin ihastuneena tuli myös nopeaan tahtiin hankittua muutama lisäosa, jotka lisäävät Dominionin uudelleenpelattavuutta merkittävästi. Lisäosien myötä korttien valinta kuitenkin vaikeutui. Pelejä tuppaakin edeltämään jonkinmoinen tovi korttien valintaa ja valmisteluja.
<p>Niinpä päätin rakentaa arvontaohjelman korttien satunnaista valintaa varten... Minien vaiheiden jälkeen, lopputlos on näillä sivuilla. Ja koska toinen haastava päätös on aina aloittajan valinta, päätin tehdä myös aloittajan arvonnan.

<h2>Palaute ja ylläpito</h2>
Palautetta ja kehitysehdotuksia voi laittaa sähköpostitse osoitteeseen: <i>mazziesaccount@gmail.com</i>. Kannattaa kuitenkin muistaa että tätä tehdään harrastuspohjalta ja huvin vuoksi :) Kukaan ei maksa tästä - päinvastoin palvelintila ja domainit maksaa.

<?php

include 'include/footer.php';
do_head("Tervetuloa - Welcome");
generate_footer(false, false);
?>

