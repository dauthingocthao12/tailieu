<?php

/**
 * @author Azet
 * @date 2022-04-06
 * @url https://unkoi.com/fortune-luck/dailyranking
 */
class Zodiac000140 extends UranaiPlugin {

	function run($CONTENTS) {
		$content = $CONTENTS[0];

        $now = self::getToday();
        $date_str = sprintf("今日の運勢ランキング %04d年%02d月%02d日", $now['year'], $now['month'], $now['day']);

		if (strpos($content, $date_str) === false) {
			return null;
		}

		foreach (explode("\n", $content) as $line) {
			if (preg_match_all('/>(\d+)位\s*(.+?座)</', $line, $rank_matches)) {
				foreach ($rank_matches[2] as $idx => $starName) {
					if (!isset(self::$starDefault[$starName])) {
						return null; // 不明な星座
					}
					$star = self::$starDefault[$starName];
					$rank = intval($rank_matches[1][$idx]);
					$RESULT[$star] = $rank;
				}

				if (count($RESULT) === 12) {
					break;
				}
			}
		}

        ksort($RESULT);

		return $RESULT;
	}

	private static $topics = ['love', 'money', 'work'];

	function topic_run($CONTENTS) {
        $now = self::getToday();
        $date_str = sprintf("運勢 %d月%d日", $now['month'], $now['day']);

		$RESULT = [];
        foreach ($CONTENTS as $star => $content) {
			if (strpos($content, $date_str) === false) {
				return null;
			}

			$topic_pattern = "/icon_day_(" . implode("|", self::$topics) . ")_(\d+)/";
			$topic_count = count(self::$topics);

			$topic_data = [];
			foreach (explode("\n", $content) as $line) {
				if (preg_match_all($topic_pattern, $line, $topic_matches)) {
					foreach ($topic_matches[1] as $idx => $data_type) {
						$topic_data[$data_type] = intval($topic_matches[2][$idx]) * 20;
					}
				}

				if (count($topic_data) === $topic_count) {
					break;
				}
			}

			if (count($topic_data) !== $topic_count) {
				return null; // 全てのトピックを取得できていない
			}

			$RESULT[$star] = $topic_data;
		}

		return $RESULT;
	}

}
