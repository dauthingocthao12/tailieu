<?php
/**
data[star] = rank;
星座（インデックス） => 順位
*/
class Zodiac000008 extends UranaiPlugin {

	function run($URL) {

		$CONTENTS = $this->load($URL);
		$content = $CONTENTS[0];
		//$content = mb_convert_encoding($content, "UTF-8", "UTF-8");
		$LINES = explode("\n", $content);

		$now = self::getToday();
		$star = self::$starDefault;

		$RESULT = array();

		$date_check = false;
		$rank_num = 0;
		foreach ($LINES AS $line) {
			if (count($RESULT) == 12) { break; }
			//print $line.PHP_EOL;

			// date
			if(!$date_check) {
				$date_check = preg_match("!<h2>{$now['month']}月{$now['day']}日のランキング</h2>!", $line);
				//print "DATE OK".PHP_EOL;
			}

			// rank
			if ($date_check && !$rank_num && preg_match("/<li class=\"rank_(\d{2})\">/", $line, $MATCHES)) {
				$rank_num = intval($MATCHES[1]);
				continue;
			}

			// star
			if ($rank_num && preg_match("/<img src=\".*?\" alt=\"([^\d]+?)\" width=\"75\" \/>/", $line, $MATCHES)) {
				$star_name = $MATCHES[1];
				$star_num = $star[$star_name];
				$RESULT[$star_num] = $rank_num;
				// reset
				$rank_num = 0;
			}
		}

		if(!$date_check) {
			print $this->logDateError().PHP_EOL;
		}

		return $RESULT;
	}
}
