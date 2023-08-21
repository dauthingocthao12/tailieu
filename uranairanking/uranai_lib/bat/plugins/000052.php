<?php
/**
 * @author Azet
 * @date 2016-02-24
 * @url http://www.tbs.co.jp/hayadoki/gudetama/
 */
class Zodiac000052 extends UranaiPlugin {

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
		$date_pattern = "/{$now['year']}年{$now['month']}月{$now['day']}日（/";

		// サイト毎に星座名の設定
		$star = self::$starDefault;

		//星座ごとにループする
		for ($i = 1; $i < 13; $i++) {
			// このプラグインでは、1~12の星座ごとのURLを使用する
			$content = $CONTENTS[$i];

			$date_check_ok = false;
			if(!$date_check_ok) {
				$date_check_ok = preg_match($date_pattern, $content);
			}

			// rank 取り出し
			if ($date_check_ok) {
				$flag = preg_match("/本日の運勢ランキング\s<span>第<i>(.*)<\/i>位<\/span><\/h3>/", $content, $MATCHES);
				if($flag) {
					$rank_num = $MATCHES[1];
					$star_num = $i;
					// RESULTの形：
					// $RESULT[<星座番号>] = <ランキング>
					$RESULT[$star_num] = $rank_num;
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
