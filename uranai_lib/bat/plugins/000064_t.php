<?php
/**
 * @author Azet
 * @date 2016-03-09
 * @url http://cocoloni.jp/daily_ranking/
 * updated: okabe 2017/06/20
 */
class Zodiac000064 extends UranaiPlugin {

	/**
	 * このファンクションは必要です！
	 * 星座（インデックス） => 順位
	 * @return array[star] = rank;
	 */
	//function run($URL) {		//del okabe 2017/06/20
	function run($CONTENTS) {	// add okabe 2017/06/20 $URL -> $CONTENTS
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
		$date_pattern = "/class=\"date\">{$now['year']}年{$now['month']}月{$now['day']}日/";

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
		$LINES = explode("number", $content);

		$date_check_ok = false;
		$rank_num = 1;
		foreach ($LINES AS $line) {
			if (count($RESULT) == 12) { break; }

			// date check
			if(!$date_check_ok) {
				$date_check_ok = preg_match($date_pattern, $line);
			}

			// rank 通常
			if ($date_check_ok && preg_match("/class=\"name\">(.*座)<\//", $line, $MATCHES)) {
				$star_name = $MATCHES[1];	//(.*?座)
				$star_num = $star[$star_name];
				// RESULTの形：
				// $RESULT[<星座番号>] = <ランキング>
				$RESULT[$star_num] = $rank_num;
				$rank_num++;
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

		// date check
		$now = self::getToday();
		//$star = self::$starDefault;
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

		foreach($TOPIC_CONTENTS AS $key => $topic_content) {
			$TOPIC_LINES = explode("\n", $topic_content);
			$date_check = false;
			$star_num = 0;
			$love_num = -1;

			foreach ($TOPIC_LINES as $topic_line) {
				//print $topic_line;
				// Date check
				if(!$date_check) {
			        $date_check = preg_match("!\"date\">{$now['year']}年{$now['month']}月{$now['day']}日!", $topic_line);
				}
				// star
				if ($star_num == 0 && preg_match("!\"name\">(.*座)</p>!", $topic_line, $MATCHES)) {
			        $star_name = $MATCHES[1];
			        $star_num = $star[$star_name];
				}

				if ($star_num > 0 && $date_check) {
					$SPLITS_LINE = explode("</dd>", $topic_line);
					foreach ($SPLITS_LINE as $s_line) {
						$chk = preg_match("!<dt>恋愛指数</dt><dd>.*!", $s_line);
						if ($chk) {
							$cnt = mb_substr_count($s_line, "★");
							$love_num = $cnt * 20;
						}
					}
					continue;
				}
			}

			if ($love_num >= 0) {
				$TOPIC_RESULT[$star_num] = array("love"=> $love_num , "money" => NULL ,"work" => NULL);
			} else {
				$TOPIC_RESULT[$star_num] = array("love"=> NULL , "money" => NULL ,"work" => NULL);
			}
			if(!$date_check) {
				print $this->logDateError().PHP_EOL;
			}

		}

		return $TOPIC_RESULT;
	}
	// add okabe end 2017/06/20

}
