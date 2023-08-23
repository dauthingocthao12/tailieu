<?php
/**
 * @author Azet
 * @date 2016-02-05
 * @url http://woman.infoseek.co.jp/fortune/horoscope/today/
 * updated: okabe 2017/06/20
 */
class Zodiac000048 extends UranaiPlugin {

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
		//$CONTENTS = array();		//del okabe 2017/06/20

		// サイトhtmlを取得
		//$CONTENTS = $this->load($URL);		//del okabe 2017/06/20
		$content = $CONTENTS[0];
		//$content = mb_convert_encoding($content, "UTF-8", "EUC-JP");
		$LINES = explode("\n", $content);

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

		// 日付の表示があれば、今日の日付と一致するか確認する！
		$now = self::getToday();
		// nowのキーは: year,month,day
		// monthは1~12の値
		// dayは1~31の値
		$date_pattern = "/<span>{$now['year']}<\/span>年<span>{$now['month']}<\/span>月<span>{$now['day']}<\/span>日/";

		$rank1_passed = false;
		$date_check_ok = false;
		$data_ptn1a = "/>1位<\/div>/";
		$data_ptn1b = "/alt=\"(.*座)\"><\/a>/";
		$data_ptn2a = "/<dt><span>(\d{1,2})<\/span>位<\/dt>/";
		$data_ptn2b = "/class=\"name\">(.*座)<\/a>/";
		$data_ptn3a = "/>12位<\/div>/";
		$data_ptn3b = "/alt=\"(.*座)\"><\/a>/";
		$rank_num = 0;

		//行ごとにパース
		foreach ($LINES AS $line) {
			if (count($RESULT) == 12) { break; }

			// date check
			if(!$date_check_ok) {
				$date_check_ok = preg_match($date_pattern, $line);
				continue;
			}

			if($date_check_ok) {
				if (!$rank1_passed) {
					$flg = preg_match($data_ptn1a, $line);	//１位の表示行
					if ($flg) {
						$rank1_passed = true;
						continue;
					}

				} else {
					if (count($RESULT) == 0) {	//まだデータが登場していない状態
						$flg = preg_match($data_ptn1b, $line, $MATCHES);	//１位の星座名があれば取り出す
						if ($flg) {
							$star_name = $MATCHES[1];
							$star_num = $star[$star_name];
							$RESULT[$star_num] = 1;
						}
						continue;

					} else {	//１位のデータを処理済みの場合、２位以降の処理
						$flg = preg_match($data_ptn2a, $line, $MATCHES);	//２位以降の情報がある行なのか確認
						if ($flg) {
							$rank_num = $MATCHES[1];
							continue;
						}

						$flg = preg_match($data_ptn3a, $line, $MATCHES);	//12位の確認
						if ($flg) {
							$rank_num = 12;
							continue;
						}

						$flg = preg_match($data_ptn2b, $line, $MATCHES);	//２位以降の星座取り出し(12位除く)
						if ($flg && $rank_num > 0) {
							$star_name = $MATCHES[1];
							$star_num = $star[$star_name];
							$RESULT[$star_num] = $rank_num;
							$rank_num = 0;
						}

						$flg = preg_match($data_ptn3b, $line, $MATCHES);	//12位の星座取り出し
						if ($flg && $rank_num > 0) {
							$star_name = $MATCHES[1];
							$star_num = $star[$star_name];
							$RESULT[$star_num] = $rank_num;
							$rank_num = 0;
						}

					}
				}

			}

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
		$date_pattern = "/<span>{$now['year']}<\/span>年<span>{$now['month']}<\/span>月<span>{$now['day']}<\/span>日/";

		// サイト毎に星座名の設定
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

		$star_num = 0;
		$love_val = -1;
		$money_val = -1;
		$work_val = -1;

		//星座ごとにループする
		for ($i = 1; $i < 13; $i++) {
			// このプラグインでは、1~12の星座ごとのURLを使用する
			$content = $TOPIC_CONTENTS[$i];

			$date_check_ok = false;

			$LINES = explode("\n", $content);

			foreach ($LINES AS $line) {
				$flg = preg_match("!<span class=\"title\">恋愛運</span><span class=\"sprite!", $line);
				if ($flg) {
					$love_val = mb_substr_count($line, "sprite-fortune-icon-heart\"");
				}
				$flg = preg_match("!<span class=\"title\">仕事運</span><span class=\"sprite!", $line);
				if ($flg) {
					$work_val = mb_substr_count($line, "sprite-fortune-icon-pencil\"");
				}
				$flg = preg_match("!<span class=\"title\">金運</span><span class=\"sprite!", $line);
				if ($flg) {
					$money_val = mb_substr_count($line, "sprite-fortune-icon-money\"");
				}
				if ($love_val >=0 && $money_val >=0 && $work_val >=0) {
					break;
				}
			}
			//マークの個数は、１～５で換算。
			$love_num = intVal($love_val * 20);
			$money_num = intVal($money_val * 20);
			$work_num = intVal($work_val * 20);
			$star_num = $i;

			$TOPIC_RESULT[$star_num] = array("love"=> $love_num , "money" => $money_num ,"work" => $work_num);

			$love_val = -1;
			$money_val = -1;
			$work_val = -1;
			$star_num = 0;

		}

		return $TOPIC_RESULT;
	}
	// add okabe end 2017/06/20

}
