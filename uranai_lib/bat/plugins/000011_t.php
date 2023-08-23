<?php
/**
data[star] = rank;
星座（インデックス） => 順位
*/
class Zodiac000011 extends UranaiPlugin {

	function run($CONTENTS) {
		$content = $CONTENTS[0];
		//$content = mb_convert_encoding($content, "UTF-8", "UTF-8");
		$LINES = explode("\n", $content);

		$star = self::$starDefault;
		$now = self::getToday();
		$date_pattern = "/<h3 class=\"ft-mainbox__today_ttl\">{$now['month']}月 *{$now['day']}日のランキング<\/h3>/";

		$RESULT = array();
		$date_check_ok = false;
		$rank_num = 0;
		foreach ($LINES AS $line) {
			if (count($RESULT) == 12) { break; }

			// date check
			if(!$date_check_ok) {
				$date_check_ok = preg_match($date_pattern, $line);
				continue;
			}

			// rank
			if ($date_check_ok && !$rank_num && preg_match("/<span class=\"ft-fortune-ranking\">(\d{1,2})位<\/span>/", $line, $MATCHES)) {
				$rank_num = $MATCHES[1];
				continue;
			}
			
			// star
			if ($rank_num && preg_match("/&nbsp;<a href=\".*?\">(.*?)<\/a>/", $line, $MATCHES)) {
				$star_name = $MATCHES[1];
				$star_num = $star[$star_name];
				$RESULT[$star_num] = $rank_num;
				$rank_num = 0;
			}
		}

		if(!$date_check_ok) {
			print $this->logDateError().PHP_EOL;
		}

		return $RESULT;
	}

	function topic_run($TOPIC_CONTENTS) {
		$TOPIC_RESULT = array();

		$star = self::$starDefault;
		$now = self::getToday();

		foreach($TOPIC_CONTENTS as $topic_content){
			if (count($TOPIC_RESULT) == 13) { break; }

			$TOPIC_LINES = explode("\n", $topic_content);

			$date_pattern = "/<p class=\"ft-detail__ttl\">{$now['month']}月 *{$now['day']}日の運勢&nbsp;&nbsp;\d{1,2}位<\/p>/";

			$date_check_ok = false;
			$rank_num = 0;
			$love = 0;
			$money = 0;
			foreach ($TOPIC_LINES as $topic_line) {

				if (preg_match("/<li><h3>(.*座)<\/h3><\/li>/", $topic_line, $MATCHES)) {
					$star_name = $MATCHES[1];
					$star_num = $star[$star_name];
				//	print $star_num;
				}

				// date check
				if($star_num && !$date_check_ok) {
					$date_check_ok = preg_match($date_pattern, $topic_line);
				}

				if($date_check_ok && preg_match("/<img alt=\"恋愛運\" src=\"\/\/cdn\.vie\.auone\.jp\/asset\/pc\/fortune\/img\/heart\.gif\"/", $topic_line)){
					$love++;
				//	print $love;
				}
				
				if($date_check_ok && preg_match("/<img alt=\"金運\" src=\"\/\/cdn\.vie\.auone\.jp\/asset\/pc\/fortune\/img\/money\.gif\"/", $topic_line)){
					$money++;
				//	print $money;
				$love_num = ($love * 20);
				$money_num = ($money * 20);
				$TOPIC_RESULT[$star_num] = array("love"=> $love_num , "money" =>$money_num ,"work" => NULL  );
				}
			}

			if(!$date_check_ok) {
				print $this->logDateError().PHP_EOL;
			}
		}
		return $TOPIC_RESULT;
	}

}
