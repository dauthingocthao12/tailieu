<?php
/**
data[star] = rank;
星座（インデックス） => 順位
*/
class Zodiac000016 extends UranaiPlugin {

	function run($CONTENTS) {

		$content = $CONTENTS[0];
		//$content = mb_convert_encoding($content, "UTF-8", "UTF-8");
		$LINES = explode("\n", $content);

		$star["水瓶座"] = 1;
		$star["魚座"] = 2;
		$star["牡羊座"] = 3;
		$star["牡牛座"] = 4;
		$star["双子座"] = 5;
		$star["蟹座"] = 6;
		$star["獅子座"] = 7;
		$star["乙女座"] = 8;
		$star["天秤座"] = 9;
		$star["蠍座"] = 10;
		$star["射手座"] = 11;
		$star["山羊座"] = 12;

		$now = self::getToday();
		$date_pattern = "!class=\"wfont-p\">{$now['month']}/{$now['day']}</span>の占いランキング!";

		$RESULT = array();
		$rank_num = 0;
		$date_check_ok = false;
		foreach ($LINES AS $key => $line) {
			if (count($RESULT) == 12) { break; }

			if(!$date_check_ok) {
				$date_check_ok = preg_match($date_pattern, $line);
			}

			// rank
			if ($date_check_ok && !$rank_num && preg_match("/class=\"rank\">(\d{1,2})<\/span>/", $line, $MATCHES)) {
				$rank_num = $MATCHES[1];
				continue;
			}
			
			// star
			if ($rank_num && preg_match("/class=\"fortune-name\">/", $line)) {
				$name_flg = true;
			}
			
			if ($name_flg && preg_match("/class=\"seiza\">(.*?)<\/a><\/span>/", $line, $MATCHES)) {
				$star_name = $MATCHES[1];
				$star_num = $star[$star_name];
				$RESULT[$star_num] = $rank_num;
				// reset
				$rank_num = 0;
				$name_flg = false;
			}
		}

		// date error?
		if(!$date_check_ok) {
			print $this->logDateError().PHP_EOL;
		}

		return $RESULT;
	}
	function topic_run($TOPIC_CONTENTS) {


		$star["水瓶座"] = 1;
		$star["魚座"] = 2;
		$star["牡羊座"] = 3;
		$star["牡牛座"] = 4;
		$star["双子座"] = 5;
		$star["蟹座"] = 6;
		$star["獅子座"] = 7;
		$star["乙女座"] = 8;
		$star["天秤座"] = 9;
		$star["蠍座"] = 10;
		$star["射手座"] = 11;
		$star["山羊座"] = 12;
		
		$topic["love"] = "恋愛運";
		$topic["money"] = "金運";
		$topic["work"] = "仕事運";

		$now = self::getToday();
		$date_pattern = "/class=\"wfont-p\">{$now['month']}\/{$now['day']}<\/span>/";

		$TOPIC_RESULT = array();
		$date_check_ok = false;
		$star_count = 0;
		$topic_name = "";

		foreach($TOPIC_CONTENTS as $topic_content) {

			$TOPIC_LINES = explode("\n", $topic_content);

			foreach ($TOPIC_LINES as $topic_line) {
				// if (count($TOPIC_RESULT) == 12) { break; }

				if(!$date_check_ok) {
					$date_check_ok = preg_match($date_pattern, $topic_line);
				}

				// star
				if ($date_check_ok && preg_match("/class=\"sign\">(.*座)<\/span>/", $topic_line, $MATCHES)) {
					$star_name = $MATCHES[1];
					$star_num = $star[$star_name];
				}
				
				if($topic_name && preg_match("/fortune-star.png\".*alt=\"star\"/", $topic_line)){
					$star_count++;
				}

				if (
					$date_check_ok && $star_num 
					&& (preg_match("/<li>(.*運):/", $topic_line,$MATCHES) || preg_match("/<\/li><li>ラッキーアイテム:/", $topic_line,$MATCHES))
				){
					if($topic_name && $star_count<6){
						$TOPIC_RESULT[$star_num][$topic_name] = $star_count * 20;
					}
					if(empty($MATCHES[1])){
						$topic_name = "";
					}else{
						$topic_name = array_search($MATCHES[1],$topic);
					}
					$star_count = 0;
				}

			}

			// date error?
			if(!$date_check_ok){
				print $this->logDateError().PHP_EOL;
			}
		}
		return $TOPIC_RESULT;
	}

	function lucky_run($TOPIC_CONTENTS) {

		$star["水瓶座"] = 1;
		$star["魚座"] = 2;
		$star["牡羊座"] = 3;
		$star["牡牛座"] = 4;
		$star["双子座"] = 5;
		$star["蟹座"] = 6;
		$star["獅子座"] = 7;
		$star["乙女座"] = 8;
		$star["天秤座"] = 9;
		$star["蠍座"] = 10;
		$star["射手座"] = 11;
		$star["山羊座"] = 12;

		$now = self::getToday();
		$date_pattern = "/class=\"wfont-p\">{$now['month']}\/{$now['day']}<\/span>/";

		$LUCKY_RESULT = array();
		$date_check_ok = false;

		foreach($TOPIC_CONTENTS as $topic_content) {
			$TOPIC_LINES = explode("\n", $topic_content);
			foreach ($TOPIC_LINES as $topic_line) {
				if(!$date_check_ok) {
					$date_check_ok = preg_match($date_pattern, $topic_line);
				}

				// star
				if ($date_check_ok && preg_match("/class=\"sign\">(.*座)<\/span>/", $topic_line, $MATCHES)) {
					$star_name = $MATCHES[1];
					$star_num = $star[$star_name];
				}
				if ($date_check_ok && $star_num && (preg_match("/<li>ラッキーアイテム:\s(.*?)<\/li>/", $topic_line,$MATCHES))){
					$lucky_item=$MATCHES[1];
				}
				if ($date_check_ok && $star_num && (preg_match("/<li>ラッキーカラー:\s(.*?)<\/li>/", $topic_line,$MATCHES))){
					$lucky_color=$MATCHES[1];
				}
				
			}
			$LUCKY_RESULT[$star_num] = array("lucky_item"=> $lucky_item , "lucky_color" => $lucky_color);
			
			// date error?
			if(!$date_check_ok){
				print $this->logDateError().PHP_EOL;
			}
		}
		return $LUCKY_RESULT;
	}
}
