<?php
/**
 * @author Azet
 * @date 2016-01-13
 * @url http://sp.asahi.jp/program/ohaasa/uranai/
 */
class Zodiac000029 extends UranaiPlugin {

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
		// (このプラグインでは、取得時に日付設定をしているので、チェックしていません)
		$now = self::getToday(); // 01のような2ケタ数字に未対応

		// nowのキーは: year,month,day
		// monthは1~12の値
		// dayは1~31の値
		$date_pattern = "/{$now['month']}月{$now['day']}日の運勢/";

		$rank_num = 0;
		$date_check_ok = false;
		foreach ($LINES AS $line) {

			if (count($RESULT) == 12) { break; }

			// date check
			if(!$date_check_ok) {
				$date_check_ok = preg_match($date_pattern, $line);
				continue;
			}

			// rank&star
			if ($date_check_ok && !$rank_num && preg_match("/<p class=\"starsign_name\">(.*?座) (\d{1,2})位<\/p>/", $line, $MATCHES)) {
				$star_name = $MATCHES[1];
				$rank_num = $MATCHES[2];

				$star_num = $star[$star_name];

				// RESULTの形：
				// $RESULT[<星座番号>] = <ランキング>
				$RESULT[$star_num] = $rank_num;

				// reset
				$rank_num = 0;

				continue;
			}
		}

		// date error?
		if(!$date_check_ok) {
			print $this->logDateError().PHP_EOL;
		}

		return $RESULT;
	}
}
