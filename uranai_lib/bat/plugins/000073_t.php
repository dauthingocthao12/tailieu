<?php
/**
 * @author Azet
 * @date 2016-03-22
 * @url http://www.propel.ne.jp/~hisatomi/palette/cgi/unsei.cgi
 * uodated: okabe 2017/06/19
*/
class Zodiac000073 extends UranaiPlugin {

	/**
	 * このファンクションは必要です！
	 * 星座（インデックス） => 順位
	 * @return array[star] = rank;
	 */
	//function run($URL) 		//del okabe 2017/06/19
	function run($CONTENTS) {	// add okabe 2017/06/19 $URL -> $CONTENTS
		// RESULTの形：12星座分, 1~12
		// $RESULT[<星座番号>] = <ランキング>
		// ランキングも1~12値です
		$RESULT = array();
		$RESULT2 = array();

		//$CONTENTS = $this->load($URL);	// del okabe 2017/06/19
		// 日付の表示があれば、今日の日付と一致するか確認する！
		// (このプラグインでは、取得時に日付設定をしているので、チェックしていません)
		$now = self::getToday();
		// nowのキーは: year,month,day
		// monthは1~12の値
		// dayは1~31の値
		$date_pattern = "/<center>{$now['month']}月{$now['day']}日\x20.*<\/center>/";

		// サイト毎に星座名のプラグイン個別設定
		$star = array();
		$star["水瓶座"] = 1;
		$star["魚　座"] = 2;
		$star["牡羊座"] = 3;
		$star["牡牛座"] = 4;
		$star["双子座"] = 5;
		$star["蟹　座"] = 6;
		$star["獅子座"] = 7;
		$star["乙女座"] = 8;
		$star["天秤座"] = 9;
		$star["蠍　座"] = 10;
		$star["射手座"] = 11;
		$star["山羊座"] = 12;

		// このプラグインは、０ の URL データしか使用しません
		$content = $CONTENTS[0];
		$content = mb_convert_encoding($content, "UTF-8","SJIS");
		$LINES = explode("\n", $content);

		$date_check_ok = false;
		$star_num = 0;
		$cnt = 0;	//星座名出現後の行カウント用
		$rank_score = 0;	//点数集計用
		foreach ($LINES AS $line) {
			if (count($RESULT) == 12) { break; }

			// date check
			if(!$date_check_ok) {
				$date_check_ok = preg_match($date_pattern, $line);
				continue;
			}

			// rank 通常
			if ($date_check_ok) {
				if ($star_num == 0) {
					//星座が出てくるのを探す
					$flg = preg_match("/align=\"center\">(.*座)<\/td>/", $line, $MATCHES);
					if ($flg) {
						$star_name_jp = $MATCHES[1];	//(.*?座)
						$star_num = $star[$star_name_jp];
						$cnt = 0;
						$rank_score = 0;
					}
					continue;

				} else {
					if ($cnt == 0) {
						$cnt = 1;
						continue;
					}
					$flg = preg_match("/align=\"center\">(\d{1,3})<\/td>/", $line, $MATCHES);
					$cnt++;
					if ($flg) {
						$rank_score += intval($MATCHES[1]);
						if ($cnt == 5) {
							// RESULTの形：
							// $RESULT[<星座番号>] = <ポイント>
							$RESULT[$star_num] = $rank_score;
							$star_num = 0;
						} else {
							continue;
						}
					} else {
						continue;
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

		//$CONTENTS = $this->load($URL);        // del okabe 2017/06/19
		// 日付の表示があれば、今日の日付と一致するか確認する！
		// (このプラグインでは、取得時に日付設定をしているので、チェックしていません)
		$now = self::getToday();
		// nowのキーは: year,month,day
		// monthは1~12の値
		// dayは1~31の値
		$date_pattern = "/<center>{$now['month']}月{$now['day']}日\x20.*<\/center>/";

		// サイト毎に星座名のプラグイン個別設定
		$star = array();
		$star["水瓶座"] = 1;
		$star["魚　座"] = 2;
		$star["牡羊座"] = 3;
		$star["牡牛座"] = 4;
		$star["双子座"] = 5;
		$star["蟹　座"] = 6;
		$star["獅子座"] = 7;
		$star["乙女座"] = 8;
		$star["天秤座"] = 9;
		$star["蠍　座"] = 10;
		$star["射手座"] = 11;
		$star["山羊座"] = 12;

		// このプラグインは、０ の URL データしか使用しません
		$content = $TOPIC_CONTENTS[0];
		$content = mb_convert_encoding($content, "UTF-8","SJIS");
		$LINES = explode("\n", $content);

		$date_check_ok = false;
		$star_num = 0;
		$cnt = 0;       //星座名出現後の行カウント用
		$rank_score = 0;        //点数集計用
		foreach ($LINES AS $line) {
			if (count($TOPIC_RESULT) == 12) { break; }

			// date check
			if(!$date_check_ok) {
				$date_check_ok = preg_match($date_pattern, $line);
				continue;
			}

			// rank 通常
			if ($date_check_ok) {
				if ($star_num == 0) {
					//星座が出てくるのを探す
					$flg = preg_match("/align=\"center\">(.*座)<\/td>/", $line, $MATCHES);
					if ($flg) {
						$star_name_jp = $MATCHES[1];    //(.*?座)
						$star_num = $star[$star_name_jp];
						$cnt = 0;
					}
					continue;

				} else {
					if ($cnt == 0) {
						$cnt = 1;
						$love_num_female = 0;
						$love_num_male = 0;
						$money_num = 0;
						$work_num = 0;
						continue;
					}
					$flg = preg_match("/align=\"center\">(\d{1,3})<\/td>/", $line, $MATCHES);
					$cnt++;
					if ($flg) {
//echo "*".$cnt.": ".intval($MATCHES[1])."*\n";
						if ($cnt == 5) {
							//$love_num = intVal( ($love_num_female + $love_num_male) / 2);
							//$TOPIC_RESULT[$star_num] = array("love"=> $love_num , "money" => $money_num ,"work" => NULL);
							$TOPIC_RESULT[$star_num] = array("love"=> NULL , "money" => $money_num ,"work" => NULL);
							$star_num = 0;
						} else {
							if ($cnt == 2) {	//恋愛・女性
								$love_num_female = intVal($MATCHES[1]);
							}
							if ($cnt == 3) {	//恋愛・男性
								$love_num_male = intVal($MATCHES[1]);
							}
							if ($cnt == 4) {	//金運
								$money_num = intVal($MATCHES[1]);
							}
							//if ($cnt == 5) {	//健康運
							//}
							continue;
						}
					} else {
						continue;
					}

				}

			}

		}


		return $TOPIC_RESULT;
	}
	// add okabe end 2017/06/19

}
