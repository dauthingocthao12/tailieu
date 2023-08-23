<?php
/**
 * @author Azet
 * @date 2016-01-05
 * @url http://kids.yahoo.co.jp/fortune/
 */
class Zodiac000013 extends UranaiPlugin {

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

		// オプション：URLに日付が必要の場合は、下記の処理をする
		// 記号を取得する日付に変換する
		// >>>
		//foreach ($URL as $key => $url) {
		//	$url = str_replace("(md)", date("md"), $url);
		//	$url = str_replace("(ymd)", date("ymd"), $url);
		//	$URL[$key] = $url;
		//}
		// <<<

		// Curlが使用したい場合は、下記の機能を使いましょう
		// $this->useCurl($params);

		$CONTENTS = $this->load($URL);

		// このプラグインが、０のURLしか使用しません
		$content = $CONTENTS[0];
		// 必要の時に、下記を直して下さい。
		//$content = mb_convert_encoding($content, "UTF-8", "UTF-8");
		$LINES = explode("\n", $content);

		// サイト毎に星座名の設定
		$star = self::$starDefault;
		/*
		 * またはカストマイズする
		 * $star["みずがめ座"] = 1;
		 * $star["うお座"] = 2;
		 * $star["おひつじ座"] = 3;
		 * $star["おうし座"] = 4;
		 * $star["ふたご座"] = 5;
		 * $star["かに座"] = 6;
		 * $star["しし座"] = 7;
		 * $star["おとめ座"] = 8;
		 * $star["てんびん座"] = 9;
		 * $star["さそり座"] = 10;
		 * $star["いて座"] = 11;
		 * $star["やぎ座"] = 12;
		 */

		// サイトによって情報を取得（しゅとく）

		// 日付の表示があれば、今日の日付と一致するか確認する！
		// (このプラグインでは、取得時に日付設定をしているので、チェックしていません)
		$now = self::getToday();
		// nowのキーは: year,month,day
		// monthは1~12の値
		// dayは1~31の値
		$date_pattern = "/<div class=\"title__day--day\">{$now['month']}月{$now['day']}日<\/div>/";

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
			if ($date_check_ok && !$rank_num && preg_match("/<div class=\"ranktoday__rank.*?\">(\d{1,2})位<\/div>/", $line, $MATCHES)) {
				$rank_num = $MATCHES[1];
				continue;
			}

			// star
			if ($rank_num && preg_match("/<div class=\".*\"><span class=\".*?\">(.*?座)<\/span><\/div>/", $line, $MATCHES)) {
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
