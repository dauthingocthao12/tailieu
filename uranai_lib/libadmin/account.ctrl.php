<?php
/*
 * MAINユーザのコントローラに組み込まれる！
 * @date 2016-02-26
 */
if(is_object($history)){
	$history->setPageBeforeLogin();
}

// account register/edit form
if($action==='form') {
	/*>>>*/
	$notificationHourOptions = array(
		// >>>
		"01:00" => "01:00",
		"02:00" => "02:00",
		"03:00" => "03:00",
		"04:00" => "04:00",
		"05:00" => "05:00",
		"06:00" => "06:00",
		"07:00" => "07:00",
		"08:00" => "08:00",
		"09:00" => "09:00",
		"10:00" => "10:00",
		"11:00" => "11:00",
		"12:00" => "12:00",
		"13:00" => "13:00",
		"14:00" => "14:00",
		"15:00" => "15:00",
		"16:00" => "16:00",
		"17:00" => "17:00",
		"18:00" => "18:00",
		"19:00" => "19:00",
		"20:00" => "20:00",
		"21:00" => "21:00",
		"22:00" => "22:00",
		"23:00" => "23:00"
		// <<<
	);
	$smarty->assign('notificationHourOptions', $notificationHourOptions);

	$prefectureOptions = array(
		// >>>
		"1" => "北海道",
		"2" => "青森県",
		"3" => "岩手県",
		"4" => "宮城県",
		"5" => "秋田県",
		"6" => "山形県",
		"7" => "福島県",
		"8" => "茨城県",
		"9" => "栃木県",
		"10" => "群馬県",
		"11" => "埼玉県",
		"12" => "千葉県",
		"13" => "東京都",
		"14" => "神奈川県",
		"15" => "新潟県",
		"16" => "富山県",
		"17" => "石川県",
		"18" => "福井県",
		"19" => "山梨県",
		"20" => "長野県",
		"21" => "岐阜県",
		"22" => "静岡県",
		"23" => "愛知県",
		"24" => "三重県",
		"25" => "滋賀県",
		"26" => "京都府",
		"27" => "大阪府",
		"28" => "兵庫県",
		"29" => "奈良県",
		"30" => "和歌山県",
		"31" => "鳥取県",
		"32" => "島根県",
		"33" => "岡山県",
		"34" => "広島県",
		"35" => "山口県",
		"36" => "徳島県",
		"37" => "香川県",
		"38" => "愛媛県",
		"39" => "高知県",
		"40" => "福岡県",
		"41" => "佐賀県",
		"42" => "長崎県",
		"43" => "熊本県",
		"44" => "大分県",
		"45" => "宮崎県",
		"46" => "鹿児島県",
		"47" => "沖縄県",
		"99"  => "(指定しない)"
		// <<<
	);
	$smarty->assign('prefectureOptions', $prefectureOptions);

	// add avatars
	$avatars = array(
		"comment-icon-noimg.png", // default
		"comment-icon-aeris.png",
		"comment-icon-aquarius.png",
		"comment-icon-cancer.png",
		"comment-icon-capricorn.png",
		"comment-icon-gemini.png",
		"comment-icon-leo.png",
		"comment-icon-libra.png",
		"comment-icon-pisces.png",
		"comment-icon-sagittarius.png",
		"comment-icon-scorpio.png",
		"comment-icon-taurus.png",
		"comment-icon-virgo.png"
	);
	$smarty->assign("avatars", $avatars);
	$smarty->assign("avatar_default", "comment-icon-noimg.png");

	//add okabe start 2016/06/08
	$notifyMailSwOptions = array("1" => "はい", "0" => "いいえ");
	$smarty->assign('notifyMailSwOptions', $notifyMailSwOptions);
	//add okabe end 2016/06/08

	//{{{
	//新規登録者のメールアドレス取得
	$sql = " SELECT mail".
		" FROM temp_ansmail".
		" WHERE 1".
		" AND md5id = '".$Q[3]."' ".
		" ;";

	$result = mysqli_query($conn, $sql);
	if($result){
		$resultFields = $result->fetch_object();
		$newUserMail = $resultFields->mail;
	}
	$smarty->assign("newUserMail",$newUserMail);
	//}}}

	$form = new FormCheckGroup();

	if(!$user) {
		// email check for new users
		$smarty->assign('hideLoginBtn', true);
		$form->addField(new FormCheckRuleEmail('メールアドレス', 'email'));
	}

	// no check for current user if not edited
	if(!$user || 
		//($user && ($_POST['password']!='' || $_POST['password2']!=''))	//del okabe 2016/06/23
		($user && ($_POST['password']!='' || $_POST['password2']!='' || $Q[2]=='emailactivated'))	//add okabe 2016/06/23
	) {
		$form->addField(new FormCheckRule('パスワード', 'password', FormCheckRule::$passwordFormat));
		$form->addField(new FormCheckRule('パスワード確認', 'password2', FormCheckRule::$passwordFormat));
	}

	$form->addField(new FormCheckRule('ハンドルネーム', 'handlename'));
	$form->addField(new FormCheckRuleDateIso('生年月日', 'birthday'));
	$smarty->assign('formFields', json_encode($form->getFields()));

	// form check
	if($_POST) {
		// Post data check logic
		// >>>
		$errors = 0;
		$formErrors = array();
		if(!$form->isValid($_POST)) {
			$formErrors = $form->getErrors();
			++$errors;
		}
		// check gender
		if(!$_POST['gender']) {
			$smarty->assign('genderError', "性別を選択して下さい");
			++$errors;
		}
		// check 2 passwords
		if($_POST['password']!=$_POST['password2']) {
			$formErrors['password2'] = "\"パスワード確認\"が\"パスワード\"と一致しません";
			++$errors;
		}
		// notifications
		if(count($_POST['notification'])===0) {
			$smarty->assign('notificationDaysError', '一つ以上を選択して下さい');
			++$errors;
		}
		if(!$_POST['notificationHolidays']) {
			$smarty->assign('notificationHolidaysError', '選択して下さい');
			++$errors;
		}
		// normal assigns
		$smarty->assign('formErrors', json_encode($formErrors));
		$smarty->assign('formData', $_POST);
		// <<<

		// no errors?
		if($errors===0) {

			// add star from birthday
			$birthdayStar = getStarFromBirthday($_POST['birthday']);

			$dup_email = 0;	//add okabe 2016/06/20

			if($user) {
				$new_account = false;
				// logged User
				$sql = "UPDATE `users` SET";
				if($_POST['password']) {
					$sql .= " `password`='".sha1($_POST['password'])."',";
				}
			} else {
				//add okabe start 2016/06/20
				//メールアドレスが既に登録されているか確認
				$sql0 = "SELECT * FROM `users` WHERE `is_delete`=0";
				$sql0 .= " AND `email`=\"".$_POST['email']."\"";
				$sql0 .= " LIMIT 1;";
				$rs0 = mysqli_query($conn, $sql0);
				if($rs0 && $rs0->num_rows==1) {
					$dup_email = 1;
				}
				//add okabe end 2016/06/20

				// new User
				$new_account = true;
				$sql = "INSERT INTO `users` SET";
				$sql .= " `email`='{$_POST['email']}',";
				$sql .= " `password`='".sha1($_POST['password'])."',";
				$sql .= " `ip`='{$_SERVER['REMOTE_ADDR']}',";
				$sql .= " `user_agent`='{$_SERVER['HTTP_USER_AGENT']}',";
				$sql .= " `activationDate` = NOW(),";
			}
			
			$handlename = $conn->real_escape_string( strip_tags($_POST['handlename']) );
			
			$sql .= " `handlename`='$handlename'";
			$sql .= " ,`avatar`='{$_POST['avatar']}'";
			$sql .= " ,`gender`='{$_POST['gender']}'";
			$sql .= " ,`birthday`='{$_POST['birthday']}'";
			$sql .= " ,`prefecture`='{$_POST['prefecture']}'";		//add okabe 2016/06/30
			$sql .= " ,`birthdayStar`='$birthdayStar'";
			$sql .= " ,`notification1`=".($_POST['notification']['monday']?1:0);
			$sql .= " ,`notification2`=".($_POST['notification']['tuesday']?1:0);
			$sql .= " ,`notification3`=".($_POST['notification']['wednesday']?1:0);
			$sql .= " ,`notification4`=".($_POST['notification']['thursday']?1:0);
			$sql .= " ,`notification5`=".($_POST['notification']['friday']?1:0);
			$sql .= " ,`notification6`=".($_POST['notification']['saturday']?1:0);
			$sql .= " ,`notification0`=".($_POST['notification']['sunday']?1:0);
			$sql .= " ,`notificationSw`='{$_POST['notifyMailSw']}'";	// add okabe 2016/06/20
			$sql .= " ,`notificationHour`='{$_POST['notificationHour']}'";
			$sql .= " ,`notificationHolidays`=".($_POST['notificationHolidays']=='YES'?1:0);
			$sql .= " ,`notificationCommentPublished`=".($_POST['notificationCommentPublished']=='YES'?1:0);
			$sql .= " ,`notificationCommentRejected`=".($_POST['notificationCommentRejected']=='YES'?1:0);
			if($user) {
				// update current user
				// del okabe start 2016/06/07
				//$sql .= " ,`date_update`=NOW()";
				//$sql .= " ,`who_update`='user'";
				//$sql .= " WHERE `user_id`={$user['user_id']}";
				// del okabe end 2016/06/07

				//resume okabe start 2016/07/01
				$sql .= " ,`date_update`=NOW()";
				$sql .= " ,`who_update`='user'";
				//resume okabe end 2016/07/01

				// add okabe start 2016/06/07
				$sql .= " WHERE user_id={$user['user_id']}";
				// del okabe end 2016/06/07
			}
			else {
				// new user
				// del okabe start 2016/06/07
				//$sql .= " ,`who_create`='user'";
				// del okabe end 2016/06/07

				// add okabe start 2016/06/07
				$activationKey = md5(time());
				//$sql .= " ,`activationKey`='$activationKey'"; //byempmailに固定
				$sql .= " ,`activationKey` = 'byempmail'";
				// only email, handlename and activationKey are required values
				$tmp_user = $_POST;
				$tmp_user['activationKey'] = $activationKey;
				// add okabe end 2016/06/07
			}
			//pre($sql);

			//add okabe start 2016/06/20
			if ($dup_email == 1) {	//すでに同じメールアドレスが存在する場合
				$smarty->assign('registration_db_error', true);
				$template_page = "account-form.tpl";
			} else {
				//add okabe start 2016/06/20

				$rs = mysqli_query($conn, $sql);
				if($rs) {
					//$ok = Account::sendAccountDataEmail($_POST, $new_account);	// del okabe 2016/06/07
					// add okabe start 2016/06/07
					//if($user) {
					$ok = Account::sendAccountDataEmail($_POST, $new_account);
					//} else {
					// only email, handlename and activationKey are required values
					//$ok = Account::sendActivateEmail($tmp_user);
					//}
					// add okabe end 2016/06/07
					if(!$ok) {
						$smarty->assign('emailError', true);
					}
					$template_page = "account-form-success.tpl";
					if(is_object($history)){
						$smarty->assign('prev_page', $history->getPageBeforeLogin());
					}
				}
				else {
					if(mysqli_errno($conn)==1062) {
						// existing email in DB error
						$smarty->assign('registration_db_error', true);
					} else {
						$smarty->assign('sql_error', mysqli_errno($conn));
					}
				}
			} //add okabe 2016/06/20
		}
		//add okabe start 2016/06/23
		if (strlen($user['notificationHour']) == 0) {
			$smarty->assign('pw_1st_required', "1");	// パスワード設定を必須にする
		}
		//add okabe end 2016/06/23
	}
	else if($user) {
		//add okabe start 2016/06/23 デフォルト選択
		if (strlen($user['notificationHour']) == 0) {
			$user['notificationHour'] = "08:00";
			$user['notificationHolidays'] = "YES";
			$user['notification']['monday'] = 1;
			$user['notification']['tuesday'] = 1;
			$user['notification']['wednesday'] = 1;
			$user['notification']['thursday'] = 1;
			$user['notification']['friday'] = 1;
			$user['notification']['saturday'] = 1;
			$user['notification']['sunday'] = 1;
			$smarty->assign('pw_1st_required', "1");	// パスワード設定を必須にする
			if ($Q[2] != "emailactivated") {
				$smarty->assign('msg1st', "まだユーザー情報の登録が完了していません。");
			}
		}
		//add okabe end 2016/06/23
		//add okabe start 2016/06/30
		if ($user['handlename'] == "ハンドルネーム未登録") {
			$user['handlename'] = "";
		}
		if (intval($user['prefecture'])==0) {
			$user['prefecture'] = "23";
		}

		// サイトのコメント機能に関して
		if (intval($user['notificationCommentPublished'])==1) {
			$user['notificationCommentPublished'] = "YES";
		}
		if (intval($user['notificationCommentRejected'])==1) {
			$user['notificationCommentRejected'] = "YES";
		}
		
		//add okabe end 2016/06/30
		$smarty->assign('formData', $user);
	}
	/*<<<*/
} // register


// add okabe stat 2016/06/08
//
// action activate-resend
if($action=='activate-resend' && $_POST) {
	// >>>
	$sql = "SELECT *
		FROM `users`
		WHERE `email`='{$_POST['email']}'
		AND `activationDate` IS NULL
		LIMIT 1";
	$rs = mysqli_query($conn, $sql);
	if($rs && $rs->num_rows==1) {
		$u = $rs->fetch_assoc();
		$ok = Account::sendActivateEmail($u);
		if($ok) {
			// mail sent
			$smarty->assign('mailSendOK', true);
		} else {
			// mail error
			$smarty->assign('mailSendERR', true);
		}
	} else {
		// email not found
		$smarty->assign('mailError', true);
	}
	// <<<
}

// account activation
if($action=='activate') {
	/*>>>*/
	$error = false;
	$sql = "SELECT *
		FROM `users`
		WHERE `activationKey`='$activationKey'
		LIMIT 1";
	//pre($sql);
	$rs = mysqli_query($conn, $sql);
	if(!$rs || $rs->num_rows!=1) {
		$error = true;
		$smarty->assign('activateErrorNotFound', true);
	}
	else {
		$user = $rs->fetch_object();
		//var_dump($user);
		if(is_null($user->activationDate)) {
			$sql = "UPDATE `users`
				SET `activationDate`=CURRENT_TIMESTAMP
				WHERE `activationKey`='$activationKey'
				LIMIT 1";
			//pre($sql);
			$rs = mysqli_query($conn, $sql);
			if($rs) {
				$smarty->assign('activateSuccess', true);
			} else {
				$smarty->assign('activateError', true);
			}
		} else {
			$smarty->assign('activateErrorDone', true);
		}
	}
	/*<<<*/
}
//
// add okabe ens 2016/06/08


// action login
if($action=='login' && $_POST) {
	//>>>
	$passwd = sha1($_POST['password']);
	$sql = "SELECT *
		FROM `users`
		WHERE `email`='{$_POST['email']}'
		AND `password`='$passwd'
		AND `is_delete`=0
		LIMIT 1";
	//pre($sql);
	$rs = mysqli_query($conn, $sql);
	if($rs && $rs->num_rows==1) {
		$user = $rs->fetch_object();
		$ok = Account::userSet($user->user_id);
		//$ok = true;
		if($ok) {
			// add okabe start 2016/06/08 ユーザー登録のメールアクティベーション
			if(is_null($user->activationDate)) {
				// not activated yet
				$smarty->assign('loginErrorActivate', true);
				Account::userUnset();
			} else {
				// add okabe end 2016/06/08
				// login ok
				$smarty->assign('loginSuccess', true);
				if(is_object($history)){
					$smarty->assign('prev_page', $history->getPageBeforeLogin());
				}
			}	// add okabe 2016/06/08 ユーザー登録のメールアクティベーション
		} else {
			// error while setting up the cookie
			$smarty->assign('loginErrorSet', true);
		}
	} else {
		// account not found or non active
		$smarty->assign('loginError', true);
		$smarty->assign('loginemail', $_POST['email']);	//add okabe 2016/07/25
	}
	//<<<
}

// passowrd lost
if($action=='password-lost' && $_POST) {
	// >>>
	$sql = "SELECT *
		FROM `users`
		WHERE email='{$_POST['email']}'
		AND is_delete=0
		LIMIT 1";
	$rs_user = mysqli_query($conn, $sql);

	if($rs_user && $rs_user->num_rows==1) {
		$newPasswd = substr(md5(time()), 0, 8);
		$newPassCrypt = sha1($newPasswd);
		$sql = "UPDATE `users`
			SET `password`='$newPassCrypt'
			WHERE `email`='{$_POST['email']}' AND is_delete=0
			LIMIT 1";
		//pre($sql);
		$rs = mysqli_query($conn, $sql);
		if(mysqli_affected_rows($conn)==1) {
			$u = $rs_user->fetch_assoc();
			$ok = Account::sendPasswordEmail($u, $newPasswd);
			if($ok) {
				// mail sent
				$smarty->assign('mailSendOK', true);
			} else {
				// mail error
				$smarty->assign('mailSendERR', true);
			}
		} else {
			// email not found
			$smarty->assign('mailError', true);
		}
	} else {
		// email not found
		$smarty->assign('mailError', true);
	}
	// <<<
}

// delete account
if($action=='delete' && $_POST) {
	// >>>
	$sql = "UPDATE `users` SET
		`date_delete`=NOW(),
		`who_delete`='user',
		`is_delete`=1
		WHERE email='{$_POST['email']}' AND is_delete='0'
		LIMIT 1";
	$rs = $conn->query($sql);
	if($rs && $conn->affected_rows==1) {
		Account::userUnset();
		$bread_del = "true";
		$smarty->assign('deleteOK', true);
	}
	else {
		$smarty->assign('mailError', true);
	}
	// <<<
}

// action logout
if($action=='logout') {
	// >>>
	Account::userUnset();
	// the following is nont needed I think
	/*
	$user = null;
	$smarty->assign('user', null);
	 */
	// <<<
	if(is_object($history)){
		$smarty->assign('prev_page', $history->getPageBeforeLogin());
	}
}

//add oakbe2016/06/24
// delete unregist (マイページから)
if($action=='unregist' && $_POST) {
	// >>>
	if ($user['email'] != $_POST['email']) {
		$smarty->assign('mailError', true);
		$write_info = "cannot delete: ".$user['user_id']." ".$_POST['email']." ? ".$user['date_create']."\n";

	} else {
		//アンケート保存処理
		//$write_info = $user['user_id']." (".$user['date_create'].") ".$user['email']."\n";
		$write_info = $user['user_id']." (".$user['date_create'].") \n";	//メールアドレスは記録しない
		$num_enquete = $_POST['num_enquete'];
		$ary_enquete = $_POST['enquete'];
		for($i=0; $i<intval($num_enquete); $i++) {
			if (strlen($ary_enquete[$i]) > 0) {
				$write_info .= $ary_enquete[$i]."\n";
			}
		}
		if (strlen($_POST['enqtext']) > 0) {
			$write_info .= "その他:".$_POST['enqtext']."\n";
		}

		//アカウント削除
		$sql = "UPDATE `users` SET
			`date_delete`=NOW(),
			`who_delete`='user',
			`is_delete`=1
			WHERE email='{$_POST['email']}' AND is_delete='0'
			LIMIT 1";
		$rs = $conn->query($sql);
		if($rs && $conn->affected_rows==1) {
			Account::userUnset();
			$template_page = "account-delete.tpl";
			$bread_del = "true";
			$smarty->assign('deleteOK', true);
		}
		else {
			$write_info .= "sql_error detected: ".$sql."\n";
			$smarty->assign('mailError', true);
		}
	}
	//アンケート書き込み
	$file_path = dirname(__FILE__)."/../log/unregist_".Date("Ym").".txt";
	$fp = fopen($file_path, 'a');
	fwrite($fp, Date("Ymd His")." ".$write_info."-----\n");
	fclose($fp);
	chmod($file_path, 0666);
	// <<<
}

/* サイトコメント用 >>> */
if($action=='comment') {
	// debug($Q);

	//  _     _     _                                                 _   
	// | |__ (_) __| | ___    ___ ___  _ __ ___  _ __ ___   ___ _ __ | |_ 
	// | '_ \| |/ _` |/ _ \  / __/ _ \| '_ ` _ \| '_ ` _ \ / _ \ '_ \| __|
	// | | | | | (_| |  __/ | (_| (_) | | | | | | | | | | |  __/ | | | |_ 
	// |_| |_|_|\__,_|\___|  \___\___/|_| |_| |_|_| |_| |_|\___|_| |_|\__|

	if($Q[2]=='hide' && $Q[3]) {
		$ok = SiteComment::hideUserComment($user['user_id'], $Q[3]);
		if($ok) {
			$smarty->assign('message', array('status' => 'success', 'content' => COMMENT_HIDE_SUCCESS));
		}
		else {
			$smarty->assign('message', array('status' => 'danger', 'content' => COMMENT_HIDE_ERROR));
		}
	}


	//      _                                                              _   
	//  ___| |__   _____      __   ___ ___  _ __ ___  _ __ ___   ___ _ __ | |_ 
	// / __| '_ \ / _ \ \ /\ / /  / __/ _ \| '_ ` _ \| '_ ` _ \ / _ \ '_ \| __|
	// \__ \ | | | (_) \ V  V /  | (_| (_) | | | | | | | | | | |  __/ | | | |_ 
	// |___/_| |_|\___/ \_/\_/    \___\___/|_| |_| |_|_| |_| |_|\___|_| |_|\__|

	if($Q[2]=='show' && $Q[3]) {
		$ok = SiteComment::showUserComment($user['user_id'], $Q[3]);
		if($ok) {
			$smarty->assign('message', array('status' => 'success', 'content' => COMMENT_SHOW_SUCCESS));
		}
		else {
			$smarty->assign('message', array('status' => 'danger', 'content' => COMMENT_SHOW_ERROR));
		}
	}


	//                                           _       
	//   ___ ___  _ __ ___  _ __ ___   ___ _ __ | |_ ___ 
	//  / __/ _ \| '_ ` _ \| '_ ` _ \ / _ \ '_ \| __/ __|
	// | (_| (_) | | | | | | | | | | |  __/ | | | |_\__ \
	//  \___\___/|_| |_| |_|_| |_| |_|\___|_| |_|\__|___/

	$comments = SiteComment::getUserComments($user['user_id']);
	// debug($comments);
	$smarty->assign('comments', $comments);


	//            _           _
	//   __ _  __| |_ __ ___ (_)_ __    _ __ ___   ___  ___ ___  __ _  __ _  ___  ___
	//  / _` |/ _` | '_ ` _ \| | '_ \  | '_ ` _ \ / _ \/ __/ __|/ _` |/ _` |/ _ \/ __|
	// | (_| | (_| | | | | | | | | | | | | | | | |  __/\__ \__ \ (_| | (_| |  __/\__ \
	//  \__,_|\__,_|_| |_| |_|_|_| |_| |_| |_| |_|\___||___/___/\__,_|\__, |\___||___/
	//                                                                |___/

	$rejected_ids = array_reduce($comments, function($carry, $elem) {
		if($elem['status'] == SiteComment::$STATUS_REJECTED) {
			$carry[] = $elem['site_comment_id'];
		}
		return $carry;
	}, array());
	// debug($parent_ids);
	$rejected = SiteComment::loadRejectedMailContent($rejected_ids);
	// debug($rejected);
	$smarty->assign('rejected', $rejected);


	//                 _     _                 
	//  _ __ _____   _(_)___(_) ___  _ __  ___ 
	// | '__/ _ \ \ / / / __| |/ _ \| '_ \/ __|
	// | | |  __/\ V /| \__ \ | (_) | | | \__ \
	// |_|  \___| \_/ |_|___/_|\___/|_| |_|___/

	// list published/hidden comments WITH the one pending if any!
	$parent_ids = array_reduce($comments, function($carry, $elem) {
		if($elem['parent_revision']) {
			$carry[] = $elem['parent_revision'];
		}
		return $carry;
	}, array());
	// debug($parent_ids);
	$revisions = SiteComment::loadRevisions($parent_ids);
	// debug($revisions);
	$smarty->assign('revisions', $revisions);

}

/* <<< */


// vim: foldmethod=marker
