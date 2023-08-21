<?php

/**
 * @author Azet
 * @date 2022-04-06
 * @url https://azby.fmworld.net/misc_top/uranai/mcUranaiData.txt ...
 */
class Zodiac000136 extends UranaiPlugin {

	function run($CONTENTS) {
		$content = mb_convert_encoding($CONTENTS[0], "UTF-8", "SJIS");

        $now = self::getToday();
        $date_str = sprintf("date[1] = \"%04d年%02d月%02d日", $now['year'], $now['month'], $now['day']);

		if (strpos($content, $date_str) === false) {
			return null;
		}

		$RESULT = [];
        foreach (explode("\n", $content) as $line) {
			if (!preg_match('/u_t\["[a-z]+"\]\s*=\s*new\s+Array\("(.+?)"\);/', $line, $matches)) {
				continue;
			}

			$data = preg_split('/",\s*"/', $matches[1]);
			$starName = $data[0];
			if (!isset(self::$starDefault[$starName])) {
				return null; // 不明な星座
			}

			$star = self::$starDefault[$starName];
			$RESULT[$star] = intval($data[5]);
		}

		ksort($RESULT);

		return $RESULT;
	}

	function topic_run($CONTENTS) {
		$content = mb_convert_encoding($CONTENTS[0], "UTF-8", "SJIS");

        $now = self::getToday();
        $date_str = sprintf("date[1] = \"%04d年%02d月%02d日", $now['year'], $now['month'], $now['day']);

		if (strpos($content, $date_str) === false) {
			return null;
		}

		$RESULT = [];
        foreach (explode("\n", $content) as $line) {
			if (!preg_match('/u_t\["[a-z]+"\]\s*=\s*new\s+Array\("(.+?)"\);/', $line, $matches)) {
				continue;
			}

			$data = preg_split('/",\s*"/', $matches[1]);
			$starName = $data[0];
			if (!isset(self::$starDefault[$starName])) {
				return null; // 不明な星座
			}

			$star = self::$starDefault[$starName];
			$RESULT[$star] = [
				'love' => intval($data[7]) * 20,
				'work' => intval($data[8]) * 20,
				'money' => intval($data[9]) * 20,
			];
		}

		ksort($RESULT);

		return $RESULT;
	}

}
