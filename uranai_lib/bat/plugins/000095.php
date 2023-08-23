<?php
/**
 * ウララの占い館
 *
 * @author Azet
 * @date 2017-06-23
 * @url http://news.mynavi.jp/horoscope/
 */
class Zodiac000095 extends UranaiPlugin {

	function run($URL) {
		$CONTENTS = $this->load($URL);
		$content = $CONTENTS[0]; //全体URLを使用する

		global $num_star;
		/*
		* $RESULT[星座番号] => 順位
		*/
		$RESULT = array();
		$LINES = explode("\n", $content);

		$date_pattern = "/<h3 class=\"side_headline\">".date("Y")."年".date("m")."月".date("d")."日の星座占い<\/h3>/";
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
			if($date_check_ok){
				if(preg_match_all("/<div class=\"info\"><p class=\"date\">(\d{1,2})位 - (.*座)<\/p>/U", $L, $MATCHES)){ //順位
					if(is_array($MATCHES[1]) && is_array($MATCHES[2])){
						for($i = 0; $i < 12; $i++){
							$star_name = $MATCHES[2][$i];	
							$star_num = $num_star[$star_name];
							$rank = $MATCHES[1][$i];
							$RESULT[$star_num] = $rank;
						}
					}
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


