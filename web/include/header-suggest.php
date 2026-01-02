<?php
function do_head($title)
{
echo '
<!DOCTYPE html>
<html>
<head>
<meta name="viewport" content="width=device-width, initial-scale=1">
<style>


/* Setup help text to be shown when hovered on image */

/* The table stuff for admin forms to work in "one form / row" way, which shouldn\'t be done using real tables */

DIV.table 
{
    display:table;
}
FORM.tr, DIV.tr
{
    display:table-row;
}
SPAN.td
{
    display:table-cell;
}

body {
  background-color: linen;
}

h1 {
  color: maroon;
  margin-left: 40px;
}

table.structure {
  width: 100%;
  border: none;
  text-align: left;
  vertical-align: top;
}
.structure td {
  border: none;
  text-align: left;
  padding-left: 30px;
  padding-right: 30px;
  vertical-align: top;
}
.structure th {
  text-align: left;
  padding-left: 30px;
  padding-right: 30px;
}
.structure {
  border: none;
  text-align: left;
  vertical-align: top;
}

table.cardlist {
  width: 100%;
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
.cardlist th.squeeze {
	width:200px;
}
.cardlist td.squeeze {
	width:200px;
}
.cardlist th.checkbox {
	width:50px;
}
.cardlist td.checkbox {
	width:50px;
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
	width: 100%;
}
.aarvonta th {
/*  height: 70px; */
  text-align: center;
  vertical-align: top;
  background-color: #d2691e;
  color: white;
  border-collapse: collapse;
}
.aarvonta td {
  font-size: 1rem;
  font-weight: 600;

  border: thin brown solid;
  border-collapse: collapse;
  padding-top: 12px;
  padding-bottom: 12px;
  vertical-align: top;
  text-align: center;
  background-color: ##ffdead;
  color: brown;
  border-radius: 10px;
}

.mandatory {
    border: thin red solid;
}

/*
 * .visible-top {
  transform: scale(-1, -1);
}
*/
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
	height: 100%;
	text-align: center;
	vertical-align: middle;
}

.sleft {
  width: 1%;
#  white-space: nowrap;
  writing-mode: vertical-rl;
}

.sright {
  writing-mode: vertical-lr;
  width: 1%;
#  white-space: nowrap;
  transform: scale(-1, -1);
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
}

/* Stolen from the web https://www.geeksforgeeks.org/how-to-detect-a-mobile-device-using-php/ */
function isMobileDevice() { 
    return preg_match("/(android|avantgo|blackberry|bolt|boost|cricket|docomo 
|fone|hiptop|mini|mobi|palm|phone|pie|tablet|up\.browser|up\.link|webos|wos)/i" 
, $_SERVER["HTTP_USER_AGENT"]); 
} 

?>
