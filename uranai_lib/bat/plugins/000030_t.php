<?php
/**
 * @author Azet
 * @date 2016-01-13
 * @url http://tjokayama.jp/horoscope
 */
class Zodiac000030 extends UranaiPlugin {

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


		// このプラグインが、０のURLしか使用しません
		$content = $CONTENTS[0];

		// 必要の時に、下記を直して下さい。
		//$content = mb_convert_encoding($content, "UTF-8", "UTF-8");
		$LINES = explode("\n", $content);

		// サイト毎に星座名の設定
		$star = self::$starDefault;

		// サイトによって情報を取得（しゅとく）

		// 日付の表示があれば、今日の日付と一致するか確認する！
		$now = self::getToday();

		// nowのキーは: year,month,day
		// monthは1~12の値(全角)
		// dayは1~31の値(全角)
		$month = mb_convert_kana($now['month'],"N","UTF-8");
		$day = mb_convert_kana($now['day'],"N","UTF-8");
		$date_pattern = "/{$month}月{$day}日の星座占いランキング/";

		$rank_num = 0;
		$date_check_ok = false;
		foreach ($LINES AS $line) {

			if (count($RESULT) == 12) { break; }

			// date check
			if(!$date_check_ok) {
				$date_check_ok = preg_match($date_pattern, $line);
				continue;
			}

			// rank
			if ($date_check_ok && !$rank_num && preg_match("/<p class=\"horoscope-panel__rank\">(\d{1,2})位<\/p>/", $line, $MATCHES)) {
				$rank_num = $MATCHES[1];
				continue;
			}

			// star
			if ($rank_num && preg_match("/<h1 class=\"horoscope-panel__name\">(.*?座)<\/h1>/", $line, $MATCHES)) {
				$star_name = $MATCHES[1];
				$star_num = $star[$star_name];

				// RESULTの形：
				// $RESULT[<星座番号>] = <ランキング>
				$RESULT[$star_num] = $rank_num;

				// reset
				$rank_num = 0;
			}

		}

		// date error?
		if(!$date_check_ok) {
			print $this->logDateError().PHP_EOL;
		}

		return $RESULT;
	}
	function topic_run($TOPIC_CONTENTS) {

		// RESULTの形：12星座分, 1~12
		// $RESULT[<星座番号>] = <ランキング>
		// ランキングも1~12値です
		$TOPIC_RESULT = array();
		// サイト毎に星座名の設定
		$star = self::$starDefault;
		// 日付の表示があれば、今日の日付と一致するか確認する！
		$now = self::getToday();
		// nowのキーは: year,month,day
		// monthは1~12の値(全角)
		// dayは1~31の値(全角)
		$month = mb_convert_kana($now['month'],"N","UTF-8");
		$day = mb_convert_kana($now['day'],"N","UTF-8");
		$date_pattern = "/{$month}月{$day}日の星座占いランキング/";

		foreach($TOPIC_CONTENTS as $topic_content) {
			// 必要の時に、下記を直して下さい。
			//$content = mb_convert_encoding($content, "UTF-8", "UTF-8");
			$TOPIC_LINES = explode("\n", $topic_content);
			$date_check_ok = false;
			foreach ($TOPIC_LINES as $topic_line) {

				if (count($TOPIC_RESULT) == 12) { break; }

				// date check
				if(!$date_check_ok) {
					$date_check_ok = preg_match($date_pattern, $topic_line);
					continue;
				}

				// star
				if ($date_check_ok && preg_match("/<h1 class=\"horoscope__title\">(.*?座)の<br>今日の運勢<\/h1>/", $topic_line, $MATCHES)) {
					$star_name = $MATCHES[1];
					$star_num = $star[$star_name];
				}

				//love
				if ($date_check_ok && preg_match("/<h1 class=\"horoscope-rate__title\">恋愛運<\/h1>/", $topic_line)) {
					$love_flg = 1;
				}

				if ($date_check_ok && $love_flg && preg_match("/<div class=\"horoscope-rate__star horoscope-rate__star-(\d{1})\">/", $topic_line, $MATCHES)) {
					$love = $MATCHES[1];
					$love_num = ($love * 20);
					$love_flg = 0;
				}

				//money
				if ($date_check_ok && preg_match("/<h1 class=\"horoscope-rate__title\">金運<\/h1>/", $topic_line)) {
					$money_flg = 1;
				}

				if ($date_check_ok && $money_flg && preg_match("/<div class=\"horoscope-rate__star horoscope-rate__star-(\d{1})\">/", $topic_line, $MATCHES)) {
					$money = $MATCHES[1];
					$money_num = ($money * 20);
					$money_flg = 0;
				}

				//work
				if ($date_check_ok && preg_match("/<h1 class=\"horoscope-rate__title\">仕事運<\/h1>/", $topic_line)) {
					$work_flg = 1;
				}

				if ($date_check_ok && $work_flg && preg_match("/<div class=\"horoscope-rate__star horoscope-rate__star-(\d{1})\">/", $topic_line, $MATCHES)) {
					$work = $MATCHES[1];
					$work_num = ($work * 20);
					$work_flg = 0;
				}
			}

			$TOPIC_RESULT[$star_num] = array("love"=> $love_num , "money" => $money_num ,"work" => $work_num);
			// date error?
			if(!$date_check_ok) {
				print $this->logDateError().PHP_EOL;
			}
		}
		return $TOPIC_RESULT;
	}
}
