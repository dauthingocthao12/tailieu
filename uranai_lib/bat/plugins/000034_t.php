<?php
/**
 * @author Azet
 * @date 2016-02-12
 * @url http://dd.hokkaido-np.co.jp/horoscope/
 */
class Zodiac000034 extends UranaiPlugin {

	/**
	 * このファンクションは必要です！
	 * 星座（インデックス） => 順位
	 * @return array[star] = rank;
	 */
	function run($CONTENTS) {

		// RESULTの形：12星座分, 1~12
		// $RESULT[<星座番号>] = <ランキング>
		// ランキングも1~12値です

		//結果を格納する配列
		$RESULTS = array();

		// サイト毎に星座名の設定
		// サイト毎に星座名のプラグイン個別設定
		//$star["水瓶座"] = 1;
		//$star["魚座"] = 2;
		//$star["牡羊座"] = 3;
		//$star["牡牛座"] = 4;
		//$star["双子座"] = 5;
		//$star["蟹座"] = 6;
		//$star["獅子座"] = 7;
		//$star["乙女座"] = 8;
		//$star["天秤座"] = 9;
		//$star["蠍座"] = 10;
		//$star["射手座"] = 11;
		//$star["山羊座"] = 12;
		$star = self::$starDefault;

		// 日付の表示があれば、今日の日付と一致するか確認する！
		$now = self::getToday();
		// nowのキーは: year,month,day
		// monthは1~12の値
		// dayは1~31の値
		$monthx = sprintf('%02d', $now['month']);
		$oneday = sprintf('%02d', $now['day']);
		$date_pattern = "/<span class=\"dates\">{$now['year']}年{$monthx}月{$oneday}日<\/span>/";


		// このプラグインでは、１ファイルで12星座の情報を抽出する
		$content = $CONTENTS[0];
		//$content = mb_convert_encoding($content, "UTF-8","SJIS");	//不要 元々がUTF-8

		$LINES = explode("\n", $content);

		//１行ずつパース処理する
		$rank_num = 0;		//ランク値の格納先
		$date_check_ok = false;		//日付のパースチェック結果

		foreach ($LINES AS $line) {
			if (count($RESULT) == 12) { break; }

			// date check
			if(!$date_check_ok) {
				$date_check_ok = preg_match($date_pattern, $line);	//日付チェック結果格納
			}

			// parse datas
			if ($date_check_ok) {
				if (preg_match("/<img .* alt=\"(\d{1,2})位\">/", $line, $MATCHES)) {
					$rank_num = $MATCHES[1];		//(\d{1,2})位
					$flag=1;
				}
				if ($flag == 1 && preg_match("/<h3><img .*alt=\"(.*?座)\"><\/h3>/", $line, $MATCHES)) {
					$star_name_jp = $MATCHES[1];	//(.*?座)
					$star_num = $star[$star_name_jp];
					$flag = 0;

					// RESULTの形：
					// $RESULT[<星座番号>] = <ランキング>
					$RESULTS[$star_num] = $rank_num;
				}
			}

		}

		return $RESULTS;
	}

	function topic_run($TOPIC_CONTENTS) {

		// RESULTの形：12星座分, 1~12
		// $RESULT[<星座番号>] = <ランキング>
		// ランキングも1~12値です
		//結果を格納する配列
		$TOPIC_RESULT = array();

		// サイト毎に星座名の設定
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
		$monthx = sprintf('%02d', $now['month']);
		$oneday = sprintf('%02d', $now['day']);
		$date_pattern = "/<span class=\"dates\">{$now['year']}年{$monthx}月{$oneday}日<\/span>/";

		// このプラグインでは、１ファイルで12星座の情報を抽出する
		$topic_content = $TOPIC_CONTENTS[0];
		//$content = mb_convert_encoding($content, "UTF-8","SJIS");	//不要 元々がUTF-8

		$TOPIC_LINES = explode("\n", $topic_content);

		//１行ずつパース処理する
		$date_check_ok = false;		//日付のパースチェック結果

		foreach ($TOPIC_LINES as $topic_line) {
			if (count($TOPIC_RESULT) == 12) { break; }

			// date check
			if(!$date_check_ok) {
					$date_check_ok = preg_match($date_pattern, $topic_line);
			}

			// star
			if ($date_check_ok && preg_match("/<img .*? alt=\"(.*?座)\">/", $topic_line, $MATCHES)) {
				$star_name = $MATCHES[1];
				$star_num = $star[$star_name];
				$flag = 1;
			}

			//love
			if ($date_check_ok && $flag && preg_match("/<span class=\"love\">恋愛運<\/span><span class=\"star\"><img .*? src=\".*?\/horoscope_star_0(\d{1})\.gif\"/", $topic_line, $MATCHES)) {
				$love = $MATCHES[1];
				$love_num = ($love * 20);
			}
			//money
			if ($date_check_ok && $flag && preg_match("/<span class=\"money\">金運<\/span><span class=\"star\"><img .*? src=\".*?\/horoscope_star_0(\d{1})\.gif\"/", $topic_line, $MATCHES)) {
				$money = $MATCHES[1];
				$money_num = ($money * 20);
			}
			//work
			if ($date_check_ok && $flag && preg_match("/<span class=\"work\">仕事運<\/span><span class=\"star\"><img .*? src=\".*?\/horoscope_star_0(\d{1})\.gif\"/", $topic_line, $MATCHES)) {
				$work = $MATCHES[1];
				$work_num = ($work * 20);
				$flag = 0;
				$TOPIC_RESULT[$star_num] = array("love"=> $love_num , "money" => $money_num ,"work" => $work_num);
			}


		}
		// date error?
		if(!$date_check_ok) {
			print $this->logDateError().PHP_EOL;
		}

		return $TOPIC_RESULT;
	}
}
