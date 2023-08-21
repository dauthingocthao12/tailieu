<?php
/**
 * @author Azet
 * @date 2016-03-23
 * @url http://panasonic.co.jp/pcmc/le/sps/horoscope/
 * uodated: okabe 2017/06/19
*/
class Zodiac000075 extends UranaiPlugin {

	/**
	 * このファンクションは必要です！
	 * 星座（インデックス） => 順位
	 * @return array[star] = rank;
	 */

	//function run($URL) {		//del okabe 2017/06/19
	function run($CONTENTS) {   // add okabe 2017/06/19 $URL -> $CONTENTS

		/* del okabe start 2017/06/19
		foreach ($URL as $key => $url) {
			$url = str_replace("(Ymd)", date("Ymd"), $url);
			$URL[$key] = $url;
		}
	    */ // del okabe end 2017/06/19

		// RESULTの形：12星座分, 1~12
		// $RESULT[<星座番号>] = <ランキング>
		// ランキングも1~12値です
		$RESULT = array();
		$RESULT2 = array();

		//$CONTENTS = $this->load($URL);		// del okabe 2017/06/19

		// 日付の表示があれば、今日の日付と一致するか確認する！
		// (このプラグインでは、取得時に日付設定をしているので、チェックしていません)
		$now = self::getToday();
		// nowのキーは: year,month,day
		// monthは1~12の値
		// dayは1~31の値
		$date_pattern = "/class=\"date\">{$now['year']}年0?{$now['month']}月0?{$now['day']}日の運勢<\//";
	
		// サイト毎に星座名の設定
		$star = self::$starDefault;

		//星座ごとにループする
		for ($i = 1; $i < 13; $i++) {

			// このプラグインでは、1~12の星座ごとのURLを使用する
			$content = $CONTENTS[$i];
//			$content = mb_convert_encoding($content, "UTF-8","SJIS");

			$LINES = explode("\n", $content);

			$date_check_ok = false;
			$star_num = 0;
			$check_point = 0;	//データ探す際に使用するフラグ
			$rank_sum_val = 0;	//ポイント計算用

			foreach ($LINES AS $line) {
				if (count($RESULT) == 12) { break; }
				if (isset($RESULT[$i])) { break; }

				// date check
				if(!$date_check_ok) {
					$date_check_ok = preg_match($date_pattern, $line);
					continue;
				}

				// rank 通常
				if ($date_check_ok) {

					if ($star_num == 0) {	//星座名の抽出
						$flag = preg_match("/alt=\"(.*座)\"/", $content, $MATCHES);
						if ($flag) {
							$star_name = $MATCHES[1];
							$star_num = $star[$star_name];
							$check_point = 0;	//データ探す際に使用するフラグ
							$rank_sum_val = 0;	//ポイント計算用
							continue;
						}

					} else {

						if ($check_point == 0) {	//"総合運" 出現待ち
							if (preg_match("/<dt>総合運<\/dt>/", $line)) {
								$check_point = 1;	//"総合運の次の行から、マークを探す"
								continue;
							}
						}
						if ($check_point == 1) {	//"総合運"の次行のマークチェック
							$check_point = 2;
							$flg = preg_match("/src=\"\.\.\/img\/general(\d{1,2})\.gif\"/", $line, $MATCHES);
							if ($flg) {
								$rank_sum_val += intval($MATCHES[1]);
							}
							continue;
						}

						if ($check_point == 2) {	//"恋愛運" 出現待ち
							if (preg_match("/<dt>恋愛運<\/dt>/", $line)) {
								$check_point = 3;	//"恋愛運の次の行から、マークを探す"
								continue;
							}
						}

						if ($check_point == 3) {	//"恋愛運"の次行のマークチェック
							$check_point = 4;
							$flg = preg_match("/src=\"\.\.\/img\/love(\d{1,2})\.gif\"/", $line, $MATCHES);
							if ($flg) {
								$rank_sum_val += intval($MATCHES[1]);
							}
							continue;
						}

						if ($check_point == 4) {	//"金運" 出現待ち
							if (preg_match("/<dt>金運<\/dt>/", $line)) {
								$check_point = 5;	//"金運の次の行から、マークを探す"
								continue;
							}
						}
						if ($check_point == 5) {	//"金運"の次行のマークチェック
							$check_point = 6;
							$flg = preg_match("/src=\"\.\.\/img\/money(\d{1,2})\.gif\"/", $line, $MATCHES);
							if ($flg) {
								$rank_sum_val += intval($MATCHES[1]);
							}
							continue;
						}

						if ($check_point == 6) {	//"仕事運" 出現待ち
							if (preg_match("/<dt>仕事運<\/dt>/", $line)) {
								$check_point = 7;	//"仕事運の次の行から、マークを探す"
								continue;
							}
						}
						if ($check_point == 7) {	//"仕事運"の次行のマークチェック
							$check_point = 0;	//１つの星座分を完了
							$flg = preg_match("/src=\"\.\.\/img\/work(\d{1,2})\.gif\"/", $line, $MATCHES);
							if ($flg) {
								$rank_sum_val += intval($MATCHES[1]);
							}
							// RESULTの形：
							// $RESULT[<星座番号>] = <ポイント>
							$RESULT[$star_num] = $rank_sum_val;
							$rank_sum_val = 0;
							break;
						}

					}

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


	// add okabe start 2017/06/19
	function topic_run($TOPIC_CONTENTS) {
		$TOPIC_RESULT = array();

		// date check
		$now = self::getToday();
		$star = self::$starDefault;

		foreach($TOPIC_CONTENTS AS $key => $topic_content) {
			//$topic_content = mb_convert_encoding($topic_content, "UTF-8", "EUC-JP");
			$TOPIC_LINES = explode("\n", $topic_content);
			$date_check = false;
			$star_num = 0;

			foreach ($TOPIC_LINES as $topic_line) {
				// Date check
				if(!$date_check) {
					$date_check = preg_match("!\"date\">{$now['year']}年0?{$now['month']}月0?{$now['day']}日の運勢</p>!", $topic_line);
				}
				// star
				if ($star_num ==0 && preg_match("!0?{$now['month']}月0?{$now['day']}日の(.*座)\s!", $topic_line, $MATCHES)) {
					$star_name = $MATCHES[1];
					$star_num = $star[$star_name];
				}


				//love
				if ($date_check && $star_num > 0 && preg_match("!<dd><img src=\"\.\./img/love(\d{2})\.gif\" alt=\"!", $topic_line, $MATCHES)){
					$love = $MATCHES[1];
					//echo "****". (intVal($love)*20)."****\n";
					$love_num = intVal($love)*20;
				}

				//money
				if ($date_check && $star_num > 0 && preg_match("!<dd><img src=\"\.\./img/money(\d{2})\.gif\" alt=\"!", $topic_line, $MATCHES)){
					$money = $MATCHES[1];
					//echo "****". (intVal($money)*20)."****\n";
					$money_num = intVal($money)*20;
				}

				//work
				if ($date_check && $star_num > 0 && preg_match("!<dd><img src=\"\.\./img/work(\d{2})\.gif\" alt=\"!", $topic_line, $MATCHES)){
					$work = $MATCHES[1];
					//echo "****". (intVal($work)*20)."****\n";
					$work_num = intVal($work)*20;
				}

			}
			$TOPIC_RESULT[$star_num] = array("love"=> $love_num , "money" => $money_num ,"work" => $work_num);

			if(!$date_check) {
				print $this->logDateError().PHP_EOL;
			}

		}
		return $TOPIC_RESULT;
	}
	// add okabe end 2017/06/19

}
