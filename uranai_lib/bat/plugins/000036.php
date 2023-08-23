<?php
/**
 * @author Azet
 * @date 2016-02-05
 * @url https://www.otenki.jp/sp/art/fortune/
 */
class Zodiac000036 extends UranaiPlugin {

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
		$CONTENTS = array();

		// サイトhtmlを取得
		$CONTENTS = $this->load($URL);
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
}
