<?php
/**
 * @author Azet
 * @date 2016-03-07
 * @url http://www.hoshiplaza.co.jp/guide/horoscope/
 */
class Zodiac000054 extends UranaiPlugin {

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
		$date_pattern = "/id=\"luck_date\">{$now['month']}月{$now['day']}日の運勢<\/div>/";

		// サイト毎に星座名のプラグイン個別設定
		$star["aquarius"] = 1;	//$star["水瓶座"] = 1;
		$star["pisces"] = 2;	//$star["魚座"] = 2;
		$star["aries"] = 3;		//$star["牡羊座"] = 3;
		$star["taurus"] = 4;	//$star["牡牛座"] = 4;
		$star["gemini"] = 5;	//$star["双子座"] = 5;
		$star["cancer"] = 6;	//$star["蟹座"] = 6;
		$star["leo"] = 7;		//$star["獅子座"] = 7;
		$star["virgo"] = 8;		//$star["乙女座"] = 8;
		$star["libra"] = 9;		//$star["天秤座"] = 9;
		$star["scorpio"] = 10;		//$star["蠍座"] = 10;
		$star["sagittarius"] = 11;	//$star["射手座"] = 11;
		$star["goat"] = 12;	//$star["山羊座"] = 12;

		// このプラグインは、０ の URL データしか使用しません
		$content = $CONTENTS[0];
		//$content = mb_convert_encoding($content, "UTF-8","JIS");
		$LINES = explode("\n", $content);

		$date_check_ok = false;
		$star_num = 0;
		foreach ($LINES AS $line) {
			if (count($RESULT) == 12) { break; }

			// date check
			if(!$date_check_ok) {
				$date_check_ok = preg_match($date_pattern, $line);
			}

			// rank 通常
			if ($date_check_ok) {
				if ($star_num == 0) {	//星座名の抽出
					$flag = preg_match("/<div\x20id=\"(.*)\"\x20class=\"constellation\">/", $line, $MATCHES);
					if ($flag) {
						$star_name = $MATCHES[1];
						$star_num = $star[$star_name];
						continue;
					}
				} else {	//順位の抽出
					$flag = preg_match("/<span>本日のランキング【(\d{1,2})位】<\/span>/", $line, $MATCHES);
					if ($flag) {
						$rank_num = $MATCHES[1];
						// RESULTの形：
						// $RESULT[<星座番号>] = <ランキング>
						$RESULT[$star_num] = $rank_num;
						$star_num = 0;
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
