<?php
/**
data[star] = rank;
星座（インデックス） => 順位
*/
class Zodiac000023 extends UranaiPlugin {

	function run($CONTENTS) {

		$star["水瓶座"] = 1;
		$star["魚座"] = 2;
		$star["牡羊座"] = 3;
		$star["牡牛座"] = 4;
		$star["双子座"] = 5;
		$star["蟹座"] = 6;
		$star["獅子座"] = 7;
		$star["乙女座"] = 8;
		$star["天秤座"] = 9;
		$star["蠍座"] = 10;
		$star["射手座"] = 11;
		$star["山羊座"] = 12;

		$now = self::getToday();
		$date_pattern = "!<li><span>{$now['month']}月{$now['day']}日.*</span></li>!";

		$RESULT = array();
		foreach($CONTENTS as $content) {
			$LINES = explode("\n", $content);
			$rank_num = 0;
			$star_num = "";
			$date_check_ok = false;
			foreach ($LINES AS $key => $line) {
				if (count($RESULT) == 12) { break; }
				$line = str_replace(" tint", "", $line);

				// date check
				if(!$date_check_ok) {
					$date_check_ok = preg_match($date_pattern, $line);
					continue;
				}

				// star
				if (!$star_num && preg_match("/img.*alt=\"(.*?座)/", $line, $MATCHES)) {
					$star_name = $MATCHES[1];
					$star_num = $star[$star_name];
					continue;
				}

				// rank
				if ($date_check_ok && $star_num && preg_match("/<dt>順位.*<span>(\d{1,2})<\/span>/", $line, $MATCHES)) {
					$rank_num = $MATCHES[1];
					$RESULT[$star_num] = $rank_num;
					// reset
					$rank_num = 0;
				}

			}

			// date error?
			if(!$date_check_ok) {
				print $this->logDateError().PHP_EOL;
			}
		}

		return $RESULT;
	}

	function topic_run($TOPIC_CONTENTS) {

		$star["水瓶座"] = 1;
		$star["魚座"] = 2;
		$star["牡羊座"] = 3;
		$star["牡牛座"] = 4;
		$star["双子座"] = 5;
		$star["蟹座"] = 6;
		$star["獅子座"] = 7;
		$star["乙女座"] = 8;
		$star["天秤座"] = 9;
		$star["蠍座"] = 10;
		$star["射手座"] = 11;
		$star["山羊座"] = 12;

		$now = self::getToday();
		$date_pattern = "/<li><span>{$now['month']}月{$now['day']}日.*<\/span><\/li>/";

		$TOPIC_RESULT = array();

		$date_check_ok = false;
		foreach($TOPIC_CONTENTS as $topic_content) {
			$TOPIC_LINES = explode("\n", $topic_content);
			$star_num = "";

			foreach ($TOPIC_LINES as $topic_line) {
				if (count($TOPIC_RESULT) == 12) { break; }

				// date check
				if(!$date_check_ok) {
					$date_check_ok = preg_match($date_pattern, $topic_content);
					continue;
				}

				// star
				if (!$star_num && preg_match("/class=\"leftdeco\">(.*?座)の運勢<\/span>/", $topic_content, $MATCHES)) {
					$star_name = $MATCHES[1];
					$star_num = $star[$star_name];
					continue;
				}
				//love
				if ($star_num && $date_check_ok && $star_num && preg_match("/<img src=\".*?\/star_love0(\d{1}).jpg\" alt=\"恋愛運\"/", $topic_line, $MATCHES)){
					$love = $MATCHES[1];
					$love_num = ($love * 20);
				}

				if ($star_num && $date_check_ok && $star_num && preg_match("/<img src=\".*?\/star_work0(\d{1}).jpg\" alt=\"仕事運\"/", $topic_line, $MATCHES)){
					$work = $MATCHES[1];
					$work_num = ($work * 20);
				}

				if ($star_num && $date_check_ok && $star_num && preg_match("/<img src=\".*?\/star_money0(\d{1}).jpg\" alt=\"金運\"/", $topic_line, $MATCHES)){
					$money = $MATCHES[1];
					$money_num = ($money * 20);
				}

			}
			$TOPIC_RESULT[$star_num] = array("love"=> $love_num , "money" => $money_num ,"work" => $work_num);

			// date error?
			if(!$date_check_ok) {
				print $this->logDateError().PHP_EOL;
			}
		}
		return $TOPIC_RESULT;
	}
}
