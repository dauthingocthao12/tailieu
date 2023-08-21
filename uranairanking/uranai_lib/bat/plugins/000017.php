<?php
/**
data[star] = rank;
星座（インデックス） => 順位
*/
class Zodiac000017 extends UranaiPlugin {

	function run($URL) {

		$CONTENTS = $this->load($URL);
		$content = $CONTENTS[0];
		$content = mb_convert_encoding($content, "UTF-8", "SJIS");
		$LINES = explode("\n", $content);

		$star = self::$starDefault;
		$now = self::getToday();
		$date_pattern = "!<strong class=\"txt12\">{$now['month']}月{$now['day']}日の占いランキング</strong>!";

		$RESULT = array();
		$date_check_ok = false;
		$rank_num = 0;
		foreach ($LINES AS $key => $line) {
			if (count($RESULT) == 12) { break; }

			// date
			if(!$date_check_ok) {
				$date_check_ok = preg_match($date_pattern, $line);
				continue;
			}

			// rank
			if ($date_check_ok && !$rank_num && preg_match("/<img src=\"http:\/\/www.img.happywoman.jp\/12star\/images\/rank_(\d{1,2}).gif\" width=\"28\" height=\"28\">/", $line, $MATCHES)) {
				$rank_num = $MATCHES[1];
				continue;
			}
			
			// star
			if ($rank_num && preg_match("/<img src='.*?' alt='(.*?座)' border='0'>/", $line, $MATCHES)) {
				$star_name = $MATCHES[1];
				$star_num = $star[$star_name];
				$RESULT[$star_num] = $rank_num;

				// rest
				$rank_num = 0;
			}
		}

		// date error?
		if(!$date_check_ok) {
			print $this->logDateError().PHP_EOL;
		}

		return $RESULT;
	}
}
