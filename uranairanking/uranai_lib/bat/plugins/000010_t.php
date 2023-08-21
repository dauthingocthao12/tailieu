<?php
/**
data[star] = rank;
星座（インデックス） => 順位
*/
class Zodiac000010 extends UranaiPlugin {

	function run($CONTENTS) {

		$content = $CONTENTS[0];
		//$content = mb_convert_encoding($content, "UTF-8", "UTF-8");
		$LINES = explode("\n", $content);

		$star = self::$starDefault;
		$now = self::getToday();

		$year = date('Y');
		$month = date('m');
		$day = date('d');
		$date_pattern = "/<h3 class=\"center_sub_title\">{$year}年{$month}月{$day}日の運勢ランキング<\/h3>/i";

		$RESULT = array();
		$rank_num = 0;
		$date_check_ok = false;
		foreach ($LINES AS $line) {
			if (count($RESULT) == 12) { break; }

			// date check
			if(!$date_check_ok) {
				$date_check_ok = preg_match($date_pattern, $line);
				continue;
			}

			// rank
			if ($date_check_ok && !$rank_num && preg_match("/総合(\d{1,2})位<\/SPAN>/", $line, $MATCHES)) {
				$rank_num = $MATCHES[1];
				continue;
			}
			
			if ($rank_num && preg_match("/<BIG><A href='.*?'>(.*?)<\/A><\/BIG><br>/", $line, $MATCHES)) {
				$star_name = $MATCHES[1];
				$star_num = $star[$star_name];
				$RESULT[$star_num] = $rank_num;
				// reset
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

		// date pattern
		$star = self::$starDefault;
		$now = self::getToday();
		$year = date('Y');
		$month = date('m');
		$day = date('d');
		$date_pattern = "/<H3 class=center_sub_title>.*座 {$year}年{$month}月{$day}日の運勢<\/H3>/i";

		foreach($TOPIC_CONTENTS as $star_num => $topic_content) {
			//$content = mb_convert_encoding($content, "UTF-8", "UTF-8");
			$TOPIC_LINES = explode("\n", $topic_content);
			$date_check_ok = false;
			$love = 0;
			$money = 0;
			$work = 0;

			foreach ($TOPIC_LINES AS $topic_line) {

				if(!$date_check_ok) {
					$date_check_ok = preg_match($date_pattern, $topic_line);
				}

				//love
				if ($date_check_ok && preg_match("/<SPAN class=uranai_col_title>恋愛運<\/SPAN>(.*?)<\/SPAN>/i", $topic_line, $MATCHES)){
					$love_content = $MATCHES[1];
					$love = substr_count($love_content , '<IMG SRC="/horoscope/icon/heart1.png" class="star_icon" alt="">');
					$love_num = ($love * 20);
				}
				//work
				if ($date_check_ok && preg_match("/<SPAN class=uranai_col_title>仕事運<\/SPAN>(.*?)<\/SPAN>/i", $topic_line, $MATCHES)){
					$work_content = $MATCHES[1];
					$work = substr_count($work_content , '<IMG SRC="/horoscope/icon/work1.png" class="star_icon" alt="">');
					$work_num = ($work * 20);
				}
				//money
				if ($date_check_ok && preg_match("/<SPAN class=uranai_col_title>金運<\/SPAN>(.*?)<\/SPAN>/i", $topic_line, $MATCHES)){
					$money_content = $MATCHES[1];
					$money = substr_count($money_content , '<IMG SRC="/horoscope/icon/yen1.png" class="star_icon" alt="">');
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
