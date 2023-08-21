<?php

class News {

	static private $newsUserFilter = " (is_delete=0 AND news_is_show = 1)";

	static private $newsAdminFilter = " (is_delete=0)";


	/**
	 * 一つにお知らせを呼び出す
	 *
	 * @author Azet
	 * @param int $id_
	 * @return array (DBからの一行)
	 */
	static function getById($id_) {
		global $conn;
		$news = array();

		$rs = $conn->query("SELECT * FROM news WHERE news_id=$id_ LIMIT 1");
		if($rs) {
			$news = $rs->fetch_assoc();
		}

		return $news;
	}


	/**
	 * お知らせの保存
	 *
	 * @author Azet
	 * @param array $data_ (post form)
	 * @return bool
	 */
	static function save($data_) {
		global $conn;

		$data = $data_;

		if(!$data['news_release_date']) $data['news_release_date'] = 'NOW()';
		else $data['news_release_date'] = "'".$conn->real_escape_string($data['news_release_date'])."'";

		$today_datetime = date('Y-m-d H:i:s');
		if(!$data['promote_from_date']) $data['promote_from_date'] = "'$today_datetime'";
		else $data['promote_from_date'] = "'".$conn->real_escape_string($data['promote_from_date'])."'";

		if(!$data['promote_until_date']) $data['promote_until_date'] = "DATE({$data['promote_from_date']}) + INTERVAL 7 DAY";
		else $data['promote_until_date'] = "'".$conn->real_escape_string($data['promote_until_date'])."'";

		if($data['news_id']) {
			$query = "UPDATE news SET 
				news_title = '".$conn->real_escape_string($data['news_title'])."'
				,news_content = '".$conn->real_escape_string($data['news_content'])."'
				,news_release_date = ".$data['news_release_date']."
				,promote_from_date = ".$data['promote_from_date']."
				,promote_until_date = ".$data['promote_until_date']."
				,news_is_show = '".$conn->real_escape_string($data['news_is_show'])."'
				,date_update = NOW()
				WHERE news_id = '".$conn->real_escape_string($data['news_id'])."'
				";
		}
		else {
			$query = "INSERT INTO news SET 
				news_title = '".$conn->real_escape_string($data['news_title'])."'
				,news_content = '".$conn->real_escape_string($data['news_content'])."'
				,news_release_date = ".$data['news_release_date']."
				,promote_from_date = ".$data['promote_from_date']."
				,promote_until_date = ".$data['promote_until_date']."
				,news_is_show = '".$conn->real_escape_string($data['news_is_show'])."'
				";
		}

		//print $query;

		return $conn->query($query);
	}


	/**
	 * お知らせを削除
	 *
	 * @author Azet
	 * @param int $id_
	 * @return array(status, message)
	 */
	static function delete($id_) {
		global $conn;
		$result = array();

		$query = "UPDATE news SET is_delete=1, date_delete=NOW() WHERE news_id=$id_";
		$ok = $conn->query($query);

		if($ok) {
			$result['status'] = 'OK';
			$result['message'] = "新着の削除はサクセスです。";
		}
		else {
			$result['status'] = 'ERR';
			$result['message'] = "新着の削除はエラーです。";
		}

		return $result;
	}


	/**
	 * 最新のお知らせを出す
	 *
	 * @author Azet
	 * @return array (newsのDBのレコード)
	 */
	static function getLast() {
		global $conn;
		$list = array();

		// list news needed
		$query = "SELECT 
				n.*
			FROM news n
			WHERE ".self::$newsUserFilter."
			AND n.promote_from_date <= NOW() AND n.promote_until_date >= NOW()
			ORDER BY n.news_release_date DESC
			";
		// TEST
		//print $query;

		$rs = $conn->query($query);

		if($rs) {
			while($row = $rs->fetch_assoc()) {
				$list[] = $row;
			}
		}

		return $list;
	}


	/**
	 * ページのnewsを準備する
	 *
	 * @author Azet
	 * @param int $currentPage_
	 * @param int $newsPer_Page_
	 * @return array (news from DB)
	 */
	static function getList($currentPage_, $newsPerPage_) {
		global $conn;

		// return list
		$list = array();

		// offset
		$start = ($currentPage_ - 1) * $newsPerPage_;
		$limit = " LIMIT $start, $newsPerPage_";

		// list news needed
		$query = "SELECT 
				n.*,
				date(n.news_release_date) as url_date
			FROM news n
			WHERE ".self::$newsUserFilter."
			AND n.promote_from_date <= now()
			ORDER BY n.news_release_date DESC
			$limit
			";

		$rs = $conn->query($query);
		while($row = $rs->fetch_assoc()) {
			$list[] = $row;
		}

		return $list;
	}


	/**
	 * 管理画面用のお知らせの一覧
	 *
	 * @author Azet
	 * @return array (news from DB)
	 */
	static function getListAdmin() {
		global $conn;

		// return list
		$list = array();

		// list news needed
		$query = "SELECT 
				n.*
				,date(n.news_release_date) as url_date
				,date(n.promote_from_date) > date(now()) as not_visible_yet
			FROM news n
			WHERE ".self::$newsAdminFilter."
			ORDER BY n.news_release_date DESC
			";

		$rs = $conn->query($query);
		while($row = $rs->fetch_assoc()) {
			$list[] = $row;
		}

		return $list;
	}


	/**
	 * お知らせのコウンと
	 *
	 * @author Azet
	 * @return int
	 */
	static function countNews() {
		global $conn;

		$news_count = 0;

		$rs = $conn->query("SELECT count(news_id) FROM news
			WHERE ".self::$newsUserFilter);
		if($rs && $row = $rs->fetch_row()) {
			$news_count = $row[0];
		}

		return $news_count;
	}


	/**
	 * お知らせのページ数を計算する
	 *
	 * @author Azet
	 * @param int $newsPerPage_
	 * @return int
	 */
	static function countPages($newsPerPage_) {
		$news_count = self::countNews();

		return ceil( $news_count / ($newsPerPage_) );
	}


	/**
	 * お知らせ詳細データ
	 *
	 * @author Azet
	 * @param string $newsDate_ (例: 2017-03-13 OR 20170313)
	 * @return array (DBの一行)
	 */
	static function getNewsDetailsByDate($newsDate_,$news_id_) {
		global $smarty, $conn;
		$data = array();

		$query = "SELECT * FROM news
			WHERE 1
			AND date(news_release_date) = $newsDate_
			AND news_id=$news_id_
			";
		// simon 2017-03-15 
		// 管理画面から確認できるために、下記の条件を外します
		// AND ". self::$newsUserFilter
//print $query;
		$rs = $conn->query($query);

		if(!$rs) {
			$data['news_title'] = 'エラー';
			$data['news_content'] = "<div class='alert alert-warning'>新着情報が見つかりません。</div>";
		}
		else {
			$data = $rs->fetch_assoc();
			$data['news_content'] = $smarty->fetch("string:".$data['news_content']);
		}

		return $data;
	}
}
