<?php

class Paginator {

	// ページに何件表示する
	static $defaultRecordsPerPage = 10; // use a better default

	private static $paging = array(); 		// records per pages per interface
	private static $recordsCount = array(); // records available per interface (total of all records)


	/**
	 * ページングのため、SQLのLIMITの計算するメソッド
	 *
	 * @author Azet
	 * @param string $name_
	 * @param int $page_ = 1
	 * @return arrat(start, size)
	 */
	static function getLimitFor($name_, $page_ = 1) {

		// size settings
		if(isset(self::$paging[$name_])) {
			$size = self::$paging[$name_];
		}
		else {
			$size = self::$defaultRecordsPerPage;
		}

		// start offset
		$start = $size * ($page_ - 1);

		return array($start, $size);
	}


	/**
	 * 各ページの設定（件数）
	 *
	 * @author Azet
	 * @param string $name_
	 * @param int $records_per_page_
	 */
	static function setPagingFor($name_, $records_per_page_) {
		self::$paging[$name_] = $records_per_page_;
	}


	/**
	 * 各ページの設定を出す
	 * 
	 * @param  int $name_ ぺージ名
	 * @return int ページングのレコード数
	 */
	static function getPagingFor($name_) {
		return self::$paging[$name_];
	}


	/**
	 * データのクエリーのすぐあとに実行してください！
	 *
	 * @author Azet
	 * @param string $name_
	 * @return int (total of records available for the previous query)
	 */
	static function  setRecordsCountFor($name_) {
		global $conn; // MySQLi connexion

		$sql = "SELECT FOUND_ROWS()";
		$rs = $conn->query($sql);
		$count = $rs->fetch_row();

		// save in memory for later use
		self::$recordsCount[$name_] = $count[0];

		return $count[0];
	}


	/**
	 * counts pages needed for given interface
	 *
	 * @author Azet
	 * @param string $name_
	 * @return int
	 */
	static function getPagesCountFor($name_) {
		$pages = 0;
		if(isset(self::$recordsCount[$name_]) && isset(self::$paging[$name_])) {
			$pages = ceil(self::$recordsCount[$name_] / self::$paging[$name_]);
		}

		return $pages;
	}


	/**
	 * set currently selected page for given interface
	 *
	 * @author Azet
	 * @param string $name_
	 * @param int $page_
	 */
	static function setCurrentPageFor($name_, $page_) {
		if(!isset($_SESSION['paginator'])) {
			$_SESSION['paginator'] = array();
		}

		$_SESSION['paginator'][$name_] = $page_;
	}


	/**
	 * current page for given interface
	 *
	 * @author Azet
	 * @param string $name_
	 * @return int
	 */
	static function getCurrentPageFor($name_) {
		$page = 1;

		if(isset($_SESSION['paginator'][$name_])) {
			$page = $_SESSION['paginator'][$name_];
		}

		return $page;
	}
}


/**
 * 画面のページングを出す
 * 使い方の例：{insert paginator page_name="admin-comments" format="xxx[PAGE]"}
 * formatを使うときに、[PAGE]の文字列はページングの番号のリンクになります。

 * @author Azet
 * @param array $params_ (page_name => string)
 * @return string HTML
 */
function smarty_insert_paginator($params_) {
	$page_name = $params_['page_name'];
	$page_count = Paginator::getPagesCountFor($page_name);

	if($page_count<=1) {
		// ページングが必要ない
		return "";
	}

	// link format

	$navigation = "
<div class='navigation-paginator navigation-$page_name-container'>
<nav aria-label='Page navigation navigation'>
  <ul class='pagination'>
";

	for($page=1; $page<=$page_count; ++$page) {
		$class = "";
		if($page == Paginator::getCurrentPageFor($page_name)) {
			$class = "class='active'";
		}

		if(isset($params_['format'])) {
			$link = str_replace('[PAGE]', $page, $params_['format']);
		}
		else {
			$link = "#page/".$page;
		}

		$navigation .= "<li $class><a href='$link'>$page</a></li>";
	}

	$navigation .= "
  </ul>
</nav>
</div>
";

	return $navigation;
}
