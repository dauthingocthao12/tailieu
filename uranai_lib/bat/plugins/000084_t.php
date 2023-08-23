<?php
/**
 * 占いしようよ
 *
 * @author Azet
 * @date 2017-06-23
 * @url http://usyo.net/a/j/sign/
 */
class Zodiac000084 extends UranaiPlugin {

	function run($CONTENTS) {
		$content = $CONTENTS[0]; //全体URLを使用する
		/*
		* $RESULT[星座番号] => 順位
		*/
		$RESULT = array();
		$LINES = explode("\n", $content);
		
		/* サイト側の星座番号の決まり　*/
		$site_star = array(
			0 => 'おひつじ座',
			1 => 'おうし座',
			2 => 'ふたご座',
			3 => 'かに座',
			4 => 'しし座',
			5 => 'おとめ座',
			6 => 'てんびん座',
			7 => 'さそり座',
			8 => 'いて座',
			9 => 'やぎ座',
			10 => 'みずがめ座',
			11 => 'うお座'
		);
		
		$star = self::$starDefault;
		
		$now = self::getToday();
		
		$date_pattern = "/{$now['month']}月{$now['day']}日の運勢<\/p>/";
		$date_check_ok = false;

		$rank = 0;
		$count = 1;
		
		foreach ($LINES AS $L) {
			/*12星座分のデータがリザルトにある時処理を抜ける*/
			if (count($RESULT) == 12) { break; }

			/*日付の判定*/
			if (!$date_check_ok) {
				$date_check_ok = preg_match($date_pattern, $L);
				continue;
			}
			/*順位,星座の取得*/
			if($date_check_ok && $rank == 0){
			
				if(preg_match("/sharedimg\/icon_12sign_(\d{1,2})\.png/", $L, $MATCHES)){ //星座番号
					$site_star_num = $MATCHES[1];
					$star_num = $star[$site_star[$site_star_num]]; //サイト側の星座番号→占いランキングの星座番号
				}
				
				if($count < 4){
					$rank_reg = "/sharedimg\/icon_rank_(\d{1,2})\.png\"/";
				}else{
					$rank_reg = "/(\d{1,2})\s?<\/span>位<\/span>/";
				}
				
				if(preg_match($rank_reg, $L, $MATCHES)){ //順位
					$rank = $MATCHES[1];
					if($count < 4){
						$rank = str_replace("0","",$rank);
					}
					$RESULT[$star_num] = $rank;
					$rank = 0;
					$count++;
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

	function topic_run($TOPIC_CONTENTS) {
		$content = $TOPIC_CONTENTS[0]; //全体URLを使用する
		/*
		* $RESULT[星座番号] => 順位
		*/
		$TOPIC_RESULT = array();
		$LINES = explode("\n", $content);
		
		/* サイト側の星座番号の決まり　*/
		//$site_star = array(
		//	0 => 'おひつじ座',
		//	1 => 'おうし座',
		//	2 => 'ふたご座',
		//	3 => 'かに座',
		//	4 => 'しし座',
		//	5 => 'おとめ座',
		//	6 => 'てんびん座',
		//	7 => 'さそり座',
		//	8 => 'いて座',
		//	9 => 'やぎ座',
		//	10 => 'みずがめ座',
		//	11 => 'うお座'
		//);
		
		$star = self::$starDefault;
		
		$now = self::getToday();
		
		$date_pattern = "/{$now['month']}月{$now['day']}日の運勢<\/p>/";
		$date_check_ok = false;
		$count = 1;
		$flag=0;
		foreach ($LINES AS $L) {
			/*12星座分のデータがリザルトにある時処理を抜ける*/
			if (count($TOPIC_RESULT) == 12) { break; }

			/*日付の判定*/
			if (!$date_check_ok) {
				$date_check_ok = preg_match($date_pattern, $L);
				continue;
			}
			/*星座の取得*/
			
			if($date_check_ok && !$flag && preg_match("/　　　　　(.*座)<div class=\"date_12sign\">/", $L, $MATCHES)){ //星座番号
					$star_name = $MATCHES[1];
					$star_num = $star[$star_name];
					$flag=1;
			}
			if($date_check_ok && $flag && $star_num && preg_match("/<b>恋愛運(.*?)<\/p>/i", $L, $MATCHES)){
				$love_content = $MATCHES[1];
				$love = substr_count($love_content , '<img src="../../../common/sharedimg/icon_heart_on.png" />');
				$love_num = ($love * 20);
			}
			if($date_check_ok && $flag && $star_num && preg_match("/<b>仕事運(.*?)<\/p>/i", $L, $MATCHES)){
				$work_content = $MATCHES[1];
				$work = substr_count($work_content , '<img src="../../../common/sharedimg/icon_job_on.png" />');
				$work_num = ($work * 20);
			}
			if($date_check_ok && $flag && $star_num && preg_match("/<b>金　運(.*?)<p>/i", $L, $MATCHES)){
				$money_content = $MATCHES[1];
				$money = substr_count($money_content , '<img src="../../../common/sharedimg/icon_money_on.png" />');
				$money_num = ($money * 20);

				$TOPIC_RESULT[$star_num] = array("love"=> $love_num , "money" => $money_num ,"work" => $work_num);
				$flag=0;
				$star_num=0;
			}
				
		}
		// エラーチェック
		if(!$date_check_ok) {
			print $this->logDateError().PHP_EOL;
		}
		//print_r ($RESULT);
		return $TOPIC_RESULT;
	}
}


