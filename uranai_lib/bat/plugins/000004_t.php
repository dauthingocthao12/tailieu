<?php
/**
data[star] = rank;
星座（インデックス） => 順位
*/
class Zodiac000004 extends UranaiPlugin {

	function run($CONTENTS) {


		// date check values
		$date_check_year = date('Y');
		$date_check_month = date('n');
		$date_check_day = date('j');

		$RESULT = array();
		foreach ($CONTENTS AS $key => $content) {
			// each star page
			//$content = mb_convert_encoding($content, "UTF-8", "SJIS");
			$LINES = explode("\n", $content);
			$rank_num = 0;
			$date_check_ok = false;
			foreach ($LINES as $line) {
				// rank
				if (preg_match("/<div class=\"ranking\"><p>(\d{1,2})<\/p><\/div>/", $line, $MATCHES)) {
					$rank_num = $MATCHES[1];
				}
				// date check
				if($rank_num && preg_match("!<h3 class=\"mb10\">あなたの今日の運勢（{$date_check_year}年{$date_check_month}月{$date_check_day}日）</h3>!", $line)) {
					$RESULT[$key] = $rank_num;
					$date_check_ok = true;
					break;
				}
			}

			// data ok?
			if(!$date_check_ok) {
				print $this->logDateError().PHP_EOL;
			}
		}

		return $RESULT;
	}
	function topic_run($TOPIC_CONTENTS) {


		// date check values
		$date_check_year = date('Y');
		$date_check_month = date('n');
		$date_check_day = date('j');

		$TOPIC_RESULT = array();
		foreach ($TOPIC_CONTENTS AS $key => $topic_content) {
			// each star page
			//$content = mb_convert_encoding($content, "UTF-8", "SJIS");
			$TOPIC_LINES = explode("\n", $topic_content);
			$rank_num = 0;
			$date_check_ok = false;
			foreach ($TOPIC_LINES as $topic_line) {
				// date check
				if(preg_match("!<h3 class=\"mb10\">あなたの今日の運勢（{$date_check_year}年{$date_check_month}月{$date_check_day}日）</h3>!", $topic_line)) {
					$date_check_ok = true;
				}
				//love
				if ($date_check_ok && preg_match("/<h4 class=\"point(\d{1})\"><img src=\"images\/12star_renai_tt\.png\" width=\"54\" height=\"19\" alt=\"恋愛運\" \/>/", $topic_line, $MATCHES)) {
					$love = $MATCHES[1];
					$love_num = ($love * 20);
				}
				//money
				if ($date_check_ok && preg_match("/<h4 class=\"point(\d{1})\"><img src=\"images\/12star_kin_tt\.png\" width=\"54\" height=\"19\" alt=\"金運\" \/>/", $topic_line, $MATCHES)) {
					$money=$MATCHES[1];
					$money_num = ($money * 20);
				}
				//work
				if ($date_check_ok && preg_match("/<h4 class=\"point(\d{1})\"><img src=\"images\/12star_shigoto_tt\.png\" width=\"54\" height=\"19\" alt=\"仕事運\" \/>/", $topic_line, $MATCHES)) {
					$work=$MATCHES[1];
					$work_num = ($work * 20);
					$TOPIC_RESULT[$key] = array("love"=> $love_num , "money" =>$money_num ,"work" => $work_num );				}

			}

			// data ok?
			if(!$date_check_ok) {
				print $this->logDateError().PHP_EOL;
			}
		}

		return $TOPIC_RESULT;
	}
}
