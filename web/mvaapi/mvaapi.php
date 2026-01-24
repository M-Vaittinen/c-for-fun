<?php
/*
 * Dominon card randomizer.
 * Uses MySQL database.
 *
 * AUTHOR: Matti Vaittinen <mazziesaccount@gmail.com>
 *
 * Written just for fun. No Warranty. Use at your own risk!
 *
 * Copyright 2026, Matti Vaittinen mazziesaccount@gmail.com>
 */

define("MVAROOTPATH", "../");

require MVAROOTPATH.'include/db.php';
require MVAROOTPATH.'include/common.php';

$id = check_post_numeric_die('cardid', 1, 3009999);
$prize = check_post_numeric_die('cardprize', 0, 100);
if (!isset($_POST['cardids']))
	die('cids');

if (!isset($_POST['expansions']))
	die('exps');

$cardid_list = explode(",", $_POST['cardids']);
$landids = null;
$eventids = null;
$prophecyids = null;

foreach($cardid_list AS $cid) {
	if (!is_numeric($cid))
		die($cid.'badcid'.$cid);
	if ($cid < LAND_ID_OFFSET)
		continue;
	if ($cid < EVENT_ID_OFFSET) {
		$landids[] = $cid - LAND_ID_OFFSET;
		continue;
	}
	if ($cid < OMENA_ID_OFFSET) {
		$eventids[] = $cid - EVENT_ID_OFFSET;
		continue;
	}

	$prophecyids[] = $cid - OMENA_ID_OFFSET;
}

$expid_list = explode(",", $_POST['expansions']);
foreach($expid_list AS $expid) {
	if (!is_numeric($expid))
		die('badeid');
}

function do_list($arr, $field, $comparison, $andor)
{
	$out = '';
	foreach($arr AS $a)
		$out .= ' '.$andor.' '.$field.' '.$comparison.' '.$a;

	return $out;
}

function do_and_list($arr, $field, $comparison)
{
	return do_list($arr, $field, $comparison, 'AND');
}

function do_or_list($arr, $field, $comparison)
{
	return do_list($arr, $field, $comparison, 'OR');
}

function card_id_is_kingdom($id)
{
	return($id < LAND_ID_OFFSET);
}

function card_id_is_landmark($id)
{
	return($id > LAND_ID_OFFSET && $id < EVENT_ID_OFFSET);
}

function card_id_is_event($id)
{
	return($id > EVENT_ID_OFFSET && $id < OMENA_ID_OFFSET);
}

function card_id_is_omena($id)
{
	return($id > OMENA_ID_OFFSET);
}

function do_query($id, $cid_list, $prize)
{
	global $landids;
	global $eventids;
	global $prophecyids;

	if (card_id_is_kingdom($id)) {
		if ($prize < 4)
			$prizelimit = "< 4";
		else if ($prize == 4)
			$prizelimit = "= 4";
		else
			$prizelimit = "> 4";

		$query_base = "SELECT c.*, e.name AS expansion_name, pt.name AS prizetype_name, ct.name AS type_name, setup.text AS setup_text FROM cards AS c ";
        	$query_base .= "LEFT JOIN expansion AS e ON c.expansion_id = e.id ";
		$query_base .= "LEFT JOIN cardtype AS ct ON c.type_id = ct.id ";
		$query_base .= "LEFT JOIN prizetype AS pt ON c.prizetype_id = pt.id ";
		$query_base .= "LEFT JOIN setup_extras AS setup ON c.setup_extras_id = setup.id WHERE";
		$query_base .= " c.prize $prizelimit";
		$query_base .= do_and_list($cid_list, 'c.id', '!=');

	} else if (card_id_is_landmark($id)) {
		$query_base = "SELECT c.*, e.name AS expansion_name, ct.name AS type_name, setup.text AS setup_text FROM landmarks AS c ";
       		$query_base .= "LEFT JOIN expansion AS e ON c.expansion_id = e.id ";
		$query_base .= "LEFT JOIN cardtype AS ct ON c.type_id = ct.id ";
		/*		$query_base .= "LEFT JOIN prizetype AS pt ON c.prizetype_id = pt.id "; */
		$query_base .= "LEFT JOIN setup_extras AS setup ON c.setup_id = setup.id WHERE";

		if ($landids)
			$query_base .= do_and_list($landids, 'c.id', '!=');

	} else if (card_id_is_event($id)) {

		$query_base = "SELECT c.*, e.name AS expansion_name, et.name AS event_type_name, ct.name AS type_name, setup.text AS setup_text FROM events AS c ";
		$query_base .= "LEFT JOIN expansion AS e ON c.expansion_id = e.id ";
		$query_base .= "LEFT JOIN cardtype AS ct ON c.type_id = ct.id ";
		$query_base .= "LEFT JOIN event_type AS et ON c.event_type_id = et.id ";
		$query_base .= "LEFT JOIN setup_extras AS setup ON c.setup_id = setup.id WHERE";
		if ($eventids)
			$query_base .= do_and_list($eventids, 'c.id', '!=');
	} else if (card_id_is_omena($id)) {

		$query_base = "SELECT c.*, e.name AS expansion_name, ct.name AS type_name, setup.text AS setup_text FROM prophecies AS c ";
	        $query_base .= "LEFT JOIN expansion AS e ON c.expansion_id = e.id ";
		$query_base .= "LEFT JOIN cardtype AS ct ON c.type_id = ct.id ";
		$query_base .= "LEFT JOIN setup_extras AS setup ON c.setup_id = setup.id WHERE";
		if ($prophecyids)
			$query_base .= do_and_list($prophecyids, 'c.id', '!=');
	}

	return $query_base;
}

function get_card_row($conn, $replid, $cid_list, $expid_list, $prize)
{
	$query = do_query($replid, $cid_list, $prize);

	if ($expid_list[0] != 0)
		$query .= do_or_list($expid_list, 'c.expansion_id', '=');
	
	$query .= " ORDER BY RAND() LIMIT 1";

	$result = mysqli_query($conn, $query);
	if (!$result)
		die("no cards");
	if (($numc = mysqli_num_rows($result)) <= 0)
		die("still no cards");

	$row = mysqli_fetch_assoc($result);

	return $row;
}

$row = get_card_row($conn, $id, $cardid_list, $expid_list, $prize);

echo json_encode($row);

?>
