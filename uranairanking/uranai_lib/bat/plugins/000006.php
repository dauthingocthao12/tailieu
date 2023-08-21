<?php
/**
data[star] = rank;
星座（インデックス） => 順位
*/
class Zodiac000006 extends UranaiPlugin {

	function run($URL) {

		$CONTENTS = $this->load($URL);
		
		$star = self::$starDefault;
		$now = self::getToday();
		
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

		$RESULT = array();
		$content = $CONTENTS[0];
		$content = mb_convert_encoding($content, "UTF-8", "EUC-JP");
		$LINES = explode("\n", $content);

	foreach ($CONTENTS AS $key => $content) {
		$LINES = explode("\n", $content);
		
		//$date_pattern = "/<span class=\"today\">{$now['month']}月{$now['day']}日.*constellation.*>(.*)<\/span>.*の運勢は･･･<\/span>/";
		$date_check_ok = false;
		foreach ($LINES as $line) {

			if ($date_check_ok && count($RESULT) == 12) { break; }
			// date check
			if(!$date_check_ok) {
				$date_check_ok = preg_match("/<span class=\"today\">{$now['month']}月{$now['day']}日.*constellation\-name.*>(.*)<\/span>.*の運勢は.*/",$line, $MATCHES);
				$star_name = $MATCHES[1];
				$star_num = $star[$star_name];
			}

			if (preg_match("/<span><img src=\"\/common\/fki\/images\/v1\/horoscope\/rank\-([0-9]{1,2}).gif\".*><\/span>/", $line, $MATCHES)) {
				$rank_num = $MATCHES[1];
				$RESULT[$star_num] = $rank_num;
			}
		}

		if(!$date_check_ok) {
			print $this->logDateError().PHP_EOL;
		}
	}

		return $RESULT;
	}
}
