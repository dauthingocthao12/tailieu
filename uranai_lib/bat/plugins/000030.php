<?php
/**
 * @author Azet
 * @date 2016-01-13
 * @url http://tjokayama.jp/horoscope
 */
class Zodiac000030 extends UranaiPlugin {

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

		// サイトhtmlを取得
		$CONTENTS = $this->load($URL);

		// このプラグインが、０のURLしか使用しません
		$content = $CONTENTS[0];

		// 必要の時に、下記を直して下さい。
		//$content = mb_convert_encoding($content, "UTF-8", "UTF-8");
		$LINES = explode("\n", $content);

		// サイト毎に星座名の設定
		$star = self::$starDefault;

		// サイトによって情報を取得（しゅとく）

		// 日付の表示があれば、今日の日付と一致するか確認する！
		$now = self::getToday();

		// nowのキーは: year,month,day
		// monthは1~12の値(全角)
		// dayは1~31の値(全角)
		$month = mb_convert_kana($now['month'],"N","UTF-8");
		$day = mb_convert_kana($now['day'],"N","UTF-8");
		$date_pattern = "/{$month}月{$day}日の星座占いランキング/";

		$rank_num = 0;
		$date_check_ok = false;
		foreach ($LINES AS $line) {

			if (count($RESULT) == 12) { break; }

			// date check
			if(!$date_check_ok) {
				$date_check_ok = preg_match($date_pattern, $line);
				continue;
			}

			// rank
			if ($date_check_ok && !$rank_num && preg_match("/<p class=\"horoscope-panel__rank\">(\d{1,2})位<\/p>/", $line, $MATCHES)) {
				$rank_num = $MATCHES[1];
				continue;
			}

			// star
			if ($rank_num && preg_match("/<h1 class=\"horoscope-panel__name\">(.*?座)<\/h1>/", $line, $MATCHES)) {
				$star_name = $MATCHES[1];
				$star_num = $star[$star_name];

				// RESULTの形：
				// $RESULT[<星座番号>] = <ランキング>
				$RESULT[$star_num] = $rank_num;

				// reset
				$rank_num = 0;
			}

		}

		// date error?
		if(!$date_check_ok) {
			print $this->logDateError().PHP_EOL;
		}

		return $RESULT;
	}
}
