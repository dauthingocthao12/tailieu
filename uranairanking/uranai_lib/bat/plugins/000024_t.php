<?php
/**
data[star] = rank;
星座（インデックス） => 順位
*/
class Zodiac000024 extends UranaiPlugin {

	function run($CONTENTS) {

		$now = self::getToday();
		$date_pattern = "!<h1 id=\"scope_head_fortune\">{$now['month']}月{$now['day']}日の.*</h1>!";

		$RESULT = array();
		foreach ($CONTENTS AS $key => $content) {
			//$content = mb_convert_encoding($content, "UTF-8", "UTF-8");		
			$LINES = explode("\n", $content);
			$rank_num = 0;
			$date_check_ok = false;

			foreach ($LINES as $line) {
				// date check
				if(!$date_check_ok) {
					$date_check_ok = preg_match($date_pattern, $line);
					continue;
				}

				if ($date_check_ok && preg_match("/HOROSCOPE RANKING<span>No.(\d{1,2})<\/span>/", $line, $MATCHES)) {
					$RESULT[$key] = $MATCHES[1];
					break;
				}
			}

			// date error?
			if(!$date_check_ok) {
				print $this->logDateError().PHP_EOL;
			}

		}

		return $RESULT;
	}

	function topic_run($TOPIC_CONTENTS) {

		$now = self::getToday();
		$date_pattern = "!<h1 id=\"scope_head_fortune\">{$now['month']}月{$now['day']}日の.*</h1>!";

		$TOPIC_RESULT = array();
		foreach($TOPIC_CONTENTS as  $key => $topic_content) {
			//$content = mb_convert_encoding($content, "UTF-8", "UTF-8");		
			$TOPIC_LINES = explode("\n", $topic_content);
			$date_check_ok = false;
			$KEY = explode("_", $key);
			$star_num = $KEY[0];
			$topic_type = $KEY[1];
			$count =0;
			foreach ($TOPIC_LINES as$topic_line) {
				// date check
				if(!$date_check_ok) {
					$date_check_ok = preg_match($date_pattern, $topic_line);
					continue;
				}
				
				if ($date_check_ok && preg_match("/<span class=\"gold\">★<\/span>/", $topic_line)) {
					$count++;
					$count_num = ( $count * 20);
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
		$TOPIC = array( 'love' => 'tab=2' ,'interpersonal' => 'tab=3' ,'work' => 'tab=4' ,'money' => 'tab=5' ,'health' => 'tab=6');
		foreach($URL as $key => $url){
			$U = explode("#", $url);
			foreach($TOPIC as $k => $topic){
				$t_key = $key."_".$k;
				$t_url[$t_key] = $U[0]."?".$topic."#".$U[1];
			}
		}
		return parent::load($t_url);
	}
}
