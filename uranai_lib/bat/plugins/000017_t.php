<?php
/**
data[star] = rank;
星座（インデックス） => 順位
*/
class Zodiac000017 extends UranaiPlugin {

	function run($CONTENTS) {

		$content = $CONTENTS[0];
		$content = mb_convert_encoding($content, "UTF-8", "SJIS");
		$LINES = explode("\n", $content);

		$star = self::$starDefault;
		$now = self::getToday();
		$date_pattern = "!<strong class=\"txt12\">{$now['month']}月{$now['day']}日の占いランキング</strong>!";

		$RESULT = array();
		$date_check_ok = false;
		$rank_num = 0;
		foreach ($LINES AS $key => $line) {
			if (count($RESULT) == 12) { break; }

			// date
			if(!$date_check_ok) {
				$date_check_ok = preg_match($date_pattern, $line);
				continue;
			}

			// rank
			if ($date_check_ok && !$rank_num && preg_match("/<img src=\"http:\/\/www.img.happywoman.jp\/12star\/images\/rank_(\d{1,2}).gif\" width=\"28\" height=\"28\">/", $line, $MATCHES)) {
				$rank_num = $MATCHES[1];
				continue;
			}
			
			// star
			if ($rank_num && preg_match("/<img src='.*?' alt='(.*?座)' border='0'>/", $line, $MATCHES)) {
				$star_name = $MATCHES[1];
				$star_num = $star[$star_name];
				$RESULT[$star_num] = $rank_num;

				// rest
				$rank_num = 0;
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

		$star = self::$starDefault;
		$now = self::getToday();
		$date_pattern = "/<strong>{$now['month']}月{$now['day']}日のあなたの運勢<\/strong>/";
		$date_check_ok = false;

		foreach($TOPIC_CONTENTS as $topic_content) {

			$topic_content = mb_convert_encoding($topic_content, "UTF-8", "SJIS");
			$TOPIC_LINES = explode("\n", $topic_content);

			foreach ($TOPIC_LINES as $topic_line) {
				if (count($TOPIC_RESULT) == 12) { break; }

				// star
				if ( preg_match("/<img src='.*?' alt='(.*?座)' border='0' width='98' height='90'>/", $topic_line, $MATCHES)) {
					$star_name = $MATCHES[1];
					$star_num = $star[$star_name];

				}

				if($star_num && !$date_check_ok) {
					$date_check_ok = preg_match($date_pattern, $topic_line);
					continue;
				}
				
				if ($date_check_ok && preg_match("/<td align=left class=txt12>愛情運：<\/td>/", $topic_line)) {
					$love_flg = 1;
				}
				if ($date_check_ok && $love_flg && preg_match("/<img src='http:\/\/www\.img\.happywoman\.jp\/12star\/images\/horoscope_star_0(\d{1})\.gif' align='absmiddle'>/", $topic_line, $MATCHES)) {
					$love = $MATCHES[1];
					$love_num = ($love * 20);
					$love_flg = 0;
//					print $love_num;
					continue;
				}
				if ($date_check_ok && preg_match("/<td align=left class=txt12>金　運：<\/td>/", $topic_line)) {
					$money_flg = 1;
				}

				if ($date_check_ok && $money_flg && preg_match("/<img src='http:\/\/www\.img\.happywoman\.jp\/12star\/images\/horoscope_star_0(\d{1})\.gif' align='absmiddle'>/", $topic_line, $MATCHES)) {
					$money= $MATCHES[1];
					$money_num = ($money * 20);
					$money_flg = 0;
//					print $money_num;
					continue;
				}

				if ($date_check_ok && preg_match("/<td align=left class=txt12>仕事運：<\/td>/", $topic_line)) {
					$work_flg = 1;
				}
				if ($date_check_ok && $work_flg && preg_match("/<img src='http:\/\/www\.img\.happywoman\.jp\/12star\/images\/horoscope_star_0(\d{1})\.gif' align='absmiddle'>/", $topic_line, $MATCHES)) {
					$work = $MATCHES[1];
					$work_num = ($work * 20);
					$work_flg = 0;
//					print $work_num;
					continue;
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
