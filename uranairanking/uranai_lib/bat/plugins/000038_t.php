<?php
/**
 * @author Azet
 * @date 2016-02-05
 * @url http://www.kiilife.jp/uranai/
 * updated: okabe 2017/06/22
 */
class Zodiac000038 extends UranaiPlugin {

	/**
	 * このファンクションは必要です！
	 * 星座（インデックス） => 順位
	 * @return array[star] = rank;
	 */
	//function run($URL) {			//del okabe 2017/06/22
	function run($CONTENTS) {   // add okabe 2017/06/22 $URL -> $CONTENTS

		// RESULTの形：12星座分, 1~12
		// $RESULT[<星座番号>] = <ランキング>
		// ランキングも1~12値です
		$RESULT = array();
		//$CONTENTS = array();		//del okabe 2017/06/22

		// サイトhtmlを取得
		//$CONTENTS = $this->load($URL);	//del okabe 2017/06/22

		// サイト毎に星座名のプラグイン個別設定
		$star = self::$starDefault;

		// 日付の表示があれば、今日の日付と一致するか確認する！
		$now = self::getToday();
		// nowのキーは: year,month,day
		// monthは1~12の値
		// dayは1~31の値

		//星座ごとにループする
		for ($i = 1; $i < 13; $i++) {
			// このプラグインでは、1~12の星座ごとのURLを使用する
			$content = $CONTENTS[$i];
//			$content = mb_convert_encoding($content);	//不要 元々がUTF-8
			//星座ごとのURLを１つずつパース処理する
			
			$LINES = explode("\n", $content);
			$star_check = false;
			$date_check_ok = false;		//日付のパースチェック結果
			$star_num = 0;

			foreach ($LINES AS $line) {


				$date_pattern ="/<div class=\"post-content\">.* (.*座).*<\/h/";
				if(!$star_check) {
					$star_check = preg_match($date_pattern, $line,$MATCHES);	//星座チェック結果格納
					$star_name = $MATCHES[1];
					$star_num = $star[$star_name];
					continue;
				}


				$date_pattern ="/h2>{$now['month']}月{$now['day']}日の運勢<\/h/";

				// date check
				if(!$date_check_ok) {
					$date_check_ok = preg_match($date_pattern, $line);	//日付チェック結果格納
					continue;
				}

				// parse datas
				if ($date_check_ok && $star_check ) {
					if (preg_match("/>《12星座中：第(\d{1,2})位》<\/h3>/", $line, $MATCHES2)) {
						$RESULT[$star_num] = $MATCHES2[1];
						break;
					}
				}
			}
		}

		return $RESULT;
	}


	// add okabe start 2017/06/22
	function topic_run($TOPIC_CONTENTS) {
		$TOPIC_RESULT = array();

		// サイト毎に星座名のプラグイン個別設定
		$star = self::$starDefault;

		// 日付の表示があれば、今日の日付と一致するか確認する！
		$now = self::getToday();
		// nowのキーは: year,month,day
		// monthは1~12の値
		// dayは1~31の値

		//星座ごとにループする
		for ($i = 1; $i < 13; $i++) {
			// このプラグインでは、1~12の星座ごとのURLを使用する
			$content = $TOPIC_CONTENTS[$i];
//			$content = mb_convert_encoding($content, "UTF-8","SJIS");
			//星座ごとのURLを１つずつパース処理する

			$star_num = 0;
			
			
			$star_check = false;
			$date_check_ok = false;		//日付のパースチェック結果
			$all_check_ok = false;		//日付のパースチェック結果
			

			$love_val =0;
			$money_val =0;
			$work_val =0;

			$LINES = explode("\n", $content);

			foreach ($LINES AS $line) {
				if (count($TOPIC_RESULT) == 12) { break; }
				
				$date_pattern ="/<div class=\"post-content\">.* (.*座).*<\/h/";
				if(!$star_check) {
					$star_check = preg_match($date_pattern, $line,$MATCHES);	//星座チェック結果格納
					$star_name = $MATCHES[1];
					$star_num = $star[$star_name];
					continue;
				}


				$date_pattern ="/h2>{$now['month']}月{$now['day']}日の運勢<\/h/";

				// date check
				if(!$date_check_ok) {
					$date_check_ok = preg_match($date_pattern, $line);	//日付チェック結果格納
					continue;
				}

				// parse datas
				if ($date_check_ok && $star_check ) {
					if (preg_match("/>《12星座中：第(\d{1,2})位》<\/h3>/", $line, $MATCHES2)) {
						$RESULT[$star_num] = $MATCHES2[1];
						$all_check_ok = true;
						continue;
					}
				}

				//恋愛運
				if($star_num > 0 && $all_check_ok) {
					$chk = preg_match("/h2>愛情運：(.*)<\/h2>/", $line, $MATCHS);
					if($chk){
						$love_val = mb_strlen($MATCHS[1]);
					}
				}

				//金銭運
				if($star_num > 0 && $all_check_ok) {
					$chk = preg_match("/h2>金　運：(.*)<\/h2>/", $line, $MATCHS);
					if($chk){
						$money_val = mb_strlen($MATCHS[1]);
					}
				}

				//仕事運
				if($star_num > 0 && $all_check_ok) {
					$chk = preg_match("/h2>仕事運：(.*)<\/h2>/", $line, $MATCHS);
					if($chk){
						$work_val = mb_strlen($MATCHS[1]);
					}
				}

				if ($star_num > 0 && $love_val > 0 && $money_val > 0 && $work_val > 0) {
					$love_num = $love_val * 20;
					$money_num = $money_val * 20;
					$work_num = $work_val * 20;

					$TOPIC_RESULT[$star_num] = array("love"=> $love_num , "money" => $money_num ,"work" => $work_num);
					break;
				}

			}
		}

		return $TOPIC_RESULT;
	}
	// add okabe end 2017/06/22

}
