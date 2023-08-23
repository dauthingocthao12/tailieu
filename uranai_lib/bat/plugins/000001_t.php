<?php
/**
data[star] = rank;
星座（インデックス） => 順位
*/
class Zodiac000001 extends UranaiPlugin {

	function run($CONTENTS) {
		// date check
		$now = self::getTodayISO();
		$star = self::$starDefault;
		$RESULT = array();
		$date_check = false;
		$rank = 0;

		$content = $CONTENTS[0];
		$LINES = explode("\n", $content);
		foreach($LINES as $line){
			preg_match("!<script id=\"__NEXT_DATA__\" type=\"application\/json\">(.*)<\/script>!",$line,$MATCHES);
			$json_content = json_decode($MATCHES[1],true);
		}
		$ranking_data = $json_content['props']['pageProps']['horoscopeRankings']['result']["horoscopeRankings"];
		foreach($ranking_data as $data){
			if(!$date_check){
				if($data['date'] == $now){
					$date_check = true;
				}
			}
			if($date_check && !$rank){
				$rank = $data['ranking'];
			}
			if($date_check && $rank){
				$star_name = $data['zodiac']['jpName'];
				$star_num = $star[$star_name];
				$RESULT[$star_num] = $rank;
			}
			$rank = 0;
		}

		if(!$date_check) {
			print $this->logDateError().PHP_EOL;
		}
		return $RESULT;

	}

	function topic_run($TOPIC_CONTENTS) {
		$TOPIC_RESULT = array();

		// date check
		$now = self::getTodayISO();
		$star = self::$starDefault;

		$content = $TOPIC_CONTENTS[0];
		$LINES = explode("\n", $content);
		$date_check = false;
		$love_score = 0;
		$money_score = 0;
		$work_score = 0;

		foreach($LINES as $line){
			preg_match("!<script id=\"__NEXT_DATA__\" type=\"application\/json\">(.*)<\/script>!",$line,$MATCHES);
			$json_content = json_decode($MATCHES[1],true);
		}

		$topic_data = $json_content['props']['pageProps']['horoscopeRankings']['result']["horoscopeRankings"];
		foreach($topic_data as $data){
			if(!$date_check){
				if($data['date'] == $now){
					$date_check = true;
				}
			}
			if($date_check){
				$star_name = $data['zodiac']['jpName'];
				$star_num = $star[$star_name];
				$love_score = $data['loveScore'];
				$love_num = $love_score * 20;
				$money_score = $data['moneyScore'];
				$money_num = $money_score * 20;
				$work_score = $data['workScore'];
				$work_num = $work_score * 20;
			}
			$TOPIC_RESULT[$star_num] = array("love"=> $love_num , "money" => $money_num ,"work" => $work_num);

		}
		
		if(!$date_check) {
			print $this->logDateError().PHP_EOL;
		}

		return $TOPIC_RESULT;
	}

	function lucky_run($TOPIC_CONTENTS){
		$LUCKY_RESULT = array();

		// date check
		$now = self::getTodayISO();
		$star = self::$starDefault;

		$content = $TOPIC_CONTENTS[0];
		$LINES = explode("\n", $content);
		$date_check = false;
		$lucky_item = null;
		$lucky_coler = null;

		foreach($LINES as $line){
			preg_match("!<script id=\"__NEXT_DATA__\" type=\"application\/json\">(.*)<\/script>!",$line,$MATCHES);
			$json_content = json_decode($MATCHES[1],true);
		}

		$topic_data = $json_content['props']['pageProps']['horoscopeRankings']['result']["horoscopeRankings"];
		foreach($topic_data as $data){
			if(!$date_check){
				if($data['date'] == $now){
					$date_check = true;
				}
			}
			if($date_check){
				$star_name = $data['zodiac']['jpName'];
				$star_num = $star[$star_name];
				$lucky_item = $data['luckyItem'];
				$lucky_color = $data['luckyColor'];
			}
			$LUCKY_RESULT[$star_num] = array("lucky_item"=> $lucky_item , "lucky_color" => $lucky_color );
		}
		return $LUCKY_RESULT;
	}
}
?>
