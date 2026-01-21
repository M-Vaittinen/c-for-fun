<?php
define("LAND_ID_OFFSET", 1000000);
define("EVENT_ID_OFFSET", 2000000);
define("OMENA_ID_OFFSET", 3000000);
define("SET_RATE_MIN", -10);
define("SET_RATE_MAX", 10);

//$DBG=true;
$DBG=false;

function return_post_numeric_array_or_ret($name, $ret)
{
	if (isset($_POST[$name]) && is_array($_POST[$name])) {
		$arr = $_POST[$name];
		foreach($arr AS $a)
			if (!is_numeric($a))
				return $ret;

		return $arr;
	}

	return $ret;
}

function return_post_numeric_array_or_zero($name)
{
	return return_post_numeric_array_or_ret($name, 0);
}

function die_and_debug($msg)
{
	debug_print($msg);
	die("Error in sent data");
}

function check_post_numeric_die($name, $min, $max)
{
	if (!isset($_POST[$name]))
		die_and_debug("$name not set");

	$val = $_POST[$name];

	if (!is_numeric($val))
		die_and_debug("Non numeric $name");

	if ($min != null)
		if ($val < $min)
			die_and_debug("$name too small, $val. Limit $min");

	if ($max != null)
		if ($val > $max)
			die_and_debug("$name too large, $val. Limit $max");

	return $val;
}

function return_post_numeric_or_ret($name, $min, $max, $ret)
{
	if (!isset($_POST[$name]))
		return $ret;

	if (!is_numeric($_POST[$name]))
		return $ret;

	if ($min != null && $_POST[$name] < $min)
		return $ret;

	if ($max != null && $_POST[$name] > $max)
		return $ret;

	return $_POST[$name];
}

function return_post_numeric_or_zero($name, $min, $max)
{
	return return_post_numeric_or_ret($name, $min, $max, 0);
}

function debug_print($str)
{
	global $DBG;

	if (!$DBG)
		return;

	echo "$str.<br />";
}



?>
