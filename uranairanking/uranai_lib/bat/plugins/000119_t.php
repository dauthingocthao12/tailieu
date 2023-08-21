<?php

class Zodiac000119 extends UranaiPlugin {

    /**
     * UranaiPlugin->getParentDataを参照ください
     */
	function run($CONTENTS) {
  
		foreach($CONTENTS as $url){
			$content = $url;
			$content = mb_convert_encoding($content, "UTF-8", "SJIS");
			$content = preg_replace("/\r\n|\r|\n/", "\n", $content);
			$LINES = explode("\n", $content);
			$now = self::getToday();
			$star = self::$starKanji;

			$date_valid = false;
			$star_name = null;

			$dptn = "/{$now['year']}年".date("m")."月".date("d")."日の運勢<\/font>/";
			foreach ($LINES as $line) {
				if(!$date_valid && preg_match($dptn, $line)){
					$date_valid = true;
					break;
				}
			}

			$passed_symbol = false;

			foreach ($LINES as $line) {
				// echo $line.PHP_EOL;
				if($date_valid && preg_match("/<title>(.*座)の運勢 :: 無料占いサイト/u", $line, $matches)){
					$star_name = $matches[1];
					$star_num = $star[$star_name];
				}

				if($date_valid && preg_match("/<tr><td>12星座中<\/td><\/tr>/", $line)){
					$passed_symbol = true;
				}

				if($date_valid && $passed_symbol && preg_match("/(\d*)位<br \/>/", $line, $matches)){
					$rank = $matches[1];
					$RESULT[$star_num] = $rank;
					$passed_symbol = false;
				}
			}
			if(!$date_valid) {
				echo $this->logDateError().PHP_EOL;
			}
		}
		asort($RESULT);
		return $RESULT;
	}

	function topic_run($CONTENTS) {

		foreach($CONTENTS as $url){

			$content = $url;
			$content = mb_convert_encoding($content, "UTF-8", "SJIS");
			$content = preg_replace("/\r\n|\r|\n/", "\n", $content);
			$LINES = explode("\n", $content);
			$now = self::getToday();
			$star = self::$starKanji;

			$date_valid = false;
			$star_name = null;

			$dptn = "/{$now['year']}年".date("m")."月".date("d")."日の運勢<\/font>/";
			foreach ($LINES as $line) {
				if(!$date_valid && preg_match($dptn, $line)){
					$date_valid = true;
					break;
				}
			}

			$passed_symbol = false;
			$topic_ = null;

			foreach ($LINES as $line) {
				// echo $line.PHP_EOL;
				if($date_valid && preg_match("/<title>(.*座)の運勢 :: 無料占いサイト/u", $line, $matches)){
					$star_name = $matches[1];
					$star_num = $star[$star_name];
				}

				if($date_valid && preg_match("/<tr><td>(.*運)<\/td><\/tr>/", $line, $matches)){
					$passed_symbol = true;
					$topic_ = $matches[1];
				}

				if($date_valid && $passed_symbol && preg_match("/<img src=\"https:\/\/yapyjp-img.sakura.ne.jp\/emoji.*$/", $line, $matches)){
					if($topic_ == "恋愛運"){
						$topic = "love";
						$emoji_str = "F991.gif";
					}else if($topic_ == "仕事運"){
						$topic = "work";
						$emoji_str = "F9BE.gif";
					}else if($topic_ == "金運"){
						$topic = "money";
						$emoji_str = "F9BA.gif";
					}
					if($topic_ != "総合運"){
						$point = (substr_count($line, $emoji_str) * 20);
						$RESULT[$star_num][$topic] = $point;
						$topic_ = $topic = null;
						$passed_symbol = false;

					}
				}
			}

			if(!$date_valid) {
				echo $this->logDateError().PHP_EOL;
			}
		}

		return $RESULT;
	}

}
