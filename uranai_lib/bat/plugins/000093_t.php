<?php
/**
 * マイナビニュース
 *
 * @author Azet
 * @date 2017-06-23
 * @url http://news.mynavi.jp/horoscope/
 */
class Zodiac000093 extends UranaiPlugin {

	function run($CONTENTS) {
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
	function topic_run($TOPIC_CONTENTS) {
		$topic_en = array(
			'恋愛運' => 'love',
			'金運' => 'money',
			'仕事運' => 'work',
			'健康運' => 'health',
		);
		$now = self::getToday();
		$date_pattern = "/class=\"today01\">({$now['month']})\/({$now['day']})</";
		/*
		* $RESULT[星座番号] => 順位
		*/
		$TOPIC_RESULT = array();
		foreach ($TOPIC_CONTENTS as $star_num => $content){
			$date_check_ok = false;
			$LINES = explode("\n", $content);
			$topic_array = array();
			$topic = '';
			$next_heart = FALSE;
			foreach ($LINES as $L){
				if (!$date_check_ok) {
					$date_check_ok = preg_match($date_pattern, $L);
					continue;
				}
				if(preg_match("/detail_item01\">(.*運)<\/span>/", $L ,$m) && $date_check_ok){
					$topic = $m[1];
				}
				if(preg_match("/class=\"red\">$/", $L) && $topic){
					$next_heart = true;
				}
				if(preg_match("/([♥]*)<\/span>/", $L,$m) && $next_heart){
					$topic_count = strlen($m[1]);
					$topic_rank = $topic_count * 20 / 3;
					//ハート文字化けで3bit扱いのため割る3
					$topic_array = $topic_array + array($topic_en[$topic] => $topic_rank);
					$next_heart = FALSE;
				}
			}
			$TOPIC_RESULT[$star_num] = $topic_array;
		}
		// エラーチェック
		if(!$date_check_ok) {
			print $this->logDateError().PHP_EOL;
		}
		return $TOPIC_RESULT;
		
	}
	
}


