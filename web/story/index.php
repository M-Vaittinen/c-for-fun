<?php
require '../include/db.php';
require '../include/header.php';
do_head("Kuinka arvontakone tehtiin...");
?>
<div class="header">
<a href="../index.php"><img src="../img/dominion-app-icon-4x4.jpg" alt="logo"></a>
<h1>Kuinka Suffle.fi tehtiin</h1>
</div>

<h3>Tausta</h3>
<p>
Kaikki alkoi Jouluna 2024, kun olimme porukalla kokoontuneet viett&auml;m&auml;&auml;n Joulua &auml;itini luokse. N&auml;ihin Jouluihin kuuluu olennaisena osana yhdess&auml; oleminen ja lautapelien pelaaminen. Nuoremman siskoni miehen Ollin my&ouml;t&auml; oli Dominion syrj&auml;ytt&auml;nyt Menolipun suosikkipelin&auml;, ja uusina lis&auml;osina oli Joulupukin tuoma Keisarikunta ja siskoni ja Ollin saama Nousukausi.
</p>
<p>
Kun ensimm&auml;iset esimerkkipelit oli pelattu, alkoi korttien valinta seuraaviin peleihin muuttua aikaaviev&auml;ksi. T&auml;ss&auml; vaiheessa p&auml;&auml;tin, ett&auml; &auml;kki&auml;k&ouml;s sit&auml; koodataan 'arvontaohjelma', joka arpoo meille satunnaiset kortit. Ei muuta kun tuumasta toimeen...
</p>
<h3>Ensimm&auml;inen pikaversio</h3>
<p>
Vanha viidakon sanonta sanoo - jos sinulla on vasara, kaikki ongelmat muistuttavat nauloja. Teen ty&ouml;kseni Linux-kernel ajureita, ja olen koodannut 20-vuotta C-kielell&auml;. Ei siis liene vaikeaa kuvitella, mik&auml; minun vasarani on ;) Kovin montaa tuntia siin&auml; ei vier&auml;ht&auml;nyt, kun koodi jo haki kortit ja korttien hinnat tekstitiedostosta - ja arpoi kortit.
</p>
<p>
<img class="screenshot" src="img/dice.png" alt="Arvontakoneen ensimm&auml;inen versio">
</p>
<p>
Kyseinen C-koodi l&ouml;ytyy yh&auml; <a href="https://github.com/M-Vaittinen/c-for-fun/tree/5042ac85b4eb93da28cafc719dd8e654950d6c6d">git repositorystani</a>.
</p>
<p>
Laiskalle ihmiselle ongelmaksi muodostuikin se, miten nuo kortit, hinnat ja lis&auml;osat oikein saadaan sinne tekstitiedostoon. Lis&auml;ksi Linux-l&auml;pp&auml;rin ottaminen mukaan pelip&ouml;yt&auml;&auml;n arvontaa varten ... Noh, se nyt ei vaan ole k&auml;yt&auml;nn&ouml;llist&auml;.
</p>
<h3>Dice-Admin ja MySQL</h3>
<p>
Niinp&auml; oli aika loikata teknologioissa eteenp&auml;in ja korvata kortit sis&auml;lt&auml;v&auml; tekstitiedosto MySQL tietokannalla. Tavoilleni uskollisena, koodailin t&auml;h&auml;n konvertointiin sopivan ty&ouml;kalun - mill&auml;p&auml; muullakaan kuin C:ll&auml;. N&auml;in sai alkunsa ty&ouml;kalu nimelt&auml; 'dice-admin'. Ensimm&auml;inen dice-adminin git-repooni tuonut committi l&ouml;ytyy <a href="https://github.com/M-Vaittinen/c-for-fun/commit/ca048c445b54773466bf02cd752c7a074549656b">t&auml;&auml;lt&auml;</a>.
</p>
<p>
Dice-admin sai viel&auml; monta p&auml;ivityst&auml;. Lopulta se toteutti ncurses-formeilla ja -menuilla, lomakepohjaisen UI:n, jossa tietoja oli esit&auml;ytetty tekstitiedostosta, ja katselmoinnin / mahdollisten muutosten j&auml;lkeen tiedot sy&ouml;tettiin MySQL-tietokantaan. Ik&auml;v&auml; kyll&auml;, dice-adminin muutoksia en koskaan tullut vieneeksi versionhallintaan, ja niinp&auml; iso osa muutoksista on kadonnut historian h&auml;m&auml;r&auml;&auml;n.
</p>
<h3>El&auml;k&ouml;&ouml;n 2000-luku! MySQL, Apache ja PHP</h3>
<p>
Se Joulu tuli ja meni, mutta Dominion-kuume j&auml;i kytem&auml;&auml;n. Lis&auml;osia tuli hankittua lis&auml;&auml;, ja MySQL:n kanssa puuhastelu oli her&auml;tt&auml;nyt muistoja 1990-luvun lopulta ja 2000-luvun alusta, kun yliopiston koneella istuessani koodailin harrastuksen vuoksi forum-softaa php:ll&auml;. Ja niin pitk&auml;st&auml; aikaa asentelin koneelleni Apachen ja PHP:n MySQL:n lis&auml;ksi - ja j&auml;tin C-kielen t&auml;st&auml; projektista... Tai, no melkein. 
</p>
<p>
Pitih&auml;n minun jollain keinolla valmistautua tulevaan ja koettaa saada lis&auml;&auml; tietoa korteista tietokantaan. Mielell&auml;&auml;n kaikki kortit kerralla. Pienen haeskelun j&auml;lkeen l&ouml;ysin netin sy&ouml;vereist&auml; sivun, jolla oli (muun tekstin seassa) my&ouml;s tietoja Dominion-korteista taulukossa! Viel&auml; kerran setti C-koodia, bash-skriptej&auml; ja reg-expej&auml;, jotka hakivat tiedot verkosta ja poistivat HTML-muotoiluita yksi kerrallaan. Lopulta C-koodini sai ongittua kaikkien korttien nimet ja tekstipohjaisen kuvauksen tietokantaan, 'parsed_cards'-nimiseen tauluun. Jonkinlainen PHP-pohjainen 'dice-admin' oli syntynyt,
</p>
<p>
<img src="img/parsed_cards.png" class="screenshot">
</p>
<p>
T&auml;m&auml;n j&auml;lkeen alkoikin varsinaisen arvontasivun ja sille soveltuvan tietokantarakenteen kehitys. Seuraavan Dominionin-lis&auml;osan hankittuani teinkin uuden PHP-pohjaisen 'admin'-sivun, joka luki 'parsed_cards'-taulukosta uuteen lis&auml;osaani kuuluvien korttien tiedot. L&ouml;ytyneiden avainsanojen perusteella, PHP esit&auml;ytti HTML-pohjaisen lomakkeen. Tarkistettuani uuden lis&auml;osan korttien tiedot (ja korjattuani/lis&auml;tty&auml;ni muutamia tietoja), PHP t&auml;ytti tiedot formista uuden arvontasivustoni tietokantarakenteeseen. Helppoa ja mukavaa, eik&ouml; :)
</p>
<p>
N&auml;lk&auml; kuitenkin kasvaa sy&ouml;dess&auml;, ja jokainen lis&auml;osa tuntui tuovan jotain uutta. Velkarahat, Suomen- ja Englanninkieliset kortit ("Katovuosien" my&ouml;t&auml; kun tarvitsin korttien nimet my&ouml;s englanniksi; "Katovuodet"-lis&auml;osaa kun en ole mist&auml;&auml;n suomenkielisen&auml; l&ouml;yt&auml;nyt!), Maamerkit ja Tapahtumat, Potionit, sek&auml; Omenit ja Prophecyt... Kaikki vaativat lis&auml;&auml; tietoja tietokantarakenteeseen. Lopulta sain "&auml;kistetty&auml;" my&ouml;s erilaisia ikoneita sek&auml; korttien kuvat. Koodin historiatietoineen l&ouml;yt&auml;&auml; tietystikin <a href="https://github.com/M-Vaittinen/c-for-fun/commits/c418a4d1a0d7e226c1f8234550bbf47074e9afed/">j&auml;lleen gitist&auml;</a>, minne t&auml;m&auml;kin sivu p&auml;&auml;tynee :)
</p>
<p>
<img src="img/domppataulut.png" class="screenshot">
</p>
<h3>K&auml;ytetyt ty&ouml;kalut</h3>
<p>
Moni muu ammattilainen vannoo modernien IDE:jen (Integrated Development Environment) kuten Visual Studio nimeen. Olen vuosien varrella koettanut tehostaa tekemist&auml;ni useammallakin ty&ouml;kalulla. Koodia on aikoinaan syntynyt ainakin Eclipsell&auml;, NetBeansilla, QT-Creatorilla, Visual Studiolla ja luultavasti joillain muillakin. Silti, ennemmin tai my&ouml;hemmin olen aina palannut vim-editoriin ja ctags:iin. N&auml;m&auml;kin sivut on siis 100% "koodattu k&auml;sin", vimi&auml; k&auml;ytt&auml;en.
</p>
<p>
Se v&auml;h&auml;inen kuvak&auml;sittely mit&auml; olen tehnyt, on tehty GIMP kuvank&auml;sittelyohjelmalla. Tietokantamodifikaatiot on tehty p&auml;&auml;osin mysql-komentoriviclientill&auml; ja PHP:lla. Toki, kuten ylemp&auml;n&auml; on kerrottu, aluksi my&ouml;s C:ll&auml;. Sivujen kehitys ja testaus tapahtuu omalla koneellani, jossa k&auml;ytt&ouml;j&auml;rjestelm&auml;n&auml; on Fedora Linux. Sivut siin&auml; Firefoxille tarjoilee Apache-palvelin.
</p>
<p>
Uutena tulokkaana kehityksess&auml; oli minullakin nyt ensimm&auml;ist&auml; kertaa teko&auml;ly. Hieman enemm&auml;n siit&auml; <a href="ai.php">t&auml;&auml;ll&auml;</a>. Mielenkiintoinen kokeilu, mutta p&auml;iv&auml;ty&ouml;ss&auml;ni teko&auml;ly ei ainakaan t&auml;ll&auml;hetkell&auml; <i>koodia</i> kirjoita.
</p>
<h3>Mit&auml;s sitten? Android?</h3>
<p>
Viimeisin, ja viel&auml; julkaisematon kehityskulku on ollut Android-applikaatio. Olisihan se hienoa saada t&auml;m&auml; arvontamasiina Googlen Play-kauppaan, eik&ouml;? Toisaalta, useamman eri version yll&auml;pito yhden miehen voimin ei oikein iske. T&auml;m&auml;n tosin voi k&auml;tev&auml;sti taklata pienell&auml; "huijauksella" - eli Android-applikaatiolla, joka n&auml;ytt&auml;&auml; arvontakoneen t&auml;&auml;lt&auml; nettisivulta :) Saa n&auml;hd&auml; eteneek&ouml; t&auml;m&auml; puuhastelu viel&auml; uusiin suuntiin, vai joko t&auml;m&auml; on n&auml;hty...
<p>
<img class="screenshot" src="img/Dominion-Android.png">
<p>

-- <a href="https://github.com/M-Vaittinen">Matti</a><br />
Oulu, 10.01.2026
</p>

<?php
require '../include/footer.php';
echo generate_footer(false, false, true);
?>
