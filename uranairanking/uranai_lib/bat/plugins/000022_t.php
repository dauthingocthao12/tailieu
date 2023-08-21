<?php
/**
data[star] = rank;
星座（インデックス） => 順位
*/
class Zodiac000022 extends UranaiPlugin {

	function run($CONTENTS) {
		$RESULT = array();

		$year = date('Y');
		$month = date('m');
		$day = date('d');
		$date_pattern = "/<span class=\".*\">{$year}年{$month}月{$day}日の運勢<\/span>/";
		$star = self::$starDefault;
	
		$LINES = explode("\n", $CONTENTS[1]);
		$date_check_ok = false;
		$rank_num = 0;

		foreach ($LINES as $line) {

			// date check
			if(!$date_check_ok) {
				$date_check_ok = preg_match($date_pattern, $line);
				continue;
			}

			if ($date_check_ok && preg_match("/<p class=\"f_marumaru\"><span class=\"astro_number .*\">(\d{1,2})<\/span><span class=\"astro_name\" class=\"fs15\">(.*座)<\/span>/", $line, $MATCHES)) {
				$rank_num = $MATCHES[1];
				$star_name =$MATCHES[2];
				$star_name_trim =  preg_replace("/( |　)/", "", $star_name );
				$star_num = $star[$star_name_trim];
				$RESULT[$star_num] = $rank_num;
				$rank_num = 0;
				continue;
			}
		}

		// date error?
		if(!$date_check_ok) {
			print $this->logDateError().PHP_EOL;
		}

		return $RESULT;
	}

	function topic_run($TOPIC_CONTENTS) {

		$star = self::$starDefault;

		$year = date('Y');
		$month = date('m');
		$day = date('d');
		$date_pattern = "/<span class=\".*\">{$year}年{$month}月{$day}日の運勢<\/span>/";

		$TOPIC_RESULT = array();

		$date_check_ok = false;
		// foreach($TOPIC_CONTENTS as $topic_content) {
		foreach($TOPIC_CONTENTS as $star_num => $topic_content) {
			$TOPIC_LINES = explode("\n", $topic_content);

			foreach ($TOPIC_LINES as $topic_line) {
				if(!$date_check_ok) {
					$date_check_ok = preg_match($date_pattern, $topic_line);
					continue;
				}

				if ($date_check_ok && preg_match("/<h2>.*>(.*座)<\/span><\/h2>/", $topic_line, $MATCHES)) {
						$star_name = $MATCHES[1];
						$star_num = $star[$star_name];
				}

				if ($date_check_ok && preg_match("/<th title=\"恋愛運\">/", $topic_line)) {
					$love_flg = 1;
				}

				if ($date_check_ok && $love_flg && preg_match("/<img (.*)title=\"☆\">/", $topic_line, $MATCHES)) {
					$love_content = $MATCHES[1];
					$love = substr_count($love_content , 'star.svg');
					$love_num = ($love * 20);
					$love_flg = 0;
				}
				
				if ($date_check_ok && preg_match("/<th title=\"金運\">/", $topic_line)) {
					$money_flg = 1;
				}

				if ($date_check_ok && $money_flg && preg_match("/<img (.*)title=\"☆\">/", $topic_line, $MATCHES)) {
					$money_content = $MATCHES[1];
					$money = substr_count($money_content , 'star.svg');
					$money_num = ($money * 20);
					$money_flg = 0;
				}
				
				if ($date_check_ok && preg_match("/<th title=\"仕事運\">/", $topic_line)) {
					$work_flg = 1;
				}

				if ($date_check_ok && $work_flg && preg_match("/<img (.*)title=\"☆\">/", $topic_line, $MATCHES)) {
					$work_content = $MATCHES[1];
					$work = substr_count($work_content , 'star.svg');
					$work_num = ($work * 20);
					$work_flg = 0;
					$TOPIC_RESULT[$star_num] = array("love"=> $love_num , "money" => $money_num ,"work" => $work_num);
				}
			}

			// date error?
			if(!$date_check_ok) {
				print $this->logDateError().PHP_EOL;
			}
		}
		return $TOPIC_RESULT;
	}

	function lucky_run($TOPIC_CONTENTS) {


		$star = self::$starDefault;

		$year = date('Y');
		$month = date('m');
		$day = date('d');
		$date_pattern = "/<span class=\".*\">{$year}年{$month}月{$day}日の運勢<\/span>/";

		$LUCKY_RESULT = array();

		$date_check_ok = false;
		foreach($TOPIC_CONTENTS as $star_num => $topic_content) {
			$TOPIC_LINES = explode("\n", $topic_content);

			foreach ($TOPIC_LINES as $topic_line) {

				if(!$date_check_ok) {
					$date_check_ok = preg_match($date_pattern, $topic_line);
					continue;
				}

				if ($date_check_ok && preg_match("/<h2>.*>(.*座)<\/span><\/h2>/", $topic_line, $MATCHES)) {
						$star_name = $MATCHES[1];
						$star_num = $star[$star_name];
				}
				
				if ($date_check_ok && preg_match("/<h4 class=\"c\">ラッキーアイテム<\/h4><\/span>(.*?)<\/div>/", $topic_line,$MATCHES)) {
					$lucky_item = $MATCHES[1];
				}
				
				if ($date_check_ok && preg_match("/<h4 class=\"c\">ラッキーカラー<\/h4><\/span>(.*?)<\/div>/", $topic_line,$MATCHES)) {
					$lucky_color = $MATCHES[1];
				}

				$LUCKY_RESULT[$star_num] = array("lucky_item"=> $lucky_item , "lucky_color" => $lucky_color);
			}

			// date error?
			if(!$date_check_ok) {
				print $this->logDateError().PHP_EOL;
			}
		}
		return $LUCKY_RESULT;
	}
}
