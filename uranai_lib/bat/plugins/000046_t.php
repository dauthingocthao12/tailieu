<?php
/**
 * @author Azet
 * @date 2016-02-05
 * @url http://lotte-koala12star.jp/pc/result.php?star=1 ～ 12
 * updated: okabe 2017/06/21
 */
class Zodiac000046 extends UranaiPlugin {

	/**
	 * このファンクションは必要です！
	 * 星座（インデックス） => 順位
	 * @return array[star] = rank;
	 */
	//function run($URL) {		//del okabe 2017/06/21
	function run($CONTENTS) {	// add okabe 2017/06/21 $URL -> $CONTENTS

		// RESULTの形：12星座分, 1~12
		// $RESULT[<星座番号>] = <ランキング>
		// ランキングも1~12値です
		//$CONTENTS = array();		//del okabe 2017/06/21

		//結果を格納する配列
		$RESULTS = array();

		// 日付の表示があれば、今日の日付と一致するか確認する！
		$now = self::getToday();
		// nowのキーは: year,month,day
		// monthは1~12の値
		// dayは1~31の値
		$data_pattern1 = "/class=\"text\">{$now['month']}月{$now['day']}日更新/";
		$data_pattern2 = "/alt=\"ランキング\".*alt=\"(.*)位\"/";

		// サイトhtmlを取得
		//$CONTENTS = $this->load($URL);	//del okabe 2017/06/21

		// サイト毎に星座名の設定
		$star = self::$starDefault;

		//星座ごとにループする
		for ($i = 1; $i < 13; $i++) {

			// このプラグインでは、1~12の星座ごとのURLを使用する
			$content = $CONTENTS[$i];
			//$content = mb_convert_encoding($content, "UTF-8", "EUC-JP");	//元がutf-8なので変換不要

			//星座番号
			$star_num = $i;

			$rank_num = 0;
			$date_check_ok = false;

			//データ日付のチェック
			if (preg_match($data_pattern1, $content)) {
				$date_check_ok = true;
			}

			//ランキング値の取り出し
			if ($date_check_ok && preg_match($data_pattern2, $content, $MATCHES)) {
				$rank_num = $MATCHES[1];
			}

			if ($star_num > 0 || $rank_num > 0) {
				$RESULTS[$star_num] = $rank_num;
			}
		}

		return $RESULTS;
	}



	// add okabe start 2017/06/19
	function topic_run($TOPIC_CONTENTS) {
		$TOPIC_RESULT = array();

		$now = self::getToday();
		// nowのキーは: year,month,day
		// monthは1~12の値
		// dayは1~31の値
		$data_pattern1 = "/class=\"text\">{$now['month']}月{$now['day']}日更新/";

		$love_val = -1;
		$money_val = -1;
		$work_val = -1;

		//星座ごとにループする
		for ($i = 1; $i < 13; $i++) {

			// このプラグインでは、1~12の星座ごとのURLを使用する
			$content = $TOPIC_CONTENTS[$i];

			//星座番号
			$star_num = $i;

			$date_check_ok = false;

			//データ日付のチェック
			if (preg_match($data_pattern1, $content)) {
				$date_check_ok = true;
			}

			//ランキング値の取り出し
			if ($date_check_ok) {
				$chk = preg_match("!alt=\"恋愛運\" /><span><img src=\"img/result/heart(\d{1})\.png\"!", $content, $MATCHES);
				if ($chk) {
					$love_val = intVal($MATCHES[1]);
				}
				$chk = preg_match("!alt=\"金　運\" /><span><img src=\"img/result/money(\d{1})\.png\"!", $content, $MATCHES);
				if ($chk) {
					$money_val = intVal($MATCHES[1]);
				}
				$chk = preg_match("!alt=\"仕事運\" /><span><img src=\"img/result/work(\d{1})\.png\"!", $content, $MATCHES);
				if ($chk) {
					$work_val = intVal($MATCHES[1]);
				}
			}

			if ($star_num > 0 && $love_val >=0 && $money_val >=0 && $work_val >=0) {
				// マークは最大５つ
				$love_num = $love_val * 20;
				$money_num = $money_val * 20;
				$work_num = $work_val * 20;
				$TOPIC_RESULT[$star_num] = array("love"=> $love_num , "money" => $money_num ,"work" => $work_num);

				$star_num = 0;
				$love_val -1;
				$money_val = -1;
				$work_val = -1;
			}

		}

		return $TOPIC_RESULT;
	}
	// add okabe end 2017/06/19

}
