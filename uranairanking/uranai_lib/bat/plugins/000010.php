<?php
/**
data[star] = rank;
星座（インデックス） => 順位
*/
class Zodiac000010 extends UranaiPlugin {

	function run($URL) {

		$CONTENTS = $this->load($URL);
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
}
