<?php

/**
 * マイナビニュース
 *
 * @author Azet
 * @date 2017-06-23
 * @url http://news.mynavi.jp/horoscope/
 */
class Zodiac000097 extends UranaiPlugin {

	function run($URL) {
		$CONTENTS = $this->load($URL);
		$content = $CONTENTS[0]; //全体URLを使用する
		/*
		 * $RESULT[星座番号] => 順位
		 */
		$RESULT = array();
		$LINES = explode("\n", $content);

		$star = self::$starDefault;

		$now = self::getToday();

		$rank_list = array('e225' => 0,'e21c' => 1, 'e21d' => 2, 'e21e' => 3, 'e21f' => 4, 'e220' => 5, 'e221' => 6,
			'e222' => 7, 'e223' => 8, 'e224' => 9);
		$date_pattern = "/class=\"date\">{$now['month']}月{$now['day']}日.*</";
		$date_check_ok = false;
		$next_explode = false;
		$star_rank = '';


		foreach ($LINES AS $L) {
			$L = mb_convert_encoding($L,"UTF-8","SJIS");
			
			/* 日付の判定 */
			if (!$date_check_ok) {
				$date_check_ok = preg_match($date_pattern, $L,$m);
				continue;
			}
			if (!$next_explode) {
				$next_explode = preg_match("/class=\"today_ranking\">/", $L);
				continue;
			}
			if ($next_explode && $date_check_ok) {
				$line = explode("/>", $L);
				
				foreach ($line AS $key => $l) {
					/* 12星座分のデータがリザルトにある時処理を抜ける */
					if (count($RESULT) == 12) {
						break;
					}
					$star_name = '';


					/* 順位,星座の取得 */

					if (preg_match("/<img border=\"0\" src=\"\/official\/img\/icon\/j\/(.*).gif\"/", $l, $MATCHES)) { //順位
						$img_num = $MATCHES[1];
						$star_rank .= $rank_list[$img_num];
					}


					if (preg_match("/a href=\".*\">(.*座)<\/a>/", $l, $m)) {
						$star_name = $m[1];
						$star_num = $star[$star_name];
						$RESULT[$star_num] = $star_rank;
						$star_rank = '';
					}
				}
				break;
			}
		}
		// エラーチェック
		if (!$date_check_ok) {
			print $this->logDateError() . PHP_EOL;
		}
		//print_r ($RESULT);
		return $RESULT;
	}

}
