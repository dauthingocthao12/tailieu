<?php

class Zodiac000105 extends UranaiPlugin {

	//サイト105での星座管理番号 => 占いランキングでの管理番号
	private static $STAR_105_TO_STAR = [
			"1"  => "3",  // 牡羊座
			"2"  => "4",  // 牡牛座
			"3"  => "5",  // 双子座
			"4"  => "6",  // 蟹座
			"5"  => "7",  // 獅子座
			"6"  => "8",  // 乙女座
			"7"  => "9",  // 天秤座
			"8"  => "10", // 蠍座
			"9"  => "11", // 射手座
			"10" => "12", // 山羊座
			"11" => "1",  // 水瓶座
			"12" => "2"   // 魚座
	];

	function run($CONTENTS) {

		$content = $CONTENTS[0];
		$LINES = explode("\n", $content);

		$now = self::getToday();
		$star = self::$starDefault;

		$RESULT = array();
		$date_valid = false;

		foreach ($LINES as $line) {
			if (count($RESULT) == 12) { break; }
			// date
			if(!$date_valid && preg_match("/<p class=\"ranking-date\">{$now['year']}年{$now['month']}月{$now['day']}日<\/p>/", $line)){
				$date_valid = true;
			}
			if($date_valid && preg_match("/<section id=\"star-(\d{1,2})\" class=\"rank-(\d{1,2})\">/", $line, $matches)){
				$star = self::$STAR_105_TO_STAR[$matches[1]];
				$rank = $matches[2];
				$RESULT[$star] = $rank;
			}
		}

		if(!$date_valid) {
			print $this->logDateError().PHP_EOL;
		}

		return $RESULT;
	}


    /**
     * UranaiPlugin->getParentDataTopicを参照ください
     */
	function topic_run($CONTENTS) {

		$content = $CONTENTS[0];
		$LINES = explode("\n", $content);
		$now = self::getToday();

		$RESULT = array();
		$date_valid = false;

		$rating_line_passed = false;

		foreach ($LINES as $line) {
// echo $line.PHP_EOL;
			// if (count($RESULT) == 12) { break; }
			// date
			if(!$date_valid && preg_match("/<p class=\"ranking-date\">{$now['year']}年{$now['month']}月{$now['day']}日<\/p>/", $line)){
				$date_valid = true;
			}

			if($date_valid && preg_match("/<section id=\"star-(\d{1,2})\"/", $line, $matches)){
				$star = self::$STAR_105_TO_STAR[$matches[1]];
			}
			if(preg_match("/<p class=\"rating\">/", $line)){
				$rating_line_passed = true;
			}
			if($date_valid && $rating_line_passed && preg_match("/\s*(.*運)\s*/", $line, $matches)){
				$topic_ = $matches[1];
				if($topic_ == "恋愛運"){
					$topic = "love";
				}else if($topic_ == "金銭運"){
					$topic = "money";
				}else if($topic_ == "仕事運"){
					$topic = "work";
				}
			}
			if($date_valid && $topic != "" && preg_match("/<span>(&#9733;.*)<\/span>/", $line, $matches)){ 
				$point = (substr_count($matches[1], "&#9733;") * 20);
				$RESULT[$star][$topic] = $point;
			}
		}
		if(!$date_valid) {
			print $this->logDateError().PHP_EOL;
		}

		return $RESULT;
	}
}
