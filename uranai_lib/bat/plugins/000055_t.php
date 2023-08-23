<?php
/**
 * @author Azet
 * @date 2016-03-08
 * @url http://www.moonlabo.com/cgi-bin/cafe/moon/pmn_12daily.cgi?OpDv=1
 * updated: okabe 2017/06/20
 */
class Zodiac000055 extends UranaiPlugin {

	/**
	 * このファンクションは必要です！
	 * 星座（インデックス） => 順位
	 * @return array[star] = rank;
	 */
	//function run($URL) {			//del okabe 2017/06/20
	function run($CONTENTS) {		// add okabe 2017/06/20 $URL -> $CONTENTS
		// RESULTの形：12星座分, 1~12
		// $RESULT[<星座番号>] = <ランキング>
		// ランキングも1~12値です
		$RESULT = array();

		//$CONTENTS = $this->load($URL);	//del okabe 2017/06/20
		// 日付の表示があれば、今日の日付と一致するか確認する！
		// (このプラグインでは、取得時に日付設定をしているので、チェックしていません)
		$now = self::getToday();
		// nowのキーは: year,month,day
		// monthは1~12の値
		// dayは1~31の値
		$date_pattern = "/\({$now['year']}年{$now['month']}月{$now['day']}日\)/";

		//星座ごとにループする
		for ($i = 1; $i < 13; $i++) {
			// このプラグインでは、1~12の星座ごとのURLを使用する
			$content = $CONTENTS[$i];
			//$content = mb_convert_encoding($content, "UTF-8","EUC-JP");

			$date_check_ok = false;
			if(!$date_check_ok) {
				$date_check_ok = preg_match($date_pattern, $content);
			}

			// rank 取り出し
			if ($date_check_ok) {
				$cnt = 0;
				$flag = preg_match("/恋愛：\x20(\d{1,3})/", $content, $MATCHES);
				if($flag) { $cnt = intval($MATCHES[1]); }
				$flag = preg_match("/仕事：\x20(\d{1,3})/", $content, $MATCHES);
				if($flag) { $cnt = $cnt + intval($MATCHES[1]); }
				$flag = preg_match("/対人：\x20(\d{1,3})/", $content, $MATCHES);
				if($flag) { $cnt = $cnt + intval($MATCHES[1]); }
//echo "*".$i.", ".$cnt."\n";
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




	// add okabe start 2017/06/20
	function topic_run($TOPIC_CONTENTS) {
		$TOPIC_RESULT = array();

		// 日付の表示があれば、今日の日付と一致するか確認する！
		// (このプラグインでは、取得時に日付設定をしているので、チェックしていません)
		$now = self::getToday();
		// nowのキーは: year,month,day
		// monthは1~12の値
		// dayは1~31の値
		$date_pattern = "/\({$now['year']}年{$now['month']}月{$now['day']}日\)/";

		//星座ごとにループする
		for ($i = 1; $i < 13; $i++) {
			// このプラグインでは、1~12の星座ごとのURLを使用する
			$content = $TOPIC_CONTENTS[$i];
			//$content = mb_convert_encoding($content, "UTF-8","EUC-JP");

			$date_check_ok = false;
			if(!$date_check_ok) {
				$date_check_ok = preg_match($date_pattern, $content);
			}

			$love_val = -1;
			$money_val = -1;
			$work_val = -1;

			// rank 取り出し
			if ($date_check_ok) {
				$cnt = 0;
				$flag = preg_match("/恋愛：\x20(\d{1,3})/", $content, $MATCHES);
				if($flag) { $love_val = intval($MATCHES[1]) * 20; }
				$flag = preg_match("/仕事：\x20(\d{1,3})/", $content, $MATCHES);
				if($flag) { $work_val = intval($MATCHES[1]) * 20; }
//				$flag = preg_match("/対人：\x20(\d{1,3})/", $content, $MATCHES);
//				if($flag) { $money_val = intval($MATCHES[1]) * 20; }
				$star_num = $i;

				if ($love_val >=0 && $work_val >= 0 /*&& $money_val >= 0*/) {
					$TOPIC_RESULT[$star_num] = array("love"=> $love_val , "money" => NULL ,"work" => $work_val);
				}
			}

		}

		return $TOPIC_RESULT;
	}
	// add okabe end 2017/06/20

}
