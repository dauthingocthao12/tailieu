<?php
/**
data[star] = rank;
星座（インデックス） => 順位
*/
class Zodiac000123 extends UranaiPlugin {

	function run($CONTENTS) {
mb_regex_encoding("UTF-8");
		//global $kanji_star;
		//$star_ = $kanji_star;
		//$star_[3] = "aries"; //誤字!
		//$star = array_flip($star_);

		//date check
		$now = self::getToday();
		$date_check_ok = false;

		$content = $CONTENTS[0];
		$LINES = explode("\n", $content);
		$RESULT = array();
		//追加コード
		$kanjiToNum = array(
		"水瓶座" => "1"
		,"魚座" => "2"
		,"牡羊座" => "3"
		,"牡牛座" => "4"
		,"双子座" => "5"
		,"蟹座" => "6"
		,"獅子座" => "7"
		,"乙女座" => "8"
		,"天秤座" => "9"
		,"蠍座" => "10"
		,"射手座" => "11"
		,"山羊座" => "12"
		);
		foreach($CONTENTS as $key => $content) {
			$LINES = explode("\n", $content);
			$date_check_ok = false;

			foreach ($LINES as $line) {
				if(!$date_check_ok){
					$day = "/date\">{$now['year']}年<span>[0]?{$now['month']}<\/span>月<span>[0]?{$now['day']}<\/span>日の運勢<\/p>/";
					if(preg_match($day, $line)){
						$date_check_ok = true;
					}
				}
				if (count($RESULT) == 12) { break; }
				//if ($date_check_ok && preg_match('/<li>(水瓶座)<\/li>/',$line,$matches) && preg_match('/<dl><dt>(\d{1,2})位<\/dt>/',$line,$matches2)) {
				if(preg_match('/<li>(.*座)<\/li>/u',$line,$matches)){
				$memory=$matches[1];
				}
				if ($date_check_ok && preg_match('/<dl><dt>(\d{1,2})位<\/dt>/',$line,$matches2)) {
					$RESULT[$kanjiToNum[$memory]]=$matches2[1];
				}
			}
			if(!$date_check_ok) {
				print $this->logDateError().PHP_EOL;
			}
		}
			return $RESULT;
	}
}
?>
