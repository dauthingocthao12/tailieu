<?php
/**
 * @author Azet
 * @date 2016-01-13
 * @url http://sp.asahi.jp/program/ohaasa/uranai/
 */
class Zodiac000029 extends UranaiPlugin {

	/**
	 * 星座変換表
	 *
	 * @var array key:取得元のhoroscope_st, value:本サイトのstar
	 * @see https://www.asahi.co.jp/data/ohaasa2020/horoscope.json
	 */
	public static $starMap = [
		'11' => 1,
		'12' => 2,
		'01' => 3,
		'02' => 4,
		'03' => 5,
		'04' => 6,
		'05' => 7,
		'06' => 8,
		'07' => 9,
		'08' => 10,
		'09' => 11,
		'10' => 12
	];

	function run($CONTENTS) {
		$horoscope_data = json_decode($CONTENTS[0]);

		foreach ($horoscope_data as $child_data) {
			$now = self::getToday();
			$ymd = sprintf('%04d%02d%02d', $now['year'], $now['month'], $now['day']);
			if ($child_data->onair_date !== $ymd) {
				echo $this->logDateError().PHP_EOL;
				return [];
			}
	
			$RESULT = array_reduce($child_data->detail, function ($result, $entry) {
				$result[self::$starMap[$entry->horoscope_st]] = intval($entry->ranking_no);
				return $result;
			}, []);
	
			return $RESULT;
		}

		return null;
	}
}
