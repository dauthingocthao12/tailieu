<?php
/**
data[star] = rank;
星座（インデックス） => 順位
*/
class Zodiac000022 extends UranaiPlugin {

	function run($URL) {

		$CONTENTS = $this->load($URL);
		$content = $CONTENTS[0];
		//$content = mb_convert_encoding($content, "UTF-8", "EUC-JP");
		$LINES = explode("\n", $content);

		$star = self::$starDefault;

		$year = date('Y');
		$month = date('m');
		$day = date('d');
		$date_pattern = "/<p>{$year}年{$month}月{$day}日の運勢<\/p>/";

		$RESULT = array();

		$date_check_ok = false;
		$rank_num = 0;

		foreach ($LINES AS $line) {

			// date check
			if(!$date_check_ok) {
				$date_check_ok = preg_match($date_pattern, $line);
				continue;
			}

			if ($date_check_ok && preg_match("/<img src=\".*?\" alt=\"(\d{1,2})位\" title=\"\d{1,2}位\">/", $line, $MATCHES)) {
				$rank_num = $MATCHES[1];
				continue;
			}
			
			if ($rank_num && preg_match("/<a href=\".*?\">(.*?座)<\/a>/", $line, $MATCHES)) {
				$star_name = $MATCHES[1];
				$star_num = $star[$star_name];
				$RESULT[$star_num] = $rank_num;
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
