<?php

class Zodiac000126 extends UranaiPlugin {

    /**
     * 1Pで取得仕様
     */
	function run($CONTENTS) {
        $content = $CONTENTS[0];
		$LINES = explode("\n", $content);
        $now = self::getToday();
		$star = self::$starDefault;
		$RESULT = [];

		$date_check = false;
		$rank_num = "";
		$star_num = "";

		foreach ($LINES as $line) {

			//print $line;
			// Date check
			if(!$date_check) {
				$date_check = preg_match("/text-center font-size-24\">{$now['year']}年0?{$now['month']}月0?{$now['day']}日.*ランキング<\/h2>/", $line);
			}
			if($date_check) {
				if (preg_match("/<span class=\"font-size-36.*\">([0-9]*)<\/span>位/", $line, $MATCHES)){
					$rank_num = $MATCHES[1];
				}
			}
			if($date_check && $rank_num) {
				if (preg_match("/text-4a4a4a.*（(.*)）<\/a><\/p>/", $line, $MATCHES)){
					$star_name = $MATCHES[1];
					$star_num = $star[$star_name];
					$RESULT[$star_num] = $rank_num;
					$rank_num = 0;
				}
			}
		}

		if(!$date_check) {
			print $this->logDateError().PHP_EOL;
		}
		return $RESULT;
	}


    /**
     * 各運勢ごとに取得仕様
     */
	function topic_run($CONTENTS) {
        $now = self::getToday();
		$star = self::$starKanji;
		$RESULT = [];
		$topic_en = array(
			"恋愛運"=> 'love',
			"金運" => 'money',
			"仕事運"=> 'work',
			"健康運" => 'health'
	   );

        foreach($CONTENTS AS $key => $content){

			$LINES = explode("\n", $content);
			$date_check = false;
			$topic_key = "";
			$star_count = 0;

			foreach ($LINES as $line) {

				//print $line;
				// Date check
				if(!$date_check) {
					$date_check = preg_match("/text-center font-size-24\">{$now['year']}年0?{$now['month']}月0?{$now['day']}日.*）(.*座)の運勢<\/h2>/", $line, $MATCHES);
					$star_name = $MATCHES[1];
					$star_num = $star[$star_name];
				}
				if ($date_check && $star_num && preg_match("/class=\"float-left min-width-150 text-4e4e4e\">(.*運)<\/span>/", $line, $MATCHES)){
					if($topic_key){
						$RESULT[$star_num][$topic_key] = $star_count*20;
					}
					$topic_name = $MATCHES[1];
					$topic_key = $topic_en[$topic_name];
					// break;
					$star_count = 0;
				}
				if ($topic_key && preg_match("/img.*\/star.png\"/", $line)) {
					$star_count++;
				}
			}
			$RESULT[$star_num][$topic_key] = $star_count*20;

			if(!$date_check) {
				print $this->logDateError().PHP_EOL;
			}
		}
		return $RESULT;
	}

	function lucky_run($CONTENTS){
		$LUCKY_RESULT = array();
		// date check
		$now = self::getToday();
		$star = self::$starKanji;
		foreach ($CONTENTS as $key => $content) {
			$LINES = explode("\n", $content);
			$date_check = false;
			$lucky_item = null;
			$lucky_color = null;
			foreach ($LINES as $line) {
				// Date check
				if (!$date_check) {
					$date_check = preg_match("/text-center font-size-24\">{$now['year']}年0?{$now['month']}月0?{$now['day']}日.*）(.*座)の運勢<\/h2>/", $line,$MATCHES);
					$star_name = $MATCHES[1];
					$star_num = $star[$star_name];
				}

				//lucky_item
				if ($date_check && preg_match("/ラッキー素材：「(.*?)」素材<\/h4>/", $line, $MATCHES)) {
					$lucky_item = $MATCHES[1];
				}

				//lucky_color
				if ($date_check && preg_match("/ラッキーカラー：「(.*?)」<\/h4>/", $line, $MATCHES)) {
					$lucky_color = $MATCHES[1];
				}
			}
			$LUCKY_RESULT[$star_num] = array("lucky_item" => $lucky_item, "lucky_color" => $lucky_color);
			if (!$date_check) {
				print $this->logDateError() . PHP_EOL;
			}
		}
		return $LUCKY_RESULT;
	}
}
