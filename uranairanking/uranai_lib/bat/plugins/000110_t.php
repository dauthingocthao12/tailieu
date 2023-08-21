<?php

class Zodiac000110 extends UranaiPlugin {

	//サイト105での星座名 => 占いランキングでの管理番号
	private static $STAR_110_TO_STAR = [
			"aqua"   => "1",  // 水瓶座
			"pis"    => "2",  // 魚座
			"seep"   => "3",  // 牡羊座
			"ousi"   => "4",  // 牡牛座
			"futago" => "5",  // 双子座
			"kani"   => "6",  // 蟹座
			"sisi"   => "7",  // 獅子座
			"otome"  => "8",  // 乙女座
			"tenbin" => "9",  // 天秤座
			"sasori" => "10", // 蠍座
			"ite"    => "11", // 射手座
			"capri"  => "12", // 山羊座
	];

	function run($CONTENTS) {
		$content = $CONTENTS[0];
		$content = mb_convert_encoding($content, "UTF-8", "SJIS");
		$LINES = explode("\n", $content);

		$now = self::getToday();
		$star = self::$starDefault;

		$RESULT = array();
		$date_valid = false;

		foreach ($LINES as $line) {
			if (count($RESULT) == 12) { break; }
			// date
			if(!$date_valid && preg_match("/<span class=\"header\">{$now['year']} 年 {$now['month']} 月 {$now['day']} 日<\/span>/", $line)){
				$date_valid = true;
			}
			if($date_valid && preg_match("/<tr><th rowspan=3   class=\"ex2\">(\d{1,2})<\/th><th rowspan=3  class=\"ex2\"><img src=(.*).gif><\/th>/", $line, $matches)){
				$star = self::$STAR_110_TO_STAR[$matches[2]];
				$rank = $matches[1];
				$RESULT[$star] = $rank;
			}
		}

		if(!$date_valid) {
			print $this->logDateError().PHP_EOL;
		}

		return $RESULT;
	}

	function topic_run($CONTENTS) {
		$content = $CONTENTS[0];
		$content = mb_convert_encoding($content, "UTF-8", "SJIS");
		$LINES = explode("\n", $content);

		$now = self::getToday();
		$star = self::$starDefault;

		$RESULT = array();
		$date_valid = false;

		foreach ($LINES as $line) {
			// date
			if(!$date_valid && preg_match("/<span class=\"header\">{$now['year']} 年 {$now['month']} 月 {$now['day']} 日<\/span>/", $line)){
				$date_valid = true;
			}

			if($date_valid && preg_match("/<tr><th rowspan=3   class=\"ex2\">(\d{1,2})<\/th><th rowspan=3  class=\"ex2\"><img src=(.*).gif><\/th>/", $line, $matches)){
				$star = self::$STAR_110_TO_STAR[$matches[2]];
			}

			$ptn = "/<img src=\"(.*).gif\" width=\"\d*\" height=\"\d*\"><\/th><td class=\"ex3\">.*<\/td>/";
			if($date_valid && preg_match($ptn, $line, $matches)){
				$topic_ = $matches[1];
				if($topic_ == "business"){
					$topic = "work";
				}else{
					$topic = $topic_;
				}
				if($topic == "love"){
					$point = substr_count($line, "<img src=\"./gauge.gif\">") * 10;
				}else if($topic == "money"){
					$point = substr_count($line, "<img src=\"./gauge2.gif\">") * 10;
				}else if($topic == "work"){
					$point = substr_count($line, "<img src=\"./gauge3.gif\">") * 10;
				}
				$RESULT[$star][$topic] = $point;
			}
		}

		if(!$date_valid) {
			print $this->logDateError().PHP_EOL;
		}

		return $RESULT;
	}
}
