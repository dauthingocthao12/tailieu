<?php
/**
 * @author Azet
 * @date 2016-01-05
 * @url http://kids.yahoo.co.jp/fortune/
 * updated: okabe 2017/06/22
 */
class Zodiac000039 extends UranaiPlugin {

    /**
     * このファンクションは必要です！
     * 星座（インデックス） => 順位
     * @return array[star] = rank;
     */	
    //function run($URL) {			//del okabe 2017/06/22
	function run($CONTENTS) {		// add okabe 2017/06/22 $URL -> $CONTENTS
        // RESULTの形：12星座分, 1~12
        // $RESULT[<星座番号>] = <ランキング>
        // ランキングも1~12値です
        $RESULT = array();

        //$CONTENTS = $this->load($URL);	//del okabe 2017/06/22
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


	// add okabe start 2017/06/22
	function topic_run($TOPIC_CONTENTS) {
		$TOPIC_RESULT = array();

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

			$content = $TOPIC_CONTENTS[$i];
			// 必要の時に、下記を直して下さい。
			$content = mb_convert_encoding($content, "UTF-8", "EUC-JP");
			$LINES = explode("\n", $content);

			$date_check_ok = false;

			$star_num = 0;
			$love_val =-1;
			$money_val =-1;
			$work_val =-1;
			$skip_flg1 = 0;
			$skip_flg2 = 0;
			$skip_flg3 = 0;

			foreach ($LINES AS $line) {
				if (count($TOPIC_RESULT) == 12) { break; }

				// 1 star
				if ($star_num == 0 && preg_match("/alt=\"(.*座)の運勢\"/", $line, $MATCHES)) {
					$star_name = $MATCHES[1];
					$star_num = $star[$star_name];
					continue;
				}

				// 2 date check
				if(!$date_check_ok) {
					$date_check_ok = preg_match($date_pattern, $line);
				}

				//恋愛運
				if($star_num > 0 && $date_check_ok) {
					if ($skip_flg1 == 0) {
						$chk = preg_match("/alt=\"恋愛運\"\sstyle=\"v/", $line);
						if ($chk) {
							$skip_flg1 = 1;
						}
					} else {
						$love_val = mb_substr_count($line, "icon_rateOn.gif");
						$skip_flg1 = 0;
					}
				}

				//金銭運
				if($star_num > 0 && $date_check_ok) {
					if ($skip_flg2 == 0) {
						$chk = preg_match("/alt=\"金銭運\"\sstyle=\"v/", $line);
						if ($chk) {
							$skip_flg2 = 1;
						}
					} else {
						$money_val = mb_substr_count($line, "icon_rateOn.gif");
						$skip_flg2 = 0;
					}
				}

				//仕事運
				if($star_num > 0 && $date_check_ok) {
					if ($skip_flg3 == 0) {
						$chk = preg_match("/alt=\"仕事運\"\sstyle=\"v/", $line);
						if ($chk) {
							$skip_flg3 = 1;
						}
					} else {
						$work_val = mb_substr_count($line, "icon_rateOn.gif");
						$skip_flg3 = 0;
					}
				}

				if ($star_num > 0 && $love_val >= 0 && $money_val >= 0 && $work_val >= 0) {
					$love_num = $love_val * 20;
					$money_num = $money_val * 20;
					$work_num = $work_val * 20;

					$TOPIC_RESULT[$star_num] = array("love"=> $love_num , "money" => $money_num ,"work" => $work_num);
					$star_num = 0;
					$love_val = -1;
					$money_val = -1;
					$work_val = -1;
				}

			}
		}

		return $TOPIC_RESULT;
	}
	// add okabe end 2017/06/22

}
