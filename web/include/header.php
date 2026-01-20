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

/* Sigh. A desperate attempt to squeeze down some expences. */

$AD_ID = '
<script async src="https://pagead2.googlesyndication.com/pagead/js/adsbygoogle.js?client=ca-pub-2999778840133894"
     crossorigin="anonymous"></script>
';

function do_head($title, $mobile = false)
{
	global $AD_ID;

	/* Different size of rate starts for mobile and web */
	$mvacont_width = ($mobile) ? '80%' : '40%';
	$mvacont_height = ($mobile) ? '40%' : '20%';

	/*
	 * The 'arvostele' -button needs different placing on
	 * mobile and web. The star label may overlap it on mobile,
	 * or the space will be ridiculously large on web version.
	 */
	$mva_height = ($mobile) ? '50vh' : '25vh';
	$mvacont_top = ($mobile) ? '10%' : '20%';

echo '
<!DOCTYPE html>
<html>
<head>
<meta name="viewport" content="width=device-width, initial-scale=1">
<style>

/* Navigation bar dropdowns */
 /* Navbar container */
.mvanavbar {
  overflow: hidden;
  background-color: #d2691e;
  font-family: Arial;
}

/* Links inside the navbar */
.mvanavbar a {
  float: left;
  font-size: 16px;
  color: white;
  text-align: center;
  padding: 14px 16px;
  text-decoration: none;
}

/* Place the "yhteystiedot" to far right */

/* The dropdown container */
.dropdown {
  float: left;
  overflow: hidden;
}

.mvaright a {
	float: right;
  	align-items: right;
}
.mvaright {
	float: right;
  	align-items: right;
}

/* Dropdown button */
.dropdown .dropbutton {
  font-size: 16px;
  border: none;
  outline: none;
  color: white;
  padding: 14px 16px;
  background-color: inherit;
  font-family: inherit; /* Important for vertical align on mobile phones */
  margin: 0; /* Important for vertical align on mobile phones */
}

.mvanavbar ul {
	list-style-type: none;
}

/* Add a red background color to navbar links on hover */
.mvanavbar a:hover, .dropdown:hover .dropbutton {
  background-color: maroon;
}

.mvaright a:hover, .dropdown:hover .dropbutton {
  background-color: maroon;
}
/* Dropdown content (hidden by default) */
.dropdown-content {
  display: none;
  position: absolute;
  background-color: #f9f9f9;
  min-width: 160px;
  box-shadow: 0px 8px 16px 0px rgba(0,0,0,0.2);
  z-index: 1;
}

/* Links inside the dropdown */
.dropdown-content a {
  float: none;
  color: black;
  padding: 12px 16px;
  text-decoration: none;
  display: block;
  text-align: left;
}

/* Add a grey background color to dropdown links on hover */
.dropdown-content a:hover {
  background-color: #ddd;
}

/* Show the dropdown menu on hover */
.dropdown:hover .dropdown-content {
  display: block;
}

/* Rating stars */

:root{
  --star-colour: hsl(38 90% 55%);
}

.mva {
  overflow: hidden;
  
  width: 50vw;
  height: '.$mva_height.';
  background: linen;
  font-family: sans-serif;
  color: #fff;
  /*
  */
}

.mvacontainer{
  position: relative;
  top: '.$mvacont_top.';
  left: 50%;
  
  width: '.$mvacont_width.';
  height: '.$mvacont_height.';
  
  transform: translate(-50%, -50%) rotateY(180deg);
}

.mvacontainer .mvacontainer__items{
  display: flex;
  align-items: center;
  justify-content: center;
  
  gap: 0 .5em;
  
  width: 100%;
  height: 100%;
}

.mvacontainer .mvacontainer__items input{
  display: none;
}

.mvacontainer .mvacontainer__items label{
  width: 20%;
  aspect-ratio: 1;
  cursor: pointer;
}

.mvacontainer__items label .star-stroke{
  transition: .3s;
  box-sizing: border-box;
  display: grid;
  place-items: center;
  width: 100%;
  height: 100%;
  background: maroon; 
  clip-path: polygon(50% 0%, 61% 35%, 98% 35%, 68% 57%, 79% 91%, 50% 70%, 21% 91%, 32% 57%, 2% 35%, 39% 35%);
}

.mvacontainer__items label .star-stroke .star-fill{
  width: 70%;
  aspect-ratio: 1;
  background: linen;
  clip-path: polygon(50% 0%, 61% 35%, 98% 35%, 68% 57%, 79% 91%, 50% 70%, 21% 91%, 32% 57%, 2% 35%, 39% 35%);
}

.mvacontainer__items input:hover ~ label .star-stroke,.container .mvacontainer__items input:checked ~ label .star-stroke{
  background: var(--star-colour);
}

.mvacontainer__items input:checked ~ label .star-stroke .star-fill{
  background: var(--star-colour);
}


.mvacontainer__items label:hover .label-description::after{
  transition: .3s;
  box-sizing: border-box;
  content: attr(data-content);
  position: fixed;
  left: 0;
  right: 0;
  
  margin-top: 1em;
  margin-inline: auto;
  
  width: 100%;
  height: 2em;
  
  /* color: #fff; */
  color: brown;
  
  text-align: center;
  font-size: 2rem;
  
  transform: rotateY(180deg);
}

/* Setup help text to be shown when hovered on image */

.image-container {
    margin-left: 5px;
    position: relative;
    display: inline-block;
}

.hover-text {
    display: none;
    position: absolute;
    bottom: 10px;
    left: 50%;
    transform: translateX(-50%);
    background-color: rgba(0, 0, 0, 0.7);
    color: white;
    padding: 5px;
    border-radius: 5px;
    z-index: 10;
}

.landcardname {
    min-width: 300px;
}

.landscape-card-img {
    width: 350px;
    height: auto;
    cursor: auto;
    z-index: 11;
}

.landcardname-mobile {
    min-width: 200px;
}
.selectscreen {
    cursor: auto;
  border: 1px solid black;
  border-radius: 10px;
    display: block;
    width: 50%;
    margin-left: auto;
    margin-right: auto;
    height: auto;
}

.screenshot {
    cursor: auto;
  border: 1px solid black;
  border-radius: 10px;
    display: block;
    width: 75%;
    margin-left: auto;
    margin-right: auto;
    height: auto;
}

.mypic {
    cursor: auto;
    border: 1px solid black;
    border-radius: 10px;
    display: block;
    width: 50%;
/*    margin-left: auto; */
    margin-right: auto;
    height: auto;
}

.landscape-card-img-mobile {
    width: 250px;
    height: auto;
    cursor: auto;
    z-index: 11;
}

.card-img {
    width: 200px;
    height: auto;
    cursor: auto;
    z-index: 11;
}

.rohto-img {
    width: auto;
    height: 30px;
}

.footer {
    color: black;
}

.ometers {
    color: black;
}

.tuhinatext {
    color: black;
}

.expansionform {
    color: black;
}

.image-container:hover .hover-text,
.image-container:focus-within .hover-text {
    display: block;
}

img {
    width: 50px; /* Set image size */
    height: auto;
    cursor: pointer;
}


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

.slidecontainer {
  display: flex;
  align-items: center;
  gap: 10px;
  margin: 25px 0;
  font-family: sans-serif;
}

.label {
  width: 30px;
  text-align: center;
}

.slider-wrapper {
  position: relative;
  width: 300px;
}

/* Slider track */
.slider {
  -webkit-appearance: none;
  width: 100%;
  height: 6px;
  border-radius: 3px;
  outline: none;
  background: linear-gradient(to right, red, blue, green);
}

/* Thumb */
.slider::-webkit-slider-thumb {
  -webkit-appearance: none;
  width: 18px;
  height: 18px;
  border-radius: 50%;
  background: #333;
  cursor: pointer;
  position: relative;
  z-index: 2;
}

.slider::-moz-range-thumb {
  width: 18px;
  height: 18px;
  border-radius: 50%;
  background: #333;
  cursor: pointer;
}

/* Floating value bubble */
.value-bubble {
  position: absolute;
  top: -35px;
  transform: translateX(-50%);
  padding: 2px 6px;
  border-radius: 4px;
  background: #333;
  color: #fff;
  font-size: 12px;
  white-space: nowrap;
  pointer-events: none;
}

body {
  background-color: linen;
  color: brown;
}


h1 {
  margin-left: 40px;
}

h1, h2, h3, h4 {
  color: maroon;
}

.imglink {
  cursor: pointer;
}

.settitle h3 {
  margin-left: 40px;
}

.ometer b {
/*  margin-left: 20px; */
  color: maroon;
}
.nolink img {
    cursor: auto;
}

.header {
    display: flex;
    align-items: center;
}

.header img {
  width: 80px;
  height: 80px;
  background: #555;
}

.header h1 {
  margin-left: 10px;
}

.shufflesubmit {
  background-color: #d2691e;
  color: white;
  border: none;
  padding: 12px 24px;
  font-size: 16px;
  font-family: sans-serif;
  border-radius: 6px;
  cursor: pointer;
  transition: background-color 0.3s ease;
}

.shufflesubmit:hover {
  background-color: maroon;
}

.shufflesubmit:active {
  background-color: #330006;
  transform: scale(0.97);
}

.listhead {
    font-size: 24px;
    font-weight: bold;
  margin-left: 20px;
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

.checkbox-container {
    display: flex;
    align-items: center; /* Align vertically center */
}

.checkboxes {
	/* word-wrap: break-word; */
    margin-right: 10px; /* Space between checkbox and label */
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
  height: 70px;
  text-align: center;
  vertical-align: top;
  background-color: #d2691e;
  color: white;
  border-collapse: collapse;
}
.aarvonta td {
  border: none;
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

.help-tip{
    text-align: center;
    background-color: #BCDBEA;
    border-radius: 50%;
    width: 24px;
    height: 24px;
    font-size: 14px;
    line-height: 26px;
    cursor: default;
}

.help-tip:before{
    content:\'?\';
    font-weight: bold;
    color:#fff;
    cursor: pointer;
}

.help-tip:hover, .help-tip:focus, .help-tip:active p{
    display:block;
    transform-origin: 100% 0%;

    -webkit-animation: fadeIn 0.3s ease-in-out;
    animation: fadeIn 0.3s ease-in-out;
}

.help-tip p{    /* The tooltip */
    display: none;
    text-align: left;
    background-color: #1E2021;
    padding: 20px;
    width: 300px;
    border-radius: 3px;
    box-shadow: 1px 1px 1px rgba(0, 0, 0, 0.2);
    z-index: 10;
   /* position:relative; */
    position:absolute;
    color: #FFF;
    font-size: 13px;
    line-height: 1.4;
}

.help-tip p:before{ /* The pointer of the tooltip */
    position: absolute;
    content: \'\';
    width:0;
    height: 0;
    border:6px solid transparent;
    border-bottom-color:#1E2021;
    left:10px;
    top:-12px;
}

.help-tip p:after{ /* Prevents the tooltip from being hidden */
    width:100%;
    height:40px;
    content:\'\';
    position: absolute;
    top:-40px;
    left:0;
}

/* Suggestions: */
.sfade-in {
  opacity: 0;
  animation: sfadeIn 2s forwards;
  animation-delay: 3s; /* Delay before the animation starts */
}

@keyframes sfadeIn {
  to {
    opacity: 1;
  }
}

/* CSS animation */

@-webkit-keyframes fadeIn {
    0% { 
        opacity:0; 
        transform: scale(0.6);
    }

    100% {
        opacity:100%;
        transform: scale(1);
    }
}

@keyframes fadeIn {
    0% { opacity:0; }
    100% { opacity:100%; }
}

/* For hidden checkboxes */
.hidden {
	display: none;
}

</style>
' . $AD_ID .'
<script>
/* Toggle checkbox visibility to display expansion specific options (for selected expansions) */
function toggleCheckboxes(checkbox, relatedClass) {
	var relatedCheckboxes = document.getElementsByClassName(relatedClass);
	for (var i = 0; i < relatedCheckboxes.length; i++) {
		relatedCheckboxes[i].classList.toggle(\'hidden\', !checkbox.checked);
		/* Clean the "child" checkboxes when parent is cleared */
		if (!checkbox.checked)
			relatedCheckboxes[i].checked = false;
	}
}

function updateSlider(slider) {
  const min = Number(slider.min);
  const max = Number(slider.max);
  const value = Number(slider.value);

  const wrapper = slider.closest(".slider-wrapper");
  const bubble = wrapper.querySelector(".value-bubble");

  // Update bubble text
  bubble.textContent = value;

  // Calculate percentage position
  const percent = ((value - min) / (max - min)) * 100;

  // Position bubble above thumb
  bubble.style.left = `${percent}%`;

  /*
    Continuous gradient:
    - Red at min
    - Blue at zero
    - Green at max
  */
  slider.style.background = `
    linear-gradient(
      to right,
      red 0%,
      blue 50%,
      green 100%
    )
  `;
}

/* Initialize all sliders */
document.addEventListener("DOMContentLoaded", function () {
  document.querySelectorAll(".slider").forEach(slider => {
    updateSlider(slider);
    slider.addEventListener("input", () => updateSlider(slider));
  });
});

</script>

<title>' . $title . '</title>
</head>
<body>'."\n";
}

/* Stolen from the web https://www.geeksforgeeks.org/how-to-detect-a-mobile-device-using-php/ */
function isMobileDevice() { 
	if (!isset($_SERVER["HTTP_USER_AGENT"]))
		return false;

	return preg_match("/(android|avantgo|blackberry|bolt|boost|cricket|docomo 
|fone|hiptop|mini|mobi|palm|phone|pie|tablet|up\.browser|up\.link|webos|wos)/i" 
, $_SERVER["HTTP_USER_AGENT"]); 
} 

?>
