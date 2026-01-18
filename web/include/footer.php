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

mysqli_close($conn);

function generate_footer($mobile, $suggest, $showad = false)
{
	if ($showad) {
		echo "\n".'<hr style="height:10px;border-width:0;color:#d2691e;background-color:#d2691e">'."\n";
		include 'horizad.php';
	}

	echo "\n".'<hr style="height:10px;border-width:0;color:#d2691e;background-color:#d2691e">'."\n";
	echo '<div class="footer">'."\n";
	echo '<table class="structure"><tr><td>'."\n";
	echo 'Copyright (c) 2025, Matti Vaittinen mazziesaccount (at) gmail com'."\n";
	if (!$suggest) {
		if (!$mobile) {
			$endlink = '</a>';
			$link_dominioncg = '<a target="_blank" href="https://dominioncg.fandom.com">';
			$link_dominionstrategy = '<a target="_blank" href="https://wiki.dominionstrategy.com">';
			$link_cc_license = '<a target="_blank" href="https://creativecommons.org/licenses/by-nc-sa/3.0/">';
		} else {
			$endlink = '';
			$link_dominioncg = '';
			$link_dominionstrategy = '';
			$link_cc_license = '';
		}

		echo ' -- Korttien kuvat on alunperin p&ouml;llitty '.$link_dominioncg.'dominioncg.fandom.com'.$endlink.' ja '.$link_dominionstrategy.'wiki.dominionstrategy.com'.$endlink.' -sivustoilta. Lisensoitu '.$link_cc_license.'CC-BY-NC-SA'.$endlink." -lisenssill&auml;. (Vinkkaathan jos l&ouml;yd&auml;t k&auml;ytett&auml;viss&auml;olevia suomenkielisten korttien kuvia)\n";
	}
	echo '</td></tr></table>'."\n";
	echo '</div>'."\n"; // footer styling ends
	echo "</body> </html>";
}
?>
