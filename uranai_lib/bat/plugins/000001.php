<?php
/**
data[star] = rank;
星座（インデックス） => 順位
*/
class Zodiac000001 extends UranaiPlugin {

	function run($URL) {

		$CONTENTS = $this->load($URL);
		$content = $CONTENTS[0];
		$content = mb_convert_encoding($content, "UTF-8", "EUC-JP");		
		$LINES = explode("\n", $content);

		// date check
		$now = self::getToday();
		$star = self::$starDefault;

		$RESULT = array();
		$date_check = false;
		$rank = 0;
		foreach ($LINES as $line) {
			//print $line;
			// Date check
			if(!$date_check) {
				$date_check = preg_match("!<p class=\"txt\">{$now['year']}年{$now['month']}月{$now['day']}日.*</p>!", $line);
				continue;
			}
			// rank
			if ($date_check && !$rank && preg_match("!<td.*><img.*alt=\"(\d{1,2})位\".*></td>!", $line, $MATCHES)) {
				$rank = $MATCHES[1];
				continue;
			}
			// star
			if ($date_check && $rank && preg_match("!<td.*seiza.*alt=\"(.*座)\">!", $line, $MATCHES)) {
				$star_name = $MATCHES[1];
				$star_num = $star[$star_name];
				$RESULT[$star_num] = $rank;
				//reset
				$rank = 0;
			}
		}

		if(!$date_check) {
			print $this->logDateError().PHP_EOL;
		}

		return $RESULT;
	}
}
?>
