<?php
/**
data[star] = rank;
星座（インデックス） => 順位
*/
class Zodiac000003 extends UranaiPlugin {

	function run($CONTENTS) {

	//	foreach ($URL as $key => $url) {
	//		$url = str_replace("(md)", date("md"), $url);
	//		$url = str_replace("(ymd)", date("ymd"), $url);
	//		$URL[$key] = $url;
	//	}

	//	$CONTENTS = $this->load($URL);

		$RESULT = array();
		foreach ($CONTENTS AS $key => $content) {
			$content = mb_convert_encoding($content, "UTF-8", "SJIS");
			$LINES = explode("\n", $content);
			$chk_flg = 0;
			foreach ($LINES as $line) {
				if (preg_match("/<table border=\"0\" cellspacing=\"0\" cellpadding=\"0\" class=\"obi-brown\">/", $line, $MATCHES)) {
					$chk_flg = 1;
					continue;
				}
				if ($chk_flg == 1 && preg_match("/(\d{1,2})位<\/h2><\/td>/", $line, $MATCHES)) {
					$RESULT[$key] = $MATCHES[1];
					break;
				}
			}
		}

		return $RESULT;
	}

	function topic_run($TOPIC_CONTENTS) {

	//	foreach ($URL as $key => $url) {
	//		$url = str_replace("(md)", date("md"), $url);
	//		$url = str_replace("(ymd)", date("ymd"), $url);
	//		$URL[$key] = $url;
	//	}

	//	$CONTENTS = $this->load($URL);

		$TOPIC_RESULT = array();
		foreach ($TOPIC_CONTENTS AS $key => $topic_content) {
			$content = mb_convert_encoding($topic_content, "UTF-8", "SJIS");
			$TOPIC_LINES = explode("\n", $topic_content);
			$chk_flg = 0;
			foreach ($TOPIC_LINES as $topic_line) {
				if (preg_match("/<table border=\"0\" cellspacing=\"0\" cellpadding=\"0\" class=\"obi-brown\">/", $topic_line, $MATCHES)) {
					$chk_flg = 1;
					continue;
				}
				if ($chk_flg == 1 && preg_match("/<img src=\"\/images\/ico_love_0(\d{1})\.gif\" alt=\"\">/", $topic_line, $MATCHES)) {
					$love=$MATCHES[1];
					$love_num = ($love * 20);
				}
				if ($chk_flg == 1 && preg_match("/<img src=\"\/images\/ico_mony_0(\d{1})\.gif\" width=\"99\" height=\"21\" alt=\"\">/", $topic_line, $MATCHES)) {
					$money=$MATCHES[1];
					$money_num = ($money * 20);
				}
				if ($chk_flg == 1 && preg_match("/<img src=\"\/images\/ico_job_0(\d{1})\.gif\" width=\"99\" height=\"21\" alt=\"\">/", $topic_line, $MATCHES)) {
					$work=$MATCHES[1];
					$work_num = ($work * 20);
				}
			}
			$TOPIC_RESULT[$key] = array("love"=> $love_num , "money" =>$money_num ,"work" => $work_num );
		}

		return $TOPIC_RESULT;
	}

	function lucky_run($TOPIC_CONTENTS){
		
		// date check
		$now = self::getToday();
		$star = self::$starDefault;
		$LUCKY_RESULT = array();
		foreach ($TOPIC_CONTENTS AS $key => $topic_content) {
			$topic_content = mb_convert_encoding($topic_content, "UTF-8", "SJIS");
			$TOPIC_LINES = explode("\n", $topic_content);
			$chk_flg = 0;
			foreach ($TOPIC_LINES as $topic_line) {
				if (preg_match("/<table border=\"0\" cellspacing=\"0\" cellpadding=\"0\" class=\"obi-brown\">/", $topic_line, $MATCHES)) {
					$chk_flg = 1;
					continue;
				}
				if ($chk_flg == 1 && preg_match("/<td width=\"443\"><h2>{$now['year']}年".date("m")."月".date("d")."日(.*座)/", $topic_line, $MATCHES)) {
					$star_name=trim($MATCHES[1]);
					$star_num = $star[$star_name];
				}
				if ($chk_flg == 1 && preg_match("/<td class=\"hako4\"><p>(.*?)<\/p><\/td>/", $topic_line, $MATCHES)) {
					$lucky_item=$MATCHES[1];
				}
				if ($chk_flg == 1 && preg_match("/<td class=\"hako2\"><p>(.*?)<\/p><\/td>/", $topic_line, $MATCHES)) {
					$lucky_color=$MATCHES[1];
				}
			}
			$LUCKY_RESULT[$star_num] = array("lucky_item"=> $lucky_item , "lucky_color" => $lucky_color);

			if($chk_flg==0){
				print $this->logDateError().PHP_EOL;
			}
		}
		return $LUCKY_RESULT;
	}
}
