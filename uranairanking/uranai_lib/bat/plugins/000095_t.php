<?php
/**
 * ウララの占い館
 *
 * @author Azet
 * @date 2017-06-23
 * @url http://news.mynavi.jp/horoscope/
 */
class Zodiac000095 extends UranaiPlugin {

	/*
	 * 総合運用メソッド
	 *
	 * @param $string $URL wget先URL
	 */
	function run($CONTENTS) {
		//$CONTENTS = $this->load($URL);
		$content = $CONTENTS[0]; //全体URLを使用する

		global $num_star;
		/*
		 * $RESULT[星座番号] => 順位
		 */
		$RESULT = array();
		$LINES = explode("\n", $content);

		$date_pattern = "/<h3 class=\"side_headline\">".date("Y")."年".date("m")."月".date("d")."日の星座占い<\/h3>/";
		$date_check_ok = false;

		$rank = 0;

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
				if(preg_match_all("/<div class=\"info\"><p class=\"date\">(\d{1,2})位 - (.*座)<\/p>/U", $L, $MATCHES)){ //順位
					if(is_array($MATCHES[1]) && is_array($MATCHES[2])){
						for($i = 0; $i < 12; $i++){
							$star_name = $MATCHES[2][$i];	
							$star_num = $num_star[$star_name];
							$rank = $MATCHES[1][$i];
							$RESULT[$star_num] = $rank;
						}
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

	/*
	 * トピック運勢メソッド
	 *
	 * @param $string $TOPIC_CONTENTS
	 */
	function topic_run($TOPIC_CONTENTS) {

		$TOPIC_RESULT = array();
		global $num_star;
		$date_check_ok = false;
		$rank = 0;

		// date pattern
		$date_pattern = "/<h3 class=\"side_headline\">".date("Y")."年".date("m")."月".date("d")."日の星座占い<\/h3>/";

		$content = $TOPIC_CONTENTS[0];
		$LINES = explode("\n", $content);

		foreach($LINES as $LINE) {

			if(!$date_check_ok) {
				$date_check_ok = preg_match($date_pattern, $LINE);
				continue;
			}

			//1行に１２星座情報がはいっているので、<li>タグごとにバラす	
			if(preg_match_all("/<li class=\"clearfix\".*<\/li>/U", $LINE, $ranking_lines)){
				//星座<li>~</li>ループ
				foreach($ranking_lines[0] as $rl){

					$love_num = 0;
					$work_num = 0;
					$health_num = 0;

					if(preg_match("/<div class=\"info\"><p class=\"date\">\d{1,2}位 - (.*座)<\/p>/", $rl, $MATCHES)){ //星座名

						$star_name = $MATCHES[1];	
						$star_num = $num_star[$star_name];

						//love
						if ($date_check_ok && preg_match_all("/star01.jpg/U", $rl, $MATCHES)){
							$love = count($MATCHES[0]);
							$love_num = ($love * 20);
						}
						//work
						if ($date_check_ok && preg_match_all("/star02.jpg/U", $rl, $MATCHES)){
							$work = count($MATCHES[0]);
							$work_num = ($work * 20);
						}
						//health
						if ($date_check_ok && preg_match_all("/star03.jpg/U", $rl, $MATCHES)){
							$health = count($MATCHES[0]);
							$health_num = ($health * 20);
						}

						$TOPIC_RESULT[$star_num] = array("love"=> $love_num , "work" => $work_num ,"health" => $health_num);
					}
				} //end 星座<li>~</li>ループ

				// date error?
				if(!$date_check_ok) {
					print $this->logDateError().PHP_EOL;
				}
				return $TOPIC_RESULT;
			}//if end
		}//foreach $LINES end
	}

}


