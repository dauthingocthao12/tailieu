<?php
/**
 * @author Azet
 * @date 2016-02-12
 * @url http://dd.hokkaido-np.co.jp/horoscope/
 */
class Zodiac000034 extends UranaiPlugin {

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

		// サイト毎に星座名の設定
		// サイト毎に星座名のプラグイン個別設定
		//$star["水瓶座"] = 1;
		//$star["魚座"] = 2;
		//$star["牡羊座"] = 3;
		//$star["牡牛座"] = 4;
		//$star["双子座"] = 5;
		//$star["蟹座"] = 6;
		//$star["獅子座"] = 7;
		//$star["乙女座"] = 8;
		//$star["天秤座"] = 9;
		//$star["蠍座"] = 10;
		//$star["射手座"] = 11;
		//$star["山羊座"] = 12;
		$star = self::$starDefault;

		// 日付の表示があれば、今日の日付と一致するか確認する！
		$now = self::getToday();
		// nowのキーは: year,month,day
		// monthは1~12の値
		// dayは1~31の値
		$monthx = sprintf('%02d', $now['month']);
		$oneday = sprintf('%02d', $now['day']);
		$date_pattern = "/<span class=\"dates\">{$now['year']}年{$monthx}月{$oneday}日<\/span>/";

		// サイトhtmlを取得
		$CONTENTS = $this->load($URL);

		// このプラグインでは、１ファイルで12星座の情報を抽出する
		$content = $CONTENTS[0];
		//$content = mb_convert_encoding($content, "UTF-8","SJIS");	//不要 元々がUTF-8

		$LINES = explode("\n", $content);

		//１行ずつパース処理する
		$rank_num = 0;		//ランク値の格納先
		$date_check_ok = false;		//日付のパースチェック結果
		$flag=0;
		foreach ($LINES AS $line) {
			if (count($RESULT) == 12) { break; }

			// date check
			if(!$date_check_ok) {
				$date_check_ok = preg_match($date_pattern, $line);	//日付チェック結果格納
			}

			// parse datas
			if ($date_check_ok) {
				if (preg_match("/<img .* alt=\"(\d{1,2})位\">/", $line, $MATCHES)) {
					$rank_num = $MATCHES[1];		//(\d{1,2})位
					$flag=1;
				}
				if ($flag == 1 && preg_match("/<h3><img .*alt=\"(.*?座)\"><\/h3>/", $line, $MATCHES)) {
					$star_name_jp = $MATCHES[1];	//(.*?座)
					$star_num = $star[$star_name_jp];
					$flag = 0;

					// RESULTの形：
					// $RESULT[<星座番号>] = <ランキング>
					$RESULTS[$star_num] = $rank_num;
				}
			}

		}

		return $RESULTS;
	}
}
