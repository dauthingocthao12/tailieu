<?php

//                                        _             _ _
//  _   _ ___  ___ _ __    ___ ___  _ __ | |_ _ __ ___ | | | ___ _ __
// | | | / __|/ _ \ '__|  / __/ _ \| '_ \| __| '__/ _ \| | |/ _ \ '__|
// | |_| \__ \  __/ |    | (_| (_) | | | | |_| | | (_) | | |  __/ |
//  \__,_|___/\___|_|     \___\___/|_| |_|\__|_|  \___/|_|_|\___|_|

// ==================================================================================


//                                           _                     _   _
//   ___ ___  _ __ ___  _ __ ___   ___ _ __ | |_   _ __   ___  ___| |_(_)_ __   __ _
//  / __/ _ \| '_ ` _ \| '_ ` _ \ / _ \ '_ \| __| | '_ \ / _ \/ __| __| | '_ \ / _` |
// | (_| (_) | | | | | | | | | | |  __/ | | | |_  | |_) | (_) \__ \ |_| | | | | (_| |
//  \___\___/|_| |_| |_|_| |_| |_|\___|_| |_|\__| | .__/ \___/|___/\__|_|_| |_|\__, |
//                                                |_|                          |___/


// check a existing/pending comment
$user_site_comment = null;
$comment_pending = false;
if($user) {
	$user_site_comment = SiteComment::getUserComment($user['user_id'], $site_id, SiteComment::$STATUS_ALL);
	$comment_pending = isset($user_site_comment) && $user_site_comment['status'] == SiteComment::$STATUS_PENDING;
}

// posting (form data?) >>>
$posting = $_POST['action'] == 'post-site-comment';

// リロード場合は、POST[current_status]情報を更新することが必要です。
if($posting
&& isset($user_site_comment)
&& $user_site_comment['status'] == SiteComment::$STATUS_PUBLISHED
&& $_POST['current_status'] == '') {
	$_POST['current_status'] = SiteComment::$STATUS_PUBLISHED;
	// debug("no double posting");
}

// フォームのアクション
if($user && $posting && !$comment_pending) {
	// save new comment (on top of existing one, ok ONLY ONCE!)
	// debug($_POST);
	$status = SiteComment::saveUserComment($site_id, $user['user_id'], $_POST);
	if($status!='error') {
		$site_description_url = smarty_function_sitelink(array("mode" => "site-description")).$site_id;
		header("location:$site_description_url");
		exit;
	}
	$smarty->assign('post_status', $status);
	// debug($status);
}
else if($posting) {
	if($comment_pending) {
		$smarty->assign('post_status', 'error_already_pending');
	}
	else {
		$smarty->assign('post_status', 'error_impossible');
	}
}
// <<<


// コメントデータ
// ==============
$site_evaluations = SiteComment::getSiteEvaluationData($site_id);
// debug($site_evaluations);
$smarty->assign('site_evaluations', $site_evaluations);


//                                           _     _     _     _
//   ___ ___  _ __ ___  _ __ ___   ___ _ __ | |_  | |__ (_) __| | ___
//  / __/ _ \| '_ ` _ \| '_ ` _ \ / _ \ '_ \| __| | '_ \| |/ _` |/ _ \
// | (_| (_) | | | | | | | | | | |  __/ | | | |_  | | | | | (_| |  __/
//  \___\___/|_| |_| |_|_| |_| |_|\___|_| |_|\__| |_| |_|_|\__,_|\___|

// コメントを未公開 >>>
if($Q[2] == 'hide' && isset($Q[3])) {
	$ok = SiteComment::hideUserComment($user['user_id'], $Q[3]);
	if($ok) {
		$smarty->assign('hide_comment_status', 'success');
	}
	else {
		$smarty->assign('hide_comment_status', 'success');
	}
}
// <<<


//  _ _ _          _           _   _
// | (_) | _____  | |__  _   _| |_| |_ ___  _ __
// | | | |/ / _ \ | '_ \| | | | __| __/ _ \| '_ \
// | | |   <  __/ | |_) | |_| | |_| || (_) | | | |
// |_|_|_|\_\___| |_.__/ \__,_|\__|\__\___/|_| |_|

// サイトコメントをいいね >>>
if(isset($_POST['likeComment'])) {
	// basic ajax/json answer
	$jsonAnswer = array(
		'status' => 'OK',
		'message' => '',
		'likes_count' => 0, // new comment likes count
		'likes' => false // is the user likes that comment?
	);

	$fav_data = SiteComment::likeComment($user['user_id'], $_POST['likeComment']);
	if($fav_data) {
		if(!IS_SERVER) {
			// debug data only for dev!
			$jsonAnswer['debug']   = $fav_data['debug'];
		}
		$jsonAnswer['likes_count'] = $fav_data['likes_count'];
		$jsonAnswer['likes']       = $fav_data['likes'];
	}
	else {
		// error somewhere?
		$jsonAnswer['status'] = 'ERR';
		$jsonAnswer['message'] = 'error setting like status on comment';
	}

	// send json answer
	header("content-type: application/json"); // important!
	die(json_encode($jsonAnswer));
}
// <<<

//                            _                                              _   
//  _ __ ___ _ __   ___  _ __| |_    ___ ___  _ __ ___  _ __ ___   ___ _ __ | |_ 
// | '__/ _ \ '_ \ / _ \| '__| __|  / __/ _ \| '_ ` _ \| '_ ` _ \ / _ \ '_ \| __|
// | | |  __/ |_) | (_) | |  | |_  | (_| (_) | | | | | | | | | | |  __/ | | | |_ 
// |_|  \___| .__/ \___/|_|   \__|  \___\___/|_| |_| |_|_| |_| |_|\___|_| |_|\__|
//          |_|                                                                  

// コメントを違反報告する >>>
if(isset($_POST['reportComment'])) {
	// basic ajax/json answer
	$jsonAnswer = array(
		'status' => 'ERR',
		'message' =>  COMMENT_REPORT_ERROR
	);

	// insert report data into DB
	$reportComment   = $conn->real_escape_string(htmlentities($_POST['violationComment'], ENT_COMPAT | ENT_HTML401, "UTF-8"));
	$reporterName    = $conn->real_escape_string(htmlentities($_POST['reporterName'], ENT_COMPAT | ENT_HTML401, "UTF-8"));
	$reporterCompany = $conn->real_escape_string(htmlentities($_POST['reporterCompany'], ENT_COMPAT | ENT_HTML401, "UTF-8"));
	$reporterEmail   = $conn->real_escape_string(htmlentities($_POST['reporterEmail'], ENT_COMPAT | ENT_HTML401, "UTF-8"));

	$sql = "INSERT INTO site_comment_report
	SET comment_id = {$_POST['reportComment']},
	violation_category = {$_POST['violationCategory']},
	violation_comment = '$reportComment',
	reporter_name = '$reporterName',
	reporter_company = '$reporterCompany',
	reporter_email = '$reporterEmail'
	";
	$ok = $conn->query($sql);

	if($ok) {
		$jsonAnswer['status']  = 'OK';
		$jsonAnswer['message'] = COMMENT_REPORT_THANKYOU;

		// send email to admin
		$comment = SiteComment::getSingleComment($_POST['reportComment']);
		// smarty data
		$smarty->assign('data', array(
			'comment'            => $comment,
			'violation_category' => $_POST['violationCategory'],
			'violation_comment'  => $reportComment,
			'reporter_name'      => $reporterName,
			'reporter_company'   => $reporterCompany,
			'reporter_email'     => $reporterEmail
		));
		// template parsing
		$body = $smarty->fetch("site-comment-report-notification-mail.tpl");

		// email
		$mail = new mail();
		$mail->set_encoding("utf-8");
		$ok = $mail->send(
			MAIL_SENDER_EMAIL,
			MAIL_SENDER_NAME,
			COMMENT_NOTIFICATION_ADMIN_EMAIL,
			COMMENT_REPORT_NOTIFICATION_EMAIL_SUBJECT,
			$body);
	}
	else {
		$jsonAnswer['sql'] = $sql;
	}

	// send json answer
	header("content-type: application/json"); // important!
	die(json_encode($jsonAnswer));
}
// <<<

//                                                                 _
//  _   _ ___  ___ _ __    ___ ___  _ __ ___  _ __ ___   ___ _ __ | |_
// | | | / __|/ _ \ '__|  / __/ _ \| '_ ` _ \| '_ ` _ \ / _ \ '_ \| __|
// | |_| \__ \  __/ |    | (_| (_) | | | | | | | | | | |  __/ | | | |_
//  \__,_|___/\___|_|     \___\___/|_| |_| |_|_| |_| |_|\___|_| |_|\__|

// ユーザのコメントを読み込む >>>
if($user) {
	$user_site_comment = SiteComment::getUserComment($user['user_id'], $site_id, SiteComment::$STATUS_ALL);
    // debug($user_site_comment);
	$smarty->assign('user_site_comment', $user_site_comment);
	
	// コメントのページ
	$user_site_comment_page = 0;
	if($user_site_comment) {
		$user_site_comment_page = SiteComment::findUserCommentPage($user_site_comment);
	}
	$smarty->assign('user_site_comment_page', $user_site_comment_page);

	// rejected comment?
	if($user_site_comment['status'] == SiteComment::$STATUS_REJECTED) {
		$comment_id = $user_site_comment['site_comment_id'];
		$rejected = SiteComment::loadRejectedMailContent(array($comment_id));
		// debug($rejected);
		$smarty->assign('rejected', $rejected);
	}
}

// <<<

//      _ _                                                  _
//  ___(_) |_ ___    ___ ___  _ __ ___  _ __ ___   ___ _ __ | |_ ___
// / __| | __/ _ \  / __/ _ \| '_ ` _ \| '_ ` _ \ / _ \ '_ \| __/ __|
// \__ \ | ||  __/ | (_| (_) | | | | | | | | | | |  __/ | | | |_\__ \
// |___/_|\__\___|  \___\___/|_| |_| |_|_| |_| |_|\___|_| |_|\__|___/

// listing >>>
if(isset($user['user_id'])) {
	$user_id = $user['user_id'];
}
else {
	$user_id = 0;
}
$comments = SiteComment::getSiteComments($site_id, $user_id);
// debug($comments);
$smarty->assign('comments', $comments);
// <<<
