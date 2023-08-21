<?php
/**
data[star] = rank;
星座（インデックス） => 順位
*/
class Zodiac000002 extends UranaiPlugin {

	function run($URL) {

		/*foreach ($URL as $key => $url) {
			$url = str_replace("(md)", date("md"), $url);
			$url = str_replace("(ymd)", date("ymd"), $url);
			$URL[$key] = $url;
		}*/
		$CONTENTS = $this->load($URL);
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
}

