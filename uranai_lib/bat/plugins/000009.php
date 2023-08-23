<?php
/**
data[star] = rank;
星座（インデックス） => 順位
*/
class Zodiac000009 extends UranaiPlugin {

	function run($URL) {

		$CONTENTS = $this->load($URL);
		$content = $CONTENTS[0];
		//$content = mb_convert_encoding($content, "UTF-8", "UTF-8");
		$LINES = explode("<", $content);

		$star = self::$starDefault;
		$now = self::getToday();
		$date_pattern = "/h2 class=\"contents_title\">{$now['month']}月{$now['day']}日の運勢/";

		$RESULT = array();
		$rank_num = 0;
		$date_check_ok = false;
		foreach ($LINES AS $line) {
			if (count($RESULT) == 12) { break; }

			// date check
			if(!$date_check_ok) {
				$date_check_ok = preg_match($date_pattern, $line);
			}

			// rank
			if ($date_check_ok && !$rank_num && preg_match("/div class=\"hitomebo_color\" style=\"margin-bottom:6px;font-size:small;text-align:left;\">(\d{1,2})位$/", $line, $MATCHES)) {
				$rank_num = $MATCHES[1];
				continue;
			}
			// star
			if ($rank_num  && preg_match("/div class=\"hitomebo_color\" style=\"margin-top:6px;font-size:small;\">(.*)$/", $line, $MATCHES)) {
				$star_name = $MATCHES[1];
				$star_num = $star[$star_name];
				$RESULT[$star_num] = $rank_num;
				// reset
				$rank_num = 0;
			}
		}

		if(!$date_check_ok) {
			print $this->logDateError().PHP_EOL;
		}

		return $RESULT;
	}
}
