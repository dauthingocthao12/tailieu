<?php
/**
data[star] = rank;
星座（インデックス） => 順位
*/
class Zodiac000020 extends UranaiPlugin {

	function run($URL) {

		$CONTENTS = $this->load($URL);
		$content = $CONTENTS[0];
		$content = mb_convert_encoding($content, "UTF-8", "SJIS");
		$LINES = explode("\n", $content);

		$star["水瓶座"] = 1;
		$star["魚　座"] = 2;
		$star["牡羊座"] = 3;
		$star["牡牛座"] = 4;
		$star["双子座"] = 5;
		$star["蟹　座"] = 6;
		$star["獅子座"] = 7;
		$star["乙女座"] = 8;
		$star["天秤座"] = 9;
		$star["蠍　座"] = 10;
		$star["射手座"] = 11;
		$star["山羊座"] = 12;

		$now = self::getToday();
		$date_pattern = "!今日の星占い幸運度星座ランキング＜{$now['month']}月{$now['day']}日＞!";

		$RESULT = array();
		$rank_num = 0;
		$date_check_ok = false;
		foreach ($LINES AS $line) {
			if (count($RESULT) == 12) { break; }

			// date check
			if(!$date_check_ok) {
				$date_check_ok = preg_match($date_pattern, $line);
				continue;
			}

			// rank
			if ($date_check_ok && !$rank_num && preg_match("/<font size=\"1\">(.*?)位<\/font>/", $line, $MATCHES)) {
				$rank_num = $MATCHES[1];
				$rank_num = mb_convert_kana($rank_num, "a", "UTF-8");
				continue;
			}

			// star
			if ($rank_num && preg_match("/<td nowrap><font size=\".*?\">(.*座)<\/font><\/td>/", $line, $MATCHES)) {
				$star_name = $MATCHES[1];
				$star_num = $star[$star_name];
				$RESULT[$star_num] = $rank_num;
				// reset
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
