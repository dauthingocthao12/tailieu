<?php
/**
data[star] = rank;
星座（インデックス） => 順位
*/
class Zodiac000011 extends UranaiPlugin {

	function run($URL) {

		//foreach ($URL as $key => $url) {
		//	$url = str_replace("(md)", date("md"), $url);
		//	$url = str_replace("(ymd)", date("ymd"), $url);
		//	$URL[$key] = $url;
		//}

		$CONTENTS = $this->load($URL);
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
}
