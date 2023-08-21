<?php
/**
 * @author Azet
 */
class Zodiac000100 extends UranaiPlugin {

	/**
	 * 星座（インデックス） => 順位
	 * @return array[star] = rank;
	 */
	function run($CONTENTS) {

		global $num_star;
		$RESULT = array();
		$content = $CONTENTS[0];
		$content = mb_convert_encoding($content, "UTF-8", "SJIS");
		$points = array();
		$now = self::getToday();
		$date_pattern = "/{$now['month']}\/{$now['day']}\(.*\)の運勢/";
		$date_check_ok = false;

		$LINES = explode("\n", $content);
		foreach ($LINES AS $line) {
			// date check
			if(!$date_check_ok) {
				$date_check_ok = preg_match($date_pattern, $line);
			}
			//星座名
			if(preg_match("/(\d{1,2})位&nbsp;<a href=\"\/t.cgi\?t=c\/dbox\/signname\&sign=\d{1,2}\">(.*座)<\/a><span>/",$line,$MATCHES)){
				$rank = $MATCHES[1];
				$star = $MATCHES[2];
				$star_num = $num_star[$star];
				$RESULT[$star_num] = $rank;
			}
		}//foreach $LINES end

		// date error?
		if(!$date_check_ok) {
			print $this->logDateError().PHP_EOL;
		}
		return $RESULT;
	}

	function topic_run($CONTENTS) {

		$stars = array(
			"水瓶座" => "1"
			,"魚座" => "2"
			,"牡羊座" => "3"
			,"牡牛座" => "4"
			,"双子座" => "5"
			,"蟹座" => "6"
			,"獅子座" => "7"
			,"乙女座" => "8"
			,"天秤座" => "9"
			,"蠍座" => "10"
			,"射手座" => "11"
			,"山羊座" => "12"
		);

		$RESULT = array();
		$date_check_ok = false;
		$now = self::getToday();
		$date_pattern = "/{$now['month']}\/{$now['day']}の運勢/";

		$love_reg = "/恋愛運：<span>(.*)<\/span><br>/";
		$interpersonal_reg = "/対人運：<span>(.*)<\/span><br>/";
		$money_reg = "/金銭運：<span>(.*)<\/span><br>/";
		$work_reg = "/仕事運：<span>(.*)<\/span><br>/";

		foreach($CONTENTS as $content){
			$content = mb_convert_encoding($content, "UTF-8", "SJIS");
			$LINES = explode("\n", $content);

			$love = 0;
			$interpersonal = 0;
			$money = 0;
			$work = 0;

			foreach ($LINES AS $L) {

				/*星座の取得*/
				if(preg_match("/\d{1,2}位&nbsp;(.*座)<br>/", $L, $MATCHES)){
					$star_name = $MATCHES[1];	
					$star_num = $stars[$star_name];
				}

				if (!$date_check_ok) {
					$date_check_ok = preg_match($date_pattern, $L);
					continue;
				}
				if(preg_match($love_reg,$L,$MATCHES)){
					$love = mb_substr_count($MATCHES[1], "★");
				}
				if(preg_match($interpersonal_reg,$L,$MATCHES)){
					$interpersonal = mb_substr_count($MATCHES[1], "★");
				}
				if(preg_match($money_reg,$L,$MATCHES)){
					$money = mb_substr_count($MATCHES[1], "★");
				}
				if(preg_match($work_reg,$L,$MATCHES)){
					$work = mb_substr_count($MATCHES[1], "★");
				}
			}

			$RESULT[$star_num]['love'] = $love * 20;
			$RESULT[$star_num]['interpersonal'] = $interpersonal * 20;
			$RESULT[$star_num]['money'] = $money * 20;
			$RESULT[$star_num]['work'] = $work * 20;
		}
		return $RESULT;
	}

}
