<?php

class History {


	function __construct() {
		if(!isset($_SESSION['history'])) {
			$_SESSION['history'] = array();
		}
	}


	function add($url_) {
		array_unshift($_SESSION['history'], $url_);
		if(count($_SESSION['history'])>5) {
			array_splice($_SESSION['history'], 5);
		}
	}


	function getList() {
		return $_SESSION['history'];
	}

	/**
	 * 現在のページを取得する
	 * @return string
	 */
	function getCurrent(){
		return $_SESSION['history'][0];
	}

	/**
	 * 前の見たページのURL
	 * 
	 * @return string           url argument previous to current page
	 */
	function getPrevious() {
		return $_SESSION['history'][1];
	}

	/**
	 * ログイン前のページ情報をセッションにセットする
	 */
	function setPageBeforeLogin() {
		//ログインフォームにいる && アカウントページからの遷移でない && セッションの「ログイン前のページ」情報がない この時だけセット
		if(($this->getCurrent() == "account/login" || $this->getCurrent() == "mypage") && !$this->fromAccountPage() && !isset($_SESSION['history_before_login'])){ //ログインページに来たら前のページを記憶する
			$_SESSION['history_before_login'] = $this->getPrevious();
		}
	}

	/**
	 * 1つ前がアカウントページにだったか
	 * @return boolean
	 */
	function fromAccountPage(){
		return preg_match("/^account.*/", $this->getPrevious());
	}

	/**
	 * アカウントページにいるか
	 * @return boolean
	 */
	function isOnAccountPage(){
		return preg_match("/^account.*/", $this->getCurrent());
	}

	/**
	 * マイページにいるか
	 * @return boolean
	 */
	function isOnMyPage(){
		return preg_match("/^mypage$/", $this->getCurrent());
	}

	/**
	 * ログアウトページにいるか
	 * @return boolean
	 */
	function isOnLogoutPage(){
		return preg_match("/^account\/logout.*$/", $this->getCurrent());
	}

	/**
	 * ログイン前にいたページを取得する( @ ---> なければNULL)
	 * @return string
	 */
	function getPageBeforeLogin() {
		return @$_SESSION['history_before_login'];
	}

	/**
	 * ログイン前にいたページ情報を消去する
	 */
	function clearPageBeforeLogin() {
		if(!$this->fromAccountPage() && !$this->isOnMyPage() && !$this->isOnLogoutPage()){
			unset($_SESSION['history_before_login']);
		}
	}

	/**
	 * app-link/love または app-link/work のURLから使うメソッドです。
	 * targetのサポート：love, work
	 * 
	 * @param  string $target_ love OR work
	 */
	function redirectAppTo($target_) {
		$targets_ok = array(
			'love',
			'work'
		);
		$targets_regx = join('|', $targets_ok);
		
		// debug($this->getPrevious());
		$prev_url = $this->getPrevious();
		
		if($target_=='default') {
			$target = '';
		}
		else {
			$target = $target_;
		}

		if($target_=='ranking-past') {
			// die($prev_url);
			if(preg_match("!^($targets_regx)/?.*!i", $prev_url, $matches)) {
				$redir_url = $matches[1].'/ranking-past';
			}
			else {
				$redir_url = 'ranking-past';
			}
		} 
		else if(self::isRankingPastUrl($prev_url)) {
			if($target_=='default') {
				$redir_url = 'ranking-past';
			}
			else {
				$redir_url = $target.'/ranking-past';
			}
		}
		else if(self::isRankingUrl($prev_url)) {
			$prev_url_strip = preg_replace("!^($targets_regx)/(.*)!i", "$2", $prev_url);
			// die($prev_url_strip);
			$redir_url = ltrim($target.'/'.$prev_url_strip, '/');
		}
		else {
			$redir_url = $target;
		}
		// die($redir_url);
		
		if(!$redir_url) {
			$new_link = '/';
		}
		else {
			$new_link = smarty_function_sitelink(array('mode' => $redir_url));
		}
		// die($new_link);

		header("location:".$new_link);
		exit;
	}


	/**
	 * URLがランキングようの判断するメソッド
	 * パターンのサンプル：
	 *  - leo
	 *  - 20180301
	 *  - 20180301/leo
	 * 
	 * @param  string  $url_ 判断したいURL
	 * @return boolean		trueの場合はURLがランキングようです
	 */
	static function isRankingUrl($url_) {
		global $en_star;

		$star_names_en = join('|', $en_star);
		return preg_match("!(\d{8}|$star_names_en)!", $url_);
	}


	/**
	 * URLが過去のランキングか判断するメソッド
	 *
	 * @param striing $url_ 例：love/ranking-past
	 * @return bool
	 */
	static function isRankingPastUrl($url_) {
		return preg_match("!(ranking-past|ranking\d{4})!", $url_);
	}
}
