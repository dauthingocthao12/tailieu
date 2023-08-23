<?php
/**
 * マイナビニュース
 *
 * @author Azet
 * @date 2017-06-23
 * @url http://news.mynavi.jp/horoscope/
 */
class Zodiac000099 extends UranaiPlugin {

	function run($CONTENTS) {
		$content = $CONTENTS[0]; //全体URLを使用する
		/*
		* $RESULT[星座番号] => 順位
		*/
		$RESULT = array();
		// date pattern
		$now = self::getToday();
		$star = self::$starKanji;
		$date_pattern = "/<span class=\"num\">{$now['month']}<\/span>月<span class=\"num\">{$now['day']}<\/span>日<span class=\"small\">のお告げ/";

		foreach($CONTENTS as $star_num => $content) {
			//$content = mb_convert_encoding($content, "UTF-8", "UTF-8");
			$LINES = explode("\n", $content);
			$date_check_ok = false;
			$star_name="";
			$star_rank="";

			foreach ($LINES AS $line) {

				// date check
				if(!$date_check_ok) {
					$date_check_ok = preg_match($date_pattern, $line);
				}
				
				if($date_check_ok && preg_match("/name tx-bl\">(.*座)<\/span/", $line,$m)){
					$star_name = $m[1];
				}

				
				if($date_check_ok && preg_match("/rank tx-bl\">No.([0-9]{0,2})<\//", $line,$m)) {
					$star_rank = $m[1];
				}
				
				if($star_name && $star_rank){
					$star_num = $star[$star_name];
					$RESULT[$star_num] = $star_rank;
				}

			}
			// date error?
			if(!$date_check_ok) {
				print $this->logDateError().PHP_EOL;
			}
		}
		//print_r ($RESULT);
		return $RESULT;
	}

	function topic_run($TOPIC_CONTENTS) {
		$now = self::getToday();
		$date_pattern = "/<span class=\"num\">{$now['month']}<\/span>月<span class=\"num\">{$now['day']}<\/span>日<span class=\"small\">のお告げ/";

		$TOPIC_RESULT = array();
		foreach($TOPIC_CONTENTS as $key => $topic_content) {
			//$content = mb_convert_encoding($content, "UTF-8", "UTF-8");		
			$TOPIC_LINES = explode("\n", $topic_content);
			$date_check_ok = false;
			$KEY = explode("_", $key);
			$star_num = $KEY[0];
			$topic_type = $KEY[1];
			$count = 0;
			$count_num = 0;
			$topic = FALSE;
			foreach ($TOPIC_LINES as $topic_line) {
				// date check
				if(!$date_check_ok) {
					$date_check_ok = preg_match($date_pattern, $topic_line);
					continue;
				}
				
				if ($date_check_ok && preg_match("/=\"title\">.*運<\//", $topic_line)) {
					$topic = true;
				}
				if($topic && preg_match("/star light\">/", $topic_line)){
					$count = mb_substr_count($topic_line,'light');
					$count_num = ( $count * 20);
					break;
				}
			}
			$TOPIC_RESULT[$star_num][$topic_type] = $count_num;

			// date error?
			if(!$date_check_ok) {
				print $this->logDateError().PHP_EOL;
			}

		}

		return $TOPIC_RESULT;
	}
	
	function topic_load($URL){
		$t_url = array();
		$TOPIC = array( 'love' => '/love' ,'interpersonal' => '/relationships' ,'money' => '/money' ,'health' => '/beauty');
		foreach($URL as $key => $url){
			$U = explode("#", $url);
			foreach($TOPIC as $k => $topic){
				$t_key = $key."_".$k;
				//登録フォームに空白が入っているとURLが正しく読み込めないため。
				$url_nospase = rtrim($U[0]);
				$t_url[$t_key] = $url_nospase.$topic."#".$U[1];
			}
		}
//		print_r ($t_url);
		return parent::load($t_url);
	}
}


