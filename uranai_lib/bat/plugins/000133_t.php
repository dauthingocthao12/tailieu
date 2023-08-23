<?php

/**
 * @author Azet
 * @date 2022-04-04
 * @url https://fortune.line.me/charmmy/horoscope
 */
class Zodiac000133 extends UranaiPlugin {

	function run($CONTENTS) {
		$content = $CONTENTS[0];

        $now = self::getToday();
        $date_str = sprintf("%02d/%02dのランキング", $now['month'], $now['day']);

		if (strpos($content, $date_str) === false) {
			return null;
		}

		if (!preg_match_all('/<div class=".+?">No\.(?:<\!-- -->)?(\d+)<\/div>/', $content, $rank_matches)) {
			return null;
		}

		// if (!preg_match_all('/<div class="cq_cat cq_cbb c9p_c9t">(.+?座)<\/div>/', $content, $name_matches)) {
		if (!preg_match_all('/<div class="cb_c6 cb_ccc caao_caas">(.+?座)<\/div>/', $content, $name_matches)) {
			return null;
		}

		if (count($rank_matches[1]) !== 12 || count($name_matches[1]) !== 12) {
			return null;
		}

		$RESULT = [];
		for ($i = 0; $i < 12; $i++) {
			$name = $name_matches[1][$i];
			if (!isset(self::$starDefault[$name])) {
				return null;
			}
			$star = self::$starDefault[$name];
			$RESULT[$star] = intval($rank_matches[1][$i]);
		}

        ksort($RESULT);

		return $RESULT;
	}

	private static $topics = [
		'恋愛運' => 'love',
		'金運' => 'money',
	];

	private static $starClasses = [
		'ctv_cvv' => 1,
		'ctv_cvx' => 2,
		'ctv_cvz' => 3,
		'ctv_cv1' => 4,
		'ctv_cv3' => 5,
	];

	function topic_run($CONTENTS) {
        $now = self::getToday();
        $date_str = sprintf("%04d.%02d.%02d", $now['year'], $now['month'], $now['day']);

		$RESULT = [];
        foreach ($CONTENTS as $star => $content) {
			if (strpos($content, $date_str) === false) {
				return null;
			}

			foreach (self::$topics as $topicName => $dataType) {
				if (!preg_match("/{$topicName}<\/div><div class=\"(\w+)/", $content, $topic_matches)) {
					return null;
				} else if (!isset(self::$starClasses[$topic_matches[1]])) {
					return null;
				}

				$RESULT[$star][$dataType] = self::$starClasses[$topic_matches[1]] * 20;
			}
		}

		return $RESULT;
	}

}
