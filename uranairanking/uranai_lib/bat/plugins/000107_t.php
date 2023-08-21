<?php

class Zodiac000107 extends UranaiPlugin {

	function date_valid(){
		$topUrl = "https://service.smt.docomo.ne.jp/portal/fortune/src/fortune.html?utm_source=dmenu_fortune&utm_medium=owned&utm_campaign=header9-1";
		$topHtml = file_get_contents($topUrl);
		$lines = explode("\n", $topHtml);
		$now = self::getToday();
		foreach($lines as $line){
			if(preg_match("/<div class=\"date\">{$now['year']}年{$now['month']}月{$now['day']}日(.*)<\/div>/", $line)){
				return true;
			}
		}
		return false;
	}

	function run($CONTENTS) {
		$RESULT = [];
		if(!$this->date_valid()){
			echo $this->logDateError().PHP_EOL;
			return $RESULT;
		}

		foreach($CONTENTS as $url){
			$content = $url;
			$LINES = explode("\n", $content);
			$star = self::$starDefault;

			foreach ($LINES as $line) {
				$star_name = null;
				if(preg_match("/<h2 class=\"ttl-main__txt\">(.*座)<\/h2>/", $line, $matches)){
					$star_name = $matches[1];
					$star_num = $star[$star_name];
				}
				if(preg_match("/<h3 class=\"review-block__ttl\">全体運：<span>(\d{1,2})位<\/span><\/h3>/", $line, $matches)){
					$rank = $matches[1];
					$RESULT[$star_num] = $rank;
				}
			}
		}
		asort($RESULT);
		// print_r($RESULT);
		return $RESULT;

	}

	function topic_run($CONTENTS) {
		$RESULT = [];
		if(!$this->date_valid()){
			echo $this->logDateError().PHP_EOL;
			return $RESULT;
		}

		foreach($CONTENTS as $url){
			$content = $url;
			$LINES = explode("\n", $content);
			$star = self::$starDefault;
			$star_name = null;

			foreach ($LINES as $line) {
				if(preg_match("/<title>.+（(.*座)）.+<\/title>/", $line, $matches)){
					$star_name = $matches[1];
					$star_num = $star[$star_name];
				}

				if(preg_match("/<h4 class=\"review-block__starTtl\">(.*)運<\/h4>/", $line, $matches)){
					$topic_ = $matches[1];
					if($topic_ == "恋愛"){
						$topic = "love";
					}else if($topic_ == "仕事"){
						$topic = "work";
					}else if($topic_ == "金"){
						$topic = "money";
					}
				}

				if(preg_match("/span style=\"width: ([0-9]{1,3})%/", $line, $matches)){
					$point = $matches[1];
					$RESULT[$star_num][$topic] = $point;
				}
			}
		}
		// print_r($RESULT);
		return $RESULT;
	}
}
