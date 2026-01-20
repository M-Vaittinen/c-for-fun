<?php
/*
 * 'Game starter' randomizer, suggest a new randomization string.
 * Uses MySQL database.
 *
 * AUTHOR: Matti Vaittinen <mazziesaccount@gmail.com>
 *
 * Written just for fun. No Warranty. Use at your own risk!
 * Copyright 2026, Matti Vaittinen mazziesaccount@gmail.com>
 */



function show_suggestion($conn, $num_to_show)
{
	$query = 'SELECT id, ehdotus FROM suggestions WHERE rejected = 0 AND added = 0 ORDER BY id ASC LIMIT ' . $num_to_show;

	$result = mysqli_query($conn, $query);
	if (!$result)
		return;

	echo '<table class="cardlist"" width="100%">';
	echo '<tr><th colspan="2">Latest suggestions</th></tr>';

	for ($i = 1; $row = mysqli_fetch_assoc($result); $i++) {
		$ehdotus = htmlspecialchars($row['ehdotus']);
		if ($ehdotus == "")
			continue;

		echo '<tr>';
		echo '<td>' . $i .'</td>';
		echo '<td>' . $ehdotus .'</td>';
		echo '</tr>';
	}
	echo '</table>';
	mysqli_free_result($result);
}

function num_suggestions($conn)
{
	$query = "select count(1) FROM suggestions WHERE added = 0 AND rejected = 0";
	$result = mysqli_query($conn, $query);
	$row = mysqli_fetch_array($result);

	$total = $row[0];

	mysqli_free_result($result);

	return $total;
}


function handle_new_suggestions($conn)
{
	if (!isset($_POST['question']) || "" != $_POST['answer'])
		return false;

	$suggestion = mysqli_real_escape_string($conn, $_POST['question']);

	if (strlen($suggestion) > 1023)
		return false;

	if (num_suggestions($conn) > 51)
		return false;

	$query = "INSERT INTO suggestions SET ehdotus='" .$suggestion . "'";
	mysqli_query($conn, $query);

	return 'Ehdotus lisätty';
}

function output_suggest_form($conn)
{
	$output = '<form action="" method="post">
		   <label for="question">Aloittajaehdotus:</label></br>
		   <textarea name="question" id="question" maxlength="1023" rows="6" cols="100"></textarea></br>
		   <label for="answer">Jätä tyhjäksi</label>
		   <input type="text" name="answer" id="answer"> </br>
		   <input type="submit" value="Lähetä ehdotus"></form>';

/*
 * In order to protect the database we only take 50 suggestions.
 * When suggestions are accepted (moved to arvonta table) or rejected
 * (deleted), the space for new suggestions gets added.
 */
	if (num_suggestions($conn) > 50)
		return;

	echo $output;
}


?>
