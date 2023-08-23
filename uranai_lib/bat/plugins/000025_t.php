<?php
/**
 * yumexnet(ユメックスネット) 今日の12星座占い
 * data[star] = rank;
 */
class Zodiac000025 extends UranaiPlugin {
	
	function run($CONTENTS) {
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

	function topic_run($TOPIC_CONTENTS) {
		$TOPIC_RESULT = array();
		// matching data
		$star = self::$starDefault;
		// date check
		$now = self::getToday();

		foreach($TOPIC_CONTENTS as $topic_content) {
			$topic_content = mb_convert_encoding($topic_content, 'UTF-8', 'SJIS');

			// data conversion (from JSON)
			$topic_content = str_replace('var data = ', '', $topic_content);
			$data = json_decode($topic_content);
			$fortune = (array) $data->fortune[0];

			if(
				$now['year'] == $fortune['year'] &&
				$now['month'] == $fortune['month'] &&
				$now['day'] == $fortune['day']
			) {
				// data
					$star_name = $fortune['kana'];
					$star_num = $star[$star_name];
					$love = $fortune['l_rating'];
					$money = $fortune['m_rating'];
					$work = $fortune['w_rating'];
					$love_num = ($love * 20);
					$money_num = ($money * 20);
					$work_num = ($work * 20);
					$TOPIC_RESULT[$star_num] = array("love"=> $love_num , "money" => $money_num ,"work" => $work_num);
			}
			else {
				// date error
				print $this->logDateError().PHP_EOL;
			}
		}
		return $TOPIC_RESULT;
	}
}
