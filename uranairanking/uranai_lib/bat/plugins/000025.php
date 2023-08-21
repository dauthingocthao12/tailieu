<?php
/**
 * yumexnet(ユメックスネット) 今日の12星座占い
 * data[star] = rank;
 */
class Zodiac000025 extends UranaiPlugin {
	
	function run($URL) {
		$CONTENTS = array();

		$CONTENTS = $this->load($URL);
		$content = mb_convert_encoding($CONTENTS[0], 'UTF-8', 'SJIS');

		// data conversion (from JSON)
		$content = str_replace('var data = ', '', $content);
		$data = json_decode($content);
		$fortune = (array) $data->fortune[0];

		$RESULTS = array();
		// matching data
		$star = self::$starDefault;

		// date check
		$now = self::getToday();
		if(
			$now['year'] == $fortune['year'] &&
			$now['month'] == $fortune['month'] &&
			$now['day'] == $fortune['day']
		) {
			// data
			for($rank=1; $rank<=12; ++$rank) {
				$key = "rank{$rank}name";
				$star_name = $fortune[$key];
				$star_num = $star[$star_name];
				$RESULTS[$star_num] = $rank;
			}
		}
		else {
			// date error
			print $this->logDateError().PHP_EOL;
		}

		return $RESULTS;
	}
}
