<?php

/**
 * @author Azet
 * @date 2022-04-04
 * @url https://beauty.biglobe.ne.jp/fortune/
 */
class Zodiac000131 extends UranaiPlugin {

	function run($CONTENTS) {
		$lines = explode("\n", $CONTENTS[0]);

		$date_check = false;
        $now = self::getToday();
        $date_str = sprintf("%02d月%02d日", $now['month'], $now['day']);

		$current_star = -1;
		$scores = [];
		foreach ($lines as $line) {
			if (strpos($line, $date_str) !== false) {
				$date_check = true;
			} else if (preg_match('/<h2 class="todays_horo_name">(.+?)<\/h2>/', $line, $title_matches)) {
				$current_star = self::$starDefault[$title_matches[1]];
			} else if ($current_star !== -1 
				&& preg_match('/<span class="todays_score">(\d+)点<\/span>/', $line, $score_matches)) {
				$scores[$current_star] = intval($score_matches[1]);
			}
		}

		if (!$date_check) {
			return null;
		}

		arsort($scores);

		// 同率1位を考慮したソート
        $RESULT = [];
        $rank = 1;
        $ptr = 1;
        $prev_cnt = -1;
        foreach ($scores as $star => $cnt) {
            if ($cnt !== $prev_cnt) {
                $rank = $ptr;
            }
            $RESULT[$star] = $rank;
            $prev_cnt = $cnt;
            $ptr++;
        }

        ksort($RESULT);

		return $RESULT;
	}

	function topic_run($CONTENTS) {
		$lines = explode("\n", $CONTENTS[0]);

		$date_check = false;
        $now = self::getToday();
        $date_str = sprintf("%02d月%02d日", $now['month'], $now['day']);

		$current_star = -1;
		$RESULT = [];
		foreach ($lines as $line) {
			if (strpos($line, $date_str) !== false) {
				$date_check = true;
			} else if (preg_match('/<h2 class="todays_horo_name">(.+?)<\/h2>/', $line, $title_matches)) {
				$current_star = self::$starDefault[$title_matches[1]];
				$RESULT[$current_star] = [];
			} else if ($current_star !== -1) {
				if (($love_count = preg_match_all('/heart-rating-pink/', $line)) || preg_match('/heart-rating-gray/', $line)) {
					// 恋愛運
					$love = $love_count * 20;
					if ($love >= 0 && $love <= 100) {
						$RESULT[$current_star]['love'] = $love;
					}
				} else if (preg_match('/<div class="money-rating-front" style="width: (\d+)%;">/', $line, $money_matches)) {
					// 金運
					$money = intval($money_matches[1]);
					if ($money >= 0 && $money <= 100) {
						$RESULT[$current_star]['money'] = $money;
					}
				}
			}
		}

		if (!$date_check) {
			return null;
		}

		ksort($RESULT);

		return $RESULT;
	}

}
