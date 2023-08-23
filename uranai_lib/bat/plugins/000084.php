<?php
/**
 * 占いしようよ
 *
 * @author Azet
 * @date 2017-06-23
 * @url http://usyo.net/a/j/sign/
 */
class Zodiac000084 extends UranaiPlugin {

	function run($URL) {
		$CONTENTS = $this->load($URL);
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
}


