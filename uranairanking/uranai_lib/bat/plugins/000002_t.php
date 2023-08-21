<?php
/**
data[star] = rank;
星座（インデックス） => 順位
*/
class Zodiac000002 extends UranaiPlugin {

	function run($CONTENTS) {

		$RESULT = array();
		$now = array('year'=> date('Y'),'month' => date('m'), 'day' =>date('d'));
		foreach($CONTENTS as $url){
			$content = $url;
			//$content = mb_convert_encoding($content,"UTF-8","SJIS");
			
			$LINES = explode("\n", $content);
			$chk_flg = 0;
			$star_number = self::$starDefault;
			foreach ($LINES as $key => $line) {
				if ($chk_flg == 0 && preg_match("/<img width=\"\d*\" alt=\"(.*)\" src=\".*$/", $line, $MATCHES)) {
					$this_page_star = $MATCHES[1];
					$chk_flg = 1;
					continue;
				}
				if ($chk_flg == 1 && preg_match("/<h3 class=\"todayMark\">{$now['month']}月{$now['day']}日の運勢<\/h3>/",$line,$MATCHES)) {
					$chk_flg = 2;
					continue;
				}
				if ($chk_flg == 2 && preg_match("/<!--\s(\d+)位\s-->/", $line, $MATCHES)) {
					$rank_num = $MATCHES[1];
					$chk_flg = 3;
				} elseif ($chk_flg == 3 && preg_match("/<dt>(.*)<\/dt>/", $line, $MATCHES)) {
					$star_name_jp = $MATCHES[1];
					if($star_name_jp == $this_page_star){
						$star_num=$star_number["$this_page_star"];
						$RESULT[$star_num] = $rank_num;
						break;
					}else{
						$chk_flg = 2;
					}
				}
			}//end foreach
			if(empty($RESULT)){
				print $this->logDateError().PHP_EOL;
			}
		}//end foreach

		return $RESULT;
	}

	function topic_run($TOPIC_CONTENTS) {

		$TOPIC_RESULT = array();
		$now = array('year'=> date('Y'),'month' => date('m'), 'day' =>date('d'));
		foreach($TOPIC_CONTENTS as $k => $topic_content){
		//	$topic_content = mb_convert_encoding($topic_content,"UTF-8","SJIS");
			
			$TOPIC_LINES = explode("\n", $topic_content);
			$date_check = false;
			$star_number = self::$starDefault;
			$love = 0;
			$money = 0;
			foreach ($TOPIC_LINES as $topic_line) {
				
				if (preg_match("/<img width=\"\d*\" alt=\"(.*)\" src=\".*$/", $topic_line, $MATCHES)) {
					$this_page_star = $MATCHES[1];					
					$star_num = $star_number["$this_page_star"];
				//	print $star_num	;
				}
				if(!$date_check && $star_num ) {
					$date_check = preg_match("/<h3 class=\"todayMark\">{$now['month']}月{$now['day']}日の運勢<\/h3>/", $topic_line);
				//	print $date_check;
				 }
				if($date_check && preg_match("/<img alt=\"▼\" src=\"\/daily\/image\/heart_red\.png\" \/>/", $topic_line)){
					$love++;
				//	print $love;
				}
				if($date_check && preg_match("/<img alt=\"\¥\" src=\"\/daily\/image\/money_yellow\.png\" \/>/", $topic_line)){
					$money++;
				//	print $money;
				$love_num = ($love * 20);
				$money_num = ($money * 20);
				$TOPIC_RESULT[$star_num] = array("love"=> $love_num , "money" =>$money_num ,"work" => NULL  );
				}
			}//end foreach
			if(!$date_check){
				print $this->logDateError().PHP_EOL;
			}
		}//end foreach

		return $TOPIC_RESULT;
	}
}

