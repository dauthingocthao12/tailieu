<?php
/**
data[star] = rank;
星座（インデックス） => 順位
*/
class Zodiac000005 extends UranaiPlugin {

	function run($CONTENTS) {

//		$CONTENTS = $this->load($URL);

		$now = self::getToday();
		$date_pattern = "!<p class=\"t12a-top-date\">{$now['year']}<span>年</span>{$now['month']}<span>月</span>{$now['day']}<span>日</span></p>!";

		$RESULT = array();

		foreach($CONTENTS as $star_num => $content) {
			$content = mb_convert_encoding($content, "UTF-8", "EUC-JP");
			$LINES = explode("\n", $content);

			$rank_num = 0;
			$date_check_ok = false;
			// star loop
			foreach ($LINES AS $line) {
				//print $line.PHP_EOL;

				// date check
				if(!$date_check_ok) {
					$date_check_ok = preg_match($date_pattern, $line);
					continue;
				}

				// rank
				if ($date_check_ok && preg_match("!<dt><span>全体運ランキング</span>(\d{1,2})<span>位</span></dt>!", $line, $MATCHES)) {
					$rank_num = $MATCHES[1];
					$RESULT[$star_num] = $rank_num;
					break;
				}
			}

			// date error?
			if(!$date_check_ok) {
				print $this->logDateError().PHP_EOL;
			}

			if (count($RESULT) == 12) { break; }
		}

		return $RESULT;
	}

	function topic_run($TOPIC_CONTENTS) {

//		$CONTENTS = $this->load($URL);

		$now = self::getToday();
		$date_pattern = "!<p class=\"t12a-top-date\">{$now['year']}<span>年</span>{$now['month']}<span>月</span>{$now['day']}<span>日</span></p>!";

		$TOPIC_RESULT = array();

		foreach($TOPIC_CONTENTS as $star_num => $topic_content) {
			$topic_content = mb_convert_encoding($topic_content, "UTF-8", "EUC-JP");
			$TOPIC_LINES = explode("\n", $topic_content);

			$date_check_ok = false;
			// star loop
			foreach ($TOPIC_LINES AS $topic_line) {
				//print $line.PHP_EOL;

				// date check
				if(!$date_check_ok) {
					$date_check_ok = preg_match($date_pattern, $topic_line);
					continue;
				}

				//love
				if ($date_check_ok && preg_match("/<img src=\"\/uranai\/img\/horoscope\/mini-hart-0(\d{1})\.gif\" width=\"121\" height=\"21\">/", $topic_line, $MATCHES)) {
					$love = $MATCHES[1];
					$love_num = ($love * 20);
				}
				//work
				if ($date_check_ok && preg_match("/<img src=\"\/uranai\/img\/horoscope\/mini-bag-0(\d{1})\.gif\" width=\"121\" height=\"21\">/", $topic_line, $MATCHES)) {
					$work = $MATCHES[1];
					$work_num = ($work * 20);
				}
				//money
				if ($date_check_ok && preg_match("/<img src=\"\/uranai\/img\/horoscope\/mini-yen-0(\d{1})\.gif\" width=\"121\" height=\"21\">/", $topic_line, $MATCHES)) {
					$money = $MATCHES[1];
					$money_num = ($money * 20);
				}
					$TOPIC_RESULT[$star_num] = array("love"=> $love_num , "money" => $money_num ,"work" => $work_num);
			}


			// date error?
			if(!$date_check_ok) {
				print $this->logDateError().PHP_EOL;
			}

			if (count($TOPIC_RESULT) == 12) { break; }
		}

		return $TOPIC_RESULT;
	}
}
