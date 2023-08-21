<?php
/**
 * @author Azet
 * @date 2016-02-24
 * @url http://www.tbs.co.jp/hayadoki/gudetama/
 * updated: okabe 2017/06/20
 */
class Zodiac000052 extends UranaiPlugin {

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
		$date_pattern = "/{$now['year']}年{$now['month']}月{$now['day']}日（/";

		// サイト毎に星座名の設定
		$star = self::$starDefault;

		//星座ごとにループする
		for ($i = 1; $i < 13; $i++) {
			// このプラグインでは、1~12の星座ごとのURLを使用する
			$content = $CONTENTS[$i];

			$date_check_ok = false;
			if(!$date_check_ok) {
				$date_check_ok = preg_match($date_pattern, $content);
			}

			// rank 取り出し
			if ($date_check_ok) {
				$flag = preg_match('/<div class="daily-rank">第<i>(\\d+)<\/i>位<\/div>/', $content, $MATCHES);
				if($flag) {
					$rank_num = $MATCHES[1];
					$star_num = $i;
					// RESULTの形：
					// $RESULT[<星座番号>] = <ランキング>
					$RESULT[$star_num] = $rank_num;
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
		$date_pattern = "/{$now['year']}年{$now['month']}月{$now['day']}日\（/";

		// サイト毎に星座名の設定
		$star = self::$starDefault;

		//星座ごとにループする
		for ($i = 1; $i < 13; $i++) {
			// このプラグインでは、1~12の星座ごとのURLを使用する
			$content = $TOPIC_CONTENTS[$i];

			$date_check_ok = false;
			if(!$date_check_ok) {
				$date_check_ok = preg_match($date_pattern, $content);
			}

			$love_val = -1;
			$money_val = -1;
			$work_val = -1;

			// rank 取り出し
			if ($date_check_ok) {
				$flag = preg_match("/\/img\/uranai\/love(\d{2})\.png/", $content, $MATCHES);
				if($flag) { $love_val = intval($MATCHES[1]) * 20;}
				$flag = preg_match("/\/img\/uranai\/money(\d{2})\.png/", $content, $MATCHES);
				if($flag) { $money_val = intval($MATCHES[1]) * 20; }
				$flag = preg_match("/\/img\/uranai\/work(\d{2})\.png/", $content, $MATCHES);
				if($flag) { $work_val = intval($MATCHES[1]) * 20; }
				$star_num = $i;

				if ($love_val >= 0 && $money_val >= 0 && $work_val >= 0) {
					$TOPIC_RESULT[$star_num] = array("love"=> $love_val , "money" => $money_val ,"work" => $work_val);
				}

			}

		}

		return $TOPIC_RESULT;
	}
	// add okabe end 2017/06/20

	function lucky_run($TOPIC_CONTENTS){
		$LUCKY_RESULT = array();

		// 日付の表示があれば、今日の日付と一致するか確認する！
		// (このプラグインでは、取得時に日付設定をしているので、チェックしていません)
		$now = self::getToday();
		// nowのキーは: year,month,day
		// monthは1~12の値
		// dayは1~31の値
		$date_pattern = "/{$now['year']}年{$now['month']}月{$now['day']}日\（/";

		// サイト毎に星座名の設定
		$star = self::$starDefault;

		//星座ごとにループする
		for ($i = 1; $i < 13; $i++) {
			// このプラグインでは、1~12の星座ごとのURLを使用する
			$content = $TOPIC_CONTENTS[$i];

			$date_check_ok = false;
			if (!$date_check_ok) {
				$date_check_ok = preg_match($date_pattern, $content);
			}

			// rank 取り出し
			if ($date_check_ok && preg_match("/ラッキーアイテム：(.*?)<br>/", $content, $MATCHES)) {
				$lucky_item = $MATCHES[1];
			}
			if ($date_check_ok && preg_match("/ラッキーカラー：(.*?)\n/", $content, $MATCHES)) {
				$lucky_color = $MATCHES[1];
			}
			$star_num = $i;
			$LUCKY_RESULT[$star_num] = array("lucky_item" => $lucky_item, "lucky_color" => $lucky_color);
			if (!$date_check_ok) {
				print $this->logDateError() . PHP_EOL;
			}
		}
		return $LUCKY_RESULT;
	}

}
