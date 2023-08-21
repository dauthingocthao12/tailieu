<?php
/**
data[star] = rank;
星座（インデックス） => 順位
*/
class Zodiac000007 extends UranaiPlugin {

	function run($URL) {

		$RESULT = array();

		$CONTENTS = $this->load($URL);
		// date pattern
		$now = self::getToday();
		$date_pattern = "/<h3>今日{$now['month']}月{$now['day']}日の運勢<br \/>/";

		foreach($CONTENTS as $star_num => $content) {
			//$content = mb_convert_encoding($content, "UTF-8", "UTF-8");
			$LINES = explode("\n", $content);
			$date_check_ok = false;

			foreach ($LINES AS $line) {

				// date check
				if(!$date_check_ok) {
					$date_check_ok = preg_match($date_pattern, $line);
				}

				// rank
				if($date_check_ok && preg_match("/<img src=\".*\" width=\"480\" height=\"70\" alt=\"(\d{1,2})位\" \/>/", $line, $MATCH)) {
					$RESULT[$star_num] = $MATCH[1];
				}
			}

			// date error?
			if(!$date_check_ok) {
				print $this->logDateError().PHP_EOL;
			}
		}

		return $RESULT;
	}
}
