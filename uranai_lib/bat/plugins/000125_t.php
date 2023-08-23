<?php

class Zodiac000125 extends UranaiPlugin {

    /**
     * 各運勢ごとに取得仕様
     */
	function run($CONTENTS) {
        $now = self::getToday();
		$star = self::$starDefault;
		$RESULT = [];

        foreach($CONTENTS AS $key => $content){

			$LINES = explode("\n", $content);
			$date_check = false;
			$star_num = '';

			foreach ($LINES as $line) {

				//print $line;
				// star check
				if(!$star_num) {
					preg_match("/<h1>.*（(.*座)）の今日の運勢｜星座占い<\/h1>/", $line, $MATCHES);
					$star_name = $MATCHES[1];
					$star_num = $star[$star_name];
				}
				// Date check
				if($star_num && !$date_check) {
					$date_check = preg_match("/alt=\"今日の運勢\"><br>今日.*{$now['month']}\/{$now['day']}.*の運勢/", $line);
				}
				//rank
				if ($date_check && $star_num && preg_match("/<span class=\"fortune_daily_rank\">([0-9]*)位<\/span>/", $line, $MATCHES)){
					$rank_num = $MATCHES[1];
					$star_num = $star[$star_name];
					$RESULT[$star_num] = $rank_num;
					break;
				}
			}

			if(!$date_check) {
				print $this->logDateError().PHP_EOL;
			}
		}
		return $RESULT;
	}


    /**
     * 各運勢ごとに取得仕様 
     */
	function topic_run($CONTENTS) {
        $now = self::getToday();
		$star = self::$starDefault;
		$TOPIC_RESULT = [];

        foreach($CONTENTS AS $key => $content){

			$LINES = explode("\n", $content);
			$date_check = false;
			$star_num = '';
			$love = 0;
			$money = 0;
			$work = 0;
			$interpersonal = 0;

			foreach ($LINES as $line) {

				//print $line;
				// star check
				if(!$star_num) {
					preg_match("/<h1>.*（(.*座)）の今日の運勢｜星座占い<\/h1>/", $line, $MATCHES);
					$star_name = $MATCHES[1];
					$star_num = $star[$star_name];
				}
				// Date check
				if($star_num && !$date_check) {
					$date_check = preg_match("/alt=\"今日の運勢\"><br>今日.*{$now['month']}\/{$now['day']}.*の運勢/", $line);
				}
				//love
				if ($date_check && $star_num && preg_match("/<span>.*img.*renaiun_on/", $line)){
					$love = substr_count($line,"renaiun_on");
				}
				//money
				if ($date_check && $star_num && preg_match("/<span>.*img.*kinun_on/", $line)){
					$money = substr_count($line,"kinun_on");
				}
				//work
				if ($date_check && $star_num && preg_match("/<span>.*img.*shigotoun_on/", $line)){
					$work = substr_count($line,"shigotoun_on");
				}
				//interpersonal
				if ($date_check && $star_num && preg_match("/<span>.*img.*taijinun_on/", $line)){
					$interpersonal = substr_count($line,"taijinun_on");
				}
			}
			$love_num = ($love * 20);
			$money_num = ($money * 20);
			$work_num = ($work * 20);
			$interpersonal_num = ($interpersonal * 20);
			$TOPIC_RESULT[$star_num] = array("love"=> $love_num , "money" => $money_num ,"work" => $work_num,"interpersonal" => $interpersonal_num);

			if(!$date_check) {
				print $this->logDateError().PHP_EOL;
			}
		}
		return $TOPIC_RESULT;
	}

	function lucky_run($CONTENTS){
		$LUCKY_RESULT = array();
		// date check
		$now = self::getToday();
		$star = self::$starDefault;
		foreach ($CONTENTS as $key => $content) {
			$LINES = explode("\n", $content);
			$date_check = false;
			$star_num = '';
			$lucky_color_flg = false;
			foreach ($LINES as $line) {
				if(!$star_num) {
					preg_match("/<h1>.*（(.*座)）の今日の運勢｜星座占い<\/h1>/", $line, $MATCHES);
					$star_name = $MATCHES[1];
					$star_num = $star[$star_name];
				}
				// Date check
				if($star_num && !$date_check) {
					$date_check = preg_match("/alt=\"今日の運勢\"><br>今日.*{$now['month']}\/{$now['day']}.*の運勢/", $line);
				}
				//lucky_color
				if ($date_check && preg_match("/<dt>ラッキーカラー<\/dt>/", $line, $MATCHES)) {
					$lucky_color_flg = true;
				}
				if ($date_check && $lucky_color_flg && preg_match("/<\/span>(.*?)<\/dd>/", $line, $MATCHES)) {
					$lucky_color = $MATCHES[1];
					$lucky_color_flg = false;
				}
			}
			$LUCKY_RESULT[$star_num] = array("lucky_item" => null, "lucky_color" => $lucky_color);
			if (!$date_check) {
				print $this->logDateError() . PHP_EOL;
			}
		}
		return $LUCKY_RESULT;
	}
}
