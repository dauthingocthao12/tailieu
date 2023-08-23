<?php
/**
 * 
 *
 * @author Azet
 * @date 2018-04-18
 * @url https://smart.yamagata-np.jp/fortune/
 */
class Zodiac000098 extends UranaiPlugin {

	function run($CONTENTS) {
		$content = $CONTENTS[0]; //全体URLを使用する
		/*
		* $RESULT[星座番号] => 順位
		*/
		$RESULT = array();
		$LINES = explode("\n", $content);

		$star = self::$starDefault;

		$now = self::getToday();

		$date_pattern = "/star_title\">{$now['month']}月{$now['day']}日の運勢ランキング</";
		$date_check_ok = false;


		foreach ($LINES AS $L) {
			/* 12星座分のデータがリザルトにある時処理を抜ける */
			if (count($RESULT) == 12) {
				break;
			}
			
			/* 日付の判定 */
			if (!$date_check_ok) {
				$date_check_ok = preg_match($date_pattern, $L);
				continue;
			}
			if ($date_check_ok) {

				/* 順位,星座の取得 */
				if (preg_match("/<span.*?>(\d+)<\/span>位.+?>(.*座)<span/", $L, $MATCHES)) { //順位
					$star_rank = intval($MATCHES[1]);
					$star_name = $MATCHES[2];
					if (!isset($star[$star_name])) {
						return null;
					}
					$star_num = $star[$star_name];
					$RESULT[$star_num] = $star_rank;
				}

			}
		}
		// エラーチェック
		if(!$date_check_ok) {
			print $this->logDateError().PHP_EOL;
		}
		//print_r ($RESULT);
		return $RESULT;
	}

}


