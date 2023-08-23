<?php
/**
 * @author Azet
 * @date 2016-02-05
 * @url http://www.kanematsu-group.co.jp/fortune/
 */
class Zodiac000037 extends UranaiPlugin {

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
		$RESULT = array();

		// サイト毎に星座名の設定
		$star = self::$starDefault;

		// 日付の表示があれば、今日の日付と一致するか確認する！
		$now = self::getToday();
		// nowのキーは: year,month,day
		// monthは1~12の値
		// dayは1~31の値
		//$date_pattern = "/<p class=\"date\">{$now['year']}年{$monthx}月{$oneday}日.*<\/p>/";
		$date_pattern = "/>{$now['month']}月{$now['day']}日運勢ランキング/";

		// サイトhtmlを取得
		$CONTENTS = $this->load($URL);

		// このプラグインでは、１ファイルで12星座の情報を抽出する
		$content = $CONTENTS[0];
		//$content = mb_convert_encoding($content, "UTF-8","SJIS");	//不要 元々がUTF-8

		$LINES = explode("\n", $content);

		//１行ずつパース処理する
		$rank_num = 0;		//ランク値の格納先
		$date_check_ok = false;		//日付のパースチェック結果

		foreach ($LINES AS $line) {
			if (count($RESULT) == 12) { break; }

			// date check
			if(!$date_check_ok) {
				$date_check_ok = preg_match($date_pattern, $line);	//日付チェック結果格納
			}

			// parse datas
			if ($date_check_ok) {
				if (preg_match("/alt=\"(\d{1,2})位\".*alt=\"(.*?座)\"/", $line, $MATCHES)) {
					$rank_num = $MATCHES[1];		//(\d{1,2})位
					$star_name_jp = $MATCHES[2];	//(.*?座)
					$star_num = $star[$star_name_jp];

					// RESULTの形：
					// $RESULT[<星座番号>] = <ランキング>
					$RESULT[$star_num] = $rank_num;
				}
			}

		}

		return $RESULT;
	}
}
