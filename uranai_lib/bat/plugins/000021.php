<?php
/**
data[star] = rank;
星座（インデックス） => 順位
*/
class Zodiac000021 extends UranaiPlugin {

	function run($URL) {

		$CONTENTS = $this->load($URL);
		$content = $CONTENTS[0];
		//$content = mb_convert_encoding($content, "UTF-8", "UTF-8");
		$LINES = explode("\n", $content);

		$star = self::$starDefault;
		$now = self::getToday();
		$date_pattern = "!<span class='f-s16 fb'>今日（{$now['year']}年{$now['month']}月{$now['day']}日）の運勢ランキング</span>!";

		$RESULT = array();
		$date_check_ok = false;
		foreach ($LINES AS $line) {
			if (count($RESULT) == 12) { break; }

			// date
			if(!$date_check_ok) {
				$date_check_ok = preg_match($date_pattern, $line);
				continue;
			}

			// rank + star
			if ($date_check_ok && preg_match("/<h3>(\d{1,2})位：(.*?座).*<\/h3>/", $line, $MATCHES)) {
				$rank_num = $MATCHES[1];
				$star_name = $MATCHES[2];
				$star_num = $star[$star_name];
				$RESULT[$star_num] = $rank_num;
			}
		}

		// date error?
		if(!$date_check_ok) {
			print $this->logDateError().PHP_EOL;
		}

		return $RESULT;
	}
}
