<?php
/**
 * @author Azet
 * @date 2016-02-24
 * @url http://blog.machikurublog.jp/riverfield/kiji/57204.html
 *        → http://blog.machikurublog.jp/riverfield/theme/1492.html
 */
class Zodiac000041 extends UranaiPlugin {

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

		$CONTENTS = $this->load($URL);
		// 日付の表示があれば、今日の日付と一致するか確認する！
		// (このプラグインでは、取得時に日付設定をしているので、チェックしていません)
		$now = self::getToday();
		// nowのキーは: year,month,day
		// monthは1~12の値
		// dayは1~31の値
		$date_pattern = "/class=\"entry_title\">0?{$now['month']}\/0?{$now['day']}今日の12星座占い<\/h2>/";

        // サイト毎に星座名の設定
		$star = self::$starDefault;

		// このプラグインは、０ の URL データしか使用しません
		$content = $CONTENTS[0];
		//$content = mb_convert_encoding($content, "UTF-8","SJIS");	//不要 元々がUTF-8
		$LINES = explode("\n", $content);

		$date_check_ok = false;
		$parse_flag = 0;	//日付出現前,1:日付出現中（星座別データの抽出中）,2:データ出現後の識別文字を確認した場合
		foreach ($LINES AS $line) {
			if (count($RESULT) == 12 || $parse_flag == 2) { break; }

			// date check
			if(!$date_check_ok && $parse_flag == 0) {
				$date_check_ok = preg_match($date_pattern, $line);
				if ($date_check_ok) {
					$parse_flag = 1;
				}
				continue;
			}

			//データ１日分が終了したことの確認
			if (preg_match("/<div class=\"entry_footer\">/", $line)) {
				$parse_flag == 2;
				break;
			}

			// rank 通常取得中
			if ($parse_flag == 1) {
				$res = preg_match("/>(\d{1,2})\.(.*座)</", $line, $MATCHES);
				if ($res) {
					$rank_num = $MATCHES[1];	//第(\d{1,2})位
					$star_name = $MATCHES[2];	//(.*?座)
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
