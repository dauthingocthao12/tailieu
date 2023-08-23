<?php

/**
 * @author Azet
 * @date 2022-03-31
 * @url https://storyweb.jp/fortune/daily_detail/?fi=11 ...
 */
class Zodiac000094 extends UranaiPlugin {

    function run($CONTENTS) {
        $star_counts = [];
        $now = self::getToday();
        $date_str = sprintf("%04d年%02d月%02d日", $now['year'], $now['month'], $now['day']);

        foreach ($CONTENTS as $star => $content) {
            if (strpos($content, $date_str) === false) {
                continue;
            }
            $star_counts[$star] = preg_match_all('/symbol-defs.svg#icon-.+?-blue/', $content);
        }

        arsort($star_counts);

        $RESULT = [];
        $rank = 1;
        $ptr = 1;
        $prev_cnt = -1;
        foreach ($star_counts as $star => $cnt) {
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

	function topic_run($TOPIC_CONTENTS) {
        $TOPICS = [
            'love' => '恋愛運',
            'money' => '金運',
            'work' => '仕事運',
        ];

        $now = self::getToday();
        $date_str = sprintf("%04d年%02d月%02d日", $now['year'], $now['month'], $now['day']);

        $TOPIC_RESULT = [];
        foreach ($TOPIC_CONTENTS as $star => $content) {
            if (strpos($content, $date_str) === false) {
                continue;
            }

            $TOPIC_RESULT[$star] = [];
            foreach ($TOPICS as $data_type => $topic_text) {
                $start = strpos($content, '<dt>' . $topic_text . '</dt>');
                $end = strpos($content, '</div>', $start);
                $target = substr($content, $start, $end - $start);
                $TOPIC_RESULT[$star][$data_type] = preg_match_all('/symbol-defs.svg#icon-.+?-blue/', $target) * 20;
            }
        }

        return $TOPIC_RESULT;
	}

}
