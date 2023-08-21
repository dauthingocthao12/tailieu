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
	function run($URL) {
		// RESULTの形：12星座分, 1~12
		// $RESULT[<星座番号>] = <ランキング>
		// ランキングも1~12値です
		$RESULT = array();

		// オプション：URLに日付が必要の場合は、下記の処理をする
		// 記号を取得する日付に変換する
		foreach ($URL as $key => $url) {
			$url = str_replace("(Y)", date("Y"), $url); // 年
			$url = str_replace("(M)", intval(date("m")), $url); // 月
			$url = str_replace("(d)", intval(date("d")), $url); // 日
			$URL[$key] = $url;
		}
		//CONTENT配列にURLごとのHTMLを格納する
		$CONTENTS = $this->load($URL);

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
		$date_pattern ="/{$now['month']}\.{$now['day']}/";
		$star_outpattern="/<h1 class=\"horoscope__single__header__content__sign\">[\s\S]*?<\/strong>/";
		$star_pattern ="/<strong>([\s\S]*?)<\/strong>/";
		$rank_outpattern="/<p class=\"horoscope__single__header__content__no\">[\s\S]*?<\/p>/";
		$rank_pattern ="/Today's No\.(\d{1,2})/";

		foreach($CONTENTS AS $content) {	//各星座ページごとのループ
			if (count($RESULT) == 12) { break; }
			//$content = mb_convert_encoding($content, "UTF-8", "SJIS");

			$rank_num = 0;
			$star_name = "";
			$date_check_ok = false;
				// date check
			if(!$date_check_ok && preg_match($date_outpattern, $content,$MATCHES)) {
				$date_out=$MATCHES[0];
				if(preg_match($date_pattern, $date_out,$MATCHES)){
					$date_check_ok = $MATCHES[0];
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
}
