<?php
/**
 * @author Azet
 * @date 2016-01-05
 * @url http://www.vogue.co.jp/horoscope/daily
 */
class Zodiac000028 extends UranaiPlugin {

	/**
	 * このファンクションは必要です！
	 * 星座（インデックス） => 順位
	 * @return array[star] = rank;
	 */
	function run($CONTENTS) {
		// RESULTの形：12星座分, 1~12
		// $RESULT[<星座番号>] = <ランキング>
		// ランキングも1~12値です
		$RESULT = array();

		// サイト毎に星座名の設定
		//$star = self::$starDefault;
		/* 星座名が英語なのでカスタマイズする */
		$star["Aquarius"] = 1;
		$star["Pisces"] = 2;
		$star["Aries"] = 3;
		$star["Taurus"] = 4;
		$star["Gemini"] = 5;
		$star["Cancer"] = 6;
		$star["Leo"] = 7;
		$star["Virgo"] = 8;
		$star["Libra"] = 9;
		$star["Scorpio"] = 10;
		$star["Sagittarius"] = 11;
		$star["Capricorn"] = 12;

		// サイトによって情報を取得（しゅとく）

		// 日付の表示があれば、今日の日付と一致するか確認する！
		$now = self::getToday();
		// nowのキーは: year,month,day
		// monthは1~12の値
		// dayは1~31の値
		
		//取得パターンの作成
		$date_outpattern="/<h1 class=\"horoscope__single__header__date horoscope__single__header__date--daily\">[\s\S]*?<\/h1>/";
		$date_pattern = "/<strong>([\s\S]*?)<\/strong>/";
		$date = $now['month'].".".$now['day'];
		$star_outpattern="/<h1 class=\"horoscope__single__header__content__sign\">[\s\S]*?<\/strong>/";
		$star_pattern ="/<strong>([\s\S]*?)<\/strong>/";
		$rank_outpattern="/<p class=\"horoscope__single__header__content__no\">[\s\S]*?<\/p>/";
		$rank_pattern ="/Today's No\.(\d{1,2})/";

		foreach($CONTENTS as $content) {	//各星座ページごとのループ
			if (count($RESULT) == 12) { break; }
			//$content = mb_convert_encoding($content, "UTF-8", "SJIS");

			$rank_num = 0;
			$star_name = "";
			$date_check_ok = false;

			if(!$date_check_ok && preg_match($date_outpattern, $content,$MATCHES)) {
				$date_out=$MATCHES[0];
				if(preg_match($date_pattern, $date_out,$MATCHES)){
					$match = preg_replace("/\n|\r|\r\n/", "", $MATCHES[1]);
					$date_today =preg_replace("/( |　)/", "", $match);
				}
				if($date == $date_today){
					$date_check_ok = true;
				}
			}
				// rank check
			if($date_check_ok && !$rank_num && preg_match($rank_outpattern, $content,$MATCHES)) {
				$rank_out=$MATCHES[0];
				if(preg_match($rank_pattern, $rank_out,$MATCHES)){
					$rank_num = $MATCHES[1];
				}
			}
				// star check
			if($date_check_ok && !$star_name && preg_match($star_outpattern, $content,$MATCHES)) {
				$star_out=$MATCHES[0];
				if(preg_match($star_pattern, $star_out,$MATCHES)){
					$star_name =  str_replace("\r\n",'',trim($MATCHES[1]));
				}
			}
			// star
			if ($rank_num && $star_name) {
				$star_num = $star[$star_name];

				// RESULTの形：
				// $RESULT[<星座番号>] = <ランキング>
				$RESULT[$star_num] = $rank_num;

			}
		}

		// date error?
		if(!$date_check_ok) {
			print $this->logDateError().PHP_EOL;
		}

//		print_r($RESULT);
		return $RESULT;
	}

	function topic_run($TOPIC_CONTENTS) {
		$now = self::getToday();
		$date_pattern = "/{$now['month']}\.{$now['day']}/";

		$TOPIC_RESULT = array();
		foreach($TOPIC_CONTENTS as $key => $topic_content) {
			//$content = mb_convert_encoding($content, "UTF-8", "UTF-8");		
			$TOPIC_LINES = explode("\n", $topic_content);
			$date_check_ok = false;
			$KEY = explode("_", $key);
			$star_num = $KEY[0];
			$topic_type = $KEY[1];
			$count = 0;
			foreach ($TOPIC_LINES as $topic_line) {
				// date check
				if(!$date_check_ok) {
					$month_check = strpos($topic_line,$now['month']);
					if($month_check == true){
						$date_check_ok = strpos($topic_line,$now['day']);
						continue;
					}
				}
				
				if ($date_check_ok && preg_match("/<strong>(\d{1})<\/strong>/", $topic_line,$MATCHES)) {
					$count = $MATCHES[1];
					$count_num = ( $count * 20);
				}
			}
			$TOPIC_RESULT[$star_num][$topic_type] = $count_num;

			// date error?
			if(!$date_check_ok) {
				print $this->logDateError().PHP_EOL;
			}

		}

		return $TOPIC_RESULT;
	}

	function topic_load($URL){
		$t_url = array();
		$TOPIC = array( 'love' => '' ,'interpersonal' => '/2' ,'work' => '/3' ,'money' => '/4','health' => '/5');
		foreach($URL as $key => $url){
			$U = explode("#", $url);
			foreach($TOPIC as $k => $topic){
				$t_key = $key."_".$k;
				$t_url[$t_key] = $U[0].$topic."#".$U[1];
			}
		}
		print_r ($t_url);
		return parent::load($t_url);
	}
}
