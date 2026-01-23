<?php
/*
 * Dominon card randomizer.
 * Uses MySQL database.
 *
 * AUTHOR: Matti Vaittinen <mazziesaccount@gmail.com>
 *
 * Written just for fun. No Warranty. Use at your own risk!
 * Copyright 2026, Matti Vaittinen mazziesaccount@gmail.com>
 */

define("MVAROOTPATH", "");

require_once 'include/common.php';
require_once 'include/db.php';
require_once 'include/header.php';
require_once 'include/nav_bar.php';
require_once 'include/rating.php';

do_head("Dominion - arvostellut korttisetit");
$out = get_navlist(); 

$out .= '<h1>Muiden pelaamia settej&auml</h1>'."\n";

$out .= '<h3>Viimeisimm&auml;t arvostelut</h3>'."\n";
$links = get_latest_rates($conn, 5);
if (!$links) {
	$out .= 'Hups... En saanut arvosteluja haettua.';
} else {
	$out .= '<ul>'."\n";
	foreach($links AS $link) {
		$out .= "<li>".$link."</li>\n";
	}
	$out .= "</ul>\n";
}
$out .= '<h3>Viimeksi lis&auml;tyt korttisetit</h3>'."\n";

$links = get_latest_sets_and_rates($conn);
if (!$links){
	$out .= 'Hups... En saanut arvosteluja haettua.';
} else{
	$out .= '<ul>'."\n";
	foreach($links AS $link) {
		$out .= "<li>".$link."</li>\n";
	}
	$out .= '</ul>'."\n";
}

echo $out;
include 'include/footer.php';
generate_footer(false, true, true);

?>

