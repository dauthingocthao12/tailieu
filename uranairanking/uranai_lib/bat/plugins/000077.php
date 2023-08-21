<?php
/*****************************************************************************/
/*   占いサイトプラグイン                                                    */
/*  夢占いの館 :https://yumeuranai-yakata.com/seiza/                         */
/*                                            作成者:山口  作成日：2017/1/20 */
/*****************************************************************************/

/**
data[star] = rank;
星座（インデックス） => 順位
*/

class Zodiac000077 extends UranaiPlugin {

	function run($URL) {
		/*サイトのhtml取得*/
		$CONTENTS = $this->load($URL);

		/*このプラグインは、０番のURLしか使用しない*/
		$content = $CONTENTS[0];

		/*結果代入用配列
			$RESULT[星座番号]=順位*/
		$RESULT = array();

		//$content = mb_convert_encoding($content, "UTF-8", "UTF-8");
		/*取得したhtmlを改行で分割し配列$LINESに代入*/
		$LINES = explode("\n", $content);
		
		/*星座名はディフォルト設定を使用*/
		$star = self::$starDefault;
		
		/*日付の表示が今日の日付と比較*/
		/*今日の日付を取得*/
		$now = self::getToday();
		/*サイトと同形式の日付データの作成*/
		$date_pattern = "/<h1>12星座占い<br>【{$now['year']}年0?{$now['month']}月0?{$now['day']}日】<\/h1>/";

		$date_check_ok = false;  //日付データのチェック状況変数
		$rank_num=0;	//順位代入用変数
		//判定
		foreach ($LINES AS $line) {
			/*12星座分のデータがリザルトにある時処理を抜ける*/
			if (count($RESULT) == 12) { break; }

			/*日付の判定*/
			
			if (!$date_check_ok) {
				$date_check_ok = preg_match($date_pattern, $line);
				continue;
			}
			
			/*順位の取得*/
			if($date_check_ok && !$rank_num && preg_match("/<div class=\"rankno rank(\d{1,2})\">(\d{1,2})位<\/div>/", $line, $MATCHES)){
				$rank_num = $MATCHES[1];
				continue;
			}
			/*星座の取得*/
			if ($rank_num && preg_match("/<h2 class=\"name\">(.*?座)<\/h2>/", $line, $MATCHES)) {
				$star_name = $MATCHES[1];
				$star_num = $star[$star_name];

				/*星座ごとに結果を$RESULTに格納*/
				$RESULT[$star_num] = $rank_num;

				/*値のリセット*/
				$rank_num = 0;
			}


		}

		// エラーチェック
		if(!$date_check_ok) {
			print $this->logDateError().PHP_EOL;
		}

		return $RESULT;
	}
}


