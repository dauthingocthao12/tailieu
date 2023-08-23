<?php
/**
 * @author Azet
 * @date 2016-02-24
 * @url http://www.tbs.co.jp/hayadoki/gudetama/
 * updated: okabe 2017/06/23
 */
class Zodiac000051 extends UranaiPlugin {

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

		//$CONTENTS = $this->load($URL);	//del okabe 2017/06/23
		// 日付の表示があれば、今日の日付と一致するか確認する！
		// (このプラグインでは、取得時に日付設定をしているので、チェックしていません)
		$now = self::getToday();
		// nowのキーは: year,month,day
		// monthは1~12の値
		// dayは1~31の値
		$date_pattern = "/{$now['year']}年0?{$now['month']}月0?{$now['day']}日/";

		// サイト毎に星座名の設定
		$star = array(	
			'mizugame' => 1,	//水瓶座
			'uo' => 2,			//魚座
			'ohitsuji' => 3,	//牡羊座
			'oushi' => 4,		//牡牛座
			'futago' => 5,		//双子座
			'kani' => 6,		//蟹座
			'shishi' => 7,		//獅子座
			'otome' => 8,		//乙女座
			'tenbin' => 9,		//天秤座
			'sasori' => 10,		//蠍座
			'ite' => 11,		//射手座
			'yagi' => 12		//山羊座
		);

		// このプラグインは、０ の URL データしか使用しません
		$content = $CONTENTS[0];
		$content = mb_convert_encoding($content, "UTF-8","JIS");
		$LINES = explode("\n", $content);

		$date_check_ok = false;
		$rank_num = 0;
		foreach ($LINES AS $line) {
			if (count($RESULT) == 12) { break; }

			// date check
			if(!$date_check_ok) {
				$date_check_ok = preg_match($date_pattern, $line);
			}

			// rank 通常
			if ($date_check_ok) {
				$flag = preg_match("/class=\"alt\">(\d{1,2})位<\/span><span\x20id=\"(.*)\"><span\x20class=\"alt\">星座<\//", $line, $MATCHES);
				if($flag) {
					$rank_num = $MATCHES[1];
					$star_name = $MATCHES[2];
					$star_num = $star[$star_name];
					// RESULTの形：
					// $RESULT[<星座番号>] = <ランキング>
					$RESULT[$star_num] = $rank_num;
				}
			}

		}

		// date error?
		if(!$date_check_ok) {
			print $this->logDateError().PHP_EOL;
		}

		return $RESULT;
	}
}
