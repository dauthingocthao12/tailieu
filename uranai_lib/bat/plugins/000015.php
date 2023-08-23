<?php
/**
data[star] = rank;
星座（インデックス） => 順位
*/
class Zodiac000015 extends UranaiPlugin {

	function run($URL) {

		$CONTENTS = $this->load($URL);
		$content = $CONTENTS[0];
		//$content = mb_convert_encoding($content, "UTF-8", "UTF-8");
		$LINES = explode("\n", $content);

		$star = self::$starDefault;

		$RESULT = array();
		$rank_num = 0;
		foreach ($LINES AS $key => $line) {
			if (count($RESULT) == 12) { break; }

			// rank
			if (preg_match("/<li class=\"dailyFortuneRank tapLink clearfix rank(\d{1,2})\">/", $line, $MATCHES)) {
				$rank_num = $MATCHES[1];
				continue;
			}
			
			if ($rank_num && preg_match("/<img src=\".*?\/zodiac\/\d{1,2}\.gif\" alt=\" *(.*?)\">/", $line, $MATCHES)) {
				$star_name = $MATCHES[1];
				$star_name = trim($star_name, " \t");
				$star_num = $star[$star_name];
				$RESULT[$star_num] = $rank_num;
				// reset
				$rank_num = 0;
			}
		}

		return $RESULT;
	}
}
