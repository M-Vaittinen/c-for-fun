<?php
/*
 * Dominon card randomizer.
 * Uses MySQL database.
 *
 * AUTHOR: Matti Vaittinen <mazziesaccount@gmail.com>
 *
 * Written just for fun. No Warranty. Use at your own risk!
 *
 * Copyright 2025, 2026 Matti Vaittinen mazziesaccount@gmail.com>
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

	$keep_box_w = ($mobile) ? '120px' : '50px';

echo '
<!DOCTYPE html>
<html>
<head>
<meta name="viewport" content="width=device-width, initial-scale=1">
<style>

.swipeicon {
	vertical-align: middle;
	padding: 18px
}

/* Lets try implementing the swipe ... */
.card-row {
    transition: transform 0.3s ease;
    touch-action: pan-y; /* Prevent vertical scrolling */
}
.swiped {
    transform: translateX(100%); /* Adjust as needed */
}

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
	width:'.$keep_box_w.';
}
.cardlist td.checkbox {
	width:'.$keep_box_w.';
	vertical-align: middle;
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

/* The swiping and AJAX stuff */

/* This should update the card information. Lets see how I manage the setup tip stuff */

function getKeepPrizeIDs()
{
	// Select all elements with the given name
	const params = [];
	const keepids = document.querySelectorAll(`input[name="keepid[]"]`);
	const idset = new Set();
	keepids.forEach(input => {
/*
		console.log(input.name);
		console.log(input.value);
*/
		if (!idset.has(input.value)) {
			idset.add(input.value);
			// Encode the parameter and push to the params array
			params.push(`${encodeURIComponent(input.name)}=${encodeURIComponent(input.value)}`);
		}
	});

	const pridset = new Set();
	const keepprizes = document.querySelectorAll(`input[name^="keepprize"]`);
	keepprizes.forEach(input => {
/*
            console.log(input.name);
	    console.log(input.value);
*/
		if (!pridset.has(input.name)) {
			pridset.add(input.value);
			// Encode the parameter and push to the params array
			params.push(`${encodeURIComponent(input.name)}=${encodeURIComponent(input.value)}`);
		}
	});

	return params;
}

function getShareUrl()
{
const params = getKeepPrizeIDs();

	// Join the parameters to create the query string
	const queryString = params.join(\'&\');

	console.log(params);
	// Construct the full GET request URL
	const baseURL = window.location.origin + window.location.pathname;

	return `${baseURL}?${queryString}`;
}

function updateShareUrls()
{
	const mailtoElement = document.getElementById(\'mailto-share-link\');
	const newUrl = encodeURIComponent(getShareUrl());
	mailtoElement.href = `mailto:?subject=Kokeilemani Dominionkortit&body=${newUrl}`;

	const tumbLinkElement = document.getElementById(\'tumblr-share-link\');
	tumbLinkElement.href = `https://www.tumblr.com/widgets/share/tool?canonicalUrl=${newUrl}`;

	const shareLinkElement = document.getElementById(\'facebook-share-link\');
	shareLinkElement.href = `https://www.tumblr.com/widgets/share/tool?canonicalUrl=${newUrl}`;

	const redditElement = document.getElementById(\'reddit-share-link\');
	redditElement.href = `https://www.reddit.com/submit?url=${newUrl}`;

	const waElement = document.getElementById(\'wa-share-link\');
	waElement.href = `https://wa.me/?text=${newUrl}`;

	const dcElement = document.getElementById(\'dc-share-link\');
	dcElement.href = `discord://send?message=${newUrl}`;
}

function updateInputValues(oldId, newId, newPrize) {
	const dataids = document.querySelectorAll(`input[data-cardid="${oldId}"]`);
	console.log("Updating IDs, old: " + oldId + "new: " + newId);

	dataids.forEach(input => {
		if (input.value === oldId.toString()) {
input.value = newId;
			console.log("Updating data-cardid");
		}
	});
	// Update keepid[] inputs
	const keepIdInputs = document.querySelectorAll(`input[name="keepid[]"]`);
	keepIdInputs.forEach(input => {
		console.log("keepid[] " + input.value);
		if (input.value === oldId.toString()) {
			console.log("CHANGING " + input.value + " => " + newId);
			input.value = newId; // Change value to new ID
		} else {
			console.log("not match");
		}
	});

	// Update keepprize{oldId} inputs
	const keepPrizeInputs = document.querySelectorAll(`input[name^="keepprize"]`);
	keepPrizeInputs.forEach(input => {
		if (input.name === `keepprize${oldId}`) {
			input.name = `keepprize${newId}`; // Rename to keepprize + new ID
			input.value = newPrize; // Change value to the new card\'s prize
		}
	});
}

function gen_card_name_html(data)
{
	let cardName;
	if (data.en_name && data.name && (data.name != data.en_name))
		cardName = data.name + " (" + data.en_name + ")";
	else
		cardName = data.name;

	//console.log(cardName);

	let cardImageHtml = `
	    <div class="image-container">
	        <p tabindex="0">${cardName}<div class="hover-text"><img class="card-img" src="../cardpics/${data.imagename}"></div></p></div>`;
	if (data.bottom_name) {
		cardImageHtml += `
	    <div class="image-container">
	        <p tabindex="0"><i>--&gt; ${data.bottom_name}</i><div class="hover-text"><img class="card-img" src="../cardpics/${data.bottom_imagename}"></div></p></div>`;
	}

	return cardImageHtml;
}

function gen_card_specials_html(data, nodash)
{
	let cardImageHtml = \'\';

	if (data.potion == 1) {
	    cardImageHtml += `
	        <div class="image-container">
	            <img src="img/potion.png" alt="Rohto" class="rohto-img" tabindex="0">
	            <div class="hover-text">Rohto</div>
	        </div>`;
		nodash = true;
	}
	if (data.curses == 1) {
	    cardImageHtml += `
	        <div class="image-container">
	            <img src="img/curse_100x100.png" alt="Kirous" tabindex="0">
	            <div class="hover-text">Kiroukset</div>
	        </div>`;
		nodash = true;
	}
	if (data.attack == 1) {
	    cardImageHtml += `
	        <div class="image-container">
	            <img src="img/speargoblin_200x200.png" alt="Goblin" tabindex="0">
	            <div class="hover-text">Hy&ouml;kk&auml;yskortti</div>
	        </div>`;
		nodash = true;
	}
	if (data.omen == 1) {
	    cardImageHtml += `
	        <div class="image-container">
	            <img src="img/omena.png" alt="Omena" tabindex="0">
	            <div class="hover-text">Olen Omena (prophecy mukaan)</div>
	        </div>`;
		nodash = true;
	}
	if (data.setup_text) {
		cardImageHtml += `
	        <div class="image-container">
	            <img src="img/peasant_200x200.png" alt="Valmistelut" tabindex="0">
		    <div class="hover-text">`;
		cardImageHtml += data.setup_text;
		cardImageHtml += `</div>
	        </div>`;
		nodash = true;
	}
	if (!nodash)
		cardImageHtml += `--`;

	return cardImageHtml;
}

function updateCardRow(cardId, data) {
	// Find the specific row to replace
	const row = document.querySelector(`tr.card-row[data-cardid="${cardId}"]`);

	row.setAttribute("data-cardid", data.id);
	row.setAttribute("data-cardprize", data.prize);

	updateInputValues(cardId, data.id, data.prize);

	if (row) {
		// Get number of cells on row. Mobile view has 3, web view more.
		const cells = row.getElementsByTagName(\'td\');

		 // Update the hidden input for keeping prize
	        const prizeInput = row.querySelector(`input[type="hidden"][name^="keepprize"]`);
	        if (prizeInput) {
			prizeInput.name = `keepprize${data.id}`; // Change input name to keepprize + new card ID
			prizeInput.value = data.prize; // Update the value to the new card\'s prize
		}
		if (cells.length === 3) {
			// Mobile display with only 3 cells
			// Name cell with formatted content
			let cardImageHtml = gen_card_name_html(data);
			cardImageHtml += gen_card_specials_html(data, true);
			cells[1].innerHTML = cardImageHtml; // Update the second cell
			cells[2].textContent = data.expansion_name; // Update expansion name
		} else if (cells.length > 3) {
			// Name cell with formatted content
			let cardImageHtml = gen_card_name_html(data);
			let cardSpecialHtml = gen_card_specials_html(data, false);

			// Update the necessary cells
			cells[1].innerHTML = cardImageHtml;
			cells[2].innerHTML = cardSpecialHtml;

			row.querySelector(\'td:nth-child(4)\').textContent = data.type_name; // update card type
			row.querySelector(\'td:nth-child(5)\').textContent = data.prize; // update card prize
			row.querySelector(\'td:nth-child(6)\').textContent = data.expansion_name; // update expansion name
		}
	}
}

function replaceCard(cardId, cardPrize) {
    // Retrieve checked expansion IDs
    const keepIDs = Array.from(document.querySelectorAll(\'input[name="keepid[]"]\')).map(input => input.value);

    const expansionCheckboxes = document.querySelectorAll(\'input[name="expansion[]"]:checked\');
    const expansions = Array.from(expansionCheckboxes).map(checkbox => checkbox.value); // Get values of checked boxes

    // Check if no expansions are selected and set the ID to 0 if so
    if (expansions.length === 0) {
        expansions.push(\'0\'); // Send 0 for no expansions checked
    }

    //console.log("expansions: "+expansions.join(\',\'));

    const tmp = keepIDs.join(\',\');
    console.log("IDs" + tmp);

    // Make an AJAX call to your server
    fetch(\'mvapi/mvaapi.php\', {
        method: \'POST\', // Use POST method
        headers: {
            \'Content-Type\': \'application/x-www-form-urlencoded\', // Set content type
        },
        body: new URLSearchParams({
	    cardid: cardId,        // Card ID to replace
	    cardids: keepIDs.join(\',\'),	// All Card IDs to prevent duplicates
	    cardprize: cardPrize,		// Prize of the card
            expansions: expansions.join(\',\')	// Join the array as a comma-separated string
        })
    })
// Debugging
//   .then(res => { console.log(res); return res.text(); })
 //   .then(txt => console.log(txt))
// Debugging ends
    .then(response => response.json())
    .then(data => {
        // Update the DOM with the new card info
        updateCardRow(cardId, data);
	updateShareUrls();
    })
    .catch(error => console.error(\'Error:\', error)); // Handle any errors
}

function startSwipe(event) {
    startX = event.touches[0].clientX;
}

function endSwipe(event) {
    const endX = event.changedTouches[0].clientX;
    const diffX = endX - startX;

    if (Math.abs(diffX) > 50) { // Swipe threshold
        const row = event.currentTarget;
        if (diffX > 0) {
                const cardId = row.getAttribute(\'data-cardid\');
		const cardPrize = row.getAttribute(\'data-cardprize\');
            // Swipe Right
//            row.classList.add(\'swiped\');
            // Add logic to replace card, e.g., randomizing new card
            replaceCard(cardId, cardPrize);
        }
    }
}


/* Swipe and ajax stuff ends */

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
