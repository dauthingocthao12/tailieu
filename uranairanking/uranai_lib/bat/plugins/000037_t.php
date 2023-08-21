<?php
/**
 * @author Azet
 * @date 2016-02-05
 * @url http://www.kanematsu-group.co.jp/fortune/
 * updated: okabe 2017/06/22
 */
class Zodiac000037 extends UranaiPlugin {

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
		//$CONTENTS = array();		//del okabe 2017/06/22

		//結果を格納する配列
		$RESULT = array();

		// サイト毎に星座名の設定
		$star = self::$starDefault;

		// 日付の表示があれば、今日の日付と一致するか確認する！
		$now = self::getToday();
		// nowのキーは: year,month,day
		// monthは1~12の値
		// dayは1~31の値
		//$date_pattern = "/<p class=\"date\">{$now['year']}年{$monthx}月{$oneday}日.*<\/p>/";
		$date_pattern = "/>{$now['month']}月{$now['day']}日運勢ランキング/";

		// サイトhtmlを取得
		//$CONTENTS = $this->load($URL);		//del okabe 2017/06/22

		// このプラグインでは、１ファイルで12星座の情報を抽出する
		$content = $CONTENTS[0];
		//$content = mb_convert_encoding($content, "UTF-8","SJIS");	//不要 元々がUTF-8

		$LINES = explode("\n", $content);

		//１行ずつパース処理する
		$rank_num = 0;		//ランク値の格納先
		$date_check_ok = false;		//日付のパースチェック結果

		foreach ($LINES AS $line) {
			if (count($RESULT) == 12) { break; }

			// date check
			if(!$date_check_ok) {
				$date_check_ok = preg_match($date_pattern, $line);	//日付チェック結果格納
			}

			// parse datas
			if ($date_check_ok) {
				if (preg_match("/alt=\"(\d{1,2})位\".*alt=\"(.*?座)\"/", $line, $MATCHES)) {
					$rank_num = $MATCHES[1];		//(\d{1,2})位
					$star_name_jp = $MATCHES[2];	//(.*?座)
					$star_num = $star[$star_name_jp];

					// RESULTの形：
					// $RESULT[<星座番号>] = <ランキング>
					$RESULT[$star_num] = $rank_num;
				}
			}

		}

		return $RESULT;
	}



	// add okabe start 2017/06/22
	function topic_run($TOPIC_CONTENTS) {
		$TOPIC_RESULT = array();

		// サイト毎に星座名の設定
		$star = self::$starDefault;

		// 日付の表示があれば、今日の日付と一致するか確認する！
		$now = self::getToday();
		// nowのキーは: year,month,day
		// monthは1~12の値
		// dayは1~31の値
		//$date_pattern = "/<p class=\"date\">{$now['year']}年{$monthx}月{$oneday}日.*<\/p>/";
		$date_pattern = "/>{$now['month']}月{$now['day']}日運勢ランキング/";


		//星座ごとにループする
		for ($i = 1; $i < 13; $i++) {
			// このプラグインでは、1~12の星座ごとのURLを使用する
			$content = $TOPIC_CONTENTS[$i];
			//星座ごとのURLを１つずつパース処理する

			$star_num = 0;
			$date_check_ok = false;         //日付のパースチェック結果

			$love_val =-1;
			$money_val =-1;
			$work_val =-1;
			$skip_flg1 = 0;
			$skip_flg2 = 0;
			$skip_flg3 = 0;

			$LINES = explode("\n", $content);

			foreach ($LINES AS $line) {
				if (count($TOPIC_RESULT) == 12) { break; }

				// date check
				if(!$date_check_ok) {
					$date_check_ok = preg_match($date_pattern, $content);   //日付チェッ ク結果格納
				}

				// parse datas
				if ($star_num == 0) {
					if (preg_match("/alt=\"(.*?座)\"/", $line, $MATCHES)) {
						$star_name = $MATCHES[1];
						$star_num = $star[$star_name];
					}
				}

				//恋愛運
				if($star_num > 0 && $date_check_ok) {
					if ($skip_flg1 == 0) {
						$chk = preg_match("/class=\"love\">愛情運<\/th>/", $line);
						if ($chk) {
							$skip_flg1 = 1;
						}
					} else {
						$chk = preg_match("/\/star_(\d{1})\.gif\"/", $line, $MATCHS);
						$love_val = intVal($MATCHS[1]);
						$skip_flg1 = 0;
					}
				}

				//金銭運
				if($star_num > 0 && $date_check_ok) {
					if ($skip_flg2 == 0) {
						$chk = preg_match("/class=\"money\">金運<\/th>/", $line);
						if ($chk) {
							$skip_flg2 = 1;
						}
					} else {
						$chk = preg_match("/\/star_(\d{1})\.gif\"/", $line, $MATCHS);
						$money_val = intVal($MATCHS[1]);
						$skip_flg2 = 0;
					}
				}

				//仕事運
				if($star_num > 0 && $date_check_ok) {
					if ($skip_flg3 == 0) {
						$chk = preg_match("/class=\"work\">仕事運<\/th>/", $line);
						if ($chk) {
							$skip_flg3 = 1;
						}
					} else {
						$chk = preg_match("/\/star_(\d{1})\.gif\"/", $line, $MATCHS);
						$work_val = intVal($MATCHS[1]);
						$skip_flg3 = 0;
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
