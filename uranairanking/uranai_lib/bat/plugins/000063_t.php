<?php
/**
 * @author Azet
 * @date 2016-03-09
 * @url http://www.szeus.co.jp/cgi-bin/fortune-d.cgi?pis ...
 * updated: okabe 2017/06/19
 */
class Zodiac000063 extends UranaiPlugin {

	/**
	 * このファンクションは必要です！
	 * 星座（インデックス） => 順位
	 * @return array[star] = rank;
	 */
	//function run($URL) {			//del okabe 2017/06/19
	function run($CONTENTS) {		// add okabe 2017/06/19 $URL -> $CONTENTS
		// RESULTの形：12星座分, 1~12
		// $RESULT[<星座番号>] = <ランキング>
		// ランキングも1~12値です
		$RESULT = array();
		$RESULT2 = array();

		//$CONTENTS = $this->load($URL);	//del okabe 2017/06/19
		// 日付の表示があれば、今日の日付と一致するか確認する！
		// (このプラグインでは、取得時に日付設定をしているので、チェックしていません)
		$now = self::getToday();
		// nowのキーは: year,month,day
		// monthは1~12の値
		// dayは1~31の値
		$date_pattern = "/>{$now['month']}月{$now['day']}日の.*座の運勢</";

		//星座ごとにループする
		for ($i = 1; $i < 13; $i++) {
			// このプラグインでは、1~12の星座ごとのURLを使用する
			$content = $CONTENTS[$i];
			$content = mb_convert_encoding($content, "UTF-8","SJIS");

			$date_check_ok = false;
			$point_num = 0;

			if(!$date_check_ok) {
				$date_check_ok = preg_match($date_pattern, $content);
			}

			if ($date_check_ok) {
				if (preg_match("/\/daily_fortune\/images\/clover(\d{1,2})\.gif/", $content, $MATCHES)) {
					$point_num = $point_num + intval($MATCHES[1]);
				}
				if (preg_match("/\/daily_fortune\/images\/heart(\d{1,2})\.gif/", $content, $MATCHES)) {
					$point_num = $point_num + intval($MATCHES[1]);
				}
				if (preg_match("/\/daily_fortune\/images\/spade(\d{1,2})\.gif/", $content, $MATCHES)) {
					$point_num = $point_num + intval($MATCHES[1]);
				}
				if (preg_match("/\/daily_fortune\/images\/diamond(\d{1,2})\.gif/", $content, $MATCHES)) {
					$point_num = $point_num + intval($MATCHES[1]);
				}
				if (preg_match("/\/daily_fortune\/images\/healty(\d{1,2})\.gif/", $content, $MATCHES)) {
					$point_num = $point_num + intval($MATCHES[1]);
				}
				if (preg_match("/\/daily_fortune\/images\/info(\d{1,2})\.gif/", $content, $MATCHES)) {
					$point_num = $point_num + intval($MATCHES[1]);
				}

				$star_num = $i;
				// RESULTの形：
				// $RESULT[<星座番号>] = 点数
				$RESULT[$star_num] = $point_num;

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


	// add okabe start 2017/06/19
	function topic_run($TOPIC_CONTENTS) {
		$TOPIC_RESULT = array();

		// 日付の表示があれば、今日の日付と一致するか確認する！
		// (このプラグインでは、取得時に日付設定をしているので、チェックしていません)
		$now = self::getToday();
		// nowのキーは: year,month,day
		// monthは1~12の値
		// dayは1~31の値
		$date_pattern = "/>{$now['month']}月{$now['day']}日の.*座の運勢</";

		//星座ごとにループする
		for ($i = 1; $i < 13; $i++) {
			// このプラグインでは、1~12の星座ごとのURLを使用する
			$content = $TOPIC_CONTENTS[$i];
			$content = mb_convert_encoding($content, "UTF-8","SJIS");

			$date_check_ok = false;
			$love_count = 0;
			$money_count = 0;
			$work_count = 0;
			$health_count = 0;
			$info_count = 0;

			if(!$date_check_ok) {
				$date_check_ok = preg_match($date_pattern, $content);
			}

			if ($date_check_ok) {
				//マークは１～５個。
				//if (preg_match("/\/daily_fortune\/images\/clover(\d{1,2})\.gif/", $content, $MATCHES)) {	//全体運
				//	$point_num = $point_num + intval($MATCHES[1]);
				//}
				if (preg_match("/\/daily_fortune\/images\/heart(\d{1,2})\.gif/", $content, $MATCHES)) {		//恋愛運
					$love_count = intval($MATCHES[1]);
				}
				if (preg_match("/\/daily_fortune\/images\/spade(\d{1,2})\.gif/", $content, $MATCHES)) {		//仕事運
					$work_count = intval($MATCHES[1]);
				}
				if (preg_match("/\/daily_fortune\/images\/diamond(\d{1,2})\.gif/", $content, $MATCHES)) {	//金運
					$money_count = intval($MATCHES[1]);
				}
				if (preg_match("/\/daily_fortune\/images\/healty(\d{1,2})\.gif/", $content, $MATCHES)) {	//健康運
					$health_count = intval($MATCHES[1]);
				}
				if (preg_match("/\/daily_fortune\/images\/info(\d{1,2})\.gif/", $content, $MATCHES)) {		//情報運
					$info_count = intval($MATCHES[1]);
				}

				$star_num = $i;
				$love_val = $love_count * 20;
				$money_val = $money_count * 20;
				$work_val = $work_count * 20;
				$health_val = $health_count * 20;
				$info_val = $info_count * 20;
				$TOPIC_RESULT[$star_num] = array("love"=> $love_val , "money" => $money_val ,"work" => $work_val,"health" => $health_val,"info" => $info_val);
			}

		}

		return $TOPIC_RESULT;
	}
	// add okabe end 2017/06/19




}
