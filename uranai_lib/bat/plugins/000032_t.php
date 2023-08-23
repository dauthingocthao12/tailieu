<?php
/**
 * @author Azet
 * @date 2016-02-05
 * @url http://www.yomiuri.co.jp/komachi/fortune/horoscope/
 *      実体はコチラ ⇒ http://server11.happywoman.jp/12star_yomiuri/index.html
 */
class Zodiac000032 extends UranaiPlugin {

	/**
	 * このファンクションは必要です！
	 * 星座（インデックス） => 順位
	 * @return array[star] = rank;
	 */
	function run($CONTENTS) {

		// RESULTの形：12星座分, 1~12
		// $RESULT[<星座番号>] = <ランキング>
		// ランキングも1~12値です

		//結果を格納する配列
		$RESULTS = array();

		// 日付の表示があれば、今日の日付と一致するか確認する！
		$now = self::getToday();
		// nowのキーは: year,month,day
		// monthは1~12の値
		// dayは1~31の値
		$date_pattern = "/<time datatime.*>{$now['year']}年0?{$now['month']}月0?{$now['day']}日<\/time>/";

		//星座ごとにループする
		for ($i = 1; $i < 13; $i++) {

			// このプラグインでは、1~12の星座ごとのURLを使用する
			$content = $CONTENTS[$i];
			//$content = mb_convert_encoding($content, "UTF-8","SJIS");	//不要 元々がUTF-8
			$LINES = explode("\n", $content);

			//星座ごとのURLを１つずつパース処理する
			$rank_num = 0;		//ランク値の格納先
			$date_check_ok = false;		//日付のパースチェック結果
			foreach ($LINES AS $line) {
				// date check
				if(!$date_check_ok) {
					$date_check_ok = preg_match($date_pattern, $line);
//					continue;
				}

				// parse datas
				if ($date_check_ok) {
					if (preg_match("/class=\"horoscope-single__resultRank.*\">(\d{1,2})<span>位<\/span>/", $line, $MATCHES)) {
						$RESULTS[$i] = $MATCHES[1];
						break;
					}
				}

			}

			if(!$date_check_ok) {
				print $this->logDateError().PHP_EOL;
			}

		}

		return $RESULTS;
	}

	function topic_run($TOPIC_CONTENTS) {

		// RESULTの形：12星座分, 1~12
		// $RESULT[<星座番号>] = <ランキング>
		// ランキングも1~12値です

		//結果を格納する配列
		$TOPIC_RESULT = array();
//		$star = self::$starDefault;

		// 日付の表示があれば、今日の日付と一致するか確認する！
		$now = self::getToday();
		// nowのキーは: year,month,day
		// monthは1~12の値
		// dayは1~31の値
		$date_pattern = "/<time datatime.*>{$now['year']}年0?{$now['month']}月0?{$now['day']}日<\/time>/";

		//星座ごとにループする
		foreach($TOPIC_CONTENTS as $key => $topic_content) {

			//$content = mb_convert_encoding($content, "UTF-8","SJIS");	//不要 元々がUTF-8
			$TOPIC_LINES = explode("\n", $topic_content);

			//星座ごとのURLを１つずつパース処理する
			$date_check_ok = false;		//日付のパースチェック結果

			$KEY = explode("_", $key);
			$star_num = $KEY[0];
			$topic_type = $KEY[1];
			$count = 0;

			foreach ($TOPIC_LINES as $topic_line) {
				// date check
				if(!$date_check_ok) {
					$date_check_ok = preg_match($date_pattern, $topic_line);
//					continue;
				}

				if ($date_check_ok && preg_match("/<p class=\"horoscope-single__grade\">(.*)<\/p>/", $topic_line, $MATCHES)) {
					$content = $MATCHES[1];
					$count = substr_count($content , '★');
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

	function lucky_run($TOPIC_CONTENTS) {

		// RESULTの形：12星座分, 1~12
		// $RESULT[<星座番号>] = <ランキング>
		// ランキングも1~12値です

		//結果を格納する配列
		$LUCKY_RESULT = array();

		// 日付の表示があれば、今日の日付と一致するか確認する！
		$now = self::getToday();
		// nowのキーは: year,month,day
		// monthは1~12の値
		// dayは1~31の値
		$date_pattern = "/<time datatime.*>{$now['year']}年0?{$now['month']}月0?{$now['day']}日<\/time>/";

		//星座ごとにループする
		foreach($TOPIC_CONTENTS as $key => $topic_content) {

			//$content = mb_convert_encoding($content, "UTF-8","SJIS");	//不要 元々がUTF-8
			$TOPIC_LINES = explode("\n", $topic_content);

			//星座ごとのURLを１つずつパース処理する
			$date_check_ok = false;		//日付のパースチェック結果

			$KEY = explode("_", $key);
			$star_num = $KEY[0];
			$topic_type = $KEY[1];

			foreach ($TOPIC_LINES as $topic_line) {
				// date check
				if(!$date_check_ok) {
					$date_check_ok = preg_match($date_pattern, $topic_line);
//					continue;
				}

				if ($date_check_ok && preg_match("/<th>ラッキーカラー：<\/th>/", $topic_line, $MATCHES)) {
					$lucky_color_flg = true;
				}

				if($date_check_ok && $lucky_color_flg && preg_match("/<td>(.*?)<\/td>/", $topic_line, $MATCHES)){
					$lucky_color = $MATCHES[1];
					$lucky_color_flg = false;
				}

				if ($date_check_ok && preg_match("/<th>ラッキーアイテム：<\/th>/", $topic_line, $MATCHES)) {
					$lucky_item_flg = true;
				}

				if($date_check_ok && $lucky_item_flg && preg_match("/<td>(.*?)<\/td>/", $topic_line, $MATCHES)){
					$lucky_item = $MATCHES[1];
					$lucky_item_flg = false;
				}
			}
			$LUCKY_RESULT[$star_num] = array("lucky_item"=> $lucky_item , "lucky_color" => $lucky_color);

			// date error?
			if(!$date_check_ok) {
				print $this->logDateError().PHP_EOL;
			}
		}
		return $LUCKY_RESULT;
	}

	function topic_load($URL){
		$t_url = array();
		$TOPIC = array( 'love' => 'type=love' ,'work' => 'type=work' ,'money' => 'type=money','health' => 'type=health');
		foreach($URL as $key => $url){
			$U = explode("#", $url);
			foreach($TOPIC as $k => $topic){
				$t_key = $key."_".$k;
//				$t_url[$t_key] = $U[0].$topic."#".$U[1];
				$t_url[$t_key] = $U[0]."/?".$topic."#".$U[1];
			}
		}
		print_r ($t_url);
		return parent::load($t_url);
	}
}
