<?php
/**
 * @author Azet
 * @date 2016-02-24
 * @url http://www.macpd.com/
 * updated: okabe 2017/06/20
 */
class Zodiac000050 extends UranaiPlugin {

	/**
	 * このファンクションは必要です！
	 * 星座（インデックス） => 順位
	 * @return array[star] = rank;
	 */
	//function run($URL) {			//del okabe 2017/06/20
	function run($CONTENTS) {		// add okabe 2017/06/20 $URL -> $CONTENTS
		// RESULTの形：12星座分, 1~12
		// $RESULT[<星座番号>] = <ランキング>
		// ランキングも1~12値です
		$RESULT = array();

		//$CONTENTS = $this->load($URL);		//del okabe 2017/06/20
		// 日付の表示があれば、今日の日付と一致するか確認する！
		// (このプラグインでは、取得時に日付設定をしているので、チェックしていません)
		$now = self::getToday();
		// nowのキーは: year,month,day
		// monthは1~12の値
		// dayは1~31の値
		$date_pattern = "/{$now['month']}月{$now['day']}日（.*）の占い/";

        // サイト毎に星座名の設定
		$star = self::$starDefault;

		// このプラグインは、０ の URL データしか使用しません
		$content = $CONTENTS[0];
		$content = mb_convert_encoding($content, "UTF-8","SJIS");
		$LINES = explode("\n", $content);

		$date_check_ok = false;
		$rank_num = 0;
		foreach ($LINES AS $line) {
			if (count($RESULT) == 12) { break; }

			// date check
			if(!$date_check_ok) {
				$date_check_ok = preg_match($date_pattern, $line);
			}

			// rank 通常
			if ($date_check_ok) {
				if ($rank_num == 0) {	//順位の抽出
					$flag = preg_match("/<img\x20class=\"rank\"\x20src=\"\/goodmorning\/uranai\/sphone\/img\/rank-(\d{1,2})\.png\"/", $line, $MATCHES);
					if ($flag) {
						$rank_num = $MATCHES[1];
						continue;
					}

				} else {
					$flag = preg_match("/<span>(.*座)<\/span>/", $line, $MATCHES);
					if ($flag) {
						$star_name = $MATCHES[1];
						$star_num = $star[$star_name];
						// RESULTの形：
						// $RESULT[<星座番号>] = <ランキング>
						$RESULT[$star_num] = $rank_num;
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

	// add okabe start 2017/06/20
	function topic_run($TOPIC_CONTENTS) {
		$TOPIC_RESULT = array();

		// 日付の表示があれば、今日の日付と一致するか確認する！
		// (このプラグインでは、取得時に日付設定をしているので、チェックしていません)
		$now = self::getToday();
		// nowのキーは: year,month,day
		// monthは1~12の値
		// dayは1~31の値
		$date_pattern = "/{$now['month']}月{$now['day']}日（.*）の占い/";

		// サイト毎に星座名の設定
		$star = self::$starDefault;

		$love_val = 0;
		$money_val = 0;
		$work_val = 0;
		$health_val = 0;

		//星座ごとにループする
		for ($i = 1; $i < 13; $i++) {
			// このプラグインでは、1~12の星座ごとのURLを使用する
			$content = $TOPIC_CONTENTS[$i];
			$content = mb_convert_encoding($content, "UTF-8","SJIS");

			$date_check_ok = false;

			// date check
			if(!$date_check_ok) {
				$date_check_ok = preg_match($date_pattern, $line);
			}

			$LINES = explode("\n", $content);

			foreach ($LINES AS $line) {
				$flg = preg_match("!/goodmorning/uranai/sphone/img/icon-love\.png!", $line);
				if ($flg) { $love_val++; }
				$flg = preg_match("!/goodmorning/uranai/sphone/img/icon-money\.png\"!", $line);
				if ($flg) { $money_val++; }
				$flg = preg_match("!/goodmorning/uranai/sphone/img/icon-work\.png\"!", $line);
				if ($flg) { $work_val++; }
				$flg = preg_match("!/goodmorning/uranai/sphone/img/icon-health\.png\"!", $line);
				if ($flg) { $health_val++; }
			}
			//マークの個数は、１～６で換算。
			$star_num = $i;
			$love_num = intVal($love_val * 16.67);
			$money_num = intVal($money_val * 16.67);
			$work_num = intVal($work_val * 16.67);
			$health_num = intVal($health_val * 16.67);
			$TOPIC_RESULT[$star_num] = array("love"=> $love_num , "money" => $money_num ,"work" => $work_num,"health" => $health_num);

			$love_val = 0;
			$money_val = 0;
			$work_val = 0;
			$health_val = 0;
		}

		return $TOPIC_RESULT;
	}
	// add okabe end 2017/06/20
}
