<?php
/**
 * マイナビニュース
 *
 * @author Azet
 * @date 2017-06-23
 * @url http://news.mynavi.jp/horoscope/
 */
class Zodiac000093 extends UranaiPlugin {

	function run($URL) {
		$CONTENTS = $this->load($URL);
		$content = $CONTENTS[0]; //全体URLを使用する
		/*
		* $RESULT[星座番号] => 順位
		*/
		$RESULT = array();
		$LINES = explode("\n", $content);

		$star = self::$starKanji;
		
		$now = self::getToday();
		
		$date_pattern = "/class=\"today01\">({$now['month']})\/({$now['day']})</";
		$date_check_ok = false;

		
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
				if(preg_match("/<img src=\".*\" alt=\"(.*座)\"/", $L, $MATCHES)){ //順位
					$star_name = $MATCHES[1];
				}
				if($star_name){
					if(preg_match("/<img src=\".*\/rank([0-9]{1,2}).png\".* alt=\"[0-9]{1,2}\"/", $L, $MATCHES)){ //星座名
						$star_num = $star[$star_name];
						$RESULT[$star_num] = $MATCHES[1];
						$star_name = "";
						
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


