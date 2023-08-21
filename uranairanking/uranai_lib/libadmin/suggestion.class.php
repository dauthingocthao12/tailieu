<?php

class UnseiSuggestion {

	const MSG_FILE = DATA_FOLDER . "suggestion.json";

	public static function suggest_html($unsei_type)
	{
	  global $topic_Jp;

	  $msg = "";
	  $msgs = self::loadMsg();

	  unset($msgs[$unsei_type]);

	  $unsei_types = array_keys($msgs);
	  $rand_unsei_key = array_rand($unsei_types);
	  $dtype = $unsei_types[$rand_unsei_key];

	  if (isset($msgs[$dtype]) && (count($msgs[$dtype]) > 0)) {
		$rand_key = array_rand($msgs[$dtype]);
		$msg = $msgs[$dtype][$rand_key];
	  }
	  return "<a id=\"unsei-suggest-link\" title=\"".$topic_Jp[$dtype]."\" class=\"unsei-suggest-{$dtype}\" href=\"/{$dtype}/\">{$msg}</a>";
	}
	/**
	 * メッセージファイルをパースして配列で返す
	 * @return boolean|array
	 */
	private static function loadMsg()
	{
		$f = self::MSG_FILE;
		//設定ファイルが無ければエラー
		if (!file_exists($f)) {
			return false;
		}
		$msgs = json_decode(file_get_contents($f), true);
		//jsonパース失敗ならエラー
		if (json_last_error() !== JSON_ERROR_NONE) {
			return false;
		}
		return $msgs;
	}
}

