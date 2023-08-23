<?php
/**
 * @author Azet
 * @date 2016-03-08
 * @url http://www.kbs-kyoto.co.jp/tv/po/
 */
class Zodiac000062 extends UranaiPlugin {

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
		$date_pattern = "/id=\"fortune_day\">{$now['month']}\/{$now['day']}\(.*\)<\/p>/";

		// サイト毎に星座名の設定
		$star = self::$starDefault;

		// このプラグインは、０ の URL データしか使用しません
		$content = $CONTENTS[0];
		//$content = mb_convert_encoding($content, "UTF-8","SJIS");
		$LINES = explode("\n", $content);

		$date_check_ok = false;
		foreach ($LINES AS $line) {
			if (count($RESULT) == 12) { break; }

			// date check
			if(!$date_check_ok) {
				$date_check_ok = preg_match($date_pattern, $line);
			}

			// rank 通常
			if ($date_check_ok) {
				$flag = preg_match("/>(\d{1,2})位(.*座)<\/span>/", $line, $MATCHES);
				if ($flag) {
					$rank_num = $MATCHES[1];
					$star_name = trim(mb_convert_kana($MATCHES[2], "s", 'UTF-8'));
					$star_num = $star[$star_name];
					// RESULTの形：
					// $RESULT[<星座番号>] = 順位
					if ($star_num > 0 && $star_num < 13) {
						$RESULT[$star_num] = $rank_num;
					}
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
