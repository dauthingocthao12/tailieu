<?php
/**
 * @author Azet
 * @date 2016-02-24
 * @url http://www.macpd.com/
 * updated: okabe 2017/06/20
 */
class Zodiac000049 extends UranaiPlugin {

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

		//$CONTENTS = $this->load($URL);	//del okabe 2017/06/20
		// 日付の表示があれば、今日の日付と一致するか確認する！
		// (このプラグインでは、取得時に日付設定をしているので、チェックしていません)
		$now = self::getToday();
		// nowのキーは: year,month,day
		// monthは1~12の値
		// dayは1~31の値
		$date_pattern = "/ステラ薫子<\/span>{$now['year']}年{$now['month']}月{$now['day']}日/";

        // サイト毎に星座名の設定
		$star = self::$starDefault;

		// このプラグインは、０ の URL データしか使用しません
		$content = $CONTENTS[0];
		//$content = mb_convert_encoding($content, "UTF-8","EUC-JP");
		$LINES = explode("\n", $content);

		$date_check_ok = false;
		foreach ($LINES AS $line) {
			if (count($RESULT) == 12) { break; }

			// date check
			if(!$date_check_ok) {
				$date_check_ok = preg_match($date_pattern, $line);
			}

			// rank 通常
			if ($date_check_ok && preg_match("/alt=\"(\d{1,2})位\".*>(.*座)<\//", $line, $MATCHES)) {
				$rank_num = $MATCHES[1];		//第(\d{1,2})位
				$star_name = $MATCHES[2];	//(.*?座)
				$star_num = $star[$star_name];

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


	// add okabe start 2017/06/20
	function topic_run($TOPIC_CONTENTS) {
		$TOPIC_RESULT = array();

		// 日付の表示があれば、今日の日付と一致するか確認する！
		// (このプラグインでは、取得時に日付設定をしているので、チェックしていません)
		$now = self::getToday();
		// nowのキーは: year,month,day
		// monthは1~12の値
		// dayは1~31の値
		$date_pattern = "/ステラ薫子<\/span>{$now['year']}年{$now['month']}月{$now['day']}日/";

		// サイト毎に星座名の設定
		$star = self::$starDefault;

		$star_num = 0;
		$love_val = 0;
		$money_val = 0;
		$work_val = 0;

		//星座ごとにループする
		for ($i = 1; $i < 13; $i++) {
			// このプラグインでは、1~12の星座ごとのURLを使用する
			$content = $TOPIC_CONTENTS[$i];
			//$content = mb_convert_encoding($content, "UTF-8","SJIS");
			$date_check_ok = false;

			// date check
			if(!$date_check_ok) {
		        $date_check_ok = preg_match($date_pattern, $content);
			}

			if ($date_check_ok && preg_match("/21\" >(.*座)<\/h2>/", $content, $MATCHES)) {
				$star_name = $MATCHES[1];
				$star_num = $star[$star_name];
			}

			$LINES = explode("\n", $content);

			foreach ($LINES AS $line) {
				$flg = preg_match("!\"img/icon_love_1\.gif\"!", $line);
				if ($flg) { $love_val++; }
				$flg = preg_match("!\"img/icon_money_1\.gif\"!", $line);
				if ($flg) { $money_val++; }
				$flg = preg_match("!\"img/icon_works_1\.gif\"!", $line);
				if ($flg) { $work_val++; }
			}
			//マークの個数は、１～５で換算。
			$love_num = intVal($love_val * 20);
			$money_num = intVal($money_val * 20);
			$work_num = intVal($work_val * 20);
			if ($star_num > 0) {
				$TOPIC_RESULT[$star_num] = array("love"=> $love_num , "money" => $money_num ,"work" => $work_num);
			}

			$love_val = 0;
			$money_val = 0;
			$work_val = 0;
			$star_num = 0;
		}

		return $TOPIC_RESULT;
	}
	// add okabe end 2017/06/20

}
