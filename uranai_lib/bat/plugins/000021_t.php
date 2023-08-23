<?php
/**
data[star] = rank;
星座（インデックス） => 順位
*/
class Zodiac000021 extends UranaiPlugin {

	function run($CONTENTS) {

		$content = $CONTENTS[0];
		//$content = mb_convert_encoding($content, "UTF-8", "UTF-8");
		$LINES = explode("\n", $content);

		$star = self::$starDefault;
		$now = self::getToday();
		$date_pattern = "/<span class='f-s16 fb'>今日の運勢（{$now['year']}年{$now['month']}月{$now['day']}日）<\/span>/";

		$RESULT = array();
		$date_check_ok = false;
		$rank_num = null;
		foreach ($LINES AS $line) {
			if (count($RESULT) == 12) { break; }

			// date
			if(!$date_check_ok) {
				$date_check_ok = preg_match($date_pattern, $line);
				continue;
			}

			// rank + star
			if ($date_check_ok && preg_match("/daily1 cengb.*\">(\d{1,2})<span class=\"rank_i\">/", $line, $MATCHES)) {
				$rank_num = $MATCHES[1];
				continue;
			}
			
			if ($rank_num && preg_match("/genkb top_trank_seiza\">(.*座)<\/p>/", $line, $MATCHES)){
				$star_name = $MATCHES[1];
				$star_num = $star[$star_name];
				$RESULT[$star_num] = $rank_num;
				$rank_num = "";
			}
		}

		// date error?
		if(!$date_check_ok) {
			print $this->logDateError().PHP_EOL;
		}

		return $RESULT;
	}

	function topic_run($TOPIC_CONTENTS) {

		$TOPIC_RESULT = array();
		$topic_content = $TOPIC_CONTENTS[0];
		//$content = mb_convert_encoding($content, "UTF-8", "UTF-8");
		$TOPIC_LINES = explode("\n", $topic_content);

		$star = self::$starDefault;
		$now = self::getToday();
		$date_pattern = "/<span class='f-s18 fb'>今日の運勢（{$now['year']}年{$now['month']}月{$now['day']}日）<\/span>/";
		var_dump($date_pattern);

		$date_check_ok = false;
		foreach ($TOPIC_LINES as $topic_line) {
			if (count($TOPIC_RESULT) == 12) { break; }

			// date
			if(!$date_check_ok) {
				$date_check_ok = preg_match($date_pattern, $topic_line);
				continue;
			}

			//star
			if ($date_check_ok && preg_match("/<td class=\"horotx\">(.*?座)<\/td>/", $topic_line, $MATCHES)) {
				$star_name = $MATCHES[1];
				$star_num = $star[$star_name];
				$flag = 1;
			}

			if ($date_check_ok && $flag && preg_match("/恋愛運 <span class=\"star\">(.*?)<\/span>/", $topic_line, $MATCHES)) {
				$love_content = $MATCHES[1];
				$love = substr_count($love_content , '★');
				$love_num = ($love * 20);
			}

			if ($date_check_ok && $flag && preg_match("/対人運 <span class=\"star\">(.*?)<\/span>/", $topic_line, $MATCHES)) {
				$interpersonal_content = $MATCHES[1];
				$interpersonal = substr_count($interpersonal_content , '★');
				$interpersonal_num = ($interpersonal * 20);
			}

			if ($date_check_ok && $flag && preg_match("/勉強・仕事運 <span class=\"star\">(.*?)<\/span>/", $topic_line, $MATCHES)) {
				$work_content = $MATCHES[1];
				$work = substr_count($work_content , '★');
				$work_num = ($work * 20);
			}

			if ($date_check_ok && $flag && preg_match("/金銭運 <span class=\"star\">(.*?)<\/span>/", $topic_line, $MATCHES)) {
				$money_content = $MATCHES[1];
				$money = substr_count($money_content , '★');
				$money_num = ($money * 20);
			}

			if ($date_check_ok && $flag && preg_match("/お出かけ運 <span class=\"star\">(.*?)<\/span>/", $topic_line, $MATCHES)) {
				$outing_content = $MATCHES[1];
				$outing = substr_count($outing_content , '★');
				$outing_num = ($outing * 20);
			}

			if ($date_check_ok && $flag && preg_match("/美容・健康運 <span class=\"star\">(.*?)<\/span>/", $topic_line, $MATCHES)) {
				$health_content = $MATCHES[1];
				$health = substr_count($health_content , '★');
				$health_num = ($health * 20);
				$flag = 0;

				$TOPIC_RESULT[$star_num] = array("love"=> $love_num , "money" => $money_num ,"work" => $work_num ,"interpersonal" => $interpersonal_num,"outing" => $outing_num,"health" => $health_num);

			}
		}

		// date error?
		if(!$date_check_ok) {
			print $this->logDateError().PHP_EOL;
		}

		return $TOPIC_RESULT;
	}

	function lucky_run($TOPIC_CONTENTS) {

		$LUCKY_RESULT = array();
		$topic_content = $TOPIC_CONTENTS[0];
		$TOPIC_LINES = explode("\n", $topic_content);

		$star = self::$starDefault;
		$now = self::getToday();
		$date_pattern = "/<span class='f-s18 fb'>今日の運勢（{$now['year']}年{$now['month']}月{$now['day']}日）<\/span>/";

		$date_check_ok = false;
		foreach ($TOPIC_LINES as $topic_line) {
			if (count($LUCKY_RESULT) == 12) {
				break;
			}

			// date
			if(!$date_check_ok) {
				$date_check_ok = preg_match($date_pattern, $topic_line);
				continue;
			}

			//star
			if ($date_check_ok && preg_match("/<td class=\"horotx\">(.*?座)<\/td>/", $topic_line, $MATCHES)) {
				$star_name = $MATCHES[1];
				$star_num = $star[$star_name];
			}

			if ($date_check_ok && preg_match("/<li>ラッキーアイテム：\s<span class=\"genkb\">(.*?)<\/span><\/li>/", $topic_line, $MATCHES)) {
				$lucky_item = $MATCHES[1];
			}

			if ($date_check_ok && preg_match("/<li>ラッキーカラー：<span class=\"genkb\">(.*?)<\/span><\/li>/", $topic_line, $MATCHES)) {
				$lucky_color = $MATCHES[1];
			}
			$LUCKY_RESULT[$star_num] = array("lucky_item"=> $lucky_item , "lucky_color" => $lucky_color);

			// date error?
			if(!$date_check_ok) {
				print $this->logDateError().PHP_EOL;
			}
		}
		return $LUCKY_RESULT;
	}
}
