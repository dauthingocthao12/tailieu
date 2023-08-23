<?php
/**
data[star] = rank;
星座（インデックス） => 順位
*/
class Zodiac000014 extends UranaiPlugin {

	function run($CONTENTS) {

		$RESULT = array();
		foreach($CONTENTS as $star_num => $content) {
			// each site
			$content = mb_convert_encoding($content, "UTF-8", "SJIS");
			$LINES = explode("\n", $content);
			$rank_num = 0;

			$line_prev = "";
			foreach ($LINES AS $line) {
				// rank
				if (preg_match("/>位<\//", $line)) {
					if (preg_match("/>(\d{1,2})<\//", $line_prev, $MATCHES)) {
						$rank_num = intval($MATCHES[1]);
						if ($rank_num >= 1 && $rank_num <= 12) {
							$RESULT[$star_num] = $rank_num;
						}
					}
				}
				$line_prev = $line;
			}	
		}

		return $RESULT;
	}

	function topic_run($TOPIC_CONTENTS) {

		$TOPIC_RESULT = array();
		$now = self::getToday();
		$star = self::$starDefault;
		$date_pattern = "/<li>{$now['year']}\/{$now['month']}\/{$now['day']} .*?座の運勢<\/li>/";


		foreach($TOPIC_CONTENTS as $topic_content) {
			// each site
			$topic_content = mb_convert_encoding($topic_content, "UTF-8", "SJIS");
			$TOPIC_LINES = explode("\n", $topic_content);

			foreach ($TOPIC_LINES as $topic_line) {

				if(!$date_check) {
					$date_check = preg_match($date_pattern, $topic_line);
				}
				// star
				if ($date_check && preg_match("/<h3 class=\"result-ttl\">(.*?座)の運勢<\/h3>/", $topic_line, $MATCHES)) {
					$star_name = $MATCHES[1];
					$star_num = $star[$star_name];
				}

				if ($date_check && preg_match("/<dt>恋愛運<\/dt>/", $topic_line)) {
					$love_flg = 1;
				}

				if ($date_check && $love_flg && preg_match("/<img src=(.*?)<\/dd>/", $topic_line, $MATCHES)) {
					$love_content = $MATCHES[1];
					$love_max = substr_count($love_content , 'icon_heart_max.png');
					$love_half = substr_count($love_content , 'icon_heart_half.png');
					//print $love_max ;
					//print $love_half;
					$love_num = ($love_max * 20) + ($love_half * 10);
					$love_flg = 0;
					continue;
				}
				if ($date_check && preg_match("/<dt>金銭運<\/dt>/", $topic_line)) {
					$money_flg = 1;
				}
				if ($date_check && $money_flg && preg_match("/<img src=(.*?)<\/dd>/", $topic_line, $MATCHES)) {
					$money_content = $MATCHES[1];
					$money_max = substr_count($money_content , 'icon_dollar_max.png');
					$money_half = substr_count($money_content , 'icon_dollar_half.png');
					$money_num = ($money_max * 20) + ($money_half * 10);
					$money_flg = 0;
					continue;
				}

				if ($date_check && preg_match("/<dt>仕事運<\/dt>/", $topic_line)) {
					$work_flg = 1;
				}
				if ($date_check && $work_flg && preg_match("/<img src=(.*?)<\/dd>/", $topic_line, $MATCHES)) {
					$work_content = $MATCHES[1];
					$work_max = substr_count($work_content , 'icon_business_max.png');
					$work_half = substr_count($work_content , 'icon_business_half.png');
					$work_num = ($work_max * 20) + ($work_half * 10);
					$work_flg = 0;
					continue;
				}

			}	
			$TOPIC_RESULT[$star_num] = array("love"=> $love_num , "money" => $money_num ,"work" => $work_num);

			if(!$date_check) {
				print $this->logDateError().PHP_EOL;
			}
		}

		return $TOPIC_RESULT;
	}

}
