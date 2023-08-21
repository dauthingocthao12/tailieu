<?php
/**
data[star] = rank;
星座（インデックス） => 順位
*/
class Zodiac000121 extends UranaiPlugin {

	function run($CONTENTS) {

		global $en_num_star;
		$star_ = $en_num_star;
		$star_[3] = "aries"; //誤字!
		$star = array_flip($star_);

		//date check
		$now = self::getToday();
		$date_check_ok = false;

		$content = $CONTENTS[0];
		$LINES = explode("\n", $content);
		$RESULT = array();

		foreach ($LINES as $line) {
			if(!$date_check_ok){
				if(preg_match("/<h2.*<span class=\"uranai_month\">0?".$now['month']."<\/span>月<span class=\"uranai_date_span\">0?".$now['day']."<\/span>日の運勢ランキング<\/h2>/", $line)){
					$date_check_ok = true;
				}
			}
			if (count($RESULT) == 12) { break; }
			if ($date_check_ok && preg_match('/<li class="uranai_rank_(\d{1,2})"><a href=".*" class="(.*)"><span>.*<\/span><\/a><\/li>/', $line, $matches)) {
				$RESULT[$star[$matches[2]]] = intval($matches[1]);
			}
		}
		if(!$date_check_ok) {
			print $this->logDateError().PHP_EOL;
		}
		return $RESULT;
	}

	function topic_run($TOPIC_CONTENTS) {

		
		$RESULT = array();
		global $en_num_star;
		$star_ = $en_num_star;
		$star_[3] = "aries"; //誤字!
		$star = array_flip($star_);
		$date_check_ok = false;
		$topic_en = array(
			
			 "仕事運"=> 'work',
			 "恋愛運"=> 'love',
			 "対人運" => 'interpersonal',
			 "金銭運" => 'money',
			 "お出かけ運" => 'outing',
			 "健康・美容運" => 'health'
				
		);

		//date check
		$now = self::getToday();
			
		foreach($TOPIC_CONTENTS AS $content){

			$content = preg_replace("/\r\n|\r|\n/", "\n", $content);
			// $LINES = explode("\r\n", $content);
			$LINES = explode("\n", $content);
			$topic = "";
			foreach ($LINES as $line) {
				if(!$date_check_ok){
					if(preg_match("/<p class=\"uranai_kiji_date\">".$now['year']."年0?".$now['month']."月0?".$now['day']."日の運勢<\/p>/", $line)){
						$date_check_ok = true;
					}
				}
				
				// star
				if ($date_check_ok && preg_match('/<p class=\"uranai_rank_\d{1,2} ([a-z]*)\"><span>.*座<\/span><\/p>/', $line, $MATCHES)) {
					$star_num = $star[$MATCHES[1]];
				}
				//work
				if ($date_check_ok && $star_num && preg_match('/<h2 class="uranai_kiji_heading h-shigoto">(.*)<\/h2>/', $line, $MATCHES)){
					$topic = $topic_en[$MATCHES[1]];
				}
				//interpersonal
				if ($date_check_ok && $star_num && preg_match('/<h2 class="uranai_kiji_heading h-taijin">(.*)<\/h2>/', $line, $MATCHES)){
					$topic = $topic_en[$MATCHES[1]];
				}
				//money
				if ($date_check_ok && $star_num && preg_match('/<h2 class="uranai_kiji_heading h-kinsen">(.*)<\/h2>/', $line, $MATCHES)){
					$topic = $topic_en[$MATCHES[1]];
				}
				//outing
				if ($date_check_ok && $star_num && preg_match('/<h2 class="uranai_kiji_heading h-odekake">(.*)<\/h2>/', $line, $MATCHES)){
					$topic = $topic_en[$MATCHES[1]];
				}
				//love
				if ($date_check_ok && $star_num && preg_match('/<h2 class="uranai_kiji_heading h-renai">(.*)<\/h2>/', $line, $MATCHES)){
					$topic = $topic_en[$MATCHES[1]];
				}
				//health
				if ($date_check_ok && $star_num && preg_match('/<h2 class="uranai_kiji_heading h-biyou">(.*)<\/h2>/', $line, $MATCHES)){
					$topic = $topic_en[$MATCHES[1]];
				}
				if($topic && preg_match('/div class=\"uranai_rating .* rating-([0-9])\"><\/div>/', $line, $MATCHES)){
					$topic_list[$topic] = ($MATCHES[1]* 20);
					$topic = "";
				}
			}
			$RESULT[$star_num] = $topic_list;
		}
		return $RESULT;
	}
}
?>
