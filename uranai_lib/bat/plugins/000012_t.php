<?php
/**
data[star] = rank;
星座（インデックス） => 順位
*/
class Zodiac000012 extends UranaiPlugin {

	function run($CONTENTS) {

		$content = $CONTENTS[0];
		//$content = mb_convert_encoding($content, "UTF-8", "UTF-8");
		$pattern = "<h3>(.*?座)<\/h3>";

		// date check
		$date_ok = false;
		$now = self::getToday();
		$date_pattern = "/<h1><strong>0?{$now['month']}月0?{$now['day']}日<\/strong>の運勢ランキング<\/h1>/";
		$date_ok = preg_match($date_pattern, $content);
		if(!$date_ok) {
			print $this->logDateError().PHP_EOL;
			return null;
		}

		preg_match_all("/$pattern/", $content, $MATCHES);

		// star: custom
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

		$RESULT = array();
		$i = 1;
		foreach ($MATCHES[1] as $key => $value) {
			$RESULT[$star[$value]] = $i;
			$i++;
		}

		return $RESULT;
	}

	function topic_run($TOPIC_CONTENTS) {
		$TOPIC_RESULT = array();

		$now = self::getToday();

		// star: custom
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

		foreach($TOPIC_CONTENTS as $topic_content){
			$TOPIC_LINES = explode("\n", $topic_content);

			$pattern = "/<h3>(.*?座)<\/h3>/";

			$date_pattern = "/<h1><strong>0?{$now['month']}月0?{$now['day']}日<\/strong>の運勢ランキング<\/h1>/";

			$date_check_ok = false;
			$love = 0;
			$money = 0;
			$work = 0;

			foreach ($TOPIC_LINES as $topic_line) {

				if(!$date_check) {
					$date_check = preg_match($date_pattern , $topic_line);
					//print $date_check;
				 }

				if(preg_match($pattern, $topic_line, $MATCHES)) {
					$this_page_star = $MATCHES[1];					
					$star_num = $star["$this_page_star"];
				//	print $star_num;
				 }

				if ($date_check && $star_num && preg_match("/<li><span>恋愛運<\/span><span>(.*?)<\/span><\/li>/", $topic_line, $MATCHES)){
					$love_content = $MATCHES[1];
					$love = substr_count($love_content , '★');
					$love_num = ($love * 20);
				}

				if ($date_check && $star_num && preg_match("/<li><span>仕事運<\/span><span>(.*?)<\/span><\/li>/", $topic_line, $MATCHES)){
					$work_content = $MATCHES[1];
					$work = substr_count($work_content , '★');
					$work_num = ($work * 20);
				}

				if ($date_check && $star_num && preg_match("/<li><span>金財運<\/span><span>(.*?)<\/span><\/li>/", $topic_line, $MATCHES)){
					$money_content = $MATCHES[1];
					$money = substr_count($money_content , '★');
					$money_num = ($money * 20);
				}

			}
			$TOPIC_RESULT[$star_num] = array("love"=> $love_num , "money" => $money_num ,"work" => $work_num);

			if(!$date_check){
				print $this->logDateError().PHP_EOL;
			}

		}
		return $TOPIC_RESULT;
	}
}
