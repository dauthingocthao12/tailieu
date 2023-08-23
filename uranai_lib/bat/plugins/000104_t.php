<?php

class Zodiac000104 extends UranaiPlugin {

	function run($CONTENTS) {

		foreach($CONTENTS as $url){
			$content = $url;
			$LINES = explode("\n", $content);
			$now = self::getToday();
			$star = self::$starKanji;

			$date_valid = false;
			$star_name = null;

			foreach ($LINES as $line) {
				if(!$date_valid && preg_match("/{$now['month']}月{$now['day']}日 今日の(.*座)運勢/", $line, $matches1)){
					$date_valid = true;
					$star_name = $matches1[1];
				}
				if($date_valid && preg_match("/本日の総合運：(\d{1,2})位/", $line, $matches2)){
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
				if(!$date_valid && preg_match("/{$now['month']}月{$now['day']}日 今日の(.*座)運勢/", $line, $matches1)){
					$date_valid = true;
					$star_name = $matches1[1];
					$star_num = $star[$star_name];
				}
				if($date_valid && preg_match("/<img src=\"12img\/icon2\/(.*)(\d+).png\" alt=\"star.*\" \/><br \/>/", $line, $matches2)){
					$topic = $matches2[1];
					if($topic == "star"){ continue; }

					if($topic == "en"){
						$topic = "money";
					}
					$star_count = intval($matches2[2]);
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
