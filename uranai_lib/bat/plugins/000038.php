<?php
/**
 * @author Azet
 * @date 2016-02-05
 * @url http://www.kiilife.jp/uranai/
 */
class Zodiac000038 extends UranaiPlugin {

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
		$CONTENTS = array();

		// サイトhtmlを取得
		$CONTENTS = $this->load($URL);

		// サイト毎に星座名のプラグイン個別設定
		$star = self::$starDefault;

		// 日付の表示があれば、今日の日付と一致するか確認する！
		$now = self::getToday();
		// nowのキーは: year,month,day
		// monthは1~12の値
		// dayは1~31の値

		//星座ごとにループする
		for ($i = 1; $i < 13; $i++) {
			// このプラグインでは、1~12の星座ごとのURLを使用する
			$content = $CONTENTS[$i];
			$content = mb_convert_encoding($content, "UTF-8","SJIS");	//不要 元々がUTF-8
			//星座ごとのURLを１つずつパース処理する

			$star_num = 0;
			$date_check_ok = false;		//日付のパースチェック結果
			$date_pattern ="/<div class=\"day\">★{$now['month']}月{$now['day']}日の運勢ランキング<\/div>/";

			// date check
			if(!$date_check_ok) {
				$date_check_ok = preg_match($date_pattern, $content);	//日付チェック結果格納
			}

			// parse datas
			if ($date_check_ok) {
				if (preg_match("/<div class=\"left\"><img src=\".*gif\" alt=\"(.*?座)\" \/>/", $content, $MATCHES)) {
					$star_name = $MATCHES[1];
					$star_num = $star[$star_name];
					if (preg_match("/<div class=\"block\"><img src=\".*\/ranking(\d{1,2})\.gif\" \/><\/div>/", $content, $MATCHES2)) {
						$RESULT[$star_num] = $MATCHES2[1];
						continue;
					}
				}
			}
		}

		return $RESULT;
	}
}
