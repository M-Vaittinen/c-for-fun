<?php

/*
 * Create the 'star rating' form. The form displays 5 rating stars and
 * a textual description for the rating. The selected amount of stars
 * is converted to 'rate' post data as follows:
 * Stars:	rate:
 * 1	=>	-10
 * 2	=>	-5
 * 3	=>	0
 * 4	=>	5
 * 5	=>	10
 *
 * The IDs of the cards that were rated, are added to the 'keepids',
 * post data which is already existing mechanism for displaying a
 * shared set of cards and also for 'keeping' some/all of the cards
 * when doing "re-randomizing". NOTE: It would probably be much
 * cleaner to have different post vars for the rating, keeping and
 * sharing, which would allow doing things differently if needed. This,
 * however, was the easy, quick'n dirty way. Currently the 'keep' and
 * 'share / rate' -options have no difference in post data handling,
 * thus all the cards will be 'selected' in UI when card set is rated
 * or shared card set is opened.
 *
 * NOTE: We can differentiate the sharing/rating and selecting use-cases
 * if we want. If 'rate' post-data exists, then this was rating (and
 * checkboxes could default unchecked). If deck was shared, the keepids
 * and keepprizes will not be in post-data but in get-data. We could
 * detect this as well and again, default checkboxes unchecked.
 *
 * @allids: an array of card IDs. Landmark, event and omen card-IDs should
 *	    have the defined ID offset applied so receiving side can separate
 *	    them from regular kingdom cards. allids array should be ordered so
 *	    that the ID in array index [X] will match the prize in @allprizes
 *	    array index [x]. NOTE: landmark, event and omen cards don't need to
 *	    have matching prize. Hence, the kingdom cards must be first in the
 *	    array, landmark, event and omen cards should be after them.
 * @allprizes: an array of kingdom card prizes. array should be ordered so
 *          that the prize in array index [X] will match the ID in @allids
 *          index [x].
 */
function ratebtn($allids, $allprizes)
{
	$output = '<form action="" method="post" id="rateform">'."\n";
	$output .= '</form>'."\n";

	$output .= '<h3>Arvioi t&auml;m&auml; korttisetti</h3>'."\n";
	$output .= '
<div class="mva">
<div class="mvacontainer">
  <div class="mvacontainer__items">
    <input form="rateform" type="radio" name="rate" id="st5" value="10">
    <label for="st5">
      <div class="star-stroke">
        <div class="star-fill"></div>
      </div>
      <div class="label-description" data-content="Mahtava!"></div>
    </label>
    <input form="rateform" type="radio" name="rate" id="st4" value="5">
    <label for="st4">
      <div class="star-stroke">
        <div class="star-fill"></div>
      </div>
      <div class="label-description" data-content="Hyv&auml;"></div>
    </label>
    <input form="rateform" type="radio" name="rate" id="st3" value="0">
    <label for="st3">
      <div class="star-stroke">
        <div class="star-fill"></div>
      </div>
      <div class="label-description" data-content="OK"></div>
    </label>
    <input form="rateform" type="radio" name="rate" id="st2" value="-5">
    <label for="st2">
      <div class="star-stroke">
        <div class="star-fill"></div>
      </div>
      <div class="label-description" data-content="Huono"></div>
    </label>
    <input form="rateform" type="radio" name="rate" id="st1" value="-10">
    <label for="st1">
      <div class="star-stroke">
        <div class="star-fill"></div>
      </div>
      
      <div class="label-description" data-content="Sysipaska"></div>
    </label>
  </div>
</div>';

	foreach($allids AS $id)
		$output .= '<input form="rateform" type="hidden" name="keepid[]" value="'.$id.'">'."\n";
	$i=0;
	foreach($allprizes AS $prize) {
		$output .= '<input form="rateform" type="hidden" name="keepprize'.$allids[$i].'" value="'.$prize.'">'."\n";
		$i++;
	}
	$output .= '<input form="rateform" class="shufflesubmit" type="submit" value="Arvostele"></div>'."\n";
	$output .= '<p>J&auml;t&auml; t&auml;m&auml; tyhj&auml;ksi<input id="rafo" form="rateform" type="text" name="foterarm" value=""></p>';

	return $output;	
}

?>
