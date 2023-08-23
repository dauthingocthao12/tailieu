<?php

class Zodiac000102 extends UranaiPlugin {

	function run($CONTENTS) {

		foreach($CONTENTS as $url){
			$content = $url;
			$LINES = explode("\n", $content);
			$now = self::getToday();
			$star = self::$starKanji;

			$date_valid = false;
			$star_name = null;

			foreach ($LINES as $line) {
				if(!$date_valid && preg_match("/<h2 class=\"horoscope_title\">{$now['month']}月{$now['day']}日の&ensp;(.*座)&ensp;運勢<\/h2>/", $line, $matches1)){
					$date_valid = true;
					$star_name = $matches1[1];
				}
				if($date_valid && preg_match("/<p>(\d+)位<\/p>/", $line, $matches2)){
					$rank = $matches2[1];
					$star_num = $star[$star_name];
					$RESULT[$star_num] = $rank;
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

		foreach($CONTENTS as $content){
			$LINES = explode("\n", $content);
			$now = self::getToday();
			$star = self::$starKanji;

			$date_valid = false;
			$star_name = null;

			foreach ($LINES as $line) {
				if(!$date_valid && preg_match("/<h2 class=\"horoscope_title\">{$now['month']}月{$now['day']}日の&ensp;(.*座)&ensp;運勢<\/h2>/", $line, $matches1)){
					$date_valid = true;
					$star_name = $matches1[1];
					$star_num = $star[$star_name];
				}
				if($date_valid && preg_match("/<h3 class=\"horoscope_s_title\">(.*)運.*<\/h3>/", $line, $matches2)){

					$topic_jpn = $matches2[1];

					if($topic_jpn == "恋愛"){
						$topic = "love";
					}elseif($topic_jpn == "金"){
						$topic = "money";
					}elseif($topic_jpn == "仕事"){
						$topic = "work";
					}

				}
				if($date_valid && $topic_jpn != "総合" && preg_match("/(★*)<span class=\"empty_star\">/u", $line, $matches3)){
					$star_count = substr_count($matches3[1], "★");
					$point = $star_count * 20;
					// echo $star_name.PHP_EOL; echo $topic.PHP_EOL; echo $point.PHP_EOL;
					$RESULT[$star_num][$topic] = $point;
					$point = 0;
					$topic = null;
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
