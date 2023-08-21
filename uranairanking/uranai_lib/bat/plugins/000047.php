<?php
/**
 * @author Azet
 * @date 2016-02-25
 * @url http://oha--asa.tumblr.com/
 */
class Zodiac000047 extends UranaiPlugin {

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

		// 月の英語表記
		$en_month[1] = "January";
		$en_month[2] = "February";
		$en_month[3] = "March";
		$en_month[4] = "April";
		$en_month[5] = "May";
		$en_month[6] = "June";
		$en_month[7] = "July";
		$en_month[8] = "August";
		$en_month[9] = "September";
		$en_month[10] = "October";
		$en_month[11] = "November";
		$en_month[12] = "December";

		$CONTENTS = $this->load($URL);
		// 日付の表示があれば、今日の日付と一致するか確認する！
		// (このプラグインでは、取得時に日付設定をしているので、チェックしていません)
		$now = self::getToday();
		// nowのキーは: year,month,day
		// monthは1~12の値
		// dayは1~31の値
		$date_pattern = "/>{$en_month[$now['month']]} {$now['day']}, {$now['year']}<\//";

		// サイト毎に星座名のプラグイン個別設定
		$star["Aquarius"] = 1;	//$star["水瓶座"] = 1;
		$star["Pisces"] = 2;	//$star["魚座"] = 2;
		$star["Aries"] = 3;		//$star["牡羊座"] = 3;
		$star["Taurus"] = 4;	//$star["牡牛座"] = 4;
		$star["Gemini"] = 5;	//$star["双子座"] = 5;
		$star["Cancer"] = 6;	//$star["蟹座"] = 6;
		$star["Leo"] = 7;		//$star["獅子座"] = 7;
		$star["Virgo"] = 8;		//$star["乙女座"] = 8;
		$star["Libra"] = 9;		//$star["天秤座"] = 9;
		$star["Scorpio"] = 10;		//$star["蠍座"] = 10;
		$star["Sagittarius"] = 11;	//$star["射手座"] = 11;
		$star["Capricorn"] = 12;	//$star["山羊座"] = 12;

		// このプラグインは、０ の URL データしか使用しません
		$content = $CONTENTS[0];
		//$content = mb_convert_encoding($content, "UTF-8","SJIS");
		$LINES = explode("\n", $content);

		$date_check_ok = false;
		$cnt = 10;
		foreach ($LINES AS $line) {

			// date check
			if(!$date_check_ok) {
				$date_check_ok = preg_match($date_pattern, $line);
			}

			//ランクデータのある行まで進む
			if ($date_check_ok && $cnt > 0) {
				$cnt--;
				if (preg_match("/data-orig-width=\"/", $line)) {
					//12星座データの抽出
					$seiza = preg_match_all("/<li>(<b>|)(.*?)(|<\/b>)<br>/", $line, $MATCHS);

					$ctx = 1;
					foreach ($MATCHS[2] AS $seiza) {
						$RESULT[$ctx] = $star[$seiza];
						$ctx++;
					}

					$cnt = 0;
					break;
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
