<?php
/**
data[star] = rank;
星座（インデックス） => 順位
*/
class Zodiac000008 extends UranaiPlugin {

	function run($CONTENTS) {

		$content = $CONTENTS[0];
		//$content = mb_convert_encoding($content, "UTF-8", "UTF-8");
		$LINES = explode("\n", $content);

		$now = self::getToday();
		$star = self::$starDefault;

		$RESULT = array();

		$date_check = false;
		$rank_num = 0;
		foreach ($LINES as $line) {
			if (count($RESULT) == 12) { break; }
			//print $line.PHP_EOL;

			// date
			if(!$date_check) {
				$date_check = preg_match("!<h2>{$now['month']}月{$now['day']}日のランキング</h2>!", $line);
				//print "DATE OK".PHP_EOL;
			}

			// rank
			if ($date_check && !$rank_num && preg_match("/<li class=\"rank_(\d{2})\">/", $line, $MATCHES)) {
				$rank_num = intval($MATCHES[1]);
				continue;
			}

			// star
			if ($rank_num && preg_match("/<img src=\".*?\" alt=\"([^\d]+?)\" width=\"75\" \/>/", $line, $MATCHES)) {
				$star_name = $MATCHES[1];
				$star_num = $star[$star_name];
				$RESULT[$star_num] = $rank_num;
				// reset
				$rank_num = 0;
			}
		}

		if(!$date_check) {
			print $this->logDateError().PHP_EOL;
		}

		return $RESULT;
	}

	function topic_run($TOPIC_CONTENTS) {
		$TOPIC_RESULT = array();

		// date check
		$now = self::getToday();
		$star = self::$starDefault;

		foreach($TOPIC_CONTENTS as $key => $topic_content){

			$TOPIC_LINES = explode("\n", $topic_content);
			$date_check = false;

			foreach ($TOPIC_LINES as $topic_line) {
				//print $topic_line;
				// Date Star check
				if(!$date_check) {
					$date_check = preg_match("/<h2>{$now['month']}月{$now['day']}日の.*座の運勢<\/h2>/", $topic_line);
					
				}
				//star
				if ($date_check && preg_match("/<h2>{$now['month']}月{$now['day']}日の(.*座)の運勢<\/h2>/", $topic_line, $MATCHES)){
					$star_name = $MATCHES[1];
					$star_num = $star[$star_name];
				}
				//love
				if ($date_check && $star_num && preg_match("/<img height=\"24\" src=\"https:\/\/www\.asahicom\.jp\/uranai\/12seiza\/images\/love_(\d{1})\.gif\"/", $topic_line, $MATCHES)){
					$love = $MATCHES[1];
					$love_num = ($love * 20);
				}
				//money
				if ($date_check && $star_num && preg_match("/<img height=\"24\" src=\"https:\/\/www\.asahicom\.jp\/uranai\/12seiza\/images\/money_(\d{1})\.gif\"/", $topic_line, $MATCHES)){
					$money = $MATCHES[1];
					$money_num = ($money * 20);
				}
				//work
				if ($date_check && $star_num && preg_match("/<img height=\"24\" src=\"https:\/\/www\.asahicom\.jp\/uranai\/12seiza\/images\/work_(\d{1})\.gif\"/", $topic_line, $MATCHES)){
					$work = $MATCHES[1];
					$work_num = ($work * 20);
				}
			}
			$TOPIC_RESULT[$star_num] = array("love"=> $love_num , "money" => $money_num ,"work" => $work_num);

			if(!$date_check) {
				print $this->logDateError().PHP_EOL;
			}
		}
		return $TOPIC_RESULT;
	}
}
