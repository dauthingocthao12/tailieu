<?php
$smarty->assign('hideLoginBtn', true);
if($user){
	Account::userUnset();
}
//==========================
// 有効期限内のmd5idを取得
//==========================
$sql = " SELECT *".
			 " FROM `temp_ansmail`".
			 " WHERE 1".
			 " AND `md5id` = '{$activationKey}' ".
			 " AND CURRENT_TIMESTAMP < `date_expire`".
			 " LIMIT 1".
			 " ;";

$md5id = "";
$rs = mysqli_query($conn, $sql);

if($rs && $rs->num_rows == 1) {
	$user_data = $rs->fetch_assoc();
	$md5id = $user_data['md5id'];
}
//==========================
// 期限切れの場合
//==========================
if (strlen($md5id) == 0) {
	$template_page = "account-expired-mail.tpl";
	$msg  = "";
	$msg .= "<span>URLが期限切れです、最初からやり直してください。</span><br/>";
	$url = smarty_function_sitelink(array('mode' => 'account/intro'));
	$msg .= "<a href=\"$url\">再度登録を行う場合はこちら</a><br>";
	$smarty->assign("errmsg",$msg);
//==========================
// 有効な場合
//==========================
} else {
	//ユーザーが既に存在するかチェックする
	$regist_user_mail = $user_data['mail'];

	$sql = " SELECT *".
				 " FROM `users`".
				 " WHERE 1".
				 " AND `is_delete` = 0".
				 " AND `email` = '{$regist_user_mail}' ".
				 " LIMIT 1".
				 " ;";

	$rs = mysqli_query($conn, $sql);
	if($rs) {
	$userInfo = $rs->fetch_object();
		//==========================
		// 既に登録済み
		//==========================
		if($rs->num_rows > 0){ //一致行がある
			$template_page = "account-expired-mail.tpl";

			$guidemsg = "<b>このメールアドレスは、すでに登録されています。</b><br/><br/>";
			$guidemsg .= "<span>パスワードがご不明な場合は、こちらからパスワードの再設定を行なうことが出来ます。</span><br/>";

			$url = smarty_function_sitelink(array('mode' => 'account/password-lost'));
			$guidemsg .= "<a href=\"$url\">パスワード再発行</a><br/><br/>";
			$guidemsg .= "<span>ログインする場合はこちら</span><br>";
			$url_login = smarty_function_sitelink(array('mode' => 'account/login'));
			$guidemsg .= "<a href=\"$url_login\">ログイン画面</a><br/>";
			$guidemsg .= "<hr>";
			$smarty->assign("errmsg", $guidemsg);
		//==========================
		// まだ登録していない
		//==========================
		}else{
			//新規登録の流れへ
			$template_page = "account-activated-mail.tpl";
			$loc_url = smarty_function_sitelink(array('mode' => 'account/form/emailactivated/'.$md5id));
			$smarty->assign("userinfo_form", $loc_url);
		}
	}
}
//==========================
// 出力
//==========================

$smarty->assign("site_root_url", SITE_ROOT_URL);
$smarty->display($template_page);
