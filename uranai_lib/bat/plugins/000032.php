<?php
/**
 * @author Azet
 * @date 2016-02-05
 * @url http://www.yomiuri.co.jp/komachi/fortune/horoscope/
 *      実体はコチラ ⇒ http://server11.happywoman.jp/12star_yomiuri/index.html
 */
class Zodiac000032 extends UranaiPlugin {

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
		$date_pattern = "/class=\"pcTtl\"><span class=\"bgTtl\">{$now['month']}月{$now['day']}日　<span>/";

		// サイトhtmlを取得
		$CONTENTS = $this->load($URL);

		//星座ごとにループする
		for ($i = 1; $i < 13; $i++) {

			// このプラグインでは、1~12の星座ごとのURLを使用する
			$content = $CONTENTS[$i];
			//$content = mb_convert_encoding($content, "UTF-8","SJIS");	//不要 元々がUTF-8
			$LINES = explode("\n", $content);

			//星座ごとのURLを１つずつパース処理する
			$rank_num = 0;		//ランク値の格納先
			$date_check_ok = false;		//日付のパースチェック結果
			foreach ($LINES AS $line) {
				// date check
				if(!$date_check_ok) {
					$date_check_ok = preg_match($date_pattern, $line);

					// parse datas
					if ($date_check_ok) {
						if (preg_match("/class=\"pcTtl\"><span class=\"bgTtl\">(\d{1,2})月(\d{1,2})日　<span>(\d{1,2})<\/span>/", $line, $MATCHES)) {
							$RESULTS[$i] = $MATCHES[3];
							break;
						}
					}

				}
			}

		}

		return $RESULTS;
	}
}
