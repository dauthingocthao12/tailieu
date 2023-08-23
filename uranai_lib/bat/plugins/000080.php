<?php
/*****************************************************************************/
/*   占いサイトプラグイン                                                    */
/*  FM FUJI Yes!Morning :http://fmfuji.jp/horoscope/index.php                */
/*                                            作成者:山口  作成日：2017/1/25 */
/*****************************************************************************/

/**
data[star] = rank;
星座（インデックス） => 順位
*/

class Zodiac000080 extends UranaiPlugin {

	function run($URL) {
		/*サイトのhtml取得*/
		$CONTENTS = $this->load($URL);

		/*このプラグインは、０番のURLしか使用しない*/
		$content = $CONTENTS[0];

		/*結果代入用配列
			$RESULT[星座番号]=順位*/
		$RESULT = array();

		//$content = mb_convert_encoding($content, "UTF-8", "SJIS-win");
		/*取得したhtmlを改行で分割し配列$LINESに代入*/
		$LINES = explode("\n", $content);
		
		/*星座名を個別設定*/
		/*$star = self::$starDefault;*/
		$star = array(
			'水瓶座' => 1,
			'魚座' => 2,
			'牡羊座' => 3,
			'牡牛座' => 4,
			'双子座' => 5,
			'蟹座' => 6,
			'獅子座' => 7,
			'乙女座' => 8,
			'天秤座' => 9,
			'蠍座' => 10,
			'射手座' => 11,
			'山羊座' => 12
		);

		/*日付の表示が今日の日付と比較*/
		/*今日の日付を取得*/
		$now = self::getToday();
		/*サイトと同形式の日付データの作成*/

		$date_pattern = "/update\">{$now['year']}\.0?{$now['month']}\.0?{$now['day']}<\/p>/";

		$date_check_ok = false;  //日付データのチェック状況変数
		$rank_num=0;	//順位代入用変数
		$flag=0;
		//判定
		foreach ($LINES AS $line) {
			/*12星座分のデータがリザルトにある時処理を抜ける*/
			if (count($RESULT) == 12) { break; }

			/*日付の判定*/
			
			if (!$date_check_ok) {
				$date_check_ok = preg_match($date_pattern, $line);
				continue;
			}
			
			/*データの取得フラグ*/
			if($date_check_ok && !$flag && preg_match("/<div class=\"row\" data-equalize-on=\"medium\">/", $line, $MATCHES)){
				$flag=1;

			}
			if($date_check_ok && $flag && preg_match("/<p>(\d{1,2})位<\/p>/", $line, $MATCHES)){
				$rank_num=$MATCHES[1];
			}
			if($date_check_ok && $flag && $rank_num && preg_match("/<dt>(.*?座)<\/dt>/", $line, $MATCHES)){
				$star_name=$MATCHES[1];

				$star_num = $star[$star_name];

				/*星座ごとに結果を$RESULTに格納*/
				$RESULT[$star_num] = $rank_num;

				/*値のリセット*/
				$rank_num = 0;
				$flag=0;

			}

		}

		// エラーチェック
		if(!$date_check_ok) {
			print $this->logDateError().PHP_EOL;
		}
		return $RESULT;
	}
}


