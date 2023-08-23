<?php
/**
 * @author Azet
 * @date 2016-03-08
 * @url http://www.japan-horoscope.com/horoscopes/today/aries.htm ...
 * updated: okabe 2017/06/20
 */
class Zodiac000059 extends UranaiPlugin {

	/**
	 * このファンクションは必要です！
	 * 星座（インデックス） => 順位
	 * @return array[star] = rank;
	 */
	//function run($URL) {		//del okabe 2017/06/20
	function run($CONTENTS) {	// add okabe 2017/06/20 $URL -> $CONTENTS
		// RESULTの形：12星座分, 1~12
		// $RESULT[<星座番号>] = <ランキング>
		// ランキングも1~12値です
		$RESULT = array();
		$RESULT2 = array();

		//$CONTENTS = $this->load($URL);	//del okabe 2017/06/20
		// 日付の表示があれば、今日の日付と一致するか確認する！
		// (このプラグインでは、取得時に日付設定をしているので、チェックしていません)
		$now = self::getToday();
		// nowのキーは: year,month,day
		// monthは1~12の値
		// dayは1~31の値
		/*$month_cnt = $now['month'];
		$month_cnt = strlen($month_cnt);
		if($month_cnt == 1){
			$date_pattern = "/0{$now['month']}{$now['day']}.jpg/";
		}else{
			$date_pattern = "/{$now['month']}{$now['day']}.jpg/";
		}*/
		$month = sprintf("%02d",$now['month']);
        $day = sprintf("%02d",$now['day']);
        $date_pattern = "/{$month}{$day}.jpg/";
		// サイト毎に星座名の設定
		//$star = self::$starDefault;

		//星座ごとにループする
		for ($i = 1; $i < 13; $i++) {
			// このプラグインでは、1~12の星座ごとのURLを使用する
			$content = $CONTENTS[$i];
			//$content = mb_convert_encoding($content, "UTF-8","EUC-JP");

			$date_check_ok = false;
			if(!$date_check_ok) {
				$date_check_ok = preg_match($date_pattern, $content);
			}
			// 星の個数を調べる
			if ($date_check_ok && preg_match_all('/<div class="horo_content_sto">(★+)</u',$content,$MATCHES)) {
				$cnt=0;
                foreach ($MATCHES[1] as $str_star) {
                    $cnt += mb_substr_count($str_star, "★"); // 星の数を出力する;
                }
				$star_num = $i;
				// RESULTの形：
				// $RESULT[<星座番号>] = 点数
				$RESULT[$star_num] = $cnt;
			}

		}
// $RESULT[<星座番号>] = 点数
//print_r($RESULT);

		//点数から順位付け
		// $RESULT2[<星座番号>] = 順位
		if (count($RESULT) == 12) {
			arsort($RESULT);
			$j = 1;
			$point_prev = -1;
			foreach($RESULT AS $k => $v) {
				if ($point_prev < 0) {
					$point_prev = $v;
					$RESULT2[$k] = 1;
				} else {
					if ($v == $point_prev) {	//１つ前のデータと同点の場合
						$RESULT2[$k] = $j;
					} else {
						$j = count($RESULT2) + 1;	//新たな順位値
						$point_prev = $v;
						$RESULT2[$k] = $j;
					}
				}
			}
		}
		return $RESULT2;
	}

	function topic_run($TOPIC_CONTENTS) {
        $TOPIC_RESULT = array();

        $now = self::getToday();

        $month = sprintf("%02d",$now['month']);
        $day = sprintf("%02d",$now['day']);
        $date_pattern = "/{$month}{$day}.jpg/";

        //星座名の設定
        $star = self::$starKanji;

        $star_num = 0;
        for($i=1; $i<13; $i++){
            $date_check_ok = preg_match($date_pattern,$TOPIC_CONTENTS[$i]);
            
            if($date_check_ok){
                $star_flg = preg_match('/<h2>星が語る全体運 (.*?座)/',$TOPIC_CONTENTS[$i],$MATCHES);
                if($star_flg && ($star_num == 0)){
                    $star_name = $MATCHES[1];
                    $star_num = $star[$star_name];
                    $love_num = 0;
                    $money_num = 0;
                    $work_num = 0;
                }
                if($star_num > 0){
                    $star_flg = preg_match_all('/<h3>(.*運)<\/h3><div class="horo_content_sto">(★+)</u',$TOPIC_CONTENTS[$i],$SCORE);
                    if($star_flg){
						foreach($SCORE[0] as $value){
							if(preg_match('/恋愛運/',$value)){
								$love_num = substr_count($value,'★');
							}
							if(preg_match('/金運/',$value)){
								$money_num = substr_count($value,'★');
							}
							if(preg_match('/仕事運/',$value)){
								$work_num = substr_count($value,'★');
							}
						}
                    }
                }
                if($love_num >= 0 || $money_num >= 0 || $work_num >= 0){
                    $love_score = $love_num * 20;
                    $money_score = $money_num * 20;
                    $work_score = $work_num * 20;
                    $TOPIC_RESULT[$star_num] = array("love"=> $love_score , "money" => $money_score ,"work" => $work_score);
					$star_num = 0;
                }
            } 
        }
        // date error?
	    if(!$date_check_ok) {
		    print $this->logDateError().PHP_EOL;
	    }
        return $TOPIC_RESULT;
    }
}