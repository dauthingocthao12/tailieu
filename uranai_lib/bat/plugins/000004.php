<?php
/**
data[star] = rank;
星座（インデックス） => 順位
*/
class Zodiac000004 extends UranaiPlugin {

	function run($URL) {

		$CONTENTS = $this->load($URL);

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
}
