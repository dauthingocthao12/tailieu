<?php
/**
 * なーのちゃんクラブ
 *
 * @author Azet
 * @date 2017-06-23
 * @url https://nano.shinmai.co.jp/enjoy/fortune/
 */
class Zodiac000085 extends UranaiPlugin {

	function run($CONTENTS) {

		$content = $CONTENTS[0]; //全体URLを使用する
		/*
		* $RESULT[星座番号] => 順位
		*/
		$RESULT = array();
		$LINES = explode("\n", $content);
		
		$star = self::$starDefault;
		//$now = self::getToday();
		$month = date('n');
		$day = date('d');
		
		$date_pattern = "/<title>今日\({$month}月{$day}日\)の運勢<\/title>/";
		//$date_pattern = "/<title>今日\({$now['month']}月{$now['day']}日\)の運勢<\/title>/";
		$date_check_ok = false;

		$rank = 0;
		
		foreach ($LINES AS $L) {
			/*12星座分のデータがリザルトにある時処理を抜ける*/
			if (count($RESULT) == 12) { break; }

			/*日付の判定*/
			if (!$date_check_ok) {
				$date_check_ok = preg_match($date_pattern, $L);
				continue;
			}
			/*順位,星座の取得*/
			if($date_check_ok && $rank == 0){
				if(preg_match("/<a\shref=\"\/enjoy\/fortune_detail\/\?ranking=(\d{1,2})&seiza=(.*座)\">/", $L, $MATCHES)){ //星座名
					$rank = $MATCHES[1];
					$star_name = $MATCHES[2];
					echo $star_name;
					echo $rank;
					$star_num = $star[$star_name];
					$RESULT[$star_num] = $rank;
					$rank = 0;
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


