<?php
/*
 * Dominon card randomizer.
 * Uses MySQL database.
 *
 * AUTHOR: Matti Vaittinen <mazziesaccount@gmail.com>
 *
 * Written just for fun. No Warranty. Use at your own risk!
 *
 * TODO: This class should be rewritten so that:
 * - A single object instantiated from this class, would represent a single set of cards.
 * - Eg, own object for cards cheaper than 4. Own object for cards costing 4,
 *   and own object for cards costing more than 4.
 * - This would simplify the logic and get rid of the 'set' arrays.
 * - If we still want to do only one query to the database for retrieving the selected
 *   cards, then we need to pull the query logic out of the class, and add a function which
 *   gets the query results as an argument, matches the IDs from results to IDs in the set,
 *   and updates the card information based on the result data. Other option would be to do
 *   a query / set, and keep the SQL in the class. It's sure cleaner but maybe less efficient.
 *   Hence I might prefer having the SQL outside this class.
 * - Furthermore, we should do the TODO: in cards.php -file. That should help quite a bit
 *   to further simplify this class. I think the cards should be having their own 'show()'
 *   function which outputs the table cell with card data. This class should just call the
 *   card's show().
 *
 * Copyright 2025, 2026 Matti Vaittinen mazziesaccount@gmail.com>
 */

class dom_card_set {
	public $set_name	= null;
	public $ids		= null;
	public $all_ids		= null;
	public $cards		= null;
	public $bottom_cards	= null;
	private $conn		= null;

	private function bottom_out($c, $mobile)
	{
		$found = null;

		foreach($this->bottom_cards AS $bc) {
			if ($bc->id == $c->dual_top_of_id)
				$found = $bc;
		}
		if (!$found) {
			debug_print("Bottom card id ".$c->dual_top_of_id." for ".htmlspecialchars($c->name)." Not Found!");
			return '';
		}
		if ($mobile) {
			$out = $found->showcard_popup('<i>'.htmlspecialchars('-> '.$found->name).'</i>');
		} else {
			$out = $found->showcard_popup('<i>'.htmlspecialchars('-> '.$found->name).'</i>');
		}
		return $out;
	}
	private function add_change_input($id, $set_num, $prize, $checked)
	{
		$out = '<input form="theform" type="checkbox" name="keepid[]" value="'.$id.'"'.$checked.'>'."\n";
		$out .= '<input form="theform" type="hidden" name="keepprize'.$id.'" value="'.$prize.'"'.$checked.'>'."\n";
		return $out;
	}
	public function add_set($skeleton_cards, $set_name) {
		$this->set_name[] = $set_name;

		$ids = array_map(fn($c) => $c->get_id(), $skeleton_cards);
		$tmp = implode(',', $ids);
		$this->ids[] = $tmp;
		if (count($this->ids) > 1)
			$this->all_ids .= ', '.$tmp;
		else
			$this->all_ids .= $tmp;
		/*
		 * Hack to handle dual decks
		 *
		 * This hack was an optimization. We queried the dual_top_of_id()
		 * already in randomizer query - so we did not need another query
		 * for dual_top_of_id() for selected cards.
		 *
		 * This, however, is not done for the 'kept (when re-randomizing)',
		 * 'rated' or 'shared' cards, where only the top-card ID is coming
		 * in via the POST/GET data.
		 *
		 * So, in order to avoid extra complexity, always perform second
		 * query for the bottom card IDs (dual_top_of_id) after cards have
		 * been selected. (Other, more complex option would be adding the
		 * bottom IDs for randomized cards here, and then finding the
		 * dual_top_of_ids separately only if rate/share/keep data is found.
		 * That would require additional logic also for preventing the bottom
		 * cards to be added twice, once here and once later.
		 *
		foreach($skeleton_cards AS $c)
			if ($c->dual_top_of_id)
				$this->all_ids .= ', '.$c->dual_top_of_id;
		 */
	}
	public function get_all_prizes()
	{
		foreach($this->cards AS $card)
			$prz[] = $card->prize;

		return $prz;
	}
	public function get_all_ids()
	{
		foreach($this->cards AS $card)
			$ids[] = $card->id;

		return $ids;
	}

	private function hack_bottoms_for_fixed()
	{
		$query = "SELECT dual_top_of_id FROM cards WHERE id IN ($this->all_ids) AND dual_top_of_id != 0";
		$result = mysqli_query($this->conn, $query);
		if (!$result || !mysqli_num_rows($result))
			return;

		while ($row = mysqli_fetch_assoc($result))
			$this->all_ids .= ', '.$row['dual_top_of_id'];
	}

	public function get_cards()
	{
		/* Add 'bottom card' IDs */
		$this->hack_bottoms_for_fixed();
		debug_print("$this->all_ids");

		$query = "SELECT c.*, e.name AS expansion_name, pt.name AS prizetype_name, ct.name AS type_name, setup.text AS setup_text FROM cards AS c ";
		$query .= "LEFT JOIN expansion AS e ON c.expansion_id = e.id ";
		$query .= "LEFT JOIN cardtype AS ct ON c.type_id = ct.id ";
		$query .= "LEFT JOIN prizetype AS pt ON c.prizetype_id = pt.id ";
		$query .= "LEFT JOIN setup_extras AS setup ON c.setup_extras_id = setup.id ";
		$query .= "WHERE c.id IN ($this->all_ids) ";
		$query .= "ORDER BY c.prize";
		$res = query_cards($this->conn, $query);
		while ($row = mysqli_fetch_assoc($res))
			if (!$row['dual_below_id'])
				$this->cards[] = dom_card::from_full_row($row);
			else
				$this->bottom_cards[] = dom_card::from_full_row($row);
	}
	private function add_card_row_swipe_info($id, $prize)
	{
		$out = 'data-cardid="'.$id.'" ';
		$out .= 'data-cardprize="'.$prize.'" ';
		$out .= 'class="card-row" ';
		$out .= 'onTouchStart="startSwipe(event)" ';
		$out .= 'onTouchEnd="endSwipe(event)"';

		return $out;
	}
	private function set_table_start($mobile)
	{
		$card_table_headers = dom_card::get_card_tablehead($mobile);
		$out = '<table class="cardlist"><tr>'."\n";
		if ($mobile)
			$out .= '<th class="checkbox">[pid&auml;] / vaihda</th>';
		else
			$out .= '<th class="checkbox">[pid&auml;]</th>';
		$out .= $card_table_headers;
		$out .= '</tr>';

		return $out;
	}
	public function show_sets($keepids, $check_selected, $mobile = 0) {
		$vals_on_sets = array(3,3,4);
		$omena = false;

		$out = "";
		for ($i = 0; $i < 3; $i++) {
			$tuhinasum = 0;

			$out .= '<div class="settitle"><h3>' . htmlspecialchars($this->set_name[$i]) . '</h3></div>'."\n";
			$out .= $this->set_table_start($mobile);

			/* This is a horrible hack, trusting sets have 3, 3, 4 cards */
			for ($j = 0; $j < $vals_on_sets[$i]; $j++) {
				$c = $this->cards[$j + 3 * $i];
				$tuhinasum += $c->tuhinakerroin;

				if ($c->dual_top_of_id) {
					$bottom_card = $this->bottom_out($c, $mobile);
					/*
					 * For dual deck cards, we add the bottom card information
					 * to the card name cell
					 */
					$c->append_card_name_cell($bottom_card, $mobile);
				}

				/* We return the information that an omen was included so we can later add the prophecies */
				if ($c->omen)
					$omena = true;

				$checked = "";
				if ($check_selected && isset($keepids[$i])) {
					foreach($keepids[$i] AS $keep) {
						if ($c->id == $keep)
							$checked = " checked";
					}
				}

				$out .= '<tr '.$this->add_card_row_swipe_info($c->id, $c->prize).'>';
				$out .= '<td class="checkbox swipeicon">' . $this->add_change_input($c->id, $i, $c->prize, $checked).'<img src="img/swipe_scaled.webp" alt="swipe-right"></td>'."\n";
				$out .= $c->get_card_row($mobile);
				$out .= '</tr>';
			}
			$out .= "</table>"."\n";
			$out .= '<div class="tuhinatext">Tuhina ' . $tuhinasum."</div>\n";
		}
		echo $out;

		return $omena;
	}

	public static function prepare_set($conn) {
		$card_set =  new self();

		$card_set->conn = $conn;
		return $card_set;
	}
}

?>
