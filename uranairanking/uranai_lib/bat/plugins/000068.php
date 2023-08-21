<?php
/**
 * @author Azet
 * @date 2016-03-15
 * @url http://www.sanspo.com/today/uranai.html
 */
class Zodiac000068 extends UranaiPlugin {

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
		$date_pattern = "/イレーネの西洋占星術.*{$now['month']}月{$now['day']}日/";

		// サイト毎に星座名の設定
		$star = self::$starDefault;

		// このプラグインは、０ の URL データしか使用しません
		$content = $CONTENTS[0];
		//$content = mb_convert_encoding($content, "UTF-8","SJIS");
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

					$flag = preg_match("/alt=\"(.*座)\"/", $line, $MATCHES);
					if ($flag) {
						$star_name = $MATCHES[1];
						$star_num = $star[$star_name];
						$check_point = 0;	//データ探す際に使用するフラグ
						$rank_sum_val = 0;	//ポイント計算用
						continue;
					}

				} else {
					if ($check_point == 0) {	//"仕事" 出現待ち
						if (preg_match("/<th class=\"small\">仕事<\/th>/", $line)) {
							$check_point = 1;	//"仕事の次の行から、マークを探す"
							continue;
						}
					}
					if ($check_point == 1) {	//"仕事"の次行のマークチェック
						$check_point = 2;
						if (preg_match("/<td class=\"small\">☆<\/td>/", $line)) { $rank_sum_val += 1; }
						if (preg_match("/<td class=\"small\">△<\/td>/", $line)) { $rank_sum_val += 2; }
						if (preg_match("/<td class=\"small\">○<\/td>/", $line)) { $rank_sum_val += 3; }
						if (preg_match("/<td class=\"small\">◎<\/td>/", $line)) { $rank_sum_val += 4; }
						continue;
					}

					if ($check_point == 2) {	//"金運" 出現待ち
						if (preg_match("/<th class=\"small\">金運<\/th>/", $line)) {
							$check_point = 3;	//"金運の次の行から、マークを探す"
							continue;
						}
					}
					if ($check_point == 3) {	//"金運"の次行のマークチェック
						$check_point = 4;
						if (preg_match("/<td class=\"small\">☆<\/td>/", $line)) { $rank_sum_val += 1; }
						if (preg_match("/<td class=\"small\">△<\/td>/", $line)) { $rank_sum_val += 2; }
						if (preg_match("/<td class=\"small\">○<\/td>/", $line)) { $rank_sum_val += 3; }
						if (preg_match("/<td class=\"small\">◎<\/td>/", $line)) { $rank_sum_val += 4; }
						continue;
					}

					if ($check_point == 4) {	//"愛情" 出現待ち
						if (preg_match("/<th class=\"small\">愛情<\/th>/", $line)) {
							$check_point = 5;	//"愛情の次の行から、マークを探す"
							continue;
						}
					}
					if ($check_point == 5) {	//"愛情"の次行のマークチェック
						$check_point = 0;	//１つの星座分を完了
						if (preg_match("/<td class=\"small\">☆<\/td>/", $line)) { $rank_sum_val += 1; }
						if (preg_match("/<td class=\"small\">△<\/td>/", $line)) { $rank_sum_val += 2; }
						if (preg_match("/<td class=\"small\">○<\/td>/", $line)) { $rank_sum_val += 3; }
						if (preg_match("/<td class=\"small\">◎<\/td>/", $line)) { $rank_sum_val += 4; }
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
