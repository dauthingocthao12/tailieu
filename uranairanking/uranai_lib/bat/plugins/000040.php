<?php
/**
 * @author Azet
 * @date 2016-02-24
 * @url http://top.tsite.jp/news/lifetrend/o/26954539/
 *        → http://free-fortune.jp/rank_fortune
 */
class Zodiac000040 extends UranaiPlugin {

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
		$date_pattern = "/\<meta property=\"og:description\" content=\"{$now['month']}月{$now['day']}日/";

        // サイト毎に星座名の設定
		// サイト毎に星座名のプラグイン個別設定
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

		$content = $CONTENTS[0];
		//$content = mb_convert_encoding($content, "UTF-8", "EUC-JP");
		$LINES = explode("\n", $content);

		$rank_num = 1;	//順位をカウントしながら星座を検索
		$data_ptn = "/alt=\"(.*座)の今日の運勢\"/";

		//行ごとにパース
		foreach ($LINES AS $line) {
			if (count($RESULT) == 12) {
				break;
			}

			$flg = preg_match($data_ptn, $line, $MATCHES);	//各順位と星座名を取り出す
			if ($flg) {
				$star_name = $MATCHES[1];
				$star_num = $star[$star_name];
				$RESULT[$star_num] = $rank_num;
				$rank_num++;
			}

		}

		return $RESULT;
	}
}
