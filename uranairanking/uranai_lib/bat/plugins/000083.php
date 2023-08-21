<?php
/**
 * ナイロンジャパン
 *
 * @author Azet
 * @date 2017-06-23
 * @url http://www.nylon.jp/horoscope
 */
class Zodiac000083 extends UranaiPlugin {

	function run($URL) {
		$CONTENTS = $this->load($URL);
		$content = $CONTENTS[0]; //全体URLを使用する
		/*
		* $RESULT[星座番号] => 順位
		*/
		$RESULT = array();
		$LINES = explode("\n", $content);
		
		$star["aquarius"] = 1;
		$star["pisces"] = 2;
		$star["aries"] = 3;
		$star["taurus"] = 4;
		$star["gemini"] = 5;
		$star["cancer"] = 6;
		$star["leo"] = 7;
		$star["virgo"] = 8;
		$star["libra"] = 9;
		$star["scorpio"] = 10;
		$star["sagittarius"] = 11;
		$star["capricorn"] = 12;
		
		$now = self::getToday();
		
		$date_pattern = "/<h3>{$now['year']}\.{$now['month']}\.{$now['day']}\s今日の運勢<\/h3>/";
		$date_check_ok = false;

		$rank=0;
		
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
				if(preg_match("/<div class=\"hrosection\"\s+id=\"(.*)\">/", $L, $MATCHES)){ //星座英語名
					$star_name = $MATCHES[1];	
					$star_num = $star[$star_name];
				}
				if(preg_match("/horoscope\/new\/no(\d{1,2})\.jpg/", $L, $MATCHES)){ //順位
					$rank = $MATCHES[1];
					$RESULT[$star_num] = $rank;
					$rank = 0;
				}
			}
		}
		asort($RESULT); //値で昇順ソート
		// エラーチェック
		if(!$date_check_ok) {
			print $this->logDateError().PHP_EOL;
		}
		//print_r ($RESULT);
		return $RESULT;
	}
}


