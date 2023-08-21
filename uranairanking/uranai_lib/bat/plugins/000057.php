<?php
/**
 * @author Azet
 * @date 2016-03-07 update 2016/08/23
 * @url http://www.ozmall.co.jp/ ⇒ http://spn.ozmall.co.jp/psychology/dailyhoroscope/
 */
class Zodiac000057 extends UranaiPlugin {

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
		//$date_pattern = "/class=\"fr\x20lh26px\">{$now['month']}月{$now['day']}日/";	//del okabe 2016/08/23
		$date_pattern = "/^<p>0?{$now['month']}月0?{$now['day']}日（/";		//add okabe 2016/08/23 取得先をスマホ版に変更

		// サイト毎に星座名のプラグイン個別設定
		$star = array();
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

		// このプラグインは、０ の URL データしか使用しません
		$content = $CONTENTS[0];
		//$content = mb_convert_encoding($content, "UTF-8","SJIS");
		$LINES = explode("\n", $content);

		$rank_num = 0;

		$date_check_ok = false;
		foreach ($LINES AS $line) {
			if (count($RESULT) == 12) { break; }

			// date check
			if(!$date_check_ok) {
				$date_check_ok = preg_match($date_pattern, $line);
			}

			// rank 通常
			if ($date_check_ok) {

				if ($rank_num == 0) {	//順位の抽出

					$flag = preg_match("/<span\sid=\"rank(\d{1,3})\">/", $line, $MATCHES);
					if ($flag) {
						$rank_num = $MATCHES[1];
						continue;
					}

				} else {

					$flag = preg_match("/<img src=\"images\/ico_zod(\d{1,3})\.png\"/", $line, $MATCHES);
					if ($flag) {
						$star_num = intval($MATCHES[1]);
						// RESULTの形：
						// $RESULT[<星座番号>] = <ランキング>
						$RESULT[$star_num] = intval($rank_num);
						$rank_num = 0;
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
