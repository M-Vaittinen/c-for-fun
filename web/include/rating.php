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

require MVAROOTPATH.'include/ratebtn.php';
require_once MVAROOTPATH.'include/common.php';

function create_seturl_from_row($conn, $row)
{
	/*	$url = "https://suffle.fi/dominionarvonta.php?"; */
	$url ="";
	$kingdom_fields = array('card0', 'card1', 'card2', 'card3', 'card4', 'card5', 'card6', 'card7', 'card8', 'card9');
	$land_fields = array('land0', 'land1');
	$event_fields = array('event0', 'event1');
	$propfield = 'prophecy';

	$query = "SELECT prize, id FROM cards WHERE id = ".$row[$kingdom_fields[0]];

	for ($i = 1; $i < 10; $i++)
		$query .= " OR id = ".$row[$kingdom_fields[$i]];

	$result = mysqli_query($conn, $query);
	if (!$result)
		die_and_debug("ratequery: $query".mysqli_error($conn));

	if ($numres = mysqli_num_rows($result) != 10) {
		debug_print("expected 10 results, got $numres");
		return false;
	}

	while ($prizerow = mysqli_fetch_assoc($result)) {
		if (!isset($prizerow['id']) || !isset($prizerow['prize'])) {
			debug_print("ID / prize not found\n");
			return false;
		}

		$id = $prizerow['id'];
		$prize = $prizerow['prize'];
		debug_print("ID: '".$id."'");
		debug_print("prize: '".$prize."'");

		if (!is_numeric($id) || $id == 0) {
			debug_print("Malformed ID!\n");
			return false;
		}
		if ($url == "")
			$url = MVAROOTPATH.'/dominionarvonta.php?keepid[]='.$id.'&keepprize'.$id.'='.$prize;
		else
			$url .= '&keepid[]='.$id.'&keepprize'.$id.'='.$prize;

		/*	debug_print("URL kierroksella $i: '".$url."'"); */
	}
	foreach ($land_fields AS $field) {
		$id = $row[$field];
		if ($id)
			$url .= '&keepid[]='.$id + LAND_ID_OFFSET;
	}
	foreach ($event_fields AS $field) {
		$id = $row[$field];
		if ($id)
			$url .= '&keepid[]='.$id + EVENT_ID_OFFSET;
	}
	if ($id = $row[$propfield])
		$url .= '&keepid[]='.$id + OMENA_ID_OFFSET;

	debug_print("returning URL $url\n");

	return $url;
}

function get_rate_desc_avg($rate)
{
	if ($rate < -7.5)
		return "1 t&auml;hti - Sysipaska";
	if ($rate < -2.5)
		return "2 t&auml;hte&auml - Huono";
	if ($rate < 2.5)
		return "3 t&auml;hte&auml - Ok";
	if ($rate < 7.5)
		return "4 t&auml;hte&auml - Hyv&auml;";

	return "5 t&auml;hte&auml - Mahtava";
}

function get_rate_desc_from_row($row)
{
	switch ($row['rating']) {
		case -10:
			return "1 t&auml;hti - Sysipaska";
		case -5:
			return "2 t&auml;hte&auml - Huono";
		case 0:
			return "3 t&auml;hte&auml - Ok";
		case 5:
			return "4 t&auml;hte&auml - Hyv&auml;";
		case 10:
			return "5 t&auml;hte&auml - Mahtava";
		default:
			debug_print("Bad set rating ".$row['rating']);
	}

	return false;
}
/*
define("LINK_ITEM_RATEDESC", "1");
define("LINK_ITEM_RATE_ONLY", "2");
define("LINK_ITEM_NUMVOTES", "4");
 */
/* TODO: Make it possible to select what info is added in link desc */
function get_link_desc_from_row($row)
{
	$ratedesc = get_rate_desc_from_row($row);
	if (!$ratedesc)
		return false;

	return $ratedesc." (".$row['numrates']." &auml;&auml;nest&auml;j&auml;&auml; t&auml;t&auml; mielt&auml;)";
}

function get_link_from_row($conn, $row)
{
	$url = create_seturl_from_row($conn, $row);
	if (!$url)
		return false;

	$link_desc = get_link_desc_from_row($row);
	if (!$link_desc)
		return false;

	$link = '<a href="'.$url.'">'.$link_desc."</a>\n";

	return $link;
}

function rate2stars($rate)
{
	/* Rate from 0 => 20 */
	$rate += 10;

	/* Stars 0 to 4 */
	$stars = $rate * 4 / 20;

	/* Stars 1 to 5 */
	$stars += 1;

	/* Sanity in case input was insane ... */
	if ($stars < 1)
		$stars = 1;
	if ($stars > 5)
		$stars = 5;

	return $stars;
}

function get_latest_sets_and_rates($conn, $numlatest = 5)
{
	$query = "SELECT * FROM cardsets ORDER BY added DESC LIMIT $numlatest";
	$links = null;

	$result = mysqli_query($conn, $query);
	if (!$result) {
		debug_print("setquery: $query".mysqli_error($conn));
		return false;
	}

	while ($row = mysqli_fetch_assoc($result)) {
		$query = "SELECT rating, numrates FROM setratings WHERE setid = ".$row['id'];

		$result2 = mysqli_query($conn, $query);
		if (!$result2) {
			debug_print("setratingquery: $query".mysqli_error($conn));
			continue;
		}

		$seturl = create_seturl_from_row($conn, $row);
		if (!$seturl) {
			debug_print("Failed to create URL for set ".$row['id']);
			continue;
		}
		$accum_rate = 0;
		$num_votes = 0;
		while ($row2 = mysqli_fetch_assoc($result2)) {
			$accum_rate += $row2['rating'] * $row2['numrates'];
			$num_votes += $row2['numrates'];
		}
		if (!$num_votes)
			continue;

		$avg = $accum_rate / $num_votes;
		$stars = rate2stars($avg);
		$link_desc = number_format($stars, 2, ",")." (".get_rate_desc_avg($avg).") - $num_votes &auml;&auml;nt&auml;";
		$links[] = '<a href="'.$seturl.'">'.$link_desc."</a>\n";
	}

	return $links;
}

function query_last_rates($conn, $numlatest)
{
	$query = "SELECT sr.rating, sr.numrates, cset.card0, cset.card1, cset.card2, cset.card3, cset.card4, cset.card5, cset.card6, cset.card7, cset.card8, cset.card9, cset.land0, cset.land1, cset.event0, cset.event1, cset.prophecy FROM setratings AS sr LEFT JOIN cardsets AS cset ON sr.setid = cset.id ORDER BY sr.time DESC LIMIT $numlatest";

	$result = mysqli_query($conn, $query);
	if (!$result)
		die_and_debug("ratequery: $query".mysqli_error($conn));

	return $result;
}

function get_last_rates_noformat($conn, $numlatest = 10, &$urls, &$rate, &$votes)
{
	$result = query_last_rates($conn, $numlatest);
	$numfound = 0;

	while ($row = mysqli_fetch_assoc($result)) {
		$url = create_seturl_from_row($conn, $row);
		if (!$url) {
			debug_print("Failed to create URL\n");
			continue;
		}
		if (!isset($row['rating']) || $row['rating'] == null) {
			debug_print("Rating not set\n");
			continue;
		}
		if (!isset($row['numrates']) || !$row['numrates']) {
			debug_print("Num rating not set\n");
			continue;
		}

		$urls[] = $url;
		$rate[] = $row['rating'];
		$votes[] = $row['numrates'];

		$numfound ++;
	}

	return $numfound;
}

function get_latest_rates($conn, $numlatest = 10)
{
	$links = null;
	$result = query_last_rates($conn, $numlatest);

	while ($row = mysqli_fetch_assoc($result)) {
		$link = get_link_from_row($conn, $row);
		if (!$link)
			return false;
		$links[] = $link;
	}
	if (!$links)
		return false;

	return $links;
}

function is_bot_spam()
{
	/* Spam check. Bot's usually fill all the input fields */
	if ((isset($_POST['foterarm']) && $_POST['foterarm'] != "") ||
	    (isset($_GET['foterarm']) && $_GET['foterarm'] != "")) {
		return true;
	}

	return false;
}

function combine_presel($preselected)
{
	if (count($preselected[0]) != 3 ||
	    count($preselected[1]) != 3 ||
	    count($preselected[2]) != 4)
	{
		if ($DBG) {
			echo "prize < 4 <br />";
			var_dump($preselected[0]);
			echo "<br />prize = 4<br />";
			var_dump($preselected[1]);
			echo "<br />prize > 4<br />";
			var_dump($preselected[2]);
		}
		debug_print("Unexpected prize groups for ratings");

		return null;
	}

	/* See if card-set exists */
	$tmparr = array_merge($preselected[0], $preselected[1]);

	return array_unique(array_merge($tmparr, $preselected[2]));	
}

function rate_sanitycheck_twoid_arr($arr)
{
	if ($arr) {
		$numvals = count($arr);
		if ($numvals > 2 || $numvals < 1)
			return -1;
		if ($numvals == 2)
			if ($arr[0] == $arr[1])
				return -1;
	}

	return 0;
}

function rating_insanity_check($ratecards, $keep_land_ids, $keep_event_ids, $keep_omena_ids) {
	if (count($ratecards) != 10) {
		debug_print("Num ratecards");
		return -1;
	}

	$ret = rate_sanitycheck_twoid_arr($keep_land_ids);
	$ret += rate_sanitycheck_twoid_arr($keep_event_ids);

	if (!$ret && $keep_omena_ids) {
		if (count($keep_omena_ids) != 1) {
			debug_print("NUM omenaIDS");
			return -1;
		}
	}

	return $ret;
}

function add_val_eq_col_query($vals, $colname, $separator, $append_colnum = false)
{
	$i = 0;
	$query = '';
	$numcards = count($vals);

	foreach($vals AS $val) {
		$col = $colname;
		if ($append_colnum)
			$col .= $i;
		if ($i == $numcards - 1)
			$query .= ' '.$col.' = '.$val;
		else
			$query .= ' '.$col.' = '.$val.' '.$separator;

		$i++;
	}
	return $query;
}

function add_rate_where_clause($idarr, $colbase, $numcol)
{
	$numcards = count($idarr);
	sort($idarr);

	if ($numcol > 1)
		$append_colid = true;
	else
		$append_colid = false;

	return add_val_eq_col_query($idarr, $colbase, 'AND', $append_colid);
}

function add_carsdet_insert_into_clause($idarr, $colbase, $append_colnum = true)
{
	/* Checks are already done in the add_rate_where_clause() */
	sort($idarr);

	return add_val_eq_col_query($idarr, $colbase, ',', $append_colnum);
}

function add_cardset_to_table($conn, $ratecards, $keep_land_ids, $keep_event_ids, $keep_omena_ids)
{
	$query = 'INSERT INTO cardsets SET';

	$query .= add_carsdet_insert_into_clause($ratecards, 'card', true);
	if ($keep_land_ids) {
		$query .= ',';
		$query .= add_carsdet_insert_into_clause($keep_land_ids, 'land', true);

	}
	if ($keep_event_ids) {
		$query .= ',';
		$query .= add_carsdet_insert_into_clause($keep_event_ids, 'event', true);
	}
	if ($keep_omena_ids) {
		$query .= ',';
		$query .= add_carsdet_insert_into_clause($keep_omena_ids, 'prophecy', false);
	}

	try {
		$result = mysqli_query($conn, $query);
	}catch (Exception $e)
	{
		debug_print("Couldn't add card set: ".$e->getMessage());
		return 0;
	}

	return mysqli_insert_id($conn);
}

function add_rate_to_table($conn, $id, $rate)
{
	/* The setratings has UNIQUE(setid, rating) constraint */
	$query = "INSERT INTO setratings SET setid = $id, rating = $rate ON DUPLICATE KEY UPDATE numrates=numrates+1, time = NOW()";

	$result = mysqli_query($conn, $query);
	if (!$result)
		die(mysql_error($conn));
}

/*
 * store_rates: Store user's rating for a card set.
 *
 * Users can send a rating for a card set they have played. The card set and rating
 * will be stored to the database. First the card-set is stored in cardsets table.
 * The table uses an unique-constraint so that each card combination will be stored
 * only once and only when rating is given. Then the rating for the set is added in
 * 'setratings' table, which again has UNIQUE constrain so that a same rating for
 * same set is added only once. If same rating is already given for a set, then a
 * counter is added to indicate how many times a specific rate is given to a
 * specific set.
 *
 * NOTE: A card set consists of exactly 10 randomized kingdom cards + optional
 * 2 landmarks, 2 events and 1 prophecy.
 *
 * @conn: Database connection
 * @rate: Rating for set. Integer from -10 to +10 (currently only -10, -5, 0, 5
 *	  and 10 are used)
 * @preselected: array containing 3 arrays of kingdom card IDs for different
 *		 prize ranges; @preselected[0] must contain 3 IDs for 'cheap'
 *		 cards. @preselected[1] must contain 3 IDs for 'mid' prize
 *		 cards. @preselected[2] must contain 4 IDs for 'expensive' cards.
 *		 (This is because the same post-data handling is used for keeping
 *		 'locked' cards when re-randomizing. for that we need to know prizes
 *		 so we can randomize missing cards for prize-ranges)
 * @keep_land_ids: Landmark-card IDs. The 'landmark offset' must be removed before
 *		   passing ID to this function.
 * @keep_event_ids: Event-card IDs. See @keep_land_ids above.
 * @keep_omena_ids: Omen-card ID. Single ID, 'omen offset' must be removed before
 *		    passing ID to this function.
 */
function store_rates($conn, $rate, $preselected, $keep_land_ids, $keep_event_ids, $keep_omena_ids)
{
	global $DBG;

	if (is_bot_spam())
		return;
	$ratecards = combine_presel($preselected);
	if ($ratecards == null)
		return;

	if (rating_insanity_check($ratecards, $keep_land_ids, $keep_event_ids, $keep_omena_ids)) {
		debug_print("Sanitychecks failed");
		return;
	}
	/*
	 * The cardset table should have UNIQUE() constraint for cards in set
	 * Hence, we should not need to check if the set already exists but we
	 * can just try adding it. If adding fails, we fetch the ID.
	 *
	 * TODO: If the amount of sets grows so that it will be likely the set
	 * is alrady added, then we can slightly optimize by trying to find the
	 * ID first, and try adding only if ID does not exist.
	 */
	$id = add_cardset_to_table($conn, $ratecards, $keep_land_ids, $keep_event_ids, $keep_omena_ids);
	if (!$id) {
		/* Adding failed. Perhaps we already have the set? */
		$query = 'SELECT id FROM cardsets WHERE';
		$query .= add_rate_where_clause($ratecards, 'card', 10);

		if ($keep_land_ids) {
			$query .= " AND";
			$query .= add_rate_where_clause($keep_land_ids, 'land', 2);
		}

		if ($keep_event_ids) {
			$query .= " AND";
			$query .= add_rate_where_clause($keep_event_ids, 'event', 2);
		}

		if ($keep_omena_ids) {
			$query .= " AND";
			$query .= add_rate_where_clause($keep_omena_ids, 'prophecy', 1);
		}

		echo $query;
		$result = mysqli_query($conn, $query);
		if (!$result) {
			debug_print($query.' '.mysql_error($conn));
			die();
		}

		if (mysqli_num_rows($result)) {
			$row = mysqli_fetch_assoc($result);
			if (!isset($row['id']))
				die('internal error');
			$id = $row['id'];
		} else {
			die('Internal error - ID not found');
		}
	}

	add_rate_to_table($conn, $id, $rate);
}


?>
