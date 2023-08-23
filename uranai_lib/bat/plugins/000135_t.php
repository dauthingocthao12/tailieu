<?php

/**
 * @author Azet
 * @date 2022-04-04
 * @url https://8761234.jp/daily/aquarius ...
 */
class Zodiac000135 extends UranaiPlugin {

	function run($CONTENTS) {
        $now = self::getToday();
        $date_str = sprintf("%d/%dの運勢", $now['month'], $now['day']);

		$RESULT = [];
        foreach ($CONTENTS as $star => $content) {
			if (strpos($content, $date_str) === false) {
				return null;
			}

			foreach (explode("\n", $content) as $line) {
				if (preg_match('/ranking-(\d+)\.jpg/', $line, $rank_matches)) {
					$RESULT[$star] = intval($rank_matches[1]);
					break;
				}
			}

			if (!isset($RESULT[$star])) {
				return null;
			}
		}

        ksort($RESULT);

		return $RESULT;
	}

	private static $topics = [
		'恋愛運' => 'love',
		'仕事・勉強運' => 'work',
		'対人運' => 'interpersonal',
	];

	function topic_run($CONTENTS) {
        $now = self::getToday();
        $date_str = sprintf("%d/%dの運勢", $now['month'], $now['day']);

		$RESULT = [];
        foreach ($CONTENTS as $star => $content) {
			if (strpos($content, $date_str) === false) {
				return null;
			}

			$topic_data = [];
			$current_topic = null;
			foreach (explode("\n", $content) as $line) {
				if (preg_match("/>(.+?運)</", $line, $topic_matches)) {
					if (!isset(self::$topics[$topic_matches[1]])) {
						continue;
					}
					$current_topic = self::$topics[$topic_matches[1]];
				} else if ($current_topic !== null
					&& preg_match("/<td.+?<span class=\"rate rate(\d+)\">/", $line, $rate_matches)) {
					$topic_data[$current_topic] = intval($rate_matches[1]) * 2;
					$current_topic = null;
				}
			}

			if (count($topic_data) !== count(self::$topics)) {
				return null; // 全てのトピックを取得できていない
			}

			$RESULT[$star] = $topic_data;
		}

		return $RESULT;
	}

}
