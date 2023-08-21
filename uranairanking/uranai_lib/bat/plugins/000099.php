<?php
/**
data[star] = rank;
星座（インデックス） => 順位
*/
class Zodiac000099 extends UranaiPlugin {

	function run($URL) {
		$CONTENTS = $this->load($URL);
		$RESULT = array();
		// date pattern
		$now = self::getToday();
		$star = self::$starKanji;
		$date_pattern = "/otsuge\">{$now['month']}月{$now['day']}日\sワラシちゃんのお告げ/";

		foreach($CONTENTS as $star_num => $content) {
			//$content = mb_convert_encoding($content, "UTF-8", "UTF-8");
			$LINES = explode("\n", $content);
			$date_check_ok = false;
			$star_name="";
			$star_rank="";

			foreach ($LINES AS $line) {

				// date check
				if(!$date_check_ok) {
					$date_check_ok = preg_match($date_pattern, $line);
				}
				
				if($date_check_ok && preg_match("/name tx-bl\">(.*座)<\/span/", $line,$m)){
					$star_name = $m[1];
				}

				
				if($date_check_ok && preg_match("/rank tx-bl\">No.([0-9]{0,2})<\//", $line,$m)) {
					$star_rank = $m[1];
				}
				
				if($star_name && $star_rank){
					$star_num = $star[$star_name];
					$RESULT[$star_num] = $star_rank;
				}

			}
			// date error?
			if(!$date_check_ok) {
				print $this->logDateError().PHP_EOL;
			}
		}
		return $RESULT;
	}
}
