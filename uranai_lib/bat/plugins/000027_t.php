<?php
/**
 * @author Azet
 * @date 2016-01-13
 * @url http://goisu.net/daily/
 */
class Zodiac000027 extends UranaiPlugin {

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
		//$star = self::$starDefault;
		/* 星座名が漢字なのでカスタマイズする */
		$star["水瓶座"] = 1;
		$star["魚座"] = 2;
		$star["牡羊座"] = 3;
		$star["牡牛座"] = 4;
		$star["双子座"] = 5;
		$star["蟹座"] = 6;
		$star["獅子座"] = 7;
		$star["乙女座"] = 8;
		$star["天秤座"] = 9;
		$star["蠍座"] = 10;
		$star["射手座"] = 11;
		$star["山羊座"] = 12;

		// サイトによって情報を取得（しゅとく）

		// 日付の表示があれば、今日の日付と一致するか確認する！
		// (このプラグインでは、取得時に日付設定をしているので、チェックしていません)
		$now = self::getToday(); // 01のような2ケタ数字に未対応
		// $now = array(
		// 	'year' => date('Y'),
		// 	'month' => date('m'),
		// 	'day' => date('d')
		// );

		// nowのキーは: year,month,day
		// monthは01~12の値
		// dayは01~31の値
		//$date_pattern = "/<h2>運勢ランキング {$now['year']}\/{$now['month']}\/{$now['day']}<\/h2>/";	//del okabe 2016/06/06 サイトデータフォーマット変更
		$date_pattern = "/<span>{$now['year']}\/0*{$now['month']}\/0*{$now['day']}<\/span>/";	//add okabe 2016/06/06

		$rank_num = 0;
		$date_check_ok = false;
		foreach ($LINES AS $line) {
			if (count($RESULT) == 12) { break; }

			// date check
			if(!$date_check_ok) {
				$date_check_ok = preg_match($date_pattern, $line);
			}
			$l_dtl = [];
            if($date_check_ok){
                $l_dtl = explode(">", $line);
            }
			if(!empty($l_dtl)){
				foreach($l_dtl AS $l){
					// rank&star
					if ($date_check_ok && !$rank_num && preg_match("/(\d{1,2})位 (.*?座)<\/h3/", $l, $MATCHES)) {
						$rank_num = $MATCHES[1];
						$star_name = $MATCHES[2];
						$star_num = $star[$star_name];

						// RESULTの形：
						// $RESULT[<星座番号>] = <ランキング>
						$RESULT[$star_num] = $rank_num;

						// reset
						$rank_num = 0;

						continue;
					}
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
