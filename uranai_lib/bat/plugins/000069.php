<?php
/**
 * @author Azet
 * @date 2016-03-15
 * @url http://uranai.eek.jp/cgibin/uranai13/uranai.cgi
 */
class Zodiac000069 extends UranaiPlugin {

	/**
	 * このファンクションは必要です！
	 * 星座（インデックス） => 順位
	 * @return array[star] = rank;
	 */
	function run($URL) {
		// RESULTの形：12星座分, 1~12
		// $RESULT[<星座番号>] = <ランキング>
		// ランキングも1~12値です
		$RESULT = array();
		$RESULT2 = array();

		$CONTENTS = $this->load($URL);
		// 日付の表示があれば、今日の日付と一致するか確認する！
		// (このプラグインでは、取得時に日付設定をしているので、チェックしていません)
		$now = self::getToday();
		// nowのキーは: year,month,day
		// monthは1~12の値
		// dayは1~31の値
		$date_pattern = "/^{$now['year']}\x20年\x20{$now['month']}\x20月\x20{$now['day']}\x20日<\//";

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
		$star_num = 0;
		$check_point = 0;	//データ探す際に使用するフラグ
		$rank_sum_val = 0;	//ポイント計算用
		foreach ($LINES AS $line) {
			if (count($RESULT) == 12) { break; }

			// date check
			if(!$date_check_ok) {
				$date_check_ok = preg_match($date_pattern, $line);
			}

			// rank 通常
			if ($date_check_ok) {

				if ($star_num == 0) {	//星座名の抽出
					$flag = preg_match("/.*rowspan=3>(.*座)<\//", $line, $MATCHES);
					if ($flag) {
						$star_name = $MATCHES[1];
						$star_num = $star[$star_name];
						$check_point = 0;	//データ探す際に使用するフラグ
						$rank_sum_val = 0;	//ポイント計算用
						continue;
					}

				} else {
					if ($check_point == 0) {	//"恋愛" 出現待ち
						if (preg_match("/<th>恋愛<\/th>/", $line)) {
							preg_match("/.*\x20(\d{1,2})<\/td>.*/", $line, $MATCHES);
							$rank_sum_val += intval($MATCHES[1]);
							$check_point = 1;	//"おかね"出現待ちへ進む
							continue;
						}
					}
					if ($check_point == 1) {	//"おかね" 出現待ち
						if (preg_match("/<th>おかね<\/th>/", $line)) {
							preg_match("/.*\x20(\d{1,2})<\/td>.*/", $line, $MATCHES);
							$rank_sum_val += intval($MATCHES[1]);
							$check_point = 2;	//"勉強・お仕事"出現待ちへ進む
							continue;
						}
					}
					if ($check_point == 2) {	//"勉強・お仕事" 出現待ち
						if (preg_match("/<th>勉強・お仕事<\/th>/", $line)) {
							preg_match("/.*\x20(\d{1,2})<\/td>.*/", $line, $MATCHES);
							$rank_sum_val += intval($MATCHES[1]);
							$check_point = 0;	//１つの星座分を完了
							// RESULTの形：
							// $RESULT[<星座番号>] = <ポイント>
							$RESULT[$star_num] = $rank_sum_val;
							$rank_sum_val = 0;
							$star_num = 0;
							continue;
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
}
