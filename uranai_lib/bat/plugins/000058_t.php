<?php
/**
 * @author Azet
 * @date 2016-03-08
 * @url http://www.so-net.ne.jp/fortunes/today/DispConstel.cgi?constellation=HO01 ...
 * updated: okabe 2017/06/20
 */
class Zodiac000058 extends UranaiPlugin {

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
		$RESULT2 = array();

		//$CONTENTS = $this->load($URL);	//del okabe 2017/06/20
		// 日付の表示があれば、今日の日付と一致するか確認する！
		// (このプラグインでは、取得時に日付設定をしているので、チェックしていません)
		$now = self::getToday();
		// nowのキーは: year,month,day
		// monthは1~12の値
		// dayは1~31の値
		$date_pattern = "/{$now['year']}年0?{$now['month']}月0?{$now['day']}日\（/";

		// サイト毎に星座名の設定
		//$star = self::$starDefault;

		//星座ごとにループする
		for ($i = 1; $i < 13; $i++) {
			// このプラグインでは、1~12の星座ごとのURLを使用する
			$content = $CONTENTS[$i];
			//$content = mb_convert_encoding($content, "UTF-8","EUC-JP");

			$date_check_ok = false;
			if(!$date_check_ok) {
				$date_check_ok = preg_match($date_pattern, $content);
			}

			// rank 取り出し
			if ($date_check_ok) {
				$flag = preg_match("/class=\"figure\">(.*)<\/span>点/", $content, $MATCHES);
				if($flag) {
					$point_num = $MATCHES[1];
					$star_num = $i;
					// RESULTの形：
					// $RESULT[<星座番号>] = 点数
					$RESULT[$star_num] = $point_num;
				}
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


	// add okabe start 2017/06/20
	function topic_run($TOPIC_CONTENTS) {
		$TOPIC_RESULT = array();

		// 日付の表示があれば、今日の日付と一致するか確認する！
		// (このプラグインでは、取得時に日付設定をしているので、チェックしていません)
		$now = self::getToday();
		// nowのキーは: year,month,day
		// monthは1~12の値
		// dayは1~31の値
		$date_pattern = "/{$now['year']}年0?{$now['month']}月0?{$now['day']}日\（/";

		// サイト毎に星座名の設定
		//$star = self::$starDefault;

		//星座ごとにループする
		for ($i = 1; $i < 13; $i++) {
			// このプラグインでは、1~12の星座ごとのURLを使用する
			$all_lines = $TOPIC_CONTENTS[$i];
			//$content = mb_convert_encoding($content, "UTF-8","EUC-JP");

			$date_check_ok = false;
			if(!$date_check_ok) {
				$date_check_ok = preg_match($date_pattern, $all_lines);
			}

			$star_num = $i;
			$skip_line1 = 0;
			$skip_line2 = 0;
			$skip_line3 = 0;
			$love_val = -1;
			$money_val = -1;
			$work_val = -1;
			$topic_lines = explode("\n", $all_lines);

			foreach($topic_lines AS $key => $content) {

				if ($love_val < 0) {
					if ($skip_line2 == 0 ) {
						$chk = preg_match("!class=\"item\">恋愛運</div>!", $content);
						if ($chk) {
							$skip_line2 = 1;
						}
					} else if ($skip_line2 == 1) {
						$skip_line2 = 2;
					} else {
						$cnt = substr_count($content, "/point_love1");
						$love_val = $cnt * 20;
						$skip_line2 = 0;
					}
				}

				if ($money_val < 0) {
					if ($skip_line1 == 0 ) {
						$chk = preg_match("!class=\"item\">金銭運</div>!", $content);
						if ($chk) {
								$skip_line1 = 1;
						}
					} else if ($skip_line1 == 1) {
						$skip_line1 = 2;
					} else {
						$cnt = substr_count($content, "/point_money1");
						$money_val = $cnt * 20;
						$skip_line1 = 0;
					}
				}

				if ($work_val < 0) {
				 	if ($skip_line3 == 0 ) {
						 $chk = preg_match("!class=\"item\">仕事運</div>!", $content);
						 if ($chk) {
							 $skip_line3 = 1;
				 		}
					} else if ($skip_line3 == 1) {
						 $skip_line3 = 2;
					} else {
						 $cnt = substr_count($content, "/point_job1");
						 $work_val = $cnt * 20;
						 $skip_line3 = 0;
					 }
				}

			}
			if ($love_val >=0 && $money_val >= 0 && $work_val >= 0) {
				$TOPIC_RESULT[$star_num] = array("love"=> $love_val , "money" => $money_val ,"work" => $work_val);
				$star_num = 0;
				$skip_line1 = 0;
				$skip_line2 = 0;
				$skip_line3 = 0;
				$love_val = -1;
				$money_val = -1;
				$work_val = -1;
				$date_check_ok = false;
			}
		}

		return $TOPIC_RESULT;
	}
	// add okabe end 2017/06/20

}
