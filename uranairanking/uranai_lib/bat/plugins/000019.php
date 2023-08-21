<?php
/**
data[star] = rank;
星座（インデックス） => 順位
*/
class Zodiac000019 extends UranaiPlugin {

	function run($URL) {

		$CONTENTS = $this->load($URL);
		$content = $CONTENTS[0];
		//$content = mb_convert_encoding($content, "UTF-8", "UTF-8");
		$LINES = explode("\n", $content);

		//$star = self::$starDefault;
		// サイト毎に星座名のプラグイン個別設定
		$star = array();
		$star["水瓶座"] = 1;
		$star["魚座"] = 2;
		$star["牡羊座"] = 3;
		$star["牡牛座"] = 4;
		$star["双子座"] = 5;
		$star["蟹座"] = 6;
		$star["獅子座"] = 7;
		$star["乙女座"] = 8;
		$star["天秤座"] = 9;
		$star["蠍座"] = 10;
		$star["射手座"] = 11;
		$star["山羊座"] = 12;

		$now = self::getToday();
		//$date_pattern = "!<h3>{$now['month']}月{$now['day']}日\(.*\)のランキング</h3>!";
		$date_pattern = "/\">0?{$now['month']}月0?{$now['day']}日/";	//edit okabe 2015/05/16

		$RESULT = array();
		$date_check_ok = false;
		$rank_num = 0;
		foreach ($LINES AS $line) {
			if (count($RESULT) == 12) { break; }

			// date check
			if(!$date_check_ok) {
				$date_check_ok = preg_match($date_pattern, $line);
				continue;
			}

			// rank
			//if ($date_check_ok && !$rank_num && preg_match("/<dt>(\d{1,2})位<\/dt>/", $line, $MATCHES)) {
			if ($date_check_ok && !$rank_num && preg_match("/png\"\salt=\"No\.(\d{1,2})\"/", $line, $MATCHES)) {	//edit okabe 2015/05/16
				$rank_num = $MATCHES[1];
				continue;
			}

			// star
			//if ($rank_num && preg_match("/<dd>(.*?座)<\/dd>/", $line, $MATCHES)) {
			if ($rank_num && preg_match("/png\"\salt=\"(.*?座)\"/", $line, $MATCHES)) {	//edit okabe 2015/05/16
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
