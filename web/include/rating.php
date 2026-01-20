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

require 'include/ratebtn.php';

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
	$query = "INSERT INTO setratings SET setid = $id, rating = $rate ON DUPLICATE KEY UPDATE numrates=numrates+1";

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

		if ($keep_land_ids)
			$query .= add_rate_where_clause($keep_land_ids, 'land', 2);

		if ($keep_event_ids)
			$query .= add_rate_where_clause($keep_event_ids, 'event', 2);

		if ($keep_omena_ids)
			$query .= add_rate_where_clause($keep_omena_ids, 'prophecy', 1);

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
