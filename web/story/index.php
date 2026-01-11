<?php
require '../include/db.php';
require '../include/header.php';
do_head("Kuinka arvontakone tehtiin...");
?>

<div class="landingpage">
<h1>Kuinka Suffle.fi tehtiin</h1>

<h3>Tausta</h3>
<p>
Kaikki alkoi Jouluna 2023 kun olimme porukalla kokoontuneet viettämään Joulua äitini luokse. Näihin Jouluihin kuuluu olennaisena osana yhdessä oleminen ja lautapelien pelaaminen. Nuoremman siskoni miehen Ollin myötä oli Dominion syrjäyttänyt Menolipun suosikkipelinä, ja uusina lisäosina oli Joulupukin tuoma Keisarikunta ja siskoni ja Ollin saama Nousukausi.
</p>
<p>
Kun ensimmäiset esimerkkipelit oli pelattu, alkoi korttien valinta seuraaviin peleihin muuttua aikaavieväksi. Tässävaiheessa päätin että äkkiäkös sitä koodataan 'arvontaohjelma' joka arpoo meille satunnaiset kortit. Ei muuta kun tuumasta toimeen...
</p>
<h3>Ensimmäinen pikaversio</h3>
<p>
Vanha viidakon sanonta sanoo - jos sinulla on vasara, kaikki ongelmat muistuttavat nauloja. Teen työkseni Linux-kernel ajureita, ja olen koodannut 20-vuotta C-kiellä. Ei siis liene vaikeaa kuvitella, mikä minun vasarani on ;) Kovin montaa tuntia siinä ei vierähtänyt kun koodi jo haki kortit ja korttien hinnat tekstitiedostosta - ja arpoi kortit.
</p>
<p>
<img class="screenshot" src="img/dice.png" alt="Arvontakoneen ensimmäinen versio">
</p>
<p>
Kyseinen C-koodi löytyy yhä <a href="https://github.com/M-Vaittinen/c-for-fun/tree/5042ac85b4eb93da28cafc719dd8e654950d6c6d">git repositorystani</a>.
</p>
<p>
Laiskalle ihmiselle ongelmaksi muodostuikin se, miten nuo kortit, hinnat ja lisäosat oikein saadaan sinne tekstitiedostoon. Lisäksi Linux-läppärin ottaminen mukaan pelipöytään arvontaa varten ... Noh, se nyt ei vaan ole käytännöllistä.
</p>
<h3>Dice-Admin ja MySQL</h3>
<p>
Niinpä oli aika loikata teknologioissa eteenpäin ja korvata kortit sisältävä tekstitiedosto MySQL tietokannalla. Tavoilleni uskollisena, koodailin tähän konvertointiin sopivan työkalun - milläpä muullakaan kuin C:llä. Näin sai alkunsa työkalu nimeltä 'dice-admin'. Ensimmäinen dice-adminin git-repooni tuonut committi löytyy <a href="ihttps://github.com/M-Vaittinen/c-for-fun/commit/ca048c445b54773466bf02cd752c7a074549656b">täältä</a>.
</p>
<p>
Dice-admin sai vielä monta päivitystä. Lopulta se toteutti ncurses formeilla ja menuilla lomakepohjaisen UI:n, jossa tietoja oli esitäytetty tekstitiedostosta, ja katselmoinnin / mahdollisten muutosten jälkeen tiedot syötettiin MySQL tietokantaan. Ikävä kyllä, dice-adminin muutoksia en koskaan tullut vieneeksi versionhallintaan, ja niinpä iso osa muutoksista on kadonnut historian hämärään.
</p>
<h3>Eläköön 2000-luku! MySQL, Apache ja PHP</h3>
<p>
Se Joulu tuli ja meni, mutta Dominion-kuume jäi kytemään. Lisäosia tuli hankittua lisää, ja MySQL:n kanssa puuhastelu oli herättänyt muistoja 1990-luvun lopulta ja 2000-luvun alusta, kun yliopiston koneella istuessani koodailin harrastuksen vuoksi forum-softaa php:llä. Ja niin pitkästä aikaa asentelin koneelleni Apachen ja PHP:n MySQL:n lisäksi - ja jätin C-kielen tästä projektista... Tai, no melkein. 
</p>
<p>
Pitihän minun jollain keinolla valmistautua tulevaan ja koettaa saada lisää tietoa korteista tietokantaan. Mielellään kaikki kortit kerralla. Pienen haeskelun jälkeen löysin netin syövereistä sivun, jolla oli (muun tekstin seassa) myös tietoja Dominion korteista taulukossa! Vielä kerran setti C-koodia, bash-skriptejä ja reg-expejä, jotka hakivat tiedot verkosta ja poistivat HTML-muotoiluita yksi kerrallaan. Lopulta C-koodini sai ongittua kaikkien korttien nimet ja tekstipohjaisen kuvauksen tietokantaan, 'parsed_cards'-nimiseen tauluun. Jonkinlainen PHP-pohjainen 'dice-admin' oli syntynyt,
</p>
<p>
<img src="img/parsed_cards.png" class="screenshot">
</p>
<p>
Tämän jälkeen alkoikin varsinaisen arvontasivun ja sille soveltuvan tietokantarakenteen kehitys. Seuraavan Dominionin lisäosan hankittuani, teinkin uuden PHP-pohjaisen 'admin' sivun, joka luki 'parsed_cards' taulukosta uuteen lisäosaani kuuluvien korttien tiedot, ja löytyneiden avainsanojen perusteella, PHP:lla esitäytti HTML-pohjaisen lomakkeen. Tarkistettuani uuden lisäosan korttien tiedot (ja korjattuani/lisättyäni muutamia tietoja), PHP täytti tiedot formista uuden arvontasivustoni tietokantarakenteeseen. Helppoa ja mukavaa, eikö :)
</p>
<p>
Nälkä kuitenkin kasvaa syödessä, ja jokainen lisäosa tuntui tuovan jotain uutta. Velkarahat, Suomen ja Englanninkieliset kortit ("Katovuosien" myötä kun tarvitsin korttien nimet myös englanniksi "Katovuodet"-lisäosaa kun en ole mistään suomenkielisenä löytänyt!), Maamerkit ja Tapahtumat, Potionit, sekä Omenit ja prophecyt. Ja lopulta myös erilaisia ikoneita sekä korttien kuvat. Koodin historiatietoineen löytää tietystikin <a href="https://github.com/M-Vaittinen/c-for-fun/commits/dominion/?after=63e2e5290a581d7c34d467c2d94d8a153beefb15+69">jälleen gitistä</a>. Sinne tämäkin sivu päätynee :)
</p>
<p>
<img src="img/domppataulut.png" class="screenshot">
</p>
<h3>Mitäs sitten? Android?</h3>
<p>
Viimeisin, ja vielä julkaisematon kehityskulku on ollut Android-applikaatio. Olisihan se hienoa saada tämä arvontamasiina Googlen Play-kauppaan, eikö? Toisaalta, useamman eri version ylläpito yhden miehen voimin ei oikein iske. Tämän tosin voi kätevästi taklata pienellä "huijauksella" - eli Android applikaatiolla joka näyttää arvontakoneen täältä nettisivulta :) Saa nähdä eteneekö tämä puuhastelu vielä uusiin suuntiin, vai joko tämä on nähty...
<p>
<img class="screenshot" src="img/Dominion-Android.png">
<p>

-- 10.01.2026, Matti
</p>
</div>

<?php
require '../include/footer.php';
echo generate_footer(false, false);
?>
