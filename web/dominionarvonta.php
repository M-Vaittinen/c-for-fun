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
 *
 * Tuhina'o-meter => paljon toimintoja (tuhinaa)
 * Tupina'o-meter => Nihilistipeli (paljon tupinaa lisäpiinalla)
 * Kapita'o-meter => Rahaa!
 *
 */

require_once 'include/common.php';

define("PRIZETYPE_ID_DEBT", 2);
define("SETUP_ID_DEBT", 44);

require 'include/db.php';
require 'include/header.php';
require 'include/dominion_common.php';
require 'include/card.php';
require 'include/dom_card_set.php';
require_once 'include/rating.php';

/*
 * Show ADS by default. Turn em off later for error page(s)
 */
$showads = true;

$allids = null;

//$DBG=true;
$DBG=false;
$TESTING=false;
$TEST_MOBILE = false;
//$TEST_MOBILE = true;

$preselected = null;
$keep_land_ids = null;
$keep_event_ids = null;
$keep_omena_ids = null;

/*
 * Handling of card rating and deck sharing is a hack. It uses same post/get data
 * structure as 'keeping' part of the cards when re-randomizing. For re-randomizing
 * we want to keep the 'keep this card'-box checked for cards user has already
 * selected. However, for shared deck or for the rating we may not want to do this.
 *
 * Add a global for indicating if the selections for 'kept cards' should be checked.
 * Default to yes, and change to no if the 'keepid'-data came from rating/sharing.
 */
$g_check_boxes = true;

if (isset($_GET['keepid']) && !isset($_POST['keepid'])) {
	$_POST['keepid'] = $_GET['keepid'];
	/* User has probably followed a link to a shared deck because forms use post. */
	$g_check_boxes = false;
}

$rate = null;

if (isset($_POST['keepid'])) {
	if (isset($_POST['rate'])) {
		$rate = check_post_numeric_die('rate', SET_RATE_MIN, SET_RATE_MAX);
		if (count($_POST['keepid']) < 10)
			die('Only full set can be rated');
		/* Page was load as a result of rating. Don't pre-select 'keep'-checkboxes */
		$g_check_boxes = false;
	}

	foreach ($_POST['keepid'] AS $keepid) {
		if (!is_numeric($keepid))
			die('Non numeric KID');

		if ($keepid < LAND_ID_OFFSET) { /* Regular Kingdom card */
			if (!isset($_POST['keepprize'.$keepid]) && !isset($_GET['keepprize'.$keepid]))
				die('Prizeless KID');
			$kidprize = (isset($_POST['keepprize'.$keepid])) ? $_POST['keepprize'.$keepid] : $_GET['keepprize'.$keepid];
			if (!is_numeric($kidprize))
				die('KID is not a number');
			if ($kidprize < 4) {
				$preselected[0][] = $keepid;
				$preselected_prize[0][] = $kidprize;
			} else if ($kidprize == 4) {
				$preselected[1][] = $keepid;
				$preselected_prize[1][] = $kidprize;
			} else {
				$preselected[2][] = $keepid;
				$preselected_prize[2][] = $kidprize;
			}
		} else if ($keepid < EVENT_ID_OFFSET) { /* Landmark card */
			$keep_land_ids[] = $keepid - LAND_ID_OFFSET;
		} else if ($keepid < OMENA_ID_OFFSET) { /* Event card */
			$keep_event_ids[] = $keepid - EVENT_ID_OFFSET;
		} else { /* keep prophecy */
			$keep_omena_ids[] = $keepid - OMENA_ID_OFFSET;
		}
	}
}

$event_exp = return_post_numeric_array_or_zero('event_expansions');
$land_exp = return_post_numeric_array_or_zero('landmark_expansions');
$exp = return_post_numeric_array_or_zero('expansion');


$tuh_inafactor = return_post_numeric_or_zero('tuhinarange', -10, 10);
$tup_inafactor = return_post_numeric_or_zero('tupinarange', -10, 10);
$kap_itafactor = return_post_numeric_or_zero('kapitarange', -10, 10);

if (isset($_POST['add_nihilism'])) {
	$nihilism = true;
} else {
	$nihilism = false;
}

debug_print("Recv'd tuhina: $tuh_inafactor, Tupina: $tup_inafactor (nihilism $nihilism), Kapita: $kap_itafactor");
/* On a mobile device we try to fit the tables on a screen */
if ($TEST_MOBILE)
	$mobile = 1;
else
	$mobile = isMobileDevice();

if ($rate != null) {
	store_rates($conn, $rate, $preselected, $keep_land_ids, $keep_event_ids, $keep_omena_ids);
}

do_head("Dominion - korttiarvonta v2", $mobile);
echo '<div class="header nolink">'."\n";
echo '    <img src="img/dominion-app-icon-4x4.jpg" alt="logo">'."\n";
echo '    <h1>Dominion - Arvo kortit v2</h1>'."\n";
echo '</div>'."\n";

echo output_input_form($conn, $mobile, $exp, $land_exp, $event_exp, $tuh_inafactor, $tup_inafactor, $kap_itafactor);

function random_card_from_array(&$card, $min_weight)
{
	$idx = 0;
	$sum_weigh = 0;

	debug_print(count($card)." cards");
	foreach($card as $c)
		$sum_weigh += $c->weight - $min_weight + 1;

	$random = mt_rand(1, $sum_weigh);

	$weigh_accum = 0;
	foreach($card as $c) {
		$weigh_accum += $c->weight - $min_weight + 1;

		if ($weigh_accum >= $random) {
			$picked = $c;
			break;
		}
		$idx++;
	}
	if (!isset($picked))
		die("<br />Weighing failed!");
	array_splice($card, $idx, 1);

	return $picked;
}

function randomize_cards($card, $tuh_inafactor, $tup_inafactor, $nihilism, $kap_itafactor, $num_cards)
{
	global $CARDTYPE_MONEY;
	global $CARD_DEFAULT_WEIGH;

	debug_print("weighing ... $num_cards");

	$min_weight = $CARD_DEFAULT_WEIGH;

	if (isset($tuh_inafactor)) {
		/*
		 * Each card has "tuhinakerroin" from 0 => 10, describing how much "hassle" the card causes.
		 * The "$tuh_inafactor" is user's preference (selected by slider), ranging from -10 to +10.
		 * If user enables the slider, we will either increase or decrease the card's chance of being
		 * selected by adding a factor to it's weigh. The addition is user's selection * card's
		 * "tuhinakerroin", so maximum 10 * 10 (100), minimum -10 * 10 (-100).
		 * Original unaltered weight for every card is 100, so this can either double or zero specific
		 * card's chances to be selected. TODO: Do we really want to zero card's chance? If so, we can
		 * simply omit all cards with weight 0 or less when randomizing.
		 */
		foreach($card as $c) {
			$c->weight += $c->tuhinakerroin * $tuh_inafactor - 10;
			if ($c->weight < $min_weight)
				$min_weight = $c->weight;
		}
	}
	if (isset($tup_inafactor)) {
		foreach($card as $c) {
			/* Let's increase/decrease the chances of cards which are attacks or curses */
			/* Attack or curse cards get -100 ... 100 added to weigh */
			if ($c->attack || $c->curse) {
				$c->weight += $tup_inafactor * 10;
			}
			if ($nihilism) {
				/*
				 * Oh, you really want to suffer ?
				 * We decrease chances of defensive cards and cards with 'actionmoney'
				 * The largest 'actionmoney' ATM is the Death Cart, 5. This means the
				 * weigh can change -50..0
				 */
				$c->weight -= $c->actionmoney * 10;
				/* And, for defence cards, another -50 */
				if ($c->defence)
					$c->weight -= 50;

			}
			if ($c->weight < $min_weight)
				$min_weight = $c->weight;
		}
	}
	if (isset($kap_itafactor)) {
		foreach($card as $c) {
			/*
			 * And here the biggest change is Death Cart's actionmoney 5 * min/max capitafactor * 2
			 * => 5 * (+/-)10 * 2 => -100 ... +100
			 */
			$c->weight += $c->actionmoney * $kap_itafactor * 2;
			if ($c->type_id == $CARDTYPE_MONEY)
				$c->weight += $kap_itafactor * 10;
			if ($c->weight < $min_weight)
				$min_weight = $c->weight;
		}
	}

	for ($i = 0; $i < $num_cards; $i++) {
		debug_print("weighing loop, $i of ".($num_cards - 1)." ...");
		$selected[] = random_card_from_array($card, $min_weight);
	}

	return $selected;
}

function get_prize_buckets($conn, $exp)
{
	return array(3, 4);
	/*
	 * Idea of this code was to split the cards to 3 buckets, cheap, mid and expensive (as previous version did).
	 * Original versions used prizes prize < 4, prize == 4 and prize > 4 for categories.
	 *
	 * While testing things with 'Nousukausi' expansion alone, I noticed that it only had 3 cards that costed less than 3.
	 * So, with the original division, this resulted same 3 cards to always be selected for 'cheap' category.
	 *
	 * So, I developed the code below, which queried the prizes of the cards in selected expansions, and tried to
	 * compute them into 3 roughly same-sized buckets. Fine ?
	 *
	 * No. There are severe problems with this approach.
	 * 
	 * 1. Several expansions have majority of cards costing around 3 and 4 coins. With the code below, for example the
	 * 'Guilds' expansion will have no cards in the 'expensive' set - and things just break apart.
	 *
	 * 2. When the game begins, there is 10 cards in hand, 7 of which are copper, 3 are victory cards. It means that the
	 * typical hand(s) during first rounds will be 3 and 4 coins - which means that if the 'cheap' bucket ends up having
	 * only cards costing 4, it gets unlikely players have an option to buy other but silver. This sounds boring. Even more
	 * boring it gets if low limit could climb to 5 or more.
	 *
	 * So, let's just keep the old ranges: ([<4], [4], [>4]) for now.
	 *
	 * TODO: Find a way to make amount of cards in each bucket more flexible. This helps if some expansions don't have
	 * enough cards in a specific bucket.
	 *
	 *
	if ($exp)
		$QUERY = "SELECT prize FROM cards WHERE" . SQL_add_expansion_where( 'expansion_id', $exp);
	else
		$QUERY = "SELECT prize FROM cards";

	$res = query_cards($conn, $QUERY);
	$num_cards = mysqli_num_rows($res);

	$bucket_optimal = $num_cards / 3;

	$prizes = array();
	while ($row = mysqli_fetch_assoc($res)) {
		$prize = $row['prize'];
		if (!isset($prizes[$prize]))
			$prizes[$prize] = 1;
		else
			$prizes[$prize] ++;
	}
	ksort($prizes);

	$sumnum = 0;
	$prevprize = 0;

	$dbg = $num_cards;

	foreach($prizes as $prize => $num) {
		debug_print("prize $prize, cards $num, bucket-size ". ($sumnum + $num) .", optimal $bucket_optimal ");
		if ($sumnum + $num > $bucket_optimal) {
			if ($sumnum > 3) {
				$boundary[] = $prevprize;
				debug_print("BOUNDARY: $prevprize, ($sumnum cards)");
				$dbg -= $sumnum;
				$sumnum = $num;
			} else {
				$boundary[] = $prize;
				debug_print("BOUNDARY: $prize, (".($sumnum + $num)." cards)");
				$dbg -= ($sumnum + $num);
				$sumnum = 0;
			}
			if (count($boundary) == 2) {
				debug_print("last bucket has $dbg cards");
				return $boundary;
			}
		} else {
			$sumnum += $num;
		}
		$prevprize = $prize;
	}

	die("Can't create prize buckets. Too few cards in selected expansions?<br />");
	*/
}

function do_prize_bucket_where($boundaries, $prize_column)
{
	$where[] = "$prize_column <= $boundaries[0]";
	$where[] = "($prize_column > $boundaries[0] AND $prize_column <= $boundaries[1])";
	$where[] = "$prize_column > $boundaries[1]";

	return $where;
}

$boundary_prizes = get_prize_buckets($conn, $exp);
$PRIZEBUCKETS = do_prize_bucket_where($boundary_prizes, 'c.prize');

$QUERY_BASE = 'SELECT c.id AS id, c.dual_top_of_id, c.tuhinakerroin AS tuhinakerroin, c.actionmoney AS actionmoney, c.curse AS curse, c.attack AS attack, c.defence AS defence, c.type_id AS type_id FROM cards AS c LEFT JOIN expansion as e ON c.expansion_id = e.id WHERE e.disabled != 1 AND c.dual_below_id = 0 AND ';

$num_cards = array(3, 3, 4);
$card_group_names = array('Halpaa ku saippua', 'Keskiluokan keskiostos', 'Nää Mää Tahdon!');

$card_set = dom_card_set::prepare_set($conn);

function add_existing(&$selected, $preselected)
{
	foreach($preselected AS $new_id) {
		$new_card = new dom_card();

		$new_card->id = $new_id;
		$selected[] = $new_card;	
	}
}

function exclude_presel_id($presel)
{
	$where = '';
	foreach($presel AS $exclude_id)
		$where .= " AND c.id != $exclude_id";

	return $where;
}

function add_landmark_kinput($land_id, $offset, $checked) {
	$land_id += $offset;
	$out = '<input form="theform" type="checkbox" name="keepid[]" value="'.$land_id.'"'.$checked.'>'."\n";
	return $out;
}

function add_landmark_cardpic($cardname, $imagename, $mobile)
{
	if (!$mobile)
		$landclass = "landscape-card-img";
	else
		$landclass = "landscape-card-img-mobile";

	$out = '<div class="image-container"><p tabindex="0">' . $cardname . '<div class="hover-text"><img class="'.$landclass.'" src="cardpics/'.$imagename.'"></div></div>'."\n";
		return $out;

}


/* TODO: Refactor this. Make a generic function which can be separately called for
 * all of the different types of cards
 */
function show_eventland($conn, $event_exp_ids, $land_exp_ids, $keep_land_ids, $keep_event_ids, $omena, $keep_omena_ids, $all_exp_ids, &$all_ids, $mobile = true)
{
	global $g_check_boxes;
	$out = "";

	if (!($event_exp_ids || $land_exp_ids || $keep_land_ids || $keep_event_ids || $omena))
		return;

	if ($omena) {
		$query = "SELECT prophecy.id, prophecy.name, prophecy.imagename, prophecy.description, setup.text, setup.id AS stupid, expansion.name AS exp_name, etype.name AS typename ";
		$query .= 'FROM prophecies AS prophecy ';
		$query .= 'LEFT JOIN setup_extras AS setup ON prophecy.setup_id = setup.id ';
		$query .= 'LEFT JOIN expansion AS expansion ON prophecy.expansion_id = expansion.id ';
		$query .= 'LEFT JOIN event_type AS etype ON prophecy.type_id = etype.id ';

		$where = "";
		if (!$keep_omena_ids) {
			if ($all_exp_ids) {
				$query .= "WHERE ";
				foreach($all_exp_ids as $expid) {
					if ($where == "")
						$where .= "expansion.id = $expid";
					else
						$where .= " OR expansion.id = $expid";
				}
				$query .= $where;
			}
			$query .= ' ORDER BY RAND()';
		} else {
			$query .= "WHERE ";
			$query .= 'prophecy.id = '.$keep_omena_ids[0];
		}
		$query .= ' LIMIT 1';

		$result = mysqli_query($conn, $query);
		if (!$result)
			die("no expansions".mysql_error($conn));

		if (!$mobile) {
			$out .= '<h3>OmenaProphecy</h3>'."\n";
			$out .= '<table class="cardlist"><tr>'."\n";
			$out .= '<th class="checkbox">[pid&auml;]</th><th class="landcardname">Kortti</th><th>Selitys</th><th class="squeeze">Specials</th><th>Hinta</th><th>Peliosa</th></tr>'."\n";
		} else {
			$out .= '<h3>OmenaProphecy</h3>'."\n";
			$out .= '<table class="cardlist"><tr>'."\n";
			$out .= '<th class="checkbox">[pid&auml;]</th><th class="landcardname-mobile">Kortti</th><th>Specials</th> <th>Peliosa</th></tr>'."\n";
		}
		while ($row = mysqli_fetch_assoc($result)) {
			$checked = "";
			$setup_tip = "";

			$cardname = htmlspecialchars($row['name']);
			$imagename = htmlspecialchars($row['imagename']);
			$expansionname = htmlspecialchars($row['exp_name']);
			$description = htmlspecialchars($row['description']);
			$cardtype = htmlspecialchars($row['typename']);

			$all_ids[] = $row['id'] + OMENA_ID_OFFSET;

			if ($g_check_boxes && $keep_omena_ids)
				if ($keep_omena_ids[0] == $row['id'])
					$checked = "checked";
			if ($row['text'] && $row['stupid'] != SETUP_ID_DEBT) {
				$setup_tip_text = htmlspecialchars($row['text']);
				$setup_tip .= '<div class="image-container">'."\n";
				$setup_tip .= '<img src="img/peasant.png" alt="Valmistelut" tabindex="0">'."\n";
				$setup_tip .= '<div class="hover-text">Extra valmisteluja: '.$setup_tip_text.'</div>'."\n";
				$setup_tip .= '</div>'."\n";
			} else if($row['text'] && $row['stupid'] == SETUP_ID_DEBT) {
				$setup_tip .= '<div class="image-container">'."\n";
				$setup_tip .= '<img src="img/debt.png" alt="Myyd&auml;&auml;n Rahoituksella" tabindex="0">'."\n";
				$setup_tip .= '<div class="hover-text">Myyd&auml;&auml;n Rahoituksella</div>'."\n";
				$setup_tip .= '</div>'."\n";
			}
			if (!$mobile) {
				$out .= '<tr>'."\n";
				$out .= '<td class="checkbox">'.add_landmark_kinput($row['id'], OMENA_ID_OFFSET, $checked).'</td>'."\n";
				$out .= '<td>'.add_landmark_cardpic($cardname, $imagename, $mobile).'</td>'."\n";
				$out .= '<td>'.$description.'</td>'."\n";
				$out .= '<td>'. (($setup_tip != '') ? $setup_tip : '--') .'</td>'."\n";
				$out .= '<td>'.$cardtype.'</td>'."\n";
				$out .= '<td>'.$expansionname.'</td>'."\n";
				$out .= '</tr>'."\n";
			} else {
				$out .= '<tr>'."\n";
				$out .= '<td class="checkbox">'.add_landmark_kinput($row['id'], OMENA_ID_OFFSET, $checked).'</td>'."\n";
				$out .= '<td>'.add_landmark_cardpic($cardname.' ('.$cardtype.')', $imagename, $mobile).'</td>'."\n";
				$out .= '<td>'. (($setup_tip != '') ? $setup_tip : '--') .'</td>'."\n";
				$out .= '<td>'.$expansionname.'</td>'."\n";
				$out .= '</tr>'."\n";
			}
		} 
		$out .= '</table>';
	} // If omena ends.

	if ($event_exp_ids || $keep_event_ids) {
		$query_base = "SELECT events.id, events.name, events.name_en, events.imagename, events.prize, events.debt, events.curses, setup.text, setup.id AS stupid, expansion.name AS exp_name ";
		$query_base .= "FROM events AS events ";
		$query_base .= "LEFT JOIN setup_extras AS setup ON events.setup_id = setup.id ";
		$query_base .= "LEFT JOIN expansion AS expansion ON events.expansion_id = expansion.id ";
		$query_base .= "WHERE ";

		$query_keep = "";

		$num_keep_evs = 0;
		if ($keep_event_ids) {
			$num_keep_evs = count($keep_event_ids);
			$query_keep = $query_base; 
			$query_keep .= "events.id = $keep_event_ids[0]";
		}
		/*
		 * There is a special case where user has selected to keep an event,
		 * but disabled the 'events' from expansion for the new query. In this
		 * case the RightThingToDo(tm) is to keep the selected event but to not
		 * randomize new card. So, let's forget all the expansion_id stuff when
		 * !$event_exp_ids.
		 */

		if ($num_keep_evs == 2 || !$event_exp_ids) {
			if ($num_keep_evs == 2)
				$query = $query_keep . " OR events.id = $keep_event_ids[1]";
			else
				$query = $query_keep;
			$query .= " LIMIT 2";
		} else {
			$query = $query_base;

			$where = "";
			foreach($event_exp_ids as $expid) {
				if ($where == "")
					$where .= "(expansion.id = $expid";
				else
					$where .= " OR expansion.id = $expid";
			}
			$where .= ')';
			$query .= $where;
			$query .= " ORDER BY RAND() LIMIT ".(2 - $num_keep_evs);

			if ($num_keep_evs) {
				$query = "(($query) UNION " . $query_keep . ") LIMIT 2";
			}
		}

		$result = mysqli_query($conn, $query);
		if (!$result)
			die("no expansions".mysql_error($conn));

		if (!$mobile) {
			$out .= '<h3>Tapahtumat</h3>'."\n";
			$out .= '<table class="cardlist"><tr>'."\n";
			$out .= '<th class="checkbox">[pid&auml;]</th><th class="landcardname">Kortti</th><th class="squeeze">Specials</th><th>Hinta</th><th>Peliosa</th></tr>'."\n";
		} else {
			$out .= '<h3>Tapahtumat</h3>'."\n";
			$out .= '<table class="cardlist"><tr>'."\n";
			$out .= '<th class="checkbox">[pid&auml;]</th><th class="landcardname-mobile">Kortti</th><th>Specials</th> <th>Peliosa</th></tr>'."\n";
		}

		while ($row = mysqli_fetch_assoc($result)) {
			$checked = "";
			$setup_tip = "";
			if ($g_check_boxes && $keep_event_ids) {
				foreach($keep_event_ids AS $kid) {
					if ($kid == $row['id']) {
						$checked = "checked";
						break;
					}
				}
			}

			$all_ids[] = $row['id'] + EVENT_ID_OFFSET;

			$cardname = htmlspecialchars($row['name']);
			$expansionname = htmlspecialchars($row['exp_name']);
			$imagename = htmlspecialchars($row['imagename']);
			$cardname_en = htmlspecialchars($row['name_en']);
			if ($cardname_en && $cardname_en != $cardname)
				$cardname .= " ($cardname_en)";

			if ($row['text']) {
				$setup_tip_text = htmlspecialchars($row['text']);
				$setup_tip .= '<div class="image-container">'."\n";
				$setup_tip .= '<img src="img/peasant.png" alt="Valmistelut" tabindex="0">'."\n";
				$setup_tip .= '<div class="hover-text">Extra valmisteluja: '.$setup_tip_text.'</div>'."\n";
				$setup_tip .= '</div>'."\n";
			}
			if ($row['debt']) {
				$setup_tip .= '<div class="image-container">'."\n";
				$setup_tip .= '<img src="img/debt.png" alt="Myyd&auml;&auml;n Rahoituksella" tabindex="0">'."\n";
				$setup_tip .= '<div class="hover-text">Myyd&auml;&auml;n Rahoituksella</div>'."\n";
				$setup_tip .= '</div>'."\n";
			}
			if (!$mobile) {
				$out .= '<tr><td class="checkbox">'.add_landmark_kinput($row['id'], EVENT_ID_OFFSET, $checked).'</td><td class="landcardname">'.add_landmark_cardpic($cardname, $imagename, $mobile).'</td><td>'. (($setup_tip != '') ? $setup_tip : '--') .'</td><td>'.$row['prize'].'</td><td>'.$expansionname.'</td></tr>'."\n";
			} else {
				$out .= '<tr><td class="checkbox">'.add_landmark_kinput($row['id'], EVENT_ID_OFFSET, $checked).'</td><td class="landcardname-mobile">'.add_landmark_cardpic($cardname, $imagename, $mobile).'</td><td>'. (($setup_tip != '') ? $setup_tip : '--') .'</td><td>'.$expansionname.'</td></tr>'."\n";
			}
		} // while () MySQL results ends
		$out .= '</table>';
	} // if $event_exp_ids ends

	if ($land_exp_ids || $keep_land_ids) {
		$query = 'SELECT landmark.id, landmark.name, landmark.name_en, landmark.imagename, landmark.description, setup.text, expansion.name AS exp_name, etype.name AS typename ';
		$query .= 'FROM landmarks AS landmark ';
		$query .= 'LEFT JOIN setup_extras AS setup ON landmark.setup_id = setup.id ';
		$query .= 'LEFT JOIN expansion AS expansion ON landmark.expansion_id = expansion.id ';
		$query .= 'LEFT JOIN event_type AS etype ON landmark.type_id = etype.id ';
		$query .= 'WHERE ';

		if (!$keep_land_ids) {
			$where = "";
			foreach($land_exp_ids as $expid) {
				if ($where == "")
					$where .= "expansion.id = $expid";
				else
					$where .= " OR expansion.id = $expid";
			}
			$query .= $where;
			$query .= ' ORDER BY RAND()';
		} else {
			$query .= 'landmark.id = '.$keep_land_ids[0];
		}
		$query .= ' LIMIT 1';

		$result = mysqli_query($conn, $query);
		if (!$result)
			die("no expansions".mysql_error($conn));

		if (!$mobile) {
			$out .= '<h3>Muokkaukset</h3>'."\n";
			$out .= '<table class="cardlist"><tr>'."\n";
			$out .= '<th class="checkbox">[pid&auml;]</th><th class="landcardname">Kortti</th><th>Selitys</th><th class="squeeze">Specials</th><th>Tyyppi</th><th>Peliosa</th></tr>'."\n";
		} else {
			$out .= '<h3>Muokkaukset</h3>'."\n";
			$out .= '<table class="cardlist"><tr>'."\n";
			$out .= '<th class="checkbox">[pid&auml;]</th><th class="landcardname-mobile">Kortti (tyyppi)</th><th>Specials</th><th>Peliosa</th></tr>'."\n";
		}

		while ($row = mysqli_fetch_assoc($result)) {
			$setup_tip = "";
			$checked = "";

			$cardname = htmlspecialchars($row['name']);
			$imagename = htmlspecialchars($row['imagename']);
			$cardname_en = htmlspecialchars($row['name_en']);
			if ($cardname_en && $cardname_en != $cardname)
				$cardname .= " ($cardname_en)";
			$expansionname = htmlspecialchars($row['exp_name']);
			$description = htmlspecialchars($row['description']);
			$cardtype = htmlspecialchars($row['typename']);

			$all_ids[] = $row['id'] + LAND_ID_OFFSET;

			if ($g_check_boxes && $keep_land_ids)
				if ($keep_land_ids[0] == $row['id'])
					$checked = "checked";
			if ($row['text']) {
				$setup_tip_text = htmlspecialchars($row['text']);
				$setup_tip .= '<div class="image-container">'."\n";
				$setup_tip .= '<img src="img/peasant.png" alt="Valmistelut" tabindex="0">'."\n";
				$setup_tip .= '<div class="hover-text">Extra valmisteluja: '.$setup_tip_text.'</div>'."\n";
				$setup_tip .= '</div>'."\n";
			}
			if (!$mobile) {
				$out .= '<tr>'."\n";
				$out .= '<td class="checkbox">'.add_landmark_kinput($row['id'], LAND_ID_OFFSET, $checked).'</td>'."\n";
				$out .= '<td class="landcardname">'. add_landmark_cardpic($cardname, $imagename, $mobile).'</td>'."\n";
				$out .= '<td>'.$description.'</td>'."\n";
				$out .= '<td>'. (($setup_tip != '') ? $setup_tip : '--') .'</td>'."\n";
				$out .= '<td>'.$cardtype.'</td>'."\n";
				$out .= '<td>'.$expansionname.'</td>'."\n";
				$out .= '</tr>'."\n";
			} else {
				$out .= '<tr>'."\n";
				$out .= '<td class="checkbox">'.add_landmark_kinput($row['id'], LAND_ID_OFFSET, $checked).'</td>'."\n";
				$out .= '<td class="landcardname-mobile">'.add_landmark_cardpic($cardname . "[$cardtype]", $imagename, $mobile).'</td>'."\n";
				$out .= '<td>'. (($setup_tip != '') ? $setup_tip : '--') .'</td>'."\n";
				$out .= '<td>'.$expansionname.'</td>'."\n";
				$out .= '</tr>'."\n";
			}
		} // while MySQL results ends
		$out .= '</table>';
	} // if ($land_exp_ids) ends

	echo $out;
}

$i = 0;
foreach($PRIZEBUCKETS as $PRIZE_LIMIT) {
	$selected = null;
	$exp_where = SQL_add_expansion_where('c.expansion_id', $exp);

	$query = $QUERY_BASE.$PRIZE_LIMIT;
	if ($exp_where)
		$query .= " AND ".$exp_where;

	if (isset($preselected[$i]))
		$num_presel=count($preselected[$i]);
	else
		$num_presel = 0;
	if ($num_presel) {
		$query .= exclude_presel_id($preselected[$i]);
	}
	$num_cards_to_rand = $num_cards[$i] - $num_presel;

	if ($num_cards_to_rand) {

		$result = query_cards($conn, $query);
		$foo = 0;
		while ($row = mysqli_fetch_assoc($result)) {
			$foo++;
			$card[] = dom_card::from_partial_row($row);
		}
		debug_print("$foo cards fetched for ".htmlspecialchars($card_group_names[$i])." - selecting from those:\n");
		if (count($card) < $num_cards_to_rand) {
			echo '<h3>Ei riitt&auml;v&auml;sti hintaryhm&auml;n <i>"'.htmlspecialchars($card_group_names[$i]).'"</i>-kortteja.</h3> Valitse useampi (tai isompi) lis&auml;osa';
			$showads = false;
			goto page_end;
		}
		$selected = randomize_cards($card, $tuh_inafactor, $tup_inafactor, $nihilism, $kap_itafactor, $num_cards_to_rand);
	}
	if ($num_presel)
		add_existing($selected, $preselected[$i]);

	$card_set->add_set($selected, $card_group_names[$i]);
	$card = array();

	$i++;

	debug_print("bucket $i: $PRIZE_LIMIT");
}
$card_set->get_cards();
$allids = $card_set->get_all_ids();
$allprizes = $card_set->get_all_prizes();
$omena = $card_set->show_sets($preselected, $g_check_boxes, $mobile);

if ($land_exp || $event_exp || $keep_land_ids || $keep_event_ids || $omena) {
	show_eventland($conn, $event_exp, $land_exp, $keep_land_ids, $keep_event_ids, $omena, $keep_omena_ids, $exp, $allids, $mobile);
}

$allink = '';
$url = 'https://';
if ($_SERVER['SERVER_PORT'] != '80')
  $url .= $_SERVER['SERVER_NAME'].':'.$_SERVER['SERVER_PORT'].$_SERVER['PHP_SELF'];
else
	$url .= $_SERVER['SERVER_NAME'].$_SERVER['PHP_SELF'];

foreach($allids AS $aid) {
	if ($allink == '')
		$allink = "$url?keepid[]=$aid";
	else
		$allink .= "&keepid[]=$aid";
}

$i=0;

foreach($allprizes AS $aip) {
	$allink .= '&keepprize'.$allids[$i]."=$aip";
	$i++;
}

require 'include/share.php';
echo '<p>Piditkö n&auml;ist&auml; korteista? Jaa arpomasi setti kaverillesikin</p>';
echo sharebtn(urlencode($allink), $allink);

//echo "<!--";
echo '<p>Korttisettien arviointi on kokeellinen ominaisuus. Voit antaa pelin j&auml;lkeen 1-5 t&auml;hte&auml; arvotuille korteille sen mukaan, miten setti mielest&auml;si toimi. T&auml;ll&auml;hetkell&auml; annetut arvosanat eiv&auml;t n&auml;y miss&auml;&auml;n mutta tavoitteena on jatkossa lis&auml;t&auml; suosittuja valmiita settej&auml; valittavaksi, satunnaisesti arvottujen lis&auml;ksi.</p>';
echo ratebtn($allids, $allprizes);
//echo "-->";
page_end:

/* Close connection, print (c) and send </body> </html> */
echo '<p><h1><a href="aloittaja.php" target="_blank">Arvo my&ouml;s aloittaja?</a></h1>';
require 'include/footer.php';
generate_footer($mobile, false, $showads);

?>
