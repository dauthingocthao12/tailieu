<?php

class Zodiac000113 extends UranaiPlugin {

	function run($CONTENTS) {

		foreach($CONTENTS as $url){
			$content = $url;
			$content = mb_convert_encoding($content, "UTF-8", "SJIS");
			$LINES = explode("\n", $content);
			$now = self::getToday();
			$star = self::$starKanji;

			$date_valid = false;
			$month = date("m");
			$day = date("d");

			$next_line_is_rank = false;

			foreach ($LINES as $line) {
				if(!$date_valid && preg_match("/今日の運勢<div>{$now['year']}年{$month}月{$day}日(.*)<\/div>/" , $line)){
					$date_valid = true;
				}
				if(preg_match("/<title>(.*座)の運勢<\/title>/", $line, $matches)){
					$star_name_kn = $matches[1];
					unset($matches);
				}
				if($date_valid && preg_match("/<div style=\"width:.*px; float:left;\">ランキング<\/div>/", $line)){
					$next_line_is_rank = true;
				}
				if($date_valid && $next_line_is_rank && preg_match("/<div>: (\d+) 位<\/div>/", $line, $matches)){
					$star_num = $star[$star_name_kn];
					$rank = $matches[1];
					$RESULT[$star_num] = $rank;
					$next_line_is_rank = false;
				}
			}
			if(!$date_valid) {
				echo $this->logDateError().PHP_EOL;
			}
		}
		asort($RESULT);
		// print_r($RESULT);
		return $RESULT;

	}


	function topic_run($CONTENTS) {

		foreach($CONTENTS as $url){
			$content = $url;
			$content = mb_convert_encoding($content, "UTF-8", "SJIS");
			$LINES = explode("\n", $content);
			$now = self::getToday();
			$star = self::$starKanji;

			$date_valid = false;
			$month = date("m");
			$day = date("d");

			$next_line_is_rank = false;

			foreach ($LINES as $line) {
				if(!$date_valid && preg_match("/今日の運勢<div>{$now['year']}年{$month}月{$day}日(.*)<\/div>/" , $line)){
					$date_valid = true;
				}
				if(preg_match("/<title>(.*座)の運勢<\/title>/", $line, $matches)){
					$star_name_kn = $matches[1];
					$star_num = $star[$star_name_kn];
					unset($matches);
				}
				if($date_valid && preg_match("/<div style=\"width:.*px; float:left;\">ランキング<\/div>/", $line)){
					$next_line_is_rank = true;
				}
				if($date_valid && preg_match("/<div>(.*運).*:.*<\/div>/", $line, $matches)){
					$topic_ = $matches[1];
					if($topic_ == "恋愛運"){
						$topic = "love";
					}else if($topic_ == "仕事運"){
						$topic = "work";
					}else if($topic_ == "金運"){
						$topic = "money";
					}
					if($topic_ != "総合運"){
						$point = (substr_count($line, "★") * 20);
						$RESULT[$star_num][$topic] = $point;
					}
				}
			}
			if(!$date_valid) {
				echo $this->logDateError().PHP_EOL;
			}
		}
		// print_r($RESULT);
		return $RESULT;


	}
}
