<?php
/**
 * @author Azet
 * @date 2016-03-08
 * @url http://www.sky-club.net/lady/seiza/index.cgi
 * updated: okabe 2017/06/20
 */
class Zodiac000060 extends UranaiPlugin {

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

		//$CONTENTS = $this->load($URL);		//del okabe 2017/06/20
		// 日付の表示があれば、今日の日付と一致するか確認する！
		// (このプラグインでは、取得時に日付設定をしているので、チェックしていません)
		$now = self::getToday();
		// nowのキーは: year,month,day
		// monthは1~12の値
		// dayは1~31の値
		$date_pattern = "/{$now['year']}年{$now['month']}月{$now['day']}日<p>/";

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
		$content = mb_convert_encoding($content, "UTF-8","SJIS");
		$LINES = explode("\n", $content);

		$date_check_ok = false;
		foreach ($LINES AS $line) {
			if (count($RESULT) == 12) { break; }

			// date check
			if(!$date_check_ok) {
				$date_check_ok = preg_match($date_pattern, $line);
			}

			// rank 通常
			if ($date_check_ok) {
				$flag = preg_match("/^(\d{1,2}):(.*座)$/", $line, $MATCHES);
				if ($flag) {
					$rank_num = $MATCHES[1];
					$star_name = $MATCHES[2];
					$star_num = $star[$star_name];
					// RESULTの形：
					// $RESULT[<星座番号>] = 順位
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
		$date_pattern = "/{$now['year']}年{$now['month']}月{$now['day']}日<p>/";

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
		$content = $TOPIC_CONTENTS[0];
		$content = mb_convert_encoding($content, "UTF-8","SJIS");
		$LINES = explode("\n", $content);
		$star_num = 0;
		$skip_line = 0;
		$love_val = -1;
		$gambling_val = -1;
		$work_val = -1;

		$date_check_ok = false;
		foreach ($LINES AS $line) {
			if (count($TOPIC_RESULT) == 12) { break; }

			// date check
			if(!$date_check_ok) {
				$date_check_ok = preg_match($date_pattern, $line);
			}

			// rank 通常
			if ($date_check_ok) {
				$flag = preg_match("/^(\d{1,2}):(.*座)$/", $line, $MATCHES);
				if ($flag) {
					$star_name = $MATCHES[2];
					$star_num = $star[$star_name];
					$skip_line = 1;
				}
			}

			if ($skip_line > 0) {
				if ($skip_line == 2) {
					$chk = preg_match("/^<br>恋愛<br>(\s?\d{1,2})$/", $line, $MATCHES);
					$love_val = intVal($MATCHES[1]) * 10;

				}
				if ($skip_line == 3) {
					$chk = preg_match("/^<br>ギャンブル<br>(\s?\d{1,2})/", $line, $MATCHES);
					$gambling_val = intVal($MATCHES[1]) * 10;

				}
				if ($skip_line == 3) {
					$chk = preg_match("/<br>勉強・お仕事<br>(\s?\d{1,2})<br>/", $line, $MATCHES);
					if ($chk) {
						$work_val = intVal($MATCHES[1]) * 10;

						$TOPIC_RESULT[$star_num] = array("love"=> $love_val , "gambling" => $gambling_val ,"work" => $work_val);

						$star_num = 0;
						$skip_line = -1;
						$love_val = -1;
						$gambling_val = -1;
						$work_val = -1;
					}
				}
				$skip_line++;
			}

		}

		// date error?
		if(!$date_check_ok) {
			print $this->logDateError().PHP_EOL;
		}

		return $TOPIC_RESULT;
	}	
	// add okabe end 2017/06/20




}
