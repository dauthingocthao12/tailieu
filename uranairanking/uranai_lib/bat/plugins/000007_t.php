<?php
/**
data[star] = rank;
星座（インデックス） => 順位
*/
class Zodiac000007 extends UranaiPlugin {

	function run($CONTENTS) {

		$RESULT = array();

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

	function topic_run($TOPIC_CONTENTS) {

		$TOPIC_RESULT = array();

		// date pattern
		$now = self::getToday();
		$date_pattern = "/<h3>今日{$now['month']}月{$now['day']}日の運勢<br \/>/";

		foreach($TOPIC_CONTENTS as $star_num => $topic_content) {
			//$content = mb_convert_encoding($content, "UTF-8", "UTF-8");
			$TOPIC_LINES = explode("\n", $topic_content);
			$date_check_ok = false;
			$love = 0;
			$money = 0;
			$work = 0;
			$health = 0;

			foreach ($TOPIC_LINES AS $topic_line) {

				// date check
				if(!$date_check_ok) {
					$date_check_ok = preg_match($date_pattern, $topic_line);
				}

				// love
				if($date_check_ok && preg_match("/<img src=\"\/themes\/nagoyatv_pc\/horoscope\/images\/icon_02.jpg\" width=\"28\" height=\"28\" alt=\"\" \/>/", $topic_line)) {
					$love++;
				//	print $love;
				}
				//money
				if($date_check_ok && preg_match("/<img src=\"\/themes\/nagoyatv_pc\/horoscope\/images\/icon_03.jpg\" width=\"28\" height=\"28\" alt=\"\" \/>/", $topic_line)) {
					$money++;
				//	print $money;
				}
				//health
				if($date_check_ok && preg_match("/<img src=\"\/themes\/nagoyatv_pc\/horoscope\/images\/icon_04.jpg\" width=\"28\" height=\"28\" alt=\"\" \/>/", $topic_line)) {
					$health++;
				//	print $money;
				}
				//work
				if($date_check_ok && preg_match("/<img src=\"\/themes\/nagoyatv_pc\/horoscope\/images\/icon_05.jpg\" width=\"28\" height=\"28\" alt=\"\" \/>/", $topic_line)) {
					$work++;
				//	print $work;
				}
				if($date_check_ok && preg_match("/<h4>明日\d{1,2}月\d{1,2}日の運勢<\/h4>/", $topic_line)) {
				break;
				}

			}
			$love_num = ($love * 25);
			$money_num = ($money * 25);
			$health_num = ($health * 25);
			$work_num = ($work * 25);
			$TOPIC_RESULT[$star_num] = array("love"=> $love_num , "money" =>$money_num ,"work" => $work_num , "health" =>$health_num );
	// date error?
			if(!$date_check_ok) {
				print $this->logDateError().PHP_EOL;
			}
		}

		return $TOPIC_RESULT;
	}
}
