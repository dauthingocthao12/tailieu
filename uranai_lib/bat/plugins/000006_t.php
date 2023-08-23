<?php
/**
data[star] = rank;
星座（インデックス） => 順位
*/
class Zodiac000006 extends UranaiPlugin {

	function run($CONTENTS) {

		
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
	foreach ($CONTENTS AS $key => $content) {
		$LINES = explode("\n", $content);
		
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
	function topic_run($TOPIC_CONTENTS) {

//		$CONTENTS = $this->load($URL);
		
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

		$TOPIC_RESULT = array();

		foreach ($TOPIC_CONTENTS AS $key => $topic_content) {
			$TOPIC_LINES = explode("\n", $topic_content);
			
			$date_check_ok = false;
			foreach ($TOPIC_LINES as $topic_line) {

				if ($date_check_ok && count($TOPIC_RESULT) == 12) { break; }
				// date check
				if(!$date_check_ok) {
					$date_check_ok = preg_match("/<span class=\"today\">{$now['month']}月{$now['day']}日.*constellation\-name.*>(.*)<\/span>.*の運勢は.*/",$topic_line, $MATCHES);
					$star_name = $MATCHES[1];
					$star_num = $star[$star_name];
				}
				//love
				if (preg_match("/<img src=\"\/common\/fki\/images\/v1\/horoscope\/love-\d{1}\.gif\" alt=\"5点満点中：(\d{1})点\">/", $topic_line, $MATCHES)) {
					$love = $MATCHES[1];
					$love_num = ($love * 20);
				}
				//money
				if (preg_match("/<img src=\"\/common\/fki\/images\/v1\/horoscope\/money-\d{1}\.gif\" alt=\"5点満点中：(\d{1})点\">/", $topic_line, $MATCHES)) {
					$money = $MATCHES[1];
					$money_num = ($money * 20);
				}
				//work
				if (preg_match("/<img src=\"\/common\/fki\/images\/v1\/horoscope\/work-\d{1}\.gif\" alt=\"5点満点中：(\d{1})点\">/", $topic_line, $MATCHES)) {
					$work = $MATCHES[1];
					$work_num = ($work * 20);
				}
			}
			$TOPIC_RESULT[$star_num] = array("love"=> $love_num , "money" => $money_num ,"work" => $work_num);

			if(!$date_check_ok) {
				print $this->logDateError().PHP_EOL;
			}
		}

		return $TOPIC_RESULT;
	}
}
