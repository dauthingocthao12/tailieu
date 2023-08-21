<?php
/**
 * @author Azet
 * @date 2016-01-05
 * @url http://kids.yahoo.co.jp/fortune/
 */
class Zodiac000039 extends UranaiPlugin {

    /**
     * このファンクションは必要です！
     * 星座（インデックス） => 順位
     * @return array[star] = rank;
     */
    function run($URL) {
        // RESULTの形：12星座分, 1~12
        // $RESULT[<星座番号>] = <ランキング>
        // ランキングも1~12値です
        $RESULT = array();

        $CONTENTS = $this->load($URL);
        // 日付の表示があれば、今日の日付と一致するか確認する！
        // (このプラグインでは、取得時に日付設定をしているので、チェックしていません)
        $now = self::getToday();
        // nowのキーは: year,month,day
        // monthは1~12の値
        // dayは1~31の値
        $date_pattern = "/【{$now['year']}.0?{$now['month']}.0?{$now['day']}】/";

        // サイト毎に性差名の設定
        $star = self::$starDefault;

        for($i=1; $i<=12; ++$i) {

            $content = $CONTENTS[$i];
            // 必要の時に、下記を直して下さい。
            $content = mb_convert_encoding($content, "UTF-8", "EUC-JP");
            $LINES = explode("\n", $content);

            $star_num = 0;
            $rank_num = 0;
            $date_check_ok = false;

            foreach ($LINES AS $line) {
                if (count($RESULT) == 12) { break; }

                // 1 rank
                if (!$rank_num && !$rank_num && preg_match("/第(\d+)位<\/p>/", $line, $MATCHES)) {
                    $rank_num = $MATCHES[1];
                    // print "Rank OK";
                    continue;
                }

                // 2 star
                if (!$star_num && preg_match("/alt=\"(.*座)の運勢\"/", $line, $MATCHES)) {
                    $star_name = $MATCHES[1];
                    $star_num = $star[$star_name];
                    // print "Star OK";
                    continue;
                }

                // 3 date check
                if(!$date_check_ok) {
                    $date_check_ok = preg_match($date_pattern, $line);
                }

                // we all all?
                if($star_num && $rank_num && $date_check_ok) {
                    // RESULTの形：
                    // $RESULT[<星座番号>] = <ランキング>
                    $RESULT[$star_num] = $rank_num;

                    // finished for that star
                    break;
                }
            }
        }

        // date error?
        if(!$date_check_ok) {
            print $this->logDateError().PHP_EOL;
        }

        return $RESULT;
    }
}
