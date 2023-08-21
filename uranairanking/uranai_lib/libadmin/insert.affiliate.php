<?php

/**
 * 広告管理画面の[script]タグを実際の<script>タグに戻す。
 *
 * @param string $str 登録した文字列
 * @return string $result 括弧を置換したスクリプトタグ
 */

function ReplaceJsTag($str)
{
	$reg_a = "/\[(?=\/?script)/";
	$reg_b = "/(?<=script)\]/";
	$reg_c = "/\](?=\<\/script)/";
	$result = preg_replace($reg_a, "<", $str);
	$result = preg_replace($reg_b, ">", $result);
	$result = preg_replace($reg_c, ">", $result);
	return $result;
}

/**
 * 管理画面に登録されている広告タグを取得する。
 *
 * @param array $param_ テンプレートからのパラメータ
 * @param int $param['id'] (管理画面に登録してある広告id)
 * @return string $tag (HTMLタグ)
 */

function smarty_insert_ad($param_)
{

	/**
	 * @var array $AFFILIATE_OFF (アフィリエイトを出さないIPリスト)
	 * @see config.php $AFFILIATE_OFF
	 * @see ad.php class Ad
	 */

	global $AFFILIATE_OFF;
	$data_ = Ad::getById($param_['id']);

	//広告が複数件だったらランダムでだす
	if (count($data_) > 0) {
		$rand_k = array_rand($data_);
		$data = $data_[$rand_k];
	}

	$ad = array();
	foreach ($data as $key => $val) {
		if ($key == "ad_id") {
			continue;
		}
		$ad[$key] = ReplaceJsTag($val);
	}
	$ad_id = $data['ad_id'];
	//UA::detect();

	/**
	 * アプリ
	 */
	if (UA::isApp()) {
		if (UA::isAndroid() && !UA::isMobile() || UA::isiPad()) {
			$tag = $ad['ad_tag'];
		} elseif (UA::isAndroid()) {
			$tag = $ad['ad_tag_Android'];
		} else {
			$tag = $ad['ad_tag_iOS'];
		}
		/**
		 * ブラウザ
		 */
	} else {
		if (UA::isPC()) {
			$tag = $ad['ad_tag'];
		} else {
			$tag = $ad['ad_tag_mobile'];
		}
	}

	/**
	 * 社内
	 * 
	 * エスケープした広告タグと本番広告をプレビューするフォームボタンを表示
	 * ページ内のすべての広告がプレビューされます。 kimura 2017/06/20
	 */
	if (!IS_SERVER || in_array($_SERVER['REMOTE_ADDR'], $AFFILIATE_OFF['IP']) || $_SERVER['REMOTE_ADDR'] == "127.0.0.1") {


		if (isset($_POST['ad-demo']) && $_POST['ad-demo'] == 'true') {
			$tag .= "<form method='post'>";
			$tag .= "<input type='hidden' name='ad-demo' value='false'>";
			$tag .= "<input type='submit' value='デバッグ用表示' class='btn btn-primary btn-xs demo-ad'>";
			$tag .= "<span class='em-ad-id'>ID:" . $ad_id . " (" . $param_['id'] . ") </span>";
			$tag .= "</form></pre>";
			return $tag;
		} else {
			$tag = "<form method='post'>";
			$tag .= "<pre><span class='em-ad-id'>広告ID:" . $ad_id . " (" . $param_['id'] . ") </span>" . htmlspecialchars($tag);
			$tag .= "<input type='hidden' name='ad-demo' value='true'>";
			$tag .= "<input type='submit' value='広告をプレビュー' class='btn btn-primary btn-xs demo-ad'>";
			$tag .= "</pre></form>";
		}
	}
	return $tag;
}

/**
 * 管理画面に登録されている広告タグを取得する。
 *
 * @param array $param_ テンプレートからのパラメータ
 * @param int $param['id'] (管理画面に登録してある広告id)
 * @return string $tag (HTMLタグ)
 */

function smarty_insert_ad_group($param_)
{

	/**
	 * @var array $AFFILIATE_OFF (アフィリエイトを出さないIPリスト)
	 * @see config.php $AFFILIATE_OFF
	 * @see ad.php class Ad
	 */

	global $AFFILIATE_OFF;

	//広告非表示モード追加　2020/06/03
	if ($_GET['noads'] === "1") {
		return null;
	}

	$data_ = Ad::getGroupById($param_['id']);
	if (empty($data_)) {
		if (is_local()) {
			return "<span>広告データがありません。(id:{$param_['id']})<span>";
		} else {
			return null;
		}
	}

	//ランダムでだす
	if (count($data_) > 0) {
		$rand_k = array_rand($data_);
		$ad_key = $rand_k;
		$data = $data_[$rand_k];
	}
	//指定があれば固定で出す
	if (isset($_POST['ad_key'])) {
		foreach ($data_ as $k => $v) {
			if ($v['ad_id'] == $_POST['ad_key']) {
				$data = $data_[$k];
			}
		}
		unset($k, $v);
	}

	$ad = array();
	foreach ($data as $key => $val) {
		if ($key == "ad_id") {
			continue;
		}
		$ad[$key] = ReplaceJsTag($val);
	}
	$ad_id = $data['ad_id'];

	/**
	 * アプリ
	 */
	if (UA::isApp()) {
		if (UA::isAndroid() && !UA::isMobile() || UA::isiPad()) {
			$tag = $ad['ad_tag'];
		} elseif (UA::isAndroid()) {
			$tag = $ad['ad_tag_Android'];
		} else {
			$tag = $ad['ad_tag_iOS'];
		}
		/**
		 * ブラウザ
		 */
	} else {
		if (UA::isPC()) {
			$tag = $ad['ad_tag'];
		} else {
			$tag = $ad['ad_tag_mobile'];
		}
	}

	//社内 かつ 広告off
	if (is_local()) {

		if ($_POST['ad-demo'] == 0) {

			$ad_listing = implode(",", array_map(function ($v) {
				return $v['ad_id'];
			}, $data_));

			$tag  = "";
			$tag .= "<div class='adp-container'>";
			// 広告がテキスト広告(id = 7,8)なら
			if ($param_['id'] == '7' || $param_['id'] == '8') {
				$tag .= "<div class='adp-container2 text-abs'>";
				$tag .= "<span class='adp-info'>ad_group_id:{$param_['id']}({$ad_listing})</span>";
				$tag .= "<span>ここにテキスト広告が入ります。</span>";
			}
			else {
				$tag .= "<div class='adp-container2'>";
				$tag .= "<span class='adp-info'>ad_group_id:{$param_['id']}({$ad_listing})</span>";
				$tag .= "<img src='/user/img_re/ad_preview.png'>";
			}
			$tag .= "</div>";
			$tag .= "</div>";
		} else {
			$ad_listing = implode(",", array_map(function ($v) use ($ad_id) {
				return "<a class='adp-link' onclick=\"pick_ad(" . $v['ad_id'] . ")\" " . ($ad_id == $v['ad_id'] ? "style='font-weight:bold;'" : "") . ">" . $v['ad_id'] . "</a>";
			}, $data_));

			$tag .= "<div class='adp-box col-md-12 d-flex' style='justify-content:center'>";
			$tag .= "<pre class='adp-desc'>";
			$tag .= "<span>ad_group_id:{$param_['id']}({$ad_listing})</span>";
			$tag .= "</pre>";
			$tag .= "</div>";
		}
	}

	return $tag;
}

/**
 * ユーザーエージェントをクラス化したもの
 *
 * @method getUA() サーバー変数のユーザーエージェントを返す
 * @method detect() デバッグライト(HTML表記)
 * @method isPC() PCであるか return bool
 * @method isMobile() モバイル端末であるか return bool
 * @method isAndroid() Android端末であるか return bool
 * @method isiPad() iPad端末であるか return bool
 * @method isApp() アゼットアプリであるか return bool
 *
 * @todo 共通で使えるところがあればconfig.phpへ移す //kimura
 */

class UA
{

	/**
	 * @return string $_SERVER['HTTP_USER_AGENT']
	 */
	public static function getUA()
	{
		return $_SERVER['HTTP_USER_AGENT'];
	}

	public static function detect()
	{
		echo "<span class='uadisplay'>";
		echo "アプリ:" . (UA::isApp() ? "true" : "false") . "&nbsp;";
		echo "PC:" . (UA::isPC() ? "true" : "false") . "&nbsp;";
		echo "モバイル:" . (UA::isMobile() ? "true" : "false") . "&nbsp;";
		echo "iPad:" . (UA::isiPad() ? "true" : "false") . "&nbsp;";
		echo "Android:" . (UA::isAndroid() ? "true" : "false") . "&nbsp;";
		echo "</span>";
	}

	public static function isPC()
	{
		$ua = self::getUA();
		if (!self::isMobile()) {
			return true;
		}
		return false;
	}

	public static function isMobile()
	{
		$ua = self::getUA();
		if ((strpos($ua, 'Android') > 0) && (strpos($ua, 'Mobile') > 0)
			|| (strpos($ua, 'iPhone') > 0)
			|| (strpos($ua, 'Windows Phone') > 0)
		) {
			return true;
		}
		return false;
	}

	public static function isAndroid()
	{
		$ua = self::getUA();
		if (strpos($ua, 'Android') > 0) {
			return true;
		}
		return false;
	}

	public static function isiPad()
	{
		$ua = self::getUA();
		if (strpos($ua, 'iPad') > 0) {
			return true;
		}
		return false;
	}

	public static function isApp()
	{
		$ua = self::getUA();
		if (preg_match("/Azet .* App/", $ua)) {
			return true;
		}
		return false;
	}
}
