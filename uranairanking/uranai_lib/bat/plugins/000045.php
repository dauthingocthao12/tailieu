<?php
/**
 * @author Azet
 * @date 2016-02-05
 * @url http://girls.livedoor.com/fortune/horoscope/?id=1 ～ 12
 */
class Zodiac000045 extends UranaiPlugin {

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
		$data_pattern1 = "/class=\"ranking\">(.*)<\/div>/";
		$data_pattern2 = "/<h1>(.*座)の{$now['month']}月{$now['day']}日の運勢/";

		// サイトhtmlを取得
		$CONTENTS = $this->load($URL);

		// サイト毎に星座名の設定
		$star = self::$starDefault;

		//星座ごとにループする
		for ($i = 1; $i < 13; $i++) {

			// このプラグインでは、1~12の星座ごとのURLを使用する
			$content = $CONTENTS[$i];
			$content = mb_convert_encoding($content, "UTF-8", "EUC-JP");

			$star_num = 0;
			$rank_num = 0;

			//星座名の取り出し
			$data2 = preg_match($data_pattern2, $content, $MATCHES2);
			if ($data2) {
				$star_name = $MATCHES2[1];
				$star_num = $star[$star_name];
			}

			//ランキング値の取り出し
			$data1 = preg_match($data_pattern1, $content, $MATCHES1);
			if ($data1) {
				$rank_num = $MATCHES1[1];
			}

			if ($star_num > 0 || $rank_num > 0) {
				$RESULTS[$star_num] = $rank_num;
			}
		}

		return $RESULTS;
	}
}
