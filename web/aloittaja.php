<?php
/*
 * Starting player randomizer.
 * Uses MySQL database.
 *
 * AUTHOR: Matti Vaittinen <mazziesaccount@gmail.com>
 *
 * Written just for fun. No Warranty. Use at your own risk!
 *
 * Copyright 2025, Matti Vaittinen mazziesaccount@gmail.com>
 */


require 'include/db.php';
require 'include/header-suggest.php';

$mobile = isMobileDevice();

//$SURPRIZE_TEST = 1;
$quantity = 0;

/*
 * Show ADS by default, but set them off for error page(s)
 */
$showad = true;

function output_starter_form()
{

	$output = '<form action="" method="post">';
	$output .= '
		<input id="yesspeed" type="checkbox" name="yesspeed" value="1">
		<label for="yesspeed">Nyt Tod Jaksaa!</label>
		<input id="nospeed" type="checkbox" name="nospeed" value="1">
		<label for="nospeed">Nyt Ei Jaksa</label>
		<button type="submit" name="players" value="1">1 player</button>
		<button type="submit" name="players" value="2">2 players</button>
		<button type="submit" name="players" value="3">3 players</button>
		<button type="submit" name="players" value="4">4 players</button>
		<button type="submit" name="players" value="5">5 players</button>
		<label for="quantity">No montako sitten? (6..10):</label>
		<input type="number" id="quantity" name="quantity" min="6" max="10">
		<input type="submit" value="Submit">
		</form>';

	/* Output form */
	return $output;
}

if (isset($_POST['quantity']))
	if (is_numeric($_POST['quantity']))
		if ($_POST['quantity'] < 11 && $_POST['quantity'] > 5)
			$quantity = $_POST['quantity'];

if (!$quantity)
	if (isset($_POST['players']))
		if (is_numeric($_POST['players']))
			if ($_POST['players'] < 6 && $_POST['players'] > 0)
				$quantity = $_POST['players'];

$nospeed = "";
if (isset($_POST['nospeed']))
	$nospeed = " WHERE surprize = 0 ";

if (isset($_POST['yesspeed'])) {
	if ($nospeed != "") {
		$output = "<h1>Et Osannut P&auml;&auml;tt&auml;&auml; jaksatko?</h1>";
		$output .= output_starter_form();
		$showad = false;
		goto out;
	}
	$nospeed = " WHERE surprize = 1 ";
}

do_head("Aloittajan arvonta", $mobile);

if (!$quantity) {
	echo "<h1>Aloittaja-arvonta... </h1>";
	echo '<table class="cardlist"><tr><td>';
	echo 'Aloittaja arvonta antaa pelin aloittajan kaksiosaisena arvontana. Ensin arvotaan yksi henkilö, jonka jälkeen annetaan tarkennus. Tarkennus voi kertoa aloittajan olevan edellä arvottu henkilö itse, tai se voi kertoa aloittajan olevan joku muu, kuitenkin siten että aloittaja määräytyy edellä arvotun henkilön mukaan. (Esim, arvotun henkilön vasemmalla puolella oleva pelaaja). Äsh, vaikea selittää, helpompi vain kun painat sitä nappia ja katsot mitä tapahtuu!<br /> <br /> Ps. Älä ota tätä liian vakavasti :) Jos asiassa on liikaa tulkinnanvaraa niin arvo rohkeasti uudelleen - tai heitä vaikka kolikkoa.';
	echo '</td></tr></table>';
}
if ($quantity) {
	if ($quantity <= 1) {
		echo "<h2>Oletko ihan tosissasi?</h2>";
		$output = output_starter_form();
		$showad = false;
		goto out;
	}

	$query = "SELECT * FROM starter $nospeed ORDER BY RAND() LIMIT 1";
	$result = mysqli_query($conn, $query);
	if (!$result)
		die("Internal error");

	if (mysqli_num_rows($result) <= 0)
		die("Internal error");

	$row = mysqli_fetch_assoc($result);
	$aloittaja_initial = htmlspecialchars($row['aloittaja']);

	/*
	 * We have 2 kinds of 'starter strings'.
	 *
	 * Ones, where the person identified by the text will be the chosen one,
	 * like 'The tallest person'.
	 *
	 * Then we have those, where the person identified by the text is not the
	 * one getting selected; for example:'The person left of the tallest person'.
	 *
	 * This, however, can make the randomizing unfair. This is easy to see
	 * if you think you weren't the tallest person in a group of 6 players,
	 * with a starter database consisting only of the two example strings.
	 *
	 * If randomizing was done without considering this, then it'd be 1/2
	 * chance to have "tallest person", and 1/2 chance of having the one
	 * left of the tallest.
	 *
	 * In a group of 6, you'd have 1/5 chance of sitting left of the
	 * tallest person, and it'd be 1/2 chance the 'left of tallest' got
	 * selected, making the chance for anyone but the tallest to be 1/10,
	 * while the tallest had a chance of 1/2.
	 *
	 * A bit unfair, eh?
	 *
	 * Hence, we first decide whether we look into the table of 'specific
	 * persons', or table of 'persons excluding specific one', weighing the
	 * table of specific persons to be 1/number of participants.
	 *
	 * This may still not be 100% ideal, but hey, life isn't always fair, right?
	 */
	$itse = rand(1, $quantity) == 1 ? 1 : 0;

	if (!$itse && !$row['surprize'] && !$SURPRIZE_TEST) {
		$query = 'SELECT * FROM position ORDER BY RAND() LIMIT 1';
		$result = mysqli_query($conn, $query);
		if (!$result)
			die("Internal error");
		
		$row = mysqli_fetch_assoc($result);
		$aloittaja = htmlspecialchars($row['pos']);

	} else { $aloittaja = 'itse!'; }

	$output_table = '<table class="aarvonta"><tr>'."\n";
	if ($row['surprize'] || $SURPRIZE_TEST) {
		$extraclass = "sfade-in";
		$output_table .= '<th colspan="3">Aseta n&auml;ytt&ouml; niin ett&auml; kaikki pelaajat n&auml;kev&auml;t sen</th></tr><tr>';
	} else {
		$extraclass = "svisible";
	}

	$output_table .= '<td colspan="3"><div class="'.$extraclass.' stop"><h3>'.$aloittaja_initial.'</h3><h3>'.$aloittaja.'</h3></div><div class="stop">Aloittaja: </div></td>'."\n";
	$output_table .= '</tr><tr>'."\n";
	$output_table .= '<td class="sleft">Aloittaja: <div class="'.$extraclass.'"><h3>'.$aloittaja_initial.'</h3><h3>'.$aloittaja."</h3></div></td>\n";
	$output_table .= '<td class="startform">'.output_starter_form().'</td>'."\n";
	$output_table .= '<td class="sright"><div class="'.$extraclass.'"><h3>'.$aloittaja.' </h3><h3> '.$aloittaja_initial.'</h3></div>Aloittaja: </td>'."\n";
	$output_table .= '</tr><tr>'."\n";
	$output_table .= '<td colspan="3">Aloittaja: <div class="'.$extraclass.'"><h3>'."$aloittaja_initial </h3><h3> $aloittaja\n </h3></div></td>";
	$output_table .= '</tr></table>'."\n";

	$output = '<div class="arvontakont">' . $output_table . '</div>'."\n";

	$output .= '<hr style="height:20px;border-width:0;color:#d2691e;background-color:#d2691e">';
} else {
	$output = output_starter_form();
}

out:

echo $output;

/* Display the 'suggest new starter lines' */

echo '<hr style="height:20px;border-width:0;color:#d2691e;background-color:#d2691e">';

include 'include/suggest.php';
if ($added = handle_new_suggestions($conn))
	echo '<h3>' . $added . '</h3>';

echo '<h2>Ehdota aloittajaa</h2>
	Jos sinulla on hyvä idea lisättäväksi "aloittaja-arpakoneeseen" niin täytä alla oleva kenttä!';

if (num_suggestions($conn) > 50)
	echo '<p>Ehdotuksia on kertynyt enemmän kun ehdin käsitellä. Kiitokseni ja pahoitteluni siitä. Palaa toki myöhemmin lisäämään ehdotuksesi - kaavake ilmestyy kyllä takaisin kunhan ehdin käsitellä entiset!';
else
	output_suggest_form($conn);

echo '<br/>';
echo '<br/>';

if ($added)
	show_suggestion($conn, 5);

/* Close connection, print (c) and send </body> </html> */
require 'include/footer.php';
generate_footer(false, true, $showad);
?>
