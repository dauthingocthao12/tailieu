<?php
/**
 * @author Azet
 * @date 2016-02-05
 * @url https://www.otenki.jp/sp/art/fortune/
 * updated: okabe 2017/06/22
 */
class Zodiac000036 extends UranaiPlugin {

	/**
	 * このファンクションは必要です！
	 * 星座（インデックス） => 順位
	 * @return array[star] = rank;
	 */
	//function run($URL) {			//del okabe 2017/06/22
	function run($CONTENTS) {		// add okabe 2017/06/22 $URL -> $CONTENTS

		// RESULTの形：12星座分, 1~12
		// $RESULT[<星座番号>] = <ランキング>
		// ランキングも1~12値です
		$RESULT = array();
		//$CONTENTS = array();		//del okabe 2017/06/22

		// サイトhtmlを取得
		//$CONTENTS = $this->load($URL);	//del okabe 2017/06/22
		$content = $CONTENTS[0];
		//$content = mb_convert_encoding($content, "UTF-8", "EUC-JP");
		$LINES = explode("\n", $content);

		// サイト毎に星座名のプラグイン個別設定
		$star = self::$starDefault;

		// 日付の表示があれば、今日の日付と一致するか確認する！
		$now = self::getToday();
		// nowのキーは: year,month,day
		// monthは1~12の値
		// dayは1~31の値
		$date_pattern = "/class=\"caption\">{$now['day']}日の運勢ランキング<\/h2>/";

		$rank1_passed = false;
		$date_check_ok = false;
		$data_ptn1 = "/images\/rank_(.*)\.png\"/";
		$data_ptn2 = "/<strong>(.*座)<\/strong>/";
		$data_ptn0 = "/images\/rank_2\.png\"/";
		$data_ptn3 = "/images\/rank_(.*?)\.png\"(.*)<strong>(.*座)<\/strong>/";

		//行ごとにパース
		foreach ($LINES AS $line) {

			// date check
			if(!$date_check_ok) {
				$date_check_ok = preg_match($date_pattern, $line);
				continue;
			}

			if($date_check_ok) {
				if (!$rank1_passed) {
					$flg = preg_match($data_ptn1, $line);	//１位の表示行
					if ($flg) {
						$rank1_passed = true;
						continue;
					}

				} else {
					if (count($RESULT) == 0) {	//まだデータが登場していない状態
						$flg = preg_match($data_ptn2, $line, $MATCHES);	//１位の星座名があれば取り出す
						if ($flg) {
							$star_name = $MATCHES[1];
							$star_num = $star[$star_name];
							$RESULT[$star_num] = 1;
						}
						continue;

					} else {	//１位のデータを処理済みの場合
						$flg = preg_match($data_ptn0, $line);	//２位以降の情報がある行なのか確認
						if ($flg) {
							$SUB_LINES = explode("icon", $line);
							foreach ($SUB_LINES AS $subl) {
								$flg = preg_match($data_ptn3, $subl, $MATCHES);	//各順位と星座名を取り出す
								if ($flg) {
									$star_name = $MATCHES[3];
									$star_num = $star[$star_name];
									$RESULT[$star_num] = $MATCHES[1];
								}
							}
						}

					}
				}

			}

		}

		return $RESULT;
	}


	// add okabe start 2017/06/22
	function topic_run($TOPIC_CONTENTS) {
		$TOPIC_RESULT = array();

		// サイト毎に星座名のプラグイン個別設定
		$star = self::$starDefault;

		// 日付の表示があれば、今日の日付と一致するか確認する！
		$now = self::getToday();
		// nowのキーは: year,month,day
		// monthは1~12の値
		// dayは1~31の値
		$date_pattern = "/class=\"tit\">{$now['day']}日の(.*?座)の運勢/";

		//星座ごとにループする
		for ($i = 1; $i < 13; $i++) {
			// このプラグインでは、1~12の星座ごとのURLを使用する
			$content = $TOPIC_CONTENTS[$i];
			//星座ごとのURLを１つずつパース処理する

			$star_num = 0;

			$love_val =-1;
			$money_val =-1;
			$work_val =-1;

			$LINES = explode("\n", $content);

			foreach ($LINES AS $line) {
				if (count($TOPIC_RESULT) == 12) { break; }

				// parse datas
				if ($star_num == 0) {
					if (preg_match($date_pattern, $line, $MATCHES)) {
						$star_name = $MATCHES[1];
						$star_num = $star[$star_name];
					}
				}

				//恋愛運
				if($star_num > 0) {
					$chk = preg_match("/src=\"\.\/images\/love_(\d{1})\.png\"/", $line, $MATCHS);
					if ($chk) {
						$love_val = intVal($MATCHS[1]);
					}
				}

				//金銭運
				if($star_num > 0) {
					$chk = preg_match("/src=\"\.\/images\/money_(\d{1})\.png\"/", $line, $MATCHS);
					if ($chk) {
						$money_val = intVal($MATCHS[1]);
					}
				}

				//仕事運
				if($star_num > 0) {
					$chk = preg_match("/src=\"\.\/images\/work_(\d{1})\.png\"/", $line, $MATCHS);
					if ($chk) {
						$work_val = intVal($MATCHS[1]);
					}
				}

				if ($star_num > 0 && $love_val >= 0 && $money_val >= 0 && $work_val >= 0) {
					$love_num = $love_val * 20;
					$money_num = $money_val * 20;
					$work_num = $work_val * 20;

					$TOPIC_RESULT[$star_num] = array("love"=> $love_num , "money" => $money_num ,"work" => $work_num);
					$star_num = 0;
					$love_val = -1;
					$money_val = -1;
					$work_val = -1;
				}

			}
		}


		return $TOPIC_RESULT;
	}
	// add okabe end 2017/06/22

}
