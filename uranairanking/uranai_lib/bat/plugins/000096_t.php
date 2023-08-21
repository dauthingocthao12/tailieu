<?php
/**
 * ウララの占い館
 *
 * @author Azet
 * @date 2017-06-23
 * @url http://news.mynavi.jp/horoscope/
 */
class Zodiac000096 extends UranaiPlugin {

	function run($CONTENTS) {
		global $num_star;
		$now = self::getToday();
		/*
		* $RESULT[星座番号] => 順位
		*/
		$RESULT = array();
		$rank = 0;
		foreach($CONTENTS as $content){

			$LINES = explode("\n", $content);
			$date_check_ok = false;

			foreach ($LINES AS $L) {
				$date_pattern = "/<h3 class=\"circle\">{$now['year']}年{$now['month']}月{$now['day']}日\（.*\）<div class=\"sub_txt clearfix\">/";

				/*12星座分のデータがリザルトにある時処理を抜ける*/
				if (count($RESULT) == 12) { break; }

				/*星座の取得*/
				if(preg_match("/<h2 class=\"underline mb25\"><span>(.*座)の運勢<\/span><\/h2>/", $L, $MATCHES)){
					$star_name = $MATCHES[1];	
					$star_num = $num_star[$star_name];
				}

				/*日付の判定*/
				if (!$date_check_ok) {
					$date_check_ok = preg_match($date_pattern, $L);
					continue;
				}

				/*順位の取得*/
				$reg = "/<p class=\"bold size18\">総合運<\/p><p class=\"size24 bold\"><span class=\"size30 bold\">(?:rank)?(\d{1,2})<\/span>位<\/p>/";
//echo $reg.PHP_EOL;
				if(preg_match($reg, $L, $MATCHES_RANK)){
					$rank = $MATCHES_RANK[1];
					$RESULT[$star_num] = $rank;
				}

				// エラーチェック
				if(!$date_check_ok) {
					print $this->logDateError().PHP_EOL;
				}
			}
		}
		return $RESULT;
	}

	function topic_run($CONTENTS) {
		global $num_star;
		$date_check_ok = false;
		$now = self::getToday();

		$love_reg = "/\/_img\/uranai\/heart_on.gif/";
		$money_reg = "/\/_img\/uranai\/money_on.gif/";
		$work_reg = "/\/_img\/uranai\/work_on.gif/";
		$health_reg = "/\/_img\/uranai\/health_on.gif/";
		$date_pattern = "/<h3 class=\"circle\">{$now['year']}年{$now['month']}月{$now['day']}日\（.*\）<div class=\"sub_txt clearfix\">/";

		foreach($CONTENTS as $content){

			$love = 0;
			$money = 0;
			$work = 0;
			$health = 0;

			$LINES = explode("\n", $content);

			foreach ($LINES AS $L) {
				/*星座の取得*/
				if(preg_match("/<h2 class=\"underline mb25\"><span>(.*座)の運勢<\/span><\/h2>/", $L, $MATCHES)){
					$star_name = $MATCHES[1];	
					$star_num = $num_star[$star_name];
				}

				/*日付の判定*/
				if (!$date_check_ok) {
					$date_check_ok = preg_match($date_pattern, $L);
					continue;
				}

				/*順位の取得*/
				if(preg_match($love_reg, $L)){
					$love++;
					$TOPIC_RESULT[$star_num]['love'] = $love * 20;
				}
				if(preg_match($money_reg, $L)){
					$money++;
					$TOPIC_RESULT[$star_num]['money'] = $money * 20;
				}
				if(preg_match($work_reg, $L)){
					$work++;
					$TOPIC_RESULT[$star_num]['work'] = $work * 20;
				}
				if(preg_match($health_reg, $L)){
					$health++;
					$TOPIC_RESULT[$star_num]['health'] = $health * 20;
				}

				// エラーチェック
				if(!$date_check_ok) {
					print $this->logDateError().PHP_EOL;
				}
			}
		}

		return $TOPIC_RESULT;
	}

}


