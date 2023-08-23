<?php
/**
 * @author Azet
 * @date 2016-02-12
 * @url http://www.tnc.ne.jp/fortune/
 */
class Zodiac000035 extends UranaiPlugin {

	/**
	 * このファンクションは必要です！
	 * 星座（インデックス） => 順位
	 * @return array[star] = rank;
	 */
	function run($URL) {

/*テスト中です
$mycurl = self::$curlParamsDefault;
$mycurl[CURLOPT_FRESH_CONNECT] = 0;
		$this->useCurl($mycurl);
*/
		// RESULTの形：12星座分, 1~12
		// $RESULT[<星座番号>] = <ランキング>
		// ランキングも1~12値です
		$CONTENTS = array();

		//結果を格納する配列
		$RESULTS = array();

		// サイト毎に星座名の設定
		$star = self::$starKanji;

		// 日付の表示があれば、今日の日付と一致するか確認する！
		$now = self::getToday();
		// nowのキーは: year,month,day
		// monthは1~12の値
		// dayは1~31の値
		$date_pattern = "/<div class=\"ftdate\">{$now['year']}年<br \/>{$now['month']}月{$now['day']}日<\/div>/";

		// サイトhtmlを取得
		$CONTENTS = $this->loadAll($URL);

		// このプラグインでは、まず一覧ページから日付をチェックする
		$content = $CONTENTS[0];
		//$content = mb_convert_encoding($content, "UTF-8","SJIS");	//不要 元々がUTF-8

		$LINES = explode("\n", $content);
		//１行ずつパースして日付をチェックする
		$date_check_ok = false;		//日付のパースチェック結果
		foreach ($LINES AS $line) {
			$date_check_ok = preg_match($date_pattern, $line);	//日付チェック結果格納
			if ($date_check_ok) { break; }
		}

		//日付のチェックがOKならば、各星座ごとのファイルから情報を抽出する
		if ($date_check_ok) {

			//星座ごとにループする
			for ($i = 1; $i < 13; $i++) {
				// このプラグインでは、1~12の星座ごとのURLを使用する
				$content = $CONTENTS[$i];
				//$content = mb_convert_encoding($content, "UTF-8","SJIS");	//不要 元々がUTF-8
				//星座ごとのURLを１つずつパース処理する
				// parse datas
				if (preg_match("/<div class=\"junni\">(\d{1,2})位<\/div>/", $content, $MATCHES)) {
					$RESULTS[$i] = $MATCHES[1];
					break;
				}
			}

		}
//print_r($RESULTS);

		return $RESULTS;
	}
}
