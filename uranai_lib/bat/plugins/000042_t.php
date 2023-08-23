<?php
/**
 * @author Azet
 * @date 2016-02-24
 * @url http://uranai.nosv.org/seizarank.php?ipn=pc
 * updated: okabe 2017/06/21
 */
class Zodiac000042 extends UranaiPlugin {

	/**
	 * このファンクションは必要です！
	 * 星座（インデックス） => 順位
	 * @return array[star] = rank;
	 */
	//function run($URL) {			//del okabe 2017/06/21
	function run($CONTENTS) {		// add okabe 2017/06/21 $URL -> $CONTENTS
		// RESULTの形：12星座分, 1~12
		// $RESULT[<星座番号>] = <ランキング>
		// ランキングも1~12値です
		$RESULT = array();

		//$CONTENTS = $this->load($URL);		//del okabe 2017/06/21
		// 日付の表示があれば、今日の日付と一致するか確認する！
		// (このプラグインでは、取得時に日付設定をしているので、チェックしていません)
		$now = self::getToday();
		// nowのキーは: year,month,day
		// monthは1~12の値
		// dayは1~31の値
		$date_pattern = "/今日\x20\({$now['year']}年{$now['month']}月{$now['day']}日\)\x20の星座占い/";

        // サイト毎に星座名の設定
		$star = self::$starDefault;

		// このプラグインは、０ の URL データしか使用しません
		$content = $CONTENTS[0];
		$content = mb_convert_encoding($content, "UTF-8","EUC-JP");
		$LINES = explode("<br>", $content);

		$date_check_ok = false;
		foreach ($LINES AS $line) {
			if (count($RESULT) == 12) { break; }

			// date check
			if(!$date_check_ok) {
				$date_check_ok = preg_match($date_pattern, $line);
			}

			// rank 通常
			if ($date_check_ok && preg_match("/>\x20(\d{1,2})位\x20(.*座)\x20/", $line, $MATCHES)) {
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


	// add okabe start 2017/06/19
	function topic_run($TOPIC_CONTENTS) {
		$TOPIC_RESULT = array();

		// 日付の表示があれば、今日の日付と一致するか確認する！
		// (このプラグインでは、取得時に日付設定をしているので、チェックしていません)
		$now = self::getToday();
		// nowのキーは: year,month,day
		// monthは1~12の値
		// dayは1~31の値
		$date_pattern = "/今日\x20\({$now['year']}年{$now['month']}月{$now['day']}日\)\x20の星座占い/";

		// サイト毎に星座名の設定
		$star = self::$starDefault;

		// このプラグインは、０ の URL データしか使用しません
		$content = $TOPIC_CONTENTS[0];
		$content = mb_convert_encoding($content, "UTF-8","EUC-JP");
		$LINES = explode("<br>", $content);

		$star_num = 0;
		$love_value = -1;
		$money_value = -1;
		$work_value = -1;
		$health_value = -1;

		$date_check_ok = false;
		foreach ($LINES AS $line) {
			if (count($TOPIC_RESULT) == 12) { break; }

			// date check
			if(!$date_check_ok) {
				$date_check_ok = preg_match($date_pattern, $line);
			}

			// rank 通常
			if ($date_check_ok) {

				if ($star_num == 0) {
					$chk = preg_match("/>\x20(\d{1,2})位\x20(.*座)\x20/", $line, $MATCHES);
					if ($chk) {
						$star_name = $MATCHES[2];
						$star_num = $star[$star_name];
					}
				}

				if ($star_num > 0) {
					$chk = preg_match("/恋愛運：<span\sstyle=\"color:#FF1493\">[☆★]*<\/span>(.*)Pt/", $line, $MATCHES);
					if ($chk) {
						$love_value = intVal($MATCHES[1]);
					}
				}

				if ($star_num > 0) {
					$chk = preg_match("/金　運：<span\sstyle=\"color:#FFA500\">[☆★]*<\/span>(.*)Pt/", $line, $MATCHES);
					if ($chk) {
						$money_value = intVal($MATCHES[1]);
					}
				}

				if ($star_num > 0) {
					$chk = preg_match("/仕事運：<span\sstyle=\"color:#00AFBF\">[☆★]*<\/span>(.*)Pt/", $line, $MATCHES);
					if ($chk) {
						$work_value = intVal($MATCHES[1]);
					}
				}

				if ($star_num > 0) {
					$chk = preg_match("/健康運：<span\sstyle=\"color:#0000FF\">[☆★]*<\/span>(.*)Pt/", $line, $MATCHES);
					if ($chk) {
						$health_value = intVal($MATCHES[1]);

						$love_num = $love_value * 20;
						$money_num = $money_value * 20;
						$work_num = $work_value * 20;
						$health_num = $health_value * 20;
						$TOPIC_RESULT[$star_num] = array("love"=> $love_num , "money" => $money_num ,"work" => $work_num,"health" => $health_num);

						$star_num = 0;
						$love_value = -1;
						$money_value = -1;
						$work_value = -1;
						$health_value = -1;
					}
				}

			}

		}

		// date error?
		if(!$date_check_ok) {
			print $this->logDateError().PHP_EOL;
		}

		return $TOPIC_RESULT;
	}
	// add okabe end 2017/06/19

}
