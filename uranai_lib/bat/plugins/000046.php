<?php
/**
 * @author Azet
 * @date 2016-02-05
 * @url http://lotte-koala12star.jp/pc/result.php?star=1 ～ 12
 */
class Zodiac000046 extends UranaiPlugin {

	/**
	 * このファンクションは必要です！
	 * 星座（インデックス） => 順位
	 * @return array[star] = rank;
	 */
	function run($URL) {

		// RESULTの形：12星座分, 1~12
		// $RESULT[<星座番号>] = <ランキング>
		// ランキングも1~12値です
		$CONTENTS = array();

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
		$CONTENTS = $this->load($URL);

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
}
