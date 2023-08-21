<?php
/**
 * @author Azet
 * @date 2016-03-08
 * @url http://www.siyasui.ne.jp/uranai/tokeihi/mizugame/uranai.htm ...
 * updated: okabe 2017/06/19
 */
class Zodiac000061 extends UranaiPlugin {

	/**
	 * このファンクションは必要です！
	 * 星座（インデックス） => 順位
	 * @return array[star] = rank;
	 */
	//function run($URL) {		//del okabe 2017/06/19
	function run($CONTENTS) {	// add okabe 2017/06/19 $URL -> $CONTENTS
		// RESULTの形：12星座分, 1~12
		// $RESULT[<星座番号>] = <ランキング>
		// ランキングも1~12値です
		$RESULT = array();
		$RESULT2 = array();

		//$CONTENTS = $this->load($URL);	//del okabe 2017/06/19
		// 日付の表示があれば、今日の日付と一致するか確認する！
		// (このプラグインでは、取得時に日付設定をしているので、チェックしていません)
		$now = self::getToday();
		// nowのキーは: year,month,day
		// monthは1~12の値
		// dayは1~31の値
		$date_pattern = "/>{$now['year']}年{$now['month']}月{$now['day']}日です/";

		//星座ごとにループする
		for ($i = 1; $i < 13; $i++) {
			// このプラグインでは、1~12の星座ごとのURLを使用する
			$content = $CONTENTS[$i];
			$content = mb_convert_encoding($content, "UTF-8","SJIS");

			$LINES = explode("\n", $content);
			$date_check_ok = false;
			$point_num = 0;

			foreach ($LINES AS $line) {
				if(!$date_check_ok) {
					$date_check_ok = preg_match($date_pattern, $line);
				}

				if ($date_check_ok && preg_match("/^<FONT\x20color=\"#FF0000\">.*<\/FONT>$/", $line)) {
					$cnt = mb_substr_count($line, "★");
					$point_num = $point_num + $cnt;
				}
			}

			if ($date_check_ok) {
				$star_num = $i;
				// RESULTの形：
				// $RESULT[<星座番号>] = 点数
				$RESULT[$star_num] = $point_num;
			}

		}
// $RESULT[<星座番号>] = 点数
//print_r($RESULT);

		//点数から順位付け
		// $RESULT2[<星座番号>] = 順位
		if (count($RESULT) == 12) {
			arsort($RESULT);
			$j = 1;
			$point_prev = -1;
			foreach($RESULT AS $k => $v) {
				if ($point_prev < 0) {
					$point_prev = $v;
					$RESULT2[$k] = 1;
				} else {
					if ($v == $point_prev) {	//１つ前のデータと同点の場合
						$RESULT2[$k] = $j;
					} else {
						$j = count($RESULT2) + 1;	//新たな順位値
						$point_prev = $v;
						$RESULT2[$k] = $j;
					}
				}
			}
		}

		return $RESULT2;
	}


	// add okabe start 2017/06/19
	function topic_run($TOPIC_CONTENTS) {
		$TOPIC_RESULT = array();

		// 日付の表示があれば、今日の日付と一致するか確認する！
		// (このプラグインでは、取得時に日付設定をしているので、チェックしていません)
		$now = self::getToday();
		// nowのキーは: year,month,day
		// monthは1~12の値
		// dayは1~31の値
		$date_pattern = "/>{$now['year']}年{$now['month']}月{$now['day']}日です/";

		//星座ごとにループする
		for ($i = 1; $i < 13; $i++) {
			// このプラグインでは、1~12の星座ごとのURLを使用する
			$content = $TOPIC_CONTENTS[$i];
			$content = mb_convert_encoding($content, "UTF-8","SJIS");

			$LINES = explode("\n", $content);
			$date_check_ok = false;
			$love_flag = 0;
			$money_flag = 0;

			foreach ($LINES AS $line) {
				if(!$date_check_ok) {
					$date_check_ok = preg_match($date_pattern, $line);
				}

				if ($love_flag == 0 && $date_check_ok) {
					if (preg_match("/<p>愛情運$/", $line)) {
						$love_flag = 1;
					}
				}
				if ($date_check_ok && $love_flag == 1 && preg_match("/^<FONT\x20color=\"#FF0000\">.*<\/FONT>$/", $line)) {
					$cnt = mb_substr_count($line, "★");
					$love_count = $cnt;
					$love_flag = 2;
				}

				if ($money_flag == 0 && $date_check_ok) {
					if (preg_match("/<p>金運$/", $line)) {
						$money_flag = 1;
					}
				}
				if ($date_check_ok && $money_flag == 1 && preg_match("/^<FONT\x20color=\"#FF0000\">.*<\/FONT>$/", $line)) {
					$cnt = mb_substr_count($line, "★");
					$money_count = $cnt;
					$money_flag = 2;
				}
			}

			if ($date_check_ok) {
				$star_num = $i;
				$love_num = $love_count * 20;
				$money_num = $money_count * 20;
				$TOPIC_RESULT[$star_num] = array("love"=> $love_num , "money" => $money_num ,"work" => NULL);
			}

		}
		return $TOPIC_RESULT;
	}
	// add okabe end 2017/06/19

	function lucky_run($TOPIC_CONTENTS){
		$LUCKY_RESULT = array();

		// 日付の表示があれば、今日の日付と一致するか確認する！
		// (このプラグインでは、取得時に日付設定をしているので、チェックしていません)
		$now = self::getToday();
		// nowのキーは: year,month,day
		// monthは1~12の値
		// dayは1~31の値
		$date_pattern = "/>{$now['year']}年{$now['month']}月{$now['day']}日です/";

		//星座ごとにループする
		for ($i = 1; $i < 13; $i++) {
			// このプラグインでは、1~12の星座ごとのURLを使用する
			$content = $TOPIC_CONTENTS[$i];
			$content = mb_convert_encoding($content, "UTF-8", "SJIS");

			$LINES = explode("\n", $content);
			$date_check_ok = false;
			$lucky_color_flag = false;

			foreach ($LINES as $line) {
				if (!$date_check_ok) {
					$date_check_ok = preg_match($date_pattern, $line);
				}

				if ($lucky_color_flag == false && $date_check_ok) {
					if (preg_match("/<p>カラー$/", $line)) {
						$lucky_color_flag = true;
					}
				}
				if ($date_check_ok && $lucky_color_flag && preg_match("/^<FONT\x20color=\"#FF0000\">(.*)<\/FONT>$/", $line, $MATCHES)) {
					$lucky_color = $MATCHES[1];
					$lucky_color_flag = false;
				}
			}

			if ($date_check_ok) {
				$star_num = $i;
				$LUCKY_RESULT[$star_num] = array("lucky_item" => null, "lucky_color" => $lucky_color);
			}
		}
		return $LUCKY_RESULT;
	}

}
