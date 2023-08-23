<?php
/**
 * @author Azet
 * @date 2016-02-25
 * @url http://www.daily.co.jp/gossip/fortune/
 * updated: okabe 2017/06/23
 */
class Zodiac000044 extends UranaiPlugin {

	/**
	 * このファンクションは必要です！
	 * 星座（インデックス） => 順位
	 * @return array[star] = rank;
	 */
	//function run($URL) {			//del okabe 2017/06/23
	function run($CONTENTS) {		// add okabe 2017/06/23 $URL -> $CONTENTS
		// RESULTの形：12星座分, 1~12
		// $RESULT[<星座番号>] = <ランキング>
		// ランキングも1~12値です
		$RESULT = array();

		//$CONTENTS = $this->load($URL);		//del okabe 2017/06/23
		// 日付の表示があれば、今日の日付と一致するか確認する！
		// (このプラグインでは、取得時に日付設定をしているので、チェックしていません)
		$now = self::getToday();
		// nowのキーは: year,month,day
		// monthは1~12の値
		// dayは1~31の値
		//$date_pattern = "/<p class=\"date\">0?{$now['month']}月0?{$now['day']}日更新<\/p>/";
		$date_pattern = "/今日の星占い\s+<time>{$now['year']}.{$now['month']}.{$now['day']}<\/time>/";

		$star = array(
			"aquarius" => "1"
			,"pisces" => "2"
			,"aries" => "3"
			,"taurus" => "4"
			,"gemini" => "5"
			,"cancer" => "6"
			,"leo" => "7"
			,"virgo" => "8"
			,"libra" => "9"
			,"scorpius" => "10"
			,"sagittarius" => "11"
			,"capriconus" => "12"
		);

		// このプラグインは、０ の URL データしか使用しません
		$content = $CONTENTS[0];
		//$content = mb_convert_encoding($content, "UTF-8","SJIS");
		$LINES = explode("\n", $content);

		$date_check_ok = false;
		foreach ($LINES AS $line) {
			if (count($RESULT) == 12) { break; }

			// date check
			if(!$date_check_ok) {
				$date_check_ok = preg_match($date_pattern, $line);
			}

			// rank 通常
			if ($date_check_ok && preg_match("/<article class=\"fortune-item rank-(\d{1,2})\s([a-z]*)\">/", $line, $MATCHES)) {
				$rank_num = $MATCHES[1];		//第(\d{1,2})位
				$star_code = $MATCHES[2];	//星座の英語名　aquarius,piscesなど...
				$star_num = $star[$star_code]; //aquarius => 1,pisces => 2

				// RESULTの形：
				// $RESULT[<星座番号>] = <ランキング>
				$RESULT[$star_num] = $rank_num;
			}

		}

		// date error?
		if(!$date_check_ok) {
			print $this->logDateError().PHP_EOL;
		}

		return $RESULT;
	}
}
