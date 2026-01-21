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

function do_head($title, $mobile)
{
	$verttxxcell_height = ($mobile) ? '' : '45vh';
$out = '
<!DOCTYPE html>
<html>
<head>
<style>

/* Setup help text to be shown when hovered on image */

/* The table stuff for admin forms to work in "one form / row" way, which shouldn\'t be done using real tables */

body {
  background-color: linen;
}

h1 {
  color: maroon;
  margin-left: 40px;
}

table.cardlist {
 /* width: 100%; */
  border: 1px solid black;
  border-radius: 10px;
  border-collapse: collapse;
}
.cardlist th {
  border: 1px solid black;
  padding-top: 12px;
  padding-bottom: 12px;
  text-align: center;
  vertical-align: top;
  background-color: #d2691e;
  color: white;
  border-collapse: collapse;
}
.cardlist td {
  border: 1px solid black;
  border-collapse: collapse;
  padding-top: 12px;
  padding-bottom: 12px;
  vertical-align: top;
  text-align: center;
  background-color: #ffdead;
  color: brown;
  border-radius: 10px;
}

table.aarvonta {
	border: none;
	table-layout: auto;
	page-break-inside:auto;
}
.aarvonta th {
  text-align: center;
  vertical-align: top;
  background-color: #d2691e;
  color: white;
  border-collapse: collapse;
}
.aarvonta td {
/*  font-size: 1rem;
  font-weight: 600; */
/*  font-size: min(1rem, 2vw); */

  border: thin brown solid;
  border-collapse: collapse;
  padding-top: 12px;
  padding-bottom: 12px;
  vertical-align: top;
  text-align: center;
  background-color: ##ffdead;
  color: brown;
  border-radius: 10px;
white-space: normal;
word-break: break-word;
font-size: 1vw;
}

.mandatory {
    border: thin red solid;
}

/* Suggestions: */

.stop {
  transform: scale(-1, -1);
  text-align: center;
}

.sfade-in {
  text-align: center;
  opacity: 0;
  animation: fadeIn 2s forwards;
  animation-delay: 3s; /* Delay before the animation starts */
}

.svisible {
  text-align: center;
}

td.startform {
	/* height: 100%; */
	text-align: center;
	vertical-align: middle;
}

.sleft {
  width: 1%;
  writing-mode: vertical-rl;
height: '. $verttxxcell_height .';
}

.sright {
/* height:' . $verttxxcell_height .'; */
    height: auto;
    width: auto;
/*  writing-mode: vertical-lr; */
 /* width: 1%; */
  /* transform: scale(-1, -1); */
    filter:  progid:DXImageTransform.Microsoft.BasicImage(rotation=0.083);  /* IE6,IE7 */
    -ms-filter: "progid:DXImageTransform.Microsoft.BasicImage(rotation=0.083)"; /* IE8 */
    -moz-transform: rotate(-90.0deg);  /* FF3.5+ */
    -ms-transform: rotate(-90.0deg);  /* IE9+ */
    -o-transform: rotate(-90.0deg);  /* Opera 10.5 */
    -webkit-transform: rotate(-90.0deg);  /* Safari 3.1+, Chrome */
  transform: rotate(-90.0deg);
}

@keyframes fadeIn {
  to {
    opacity: 1;
  }
}

/* For hidden checkboxes */
.hidden {
	display: none;
}

</style>

<script>
</script>

<title>' . $title . '</title>
</head>
<body>';

echo $out;

}

/* Stolen from the web https://www.geeksforgeeks.org/how-to-detect-a-mobile-device-using-php/ */
function isMobileDevice() { 
    return preg_match("/(android|avantgo|blackberry|bolt|boost|cricket|docomo 
|fone|hiptop|mini|mobi|palm|phone|pie|tablet|up\.browser|up\.link|webos|wos)/i" 
, $_SERVER["HTTP_USER_AGENT"]); 
} 

?>
