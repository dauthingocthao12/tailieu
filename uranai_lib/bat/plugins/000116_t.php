<?php

class Zodiac000116 extends UranaiPlugin {

    /**
     * 1Pで取得仕様
     */
	function run($CONTENTS) {
        $content = $CONTENTS[0];
        $content = mb_convert_encoding($content, "UTF-8", "SJIS");
		$LINES = explode("\n", $content);
        $now = self::getToday();
		$star = self::$starDefault;

        $date_pattern = "/◆今日の星占いランキング\({$now['year']}\/0?{$now['month']}\/0?{$now['day']}\)/";

		$RESULT = array();
		$rank_num = 0;
		$date_check_ok = false;
		foreach ($LINES AS $key => $line) {
			if (count($RESULT) == 12) { break; }

			if(!$date_check_ok) {
				$date_check_ok = preg_match($date_pattern, $line);
			}
            $l_dtl = [];
            if($date_check_ok){
                $l_dtl = explode(">", $line);
            }
            if(!empty($l_dtl)){
                $rank_num = 0;
                foreach($l_dtl AS $l){
                    if(preg_match("/(.*)位　<img/", $l, $MATCHES)){
                        $rank_num = mb_convert_kana($MATCHES[1], "as");
                        $rank_num = trim($rank_num);
                    }

                    if($rank_num && preg_match("/(.*座)<\/b/", $l, $MATCHES)){
                        $star_name = $MATCHES[1];
                        $star_num = $star[$star_name];
                        $RESULT[$star_num] = $rank_num;
                        $rank_num = 0;
                    }

                    if(count($RESULT[$star_num]) == 12){
                        break;
                    }
                }

            }
            if(count($RESULT[$star_num]) == 12){
                break;
            }
		}

		// date error?
		if(!$date_check_ok) {
			print $this->logDateError().PHP_EOL;
		}

		return $RESULT;
	}


    /**
     * 1Pで取得仕様
     */
	function topic_run($CONTENTS) {
		$content = $CONTENTS[0];
        $content = mb_convert_encoding($content, "UTF-8", "SJIS");
		$LINES = explode("\n", $content);
        $now = self::getToday();
		$star = self::$starDefault;

        $date_pattern = "/◆今日の星占いランキング\({$now['year']}\/0?{$now['month']}\/0?{$now['day']}\)/";

		$RESULT = array();
		$date_check_ok = false;
		foreach ($LINES AS $key => $line) {
			if (count($RESULT) == 12) { break; }

			if(!$date_check_ok) {
				$date_check_ok = preg_match($date_pattern, $line);
			}
            $l_dtl = [];
            if($date_check_ok){
                $l_dtl = explode(">", $line);
            }
            if(!empty($l_dtl)){
                $money_count = 0;
                $work_count = 0;
                $love_count = 0;
                $star_num = 0;
                foreach($l_dtl AS $l){
                    if(preg_match("/(.*座)<\/b/", $l, $MATCHES)){
                        if($star_num){
                            $RESULT[$star_num]['money'] = $money_count * 20;
                            $RESULT[$star_num]['work'] = $work_count * 20;
                            $RESULT[$star_num]['love'] = $love_count * 20;
                            $money_count = 0;
                            $work_count = 0;
                            $love_count = 0;
                        }
                        $star_name = $MATCHES[1];
                        $star_num = $star[$star_name];
                    }

                    if($star_num){
                        if(preg_match("/images\/money.gif/", $l)){
                            $money_count++;
                        } elseif (preg_match("/images\/work.gif/", $l)) {
                            $work_count++;
                        } elseif (preg_match("/images\/love.gif/", $l)) {
                            $love_count++;
                        }
                    }

                }
                $RESULT[$star_num]['money'] = $money_count * 20;
                $RESULT[$star_num]['work'] = $work_count * 20;
                $RESULT[$star_num]['love'] = $love_count * 20;

            }
            if(count($RESULT[$star_num]) == 12){
                break;
            }
		}

		// date error?
		if(!$date_check_ok) {
			print $this->logDateError().PHP_EOL;
		}
        return $RESULT;
	}
}
