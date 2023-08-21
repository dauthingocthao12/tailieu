<?php
/**
 * @author Azet
 * @date 2016-02-05
 * @url http://woman.infoseek.co.jp/fortune/horoscope/today/
 */
class Zodiac000048 extends UranaiPlugin {

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
		$CONTENTS = array();

		// サイトhtmlを取得
		$CONTENTS = $this->load($URL);
		$content = $CONTENTS[0];
		//$content = mb_convert_encoding($content, "UTF-8", "EUC-JP");
		$LINES = explode("\n", $content);

		// サイト毎に星座名のプラグイン個別設定
		$star = array();
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

		// 日付の表示があれば、今日の日付と一致するか確認する！
		$now = self::getToday();
		// nowのキーは: year,month,day
		// monthは1~12の値
		// dayは1~31の値
		$date_pattern = "/<span>{$now['year']}<\/span>年<span>{$now['month']}<\/span>月<span>{$now['day']}<\/span>日/";

		$rank1_passed = false;
		$date_check_ok = false;
		$data_ptn1a = "/>1位<\/div>/";
		$data_ptn1b = "/alt=\"(.*座)\"><\/a>/";
		$data_ptn2a = "/<dt><span>(\d{1,2})<\/span>位<\/dt>/";
		$data_ptn2b = "/class=\"name\">(.*座)<\/a>/";
		$data_ptn3a = "/>12位<\/div>/";
		$data_ptn3b = "/alt=\"(.*座)\"><\/a>/";
		$rank_num = 0;

		//行ごとにパース
		foreach ($LINES AS $line) {
			if (count($RESULT) == 12) { break; }

			// date check
			if(!$date_check_ok) {
				$date_check_ok = preg_match($date_pattern, $line);
				continue;
			}

			if($date_check_ok) {
				if (!$rank1_passed) {
					$flg = preg_match($data_ptn1a, $line);	//１位の表示行
					if ($flg) {
						$rank1_passed = true;
						continue;
					}

				} else {
					if (count($RESULT) == 0) {	//まだデータが登場していない状態
						$flg = preg_match($data_ptn1b, $line, $MATCHES);	//１位の星座名があれば取り出す
						if ($flg) {
							$star_name = $MATCHES[1];
							$star_num = $star[$star_name];
							$RESULT[$star_num] = 1;
						}
						continue;

					} else {	//１位のデータを処理済みの場合、２位以降の処理
						$flg = preg_match($data_ptn2a, $line, $MATCHES);	//２位以降の情報がある行なのか確認
						if ($flg) {
							$rank_num = $MATCHES[1];
							continue;
						}

						$flg = preg_match($data_ptn3a, $line, $MATCHES);	//12位の確認
						if ($flg) {
							$rank_num = 12;
							continue;
						}

						$flg = preg_match($data_ptn2b, $line, $MATCHES);	//２位以降の星座取り出し(12位除く)
						if ($flg && $rank_num > 0) {
							$star_name = $MATCHES[1];
							$star_num = $star[$star_name];
							$RESULT[$star_num] = $rank_num;
							$rank_num = 0;
						}

						$flg = preg_match($data_ptn3b, $line, $MATCHES);	//12位の星座取り出し
						if ($flg && $rank_num > 0) {
							$star_name = $MATCHES[1];
							$star_num = $star[$star_name];
							$RESULT[$star_num] = $rank_num;
							$rank_num = 0;
						}

					}
				}

			}

		}

		return $RESULT;
	}
}
