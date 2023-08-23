<?php

/**
 * @author Azet
 * @date 2022-04-06
 * @url https://www.nbc-nagasaki.co.jp/horoscope/
 */
class Zodiac000139 extends UranaiPlugin {

	function run($CONTENTS) {
		$content = $CONTENTS[0];

        $now = self::getToday();
        $date_str = sprintf("%04d-%02d-%02dのランキング", $now['year'], $now['month'], $now['day']);

		if (strpos($content, $date_str) === false) {
			return null;
		}

		foreach (explode("\n", $content) as $line) {
			if (preg_match('/第(\d+)位\s*(.+?座)/', $line, $rank_matches)) {
				$rank = intval($rank_matches[1]);
				$star_name = $rank_matches[2];
				if (!isset(self::$starKanji[$star_name])) {
					return null;
				}
				$star = self::$starKanji[$star_name];
	
				$RESULT[$star] = $rank;
			}
		}

        ksort($RESULT);

		return $RESULT;
	}

	private static $topics = [
		'恋愛運' => 'love',
		'仕事運' => 'work',
		'金銭運' => 'money',
	];

	function topic_run($CONTENTS) {
		$content = $CONTENTS[0];

        $now = self::getToday();
        $date_str = sprintf("%04d-%02d-%02dのランキング", $now['year'], $now['month'], $now['day']);

		if (strpos($content, $date_str) === false) {
			return null;
		}

		$current_star = null;
		foreach (explode("\n", $content) as $line) {
			if (preg_match('/第(\d+)位\s*(.+?座)/', $line, $rank_matches)) {
				$star_name = $rank_matches[2];
				if (!isset(self::$starKanji[$star_name])) {
					return null;
				}
				$current_star = self::$starKanji[$star_name];
				$RESULT[$current_star] = [];
			} else if (preg_match('/.+>(.+?運)</', $line, $topic_matches)) {
				if (!isset(self::$topics[$topic_matches[1]])) {
					continue;
				}
				$data_type = self::$topics[$topic_matches[1]];
				$RESULT[$current_star][$data_type] = substr_count($line, '<i>') * 20;
			}
		}

        ksort($RESULT);

		$total_data_count = array_reduce($RESULT, function ($total, $r) {
			return $total + count($r);
		}, 0);
		if ($total_data_count !== 12 * count(self::$topics)) {
			// 全て取得できていない
			return null;
		}

		return $RESULT;
	}

}
