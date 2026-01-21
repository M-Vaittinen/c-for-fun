<?php
define("MVAROOTPATH", "../");
require MVAROOTPATH.'include/db.php';
require MVAROOTPATH.'include/header.php';
require MVAROOTPATH.'include/nav_bar.php';

do_head("Teko&auml;lyhoi!");
echo get_navlist("..");
?>

<div class="header">
<a href="../index.php"><img src="../img/dominion-app-icon-4x4.jpg" alt="logo"></a>
<h1>Teko&auml;ly ja n&auml;iden sivujen teko.</h1>
</div>

Pari pakollista sanaa teko&auml;lyst&auml;.

<h3>On trendik&auml;st&auml; puhua teko&auml;lyst&auml;...</h3>
<p>
<img class="mypic" src="img/trendyme.webp" alt="Min&auml; betonivalut&ouml;iss&auml;">
 <b>...ja min&auml;h&auml;n olen <i>eritt&auml;in</i> trendik&auml;s.</b>
</p>
<p> Lis&auml;ksi teko&auml;ly jakaa mielipiteit&auml;. Toiset vannoo teko&auml;lyn olevan tulevaisuus, toiset taas kertovat sen tuhoavan meid&auml;t. Olen n&auml;hnyt kiihkeit&auml; postauksia siit&auml;, kuinka teko&auml;lyn tuottama koodi on kamalaa kuraa, johon maailma hukkuu. Projektien yll&auml;pito muuttuu mahdottomaksi ja lopulta p&auml;tevi&auml; koodareita tarvitaan siivoamaan "vibe-koodauksen" aiheuttama sotku. Toisaalta, osa uskoo teko&auml;lyn tekev&auml;n tulevaisuudessa kaiken koodausty&ouml;n, koodareiden p&auml;&auml;tyess&auml; kilometritehtaalle. Kaikilla tuntuu kuitenkin olevan vahva mielipide asiasta.</p>
<p>
Kun siirr&auml;n omat mielipiteeni syrj&auml;&auml;n, paljastuu alta pari faktaa:
<ul>
  <li>Tiedonhakuni on muuttunut.</li>
  <li>Voin tehd&auml; asioita joita en osaa, kuten tuottaa kuvia.</li>
</ul>
</p>
<p>
<b>Tiedonhaun muutos</b> voi n&auml;ky&auml; siin&auml;, miten pyrin ratkaisemaan jonkin erityisen ongelman alueella, jolla minulla ei ole osaamista. Aiemmin olisin hakukoneen avulla etsinyt sivustoja, joilla asiaa olisi k&auml;sitelty. Sen j&auml;lkeen olisin vieraillut ko. sivuilla ja koettanut soveltaa lukemiani ohjeita omaan ongelmaani. Oli my&ouml;s ajanjakso jolloin saatoin lopulta kysy&auml; neuvoa vaikkapa internet-foorumilla.
</p>
<h3>Mihin k&auml;ytin teko&auml;ly&auml;</h3>
<p>
Dominion korttiarvontaa tehdess&auml; t&ouml;rm&auml;sin useampaankin tuollaiseen ongelmaan, sill&auml; UI:t ja web-koodaaminen eiv&auml;t tule selk&auml;rangasta. Niinp&auml; DuckDuckGo -hakuun ymp&auml;tty teko&auml;lyapuri p&auml;&auml;tyi k&auml;ytt&ouml;&ouml;n. "o'-meter" slidereiden muotoilut ovat l&auml;hes suoraan teko&auml;lyn tuottamaa koodia.
</p>
<p>
Helposti k&auml;ytett&auml;viss&auml; olevasta teko&auml;lyst&auml; huolimatta, p&auml;&auml;dyin silti, huomattavan usein, yh&auml; surffaamaan <a href="https://www.w3schools.com">W3C-schools</a> sivuja. Testimieless&auml; kokeilin my&ouml;s teko&auml;lyn PHP-tuotantoa. Annoin teko&auml;lylle suoraan mysql-clientin tulostuksen 'table-descriptionista', ja pyysin sit&auml; tekem&auml;&auml;n sivun joka n&auml;ytt&auml;&auml; taulukon 10-ensimm&auml;isen rivin sis&auml;ll&ouml;n ja sallii editoinnin. "N&auml;ps ja kops" - PHP-koodi pullahti ulos. Lis&auml;ksi teko&auml;ly ehdotti voivansa tehd&auml; sivutiksen (eli %lt;- edellinen ja -&gt; seuraava -linkit). Yll&auml;tyin huomatessani ett&auml; koodi oli jokseenkin toimivaa, joskin sy&ouml;tett&auml;v&auml;n datan tarkistukset taisivat olla tekem&auml;tt&auml; - mik&auml; lienee jokseenkin "vaarantava virhe". T&auml;m&auml; koodi ei tosin tullut mihink&auml;&auml;n k&auml;ytt&ouml;&ouml;n.
</p>
<p><b>Grafiikan tekoa en osaa</b>. Minulla ei ole mink&auml;&auml;n tasoisia taiteellisia lahjoja. Jos joskus harvoin saankin jonkin vision n&auml;ytett&auml;v&auml;st&auml; sis&auml;ll&ouml;st&auml;, k&auml;teni eiv&auml;t kertakaikkiaan kykene tuottamaan p&auml;&auml;ss&auml;ni visualisoimiani kuvia. Musiikista en yleens&auml; saa edes visioita. Sanat ovat ainoa asia joita kykenen tuottamaan :) T&auml;ss&auml;suhteessa AI avaa minulle aivan uusia ovia. AI:n avulla voin halutessani tehd&auml; edes jonkinlaisia kuvia. Dominion arvontakoneessa n&auml;kyv&auml; 'velkakorttien tunnus', samoin kuin sivun yl&auml;reunan 'arvontakoneen logo' ovat teko&auml;lyn luomuksia. Ilman teko&auml;lyn apua ne olisivat j&auml;&auml;neet tekem&auml;tt&auml;.
</p>
<h3>Lopuksi minun mielipide - vaikkei sit&auml; kysytty :)</h3>
<p>
Teko&auml;ly on kiistatta houkutteleva apuri. Samalla siihen minun n&auml;k&ouml;kulmasta sis&auml;ltyy suuri moraalinen ongelma liittyen opetusdatan hankkimiseen ja lisensseihin. Kaikki mit&auml; teko&auml;ly kykenee luomaan kuten koodi, kuvat ja vastaukset, on itse asiassa opetusdatasta johdettu tuote. Ilman opetusdataansa teko&auml;ly ei siis tuottaisi mit&auml;&auml;n j&auml;rkev&auml;&auml;. Kaikki perustuu opetusdataan.
</p>
<p>
No, mist&auml; teko&auml;ly on saanut opetusdatansa? Netist&auml;. Teko&auml;ly&auml;&auml;n kehitt&auml;v&auml;t yritykset kaapivat netist&auml; tietoja koneellisesti kielimallien dataksi. Jopa siin&auml; mittakaavassa, ett&auml; t&auml;m&auml;n koneellisen tiedonhaun aiheuttama kuorma on muodostunut sivustoille ongelmaksi. Uset sivustot joutuvat taistelemaan t&auml;t&auml; vastaan. Mik&auml;li olet t&ouml;rm&auml;nnyt sivuun joka latautuessaan n&auml;ytt&auml;&auml; 'Anubis-animetyt&ouml;n' ja tekstin: "Making sure you're not a bot!" - on kyse luultavasti juuri t&auml;st&auml;.
</p>
<p>No, mist&auml; opetusdatasta tuotettiin vaikkapa se koodi, kun kysyin PHP:t&auml; tietokannan sis&auml;ll&ouml;n tulostamiseen? Tai, jos kysyisin apua C-koodin tekoon? Kukaan ei pysty t&auml;h&auml;n tarkasti vastaamaan, mutta hyvin luultavasti kaikesta samankaltaisesta koodista, jota teko&auml;lyn opetusdatassa k&auml;ytettiin.
</p>
<p>
Kun muistetaan se, ett&auml; opetusdata on kaavittu netist&auml;, voidaan pohtia sit&auml; mink&auml;laista koodia avoimesta netist&auml; l&ouml;ytyy? Harvoin yritysten omaa koodia. Varsinkaan teko&auml;ly-yritysten, eik&auml; ainakaan sit&auml; "salaista". Vuosien Linux kernel ty&ouml;n v&auml;ritt&auml;mien linssien l&auml;pi katsottuna, suuri osa avoimesta koodista on julkaistu erilaisten GPL lisenssien alla. GPL-lisenssin ydin on se ett&auml; avointa koodia saa kyll&auml; k&auml;ytt&auml;&auml; ja muokata - mutta muokatun koodin tulee my&ouml;s olla avointa. Se ett&auml; GPL-koodia muokataan koneellisesti ei minusta saisi muuttaa t&auml;t&auml;. Ts, minun silmiss&auml;ni GPL-koodilla opetetun teko&auml;lyn tuottaman koodin tulisi my&ouml;s, GPL lisenssiehtojen mukaisesti, olla GPL-lisenssin alaista. (Muilla lisensseill&auml; julkaistun koodin k&auml;yt&ouml;ss&auml; voi olla muita ongelmia). En tied&auml; kertooko mik&auml;&auml;n teko&auml;ly sit&auml;, mink&auml;laisen lisenssin alla julkaistusta opetusdatasta tuotettua generoitu koodi on. Luultavasti ei.
</p>
<p>
Teko&auml;ly tuo muitakin muutoksia joista kaikista en pid&auml;. Vaikkapa internetin muuttuminen. Esimerkisi <a href="https://dawn.fi/uutiset/2026/01/06/stack-overflow-on-lahes-kuollut-tekoly">Stack Overflow on vaikeuksissa</a>. <a href="https://dawn.fi/uutiset/2026/01/08/tailwind-css-tekoaly-irtisanomiset">Tailwind romahti</a> vaikka on k&auml;ytetympi kuin koskaan. 
</p>
<p>
T&auml;m&auml; pist&auml;&auml; my&ouml;s miettim&auml;&auml;n sit&auml;, mik&auml; on perinteikk&auml;&auml;n opetus- ja konsultointit&ouml;n tulevaisuus teko&auml;lyn kyetess&auml; auttamaan uusien teknologioiden oppimisessa? Omaa syd&auml;nt&auml;ni l&auml;hell&auml; kun on avoin l&auml;hdekoodi jonka kehitt&auml;j&auml;t ovat voineet ty&ouml;llist&auml;&auml; itsens&auml; auttamalla ja kouluttamalla n&auml;iden ohjelmien k&auml;ytt&auml;ji&auml;. Onko avoimella ja ilmaisella koodilla tulevaisuutta vai valuuko sen hy&ouml;dyt tulevaisuudessa teko&auml;lyfirmojen taskuun? Ovatko tulevaisuuden voittajia ne, jotka kykenev&auml;t rahastamaan teko&auml;lyn tuottamalla tiedolla - niiden kustannuksella jotka tuon tiedon ovat alunperin tutkineet/tuottaneet, ja joilta teko&auml;ly on ne sittemmin opetusdataansa sy&ouml;nyt?
</p>
<p>
Loppukaneettina kuitenkin lienee se, ett&auml; sill&auml; pid&auml;nk&ouml; teko&auml;lyst&auml; vaiko en ei ole mit&auml;&auml;n v&auml;li&auml;. On naiivia kuvitella ett&auml; teko&auml;lyn k&auml;ytt&ouml; tai kehitys jotenkin loppuisi. Teko&auml;ly on tullut j&auml;&auml;d&auml;kseen ja meid&auml;n on vain l&ouml;ydett&auml;v&auml; parhaat keinot sen kanssa el&auml;miseen. Tehk&auml;&auml;mme jokainen omat ratkaisumme sen suhteen mihin ja miss&auml; sit&auml; k&auml;yt&auml;mme - tulevaisuus n&auml;ytt&auml;nee sen, ketk&auml; siit&auml; lopulta hy&ouml;tyv&auml;t ja ketk&auml; h&auml;vi&auml;v&auml;t. Ehk&auml; n&auml;m&auml;kin lauseet j&auml;&auml;v&auml;t leijumaan, kenenk&auml;&auml;n koskaan lukematta, teko&auml;lyn autioittaman netin laitamille.
</p>
<?php
require MVAROOTPATH.'include/footer.php';
echo generate_footer(false, false, true);
?>
