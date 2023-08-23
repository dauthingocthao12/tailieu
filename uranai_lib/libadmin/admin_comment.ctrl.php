<?php

//            _           _                         _             _ _
//   __ _  __| |_ __ ___ (_)_ __     ___ ___  _ __ | |_ _ __ ___ | | | ___ _ __
//  / _` |/ _` | '_ ` _ \| | '_ \   / __/ _ \| '_ \| __| '__/ _ \| | |/ _ \ '__|
// | (_| | (_| | | | | | | | | | | | (_| (_) | | | | |_| | | (_) | | |  __/ |
//  \__,_|\__,_|_| |_| |_|_|_| |_|  \___\___/|_| |_|\__|_|  \___/|_|_|\___|_|


/**
 * サイトコメント管理一覧
 * 
 * @author Azet
 * @input _POST[filter]
 * @return array(comments data)
 */
function comments_listing() {
	global $smarty;

	// default listing filter (all)
	$status = SiteComment::$STATUS_ALL;

	// list of sites with comments
	$smarty->assign('sites', SiteComment::listSites()); // array(id => name)
	// list of users who commented
	$smarty->assign('users', SiteComment::listUsers()); // array(id => name)

	// basics
	$status = SiteComment::$STATUS_ALL;

	// specific filter (status)
	if(isset($_POST['filter_status'])) {
		// 絞り込む
		$status = $_POST['filter_status'];

		// memo in session
		$_SESSION['admin-comment-filter-status'] = $status;
	}
	else if(isset($_SESSION['admin-comment-filter-status'])) {
		// セッションから引き出す
		$status = $_SESSION['admin-comment-filter-status'];
	}

	// filter (user)
	if(isset($_POST['filter_user'])) {
		$user = $_POST['filter_user'];
		$_SESSION['admin-comment-filter-user'] = $user;
	}
	else if(isset($_SESSION['admin-comment-filter-user'])) {
		$user = $_SESSION['admin-comment-filter-user'];
	}
	else {
		$user = 0;
	}

	// filter (site)
	if(isset($_POST['filter_site'])) {
		$site = $_POST['filter_site'];
		$_SESSION['admin-comment-filter-site'] = $site;
	}
	else if(isset($_SESSION['admin-comment-filter-site'])) {
		$site = $_SESSION['admin-comment-filter-site'];
	}
	else {
		$site = 0;
	}

	// ページング設定
	if(isset($_POST['page'])) {
		Paginator::setCurrentPageFor('admin-comments', intval($_POST['page']));
	}

	// list of comments (filtered)
	$comments = SiteComment::getSitesComments($status, $user, $site);

	// list of reports
	$comments_id = array_reduce($comments, function($acc, $elem) {
		$acc[] = $elem['site_comment_id'];
		return $acc;
	}, array());
	// debug($comments_id);

	$reports = SiteCommentReport::getNewReports($comments_id);
	$smarty->assign('reports', $reports);

	return $comments;
}


/**
 * サイトコメント管理フォーム表示
 * 
 * @author Azet
 * @param int $id_ comment ID
 * @return aray (comment data)
 */
function comment_input($id_) {
	$comment = SiteComment::getSingleComment($id_);
	// debug($comment);

	// add revision data (previous comment) if available
	$comment['history'] = SiteComment::getAdminHistory($id_);

	return $comment;
}


/**
 * 管理者がコメントを公開する
 * 一覧画面から
 * 
 * @param  int $id_ comment ID
 * @return boolean (success or not)
 */
function comment_publish($id_) {
	// get comment data
	$comment = SiteComment::getSingleComment($id_, false);

	$data = array(
		// pre-set data like it would be from details screen
		'id' => $id_,
		'status' => SiteComment::$STATUS_PUBLISHED,
		'memo' => $comment['admin_memo'],
		'mail_sent' => 'yes',
		'mail_content' => '',
		'comment_content' => $comment['comment']
	);

	return SiteComment::adminSave($data);
}
