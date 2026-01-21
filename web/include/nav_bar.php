<?php
/*
 * Dominon card randomizer.
 * Uses MySQL database.
 *
 * AUTHOR: Matti Vaittinen <mazziesaccount@gmail.com>
 *
 * Written just for fun. No Warranty. Use at your own risk!
 *
 * Copyright 2025, Matti Vaittinen mazziesaccount@gmail.com>
 */

require_once MVAROOTPATH.'include/rating.php';

function nav_last_rated()
{
	global $conn;

	$urls = null;
	$rates = null;
	$votes = null;
	$link = null;

	if (get_last_rates_noformat($conn, 1, $urls, $rates, $votes) == 1) {
		$stars = rate2stars($rates[0]);
		$stardesc = ($stars > 1) ? "$stars T&auml;hte&auml;" : "1 Tähti";
		$link = '<a href="'.$urls[0].'">Viimeksi arvosteltu: ('.$stardesc.', t&aumlt&auml; mielt&auml; '.$votes[0].')</a>';
	}
	return $link;
}

function get_navlist($folder = '')
{
	$link = nav_last_rated();

	$out = '
	<div class="mvanavbar">
	<nav role="navigation">
	<ul>
		<li><a href="'.$folder.'/index.php">Etusivu</a></li>
		<!-- <li><a href="'.$folder.'/index.php"><img src="'.$folder.'/img/dominion-app-icon-4x4.jpg" alt="suffle-logo">Etusivu</a></li> -->
		<div class="dropdown">
			<button class="dropbutton">Dominion korttiarvonta<i class"fa fa-caret-down"></i></button>
			<div class="dropdown-content">
				<li><a href="'.$folder.'/dominionarvonta.php" target="_blank">Dominion korttiarvonta (uusi ikkuna)</a></li>
				<!-- <li><a href="'.$folder.'/dominiosetit.php">Muiden pelaamia</a></li> -->
				     <li>'.$link.'</li>
				<li><a href="'.$folder.'/index.php#dominfo">Tietoa Dominion korttiarvonnasta</a></li>
				<li><a href="'.$folder.'/story/index.php">Tarinaa arvontakoneen teosta</a></li>
				<li><a href="'.$folder.'/story/ai.php">AI arvontakoneen teossa</a></li>
			</div>
		</div>
		<div class="dropdown">
		<button class="dropbutton">Aloittajan arvonta<i class"fa fa-caret-down"></i></button>
			<div class="dropdown-content">
				<li><a href="'.$folder.'/aloittaja.php" target="_blank">Aloittajan arvonta (uusi ikkuna)</a></li>
				<li><a href="'.$folder.'/index.php#starterinfo">Tietoa aloittajan arvonnasta</a></li>
			</div>
		</div>
		<div class="dropdown mvaright">
			<button class="dropbutton">Yhteystiedot<i class"fa fa-caret-down"></i></button>
			<div class="dropdown-content mvaright">
				<li><a href="https://github.com/M-Vaittinen">Syyllinen</a></li>
				<li><a href="https://github.com/M-Vaittinen/c-for-fun/issues">Vikaraportit</a></li>
			</div>
		</div>
	</ul></nav></div>';

	return $out;
}

?>

