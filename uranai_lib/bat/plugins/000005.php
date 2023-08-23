<?php
/**
data[star] = rank;
星座（インデックス） => 順位
*/
class Zodiac000005 extends UranaiPlugin {

	function run($URL) {

		$CONTENTS = $this->load($URL);

		$now = self::getToday();
		$date_pattern = "!<p class=\"t12a-top-date\">{$now['year']}<span>年</span>{$now['month']}<span>月</span>{$now['day']}<span>日</span></p>!";

		$RESULT = array();

		foreach($CONTENTS as $star_num => $content) {
			$content = mb_convert_encoding($content, "UTF-8", "EUC-JP");
			$LINES = explode("\n", $content);

			$rank_num = 0;
			$date_check_ok = false;
			// star loop
			foreach ($LINES AS $line) {
				//print $line.PHP_EOL;

				// date check
				if(!$date_check_ok) {
					$date_check_ok = preg_match($date_pattern, $line);
					continue;
				}

				// rank
				if ($date_check_ok && preg_match("!<dt><span>全体運ランキング</span>(\d{1,2})<span>位</span></dt>!", $line, $MATCHES)) {
					$rank_num = $MATCHES[1];
					$RESULT[$star_num] = $rank_num;
					break;
				}
			}

			// date error?
			if(!$date_check_ok) {
				print $this->logDateError().PHP_EOL;
			}

			if (count($RESULT) == 12) { break; }
		}

		return $RESULT;
	}
}
