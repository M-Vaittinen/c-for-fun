<?php

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
