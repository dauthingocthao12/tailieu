<?php

/**
 * @author Azet
 * @date 2022-03-30
 * @url https://uranai.d-square.co.jp/12seiza_today_mizugame.html ...
 */
class Zodiac000109 extends UranaiPlugin {

    function run($CONTENTS) {
        $RESULT = [];
        $now = self::getToday();
        $date_str = "{$now['year']}年{$now['month']}月{$now['day']}日";

        foreach ($CONTENTS as $star => $content) {
            if (
                strpos($content, $date_str) !== false
                && preg_match('/"images\/pc_12seiza_.+?_rank(\d+)\.png"/', $content, $matches)
            ) {
                $RESULT[$star] = intval($matches[1]);
            }
        }

        return $RESULT;
    }

}
