<?php
/**
 * @author Azet
 * @date 2016-02-05
 * @url http://www.yomiuri.co.jp/komachi/fortune/horoscope/
 *      実体はコチラ ⇒ http://server11.happywoman.jp/12star_yomiuri/index.html
 */
class Zodiac000033 extends UranaiPlugin {

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

		// このプラグインが、０のURLしか使用しません
		$content = $CONTENTS[0];
		//$content = mb_convert_encoding($content, "UTF-8","SJIS");	//不要 元々がUTF-8
		$LINES = explode("\n", $content);

		// サイト毎に星座名のプラグイン個別設定
		$star["水瓶座"] = 1;
		$star["魚座"] = 2;
		$star["牡羊座"] = 3;
		$star["牡牛座"] = 4;
		$star["双子座"] = 5;
		$star["蟹座"] = 6;
		$star["獅子座"] = 7;
		$star["乙女座"] = 8;
		$star["天秤座"] = 9;
		$star["蠍座"] = 10;
		$star["射手座"] = 11;
		$star["山羊座"] = 12;

		// サイトによって情報を取得（しゅとく）

		// 日付の表示があれば、今日の日付と一致するか確認する！
		$now = self::getToday();

		// nowのキーは: year,month,day
		// monthは1~12の値
		// dayは1~31の値
		$date_pattern = "/詳しくはこちら<\/a><\/p><p class=\"dates\">{$now['year']}年(\d*){$now['month']}月(\d*){$now['day']}日/";
	
		$date_check_ok = false;
		foreach ($LINES AS $line) {
			if (count($RESULT) == 12) { break; }

			// date check
			if(!$date_check_ok) {
				$date_check_ok = preg_match($date_pattern, $line);
			}

			// rank 通常
			if ($date_check_ok && preg_match("/<img alt=\"第(\d{1,2})位\" .* alt=\"(.*?座)/", $line, $MATCHES)) {
				$rank_num = $MATCHES[1];		//第(\d{1,2})位
				$star_name_jp = $MATCHES[2];	//(.*?座)
				$star_num = $star[$star_name_jp];

				// RESULTの形：
				// $RESULT[<星座番号>] = <ランキング>
				$RESULT[$star_num] = $rank_num;
			}

		}

		// date error?
		if(!$date_check_ok) {
			print $this->logDateError().PHP_EOL;
		}

		return $RESULT;
	}
}
