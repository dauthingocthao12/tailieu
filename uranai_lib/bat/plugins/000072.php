<?php
/**
 * @author Azet
 * @date 2016-03-18
 * @url http://divination777.jugem.jp/
 */
class Zodiac000072 extends UranaiPlugin {

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
		$date_pattern = "/{$now['year']}年0?{$now['month']}月0?{$now['day']}日/";

		// サイト毎に星座名の設定
		$star = self::$starDefault;

		// このプラグインは、０ の URL データしか使用しません
		$content = $CONTENTS[0];
		$content = mb_convert_encoding($content, "UTF-8","EUC-JP");
		$LINES = explode("\n", $content);

		$date_check_ok = false;
		$star_num = 0;
		foreach ($LINES AS $line) {
			if (count($RESULT) == 12) { break; }

			// date check
			if(!$date_check_ok) {
				$date_check_ok = preg_match($date_pattern, $line);
			}

			// rank 通常
			if ($date_check_ok) {
				//行を、さらに分解する
				$line = strtolower($line);
				$line = str_replace("<br />", "<br>", $line);
				$line = str_replace("<br/>", "<br>", $line);
				$LINES2 = explode("<br>", $line);
				foreach ($LINES2 AS $line2) {
					//$flg = preg_match("/^■(.*座).*</", $line2, $MATCHES);	//del okabe 2016/05/17
					$flg = preg_match("/>■(.*座).*</", $line2, $MATCHES);	//add okabe 2016/05/17
					if ($flg && ($star_num == 0)) {
						$star_name = $MATCHES[1];
						$star_num = $star[$star_name];
						$rank_sum_val = 0;	//ポイント集計用
						$sum_count = 0;		//集計項目のカウント用
						continue;
					}

					if ($star_num > 0) {	//星座名が見つかっている状態であること
						$flg = preg_match("/【.*運】.*【(\d{1,2})】/", $line2, $MATCHES);
						if ($flg) {
							$sum_count++;
							$rank_sum_val += intval($MATCHES[1]);
						}
					}

					if ($sum_count == 4 && $star_num > 0) {	//すべての項目処理終了
						// RESULTの形：
						// $RESULT[<星座番号>] = <ポイント>
						$RESULT[$star_num] = $rank_sum_val;
						$star_num = 0;
						continue;
					}

				}

			}

		}

// $RESULT[<星座番号>] = 点数
//print_r($RESULT);
//exit;
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
